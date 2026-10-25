// wikigen.cpp -- the wikitext/wikicode emanation (syntax 2.4, third output).
// ===========================================================================
#include "wikigen.h"
#include "ast.h"
#include "../../ledger/ledger_core.h"
#include <fstream>
#include <iostream>
#include <map>
#include <sys/stat.h>
#include <time.h>

namespace sleela {

static std::string baseName(const std::string& path){
    size_t slash=path.find_last_of("/\\");
    return slash==std::string::npos?path:path.substr(slash+1);
}
static std::string dirName(const std::string& path){
    size_t slash=path.find_last_of("/\\");
    return slash==std::string::npos?std::string():path.substr(0,slash);
}
static std::string stripExt(const std::string& name,const std::string& ext){
    if(name.size()>=ext.size()&&name.compare(name.size()-ext.size(),ext.size(),ext)==0)
        return name.substr(0,name.size()-ext.size());
    return name;
}

// The source edit date (YYYY-MM-DD) from the file mtime; today on failure.
static std::string editDate(const std::string& path){
    struct stat st; time_t t;
    if(stat(path.c_str(),&st)==0) t=st.st_mtime; else t=time(nullptr);
    struct tm tmv;
#if defined(_WIN32)
    localtime_s(&tmv,&t);
#else
    localtime_r(&t,&tmv);
#endif
    char buf[16]; strftime(buf,sizeof(buf),"%Y-%m-%d",&tmv);
    return std::string(buf);
}

// The parameter type list restricted to the tokens that appear; only int and
// String are brander-eligible, matching the originated-document contract.
static std::string paramTypeList(const Method& m){
    std::string out; bool first=true;
    for(const auto& p:m.params){ if(!first)out+=","; first=false; out+=p.type; }
    return out;
}
static std::string paramFull(const Method& m){
    std::string out; bool first=true;
    for(const auto& p:m.params){ if(!first)out+=", "; first=false; out+=p.type+" "+p.name; }
    return out;
}
static bool eligible(const Method& m){
    for(const auto& p:m.params) if(p.type!="int"&&p.type!="String") return false;
    return true;
}
static std::string brand(const std::string& doc,const std::string& member,
                         const std::string& sig,const std::string& date){
    return ledger::sha256_hex(doc+"|"+member+"|"+sig+"|"+date);
}

std::string writeWikitextDocument(const Program& prog,
                                  const std::string& srcPath,
                                  const std::string& wikiDir){
    const std::string doc=baseName(srcPath);
    const std::string date=editDate(srcPath);
    const std::string stamp=ledger::utc_iso8601();  // the compile moment (the "second")

    std::string outDir=wikiDir.empty()?dirName(srcPath):wikiDir;
    std::string outBase=stripExt(doc,".sleela");
    std::string outPath=(outDir.empty()?outBase:outDir+"/"+outBase)+".wiki";

    // Tally the work's dimensions so the output can scale with the program.
    int classCount=(int)prog.classes.size();
    int fieldCount=0, methodCount=0, dupNameCount=0;
    for(const auto& c:prog.classes){
        fieldCount+=(int)c.fields.size();
        methodCount+=(int)c.methods.size();
        std::map<std::string,int> nc;
        for(const auto& m:c.methods) nc[m.name]++;
        for(auto& kv:nc) if(kv.second>1) dupNameCount++;
    }

    std::ofstream o(outPath,std::ios::trunc);
    if(!o){ std::cerr<<"wikigen: cannot write '"<<outPath<<"'\n"; return ""; }

    // ---- Page header + infobox --------------------------------------------
    o<<"<!-- Wikitext emanated by the SLeeLa compiler (syntax 2.4) as a\n"
     <<"     consequence of compiling "<<doc<<". Factual, structured, and\n"
     <<"     intended for a self-hosted wiki. Do not submit auto-generated\n"
     <<"     pages to any public encyclopedia without that project's approval. -->\n";
    o<<"{{Infobox software\n";
    o<<"| name          = "<<stripExt(doc,".sleela")<<"\n";
    o<<"| title         = "<<stripExt(doc,".sleela")<<" (SLeeLa document)\n";
    o<<"| genre         = SLeeLa source document\n";
    o<<"| released      = "<<date<<"\n";
    o<<"| latest release date = "<<stamp<<"\n";
    o<<"| programming language = SLeeLa (syntax 2.4)\n";
    o<<"| size          = "<<classCount<<" classes, "<<fieldCount
     <<" fields, "<<methodCount<<" methods\n";
    o<<"}}\n\n";

    o<<"'''"<<stripExt(doc,".sleela")<<"''' is a SLeeLa document compiled under "
     <<"syntax 2.4. This page was emanated as a third compile output alongside "
     <<"the runnable artifact and the [[#Originated source|originated source "
     <<"document]]. The emanation is a consequence of compilation; a larger "
     <<"work produces a larger corpus.\n\n";

    o<<"__TOC__\n\n";

    // ---- The temporal axis (time as a second) -----------------------------
    o<<"== Time ==\n";
    o<<"The document is stamped on two temporal axes. The '''edit date''' is the "
     <<"last modification of the source; the '''compile stamp''' is the UTC "
     <<"instant of this emanation (the second).\n\n";
    o<<"{| class=\"wikitable\"\n";
    o<<"! Axis !! Value\n";
    o<<"|-\n| Source edit date || "<<date<<"\n";
    o<<"|-\n| Compile stamp (UTC) || "<<stamp<<"\n";
    o<<"|-\n| Syntax version || 2.4\n";
    o<<"|}\n\n";

    // ---- Structure: every class and member, with its brander --------------
    o<<"== Structure ==\n";
    o<<"Every field and method, with its declaration-order id and SHA-256 "
     <<"brander (the same identity carried by the originated document). The "
     <<"brander preimage is <code><doc>|<member>|<paramTypes>|<editDate></code>.\n\n";

    for(const auto& c:prog.classes){
        o<<"=== Class "<<c.name<<" ===\n";
        o<<"{| class=\"wikitable sortable\"\n";
        o<<"! id !! Kind !! Member !! Signature !! Slot !! Brander (SHA-256, first 16)\n";
        int id=0;
        for(const auto& f:c.fields){
            ++id;
            std::string h=brand(doc,f.name,"",date);
            o<<"|-\n| "<<id<<" || field || <code>"<<f.name<<"</code> || <code>"
             <<f.type<<"</code> || || <code>"<<h.substr(0,16)<<"</code>\n";
        }
        std::map<std::string,int> nameCount;
        for(const auto& m:c.methods) nameCount[m.name]++;
        std::map<std::string,int> nameSeen;
        for(const auto& m:c.methods){
            ++id;
            std::string sig=paramTypeList(m);
            bool ok=eligible(m);
            std::string h=ok?brand(doc,m.name,sig,date):std::string("n/a");
            int dupTotal=nameCount[m.name];
            int dupSlot=++nameSeen[m.name];
            std::string slot = dupTotal>1 ? (std::to_string(dupSlot)+"/"+std::to_string(dupTotal)) : std::string("");
            o<<"|-\n| "<<id<<" || method || <code>"<<m.name<<"</code> || <code>"
             <<m.retType<<" "<<m.name<<"("<<paramFull(m)<<")</code> || "<<slot
             <<" || <code>"<<(ok?h.substr(0,16):h)<<"</code>\n";
        }
        o<<"|}\n\n";

        // Duplicate-dispatch ordering table, if any.
        bool anyDup=false; for(auto& kv:nameCount) if(kv.second>1){anyDup=true;break;}
        if(anyDup){
            o<<"==== Duplicate-dispatch ordering ("<<c.name<<") ====\n";
            o<<"{| class=\"wikitable\"\n! Name !! Ordering (forward probe from slot 1)\n";
            for(auto& kv:nameCount){
                if(kv.second<=1) continue;
                o<<"|-\n| <code>"<<kv.first<<"</code> || [";
                for(int s=1;s<=kv.second;++s){ if(s>1)o<<", "; o<<"slot "<<s; }
                o<<"]\n";
            }
            o<<"|}\n\n";
        }
    }

    // ---- Movement: members in declaration order (the tempo) ---------------
    o<<"== Movement ==\n";
    o<<"The members in declaration order -- the tempo of the work, read top to "
     <<"bottom as it was authored.\n\n";
    for(const auto& c:prog.classes){
        o<<"=== "<<c.name<<" (movement) ===\n";
        o<<"# '''"<<c.name<<"''' \u2014 class\n";
        for(const auto& f:c.fields)
            o<<"## <code>"<<f.type<<" "<<f.name<<"</code> \u2014 field\n";
        for(const auto& m:c.methods)
            o<<"## <code>"<<m.retType<<" "<<m.name<<"("<<paramFull(m)<<")</code> \u2014 method\n";
        o<<"\n";
    }

    // ---- Provenance + categories ------------------------------------------
    o<<"== Provenance ==\n";
    o<<"This page was emanated by the SLeeLa compiler (Sleelvac\u2122) from the "
     <<"author-marked document <code>"<<doc<<"</code>. It is factual and "
     <<"structured; it asserts nothing beyond the document's own declared "
     <<"structure and timestamps.\n\n";
    o<<"* Classes: "<<classCount<<"\n";
    o<<"* Fields: "<<fieldCount<<"\n";
    o<<"* Methods: "<<methodCount<<"\n";
    o<<"* Duplicated member names: "<<dupNameCount<<"\n\n";

    o<<"[[Category:SLeeLa]]\n";
    o<<"[[Category:SLeeLa documents]]\n";
    o<<"[[Category:Emanated wikitext]]\n";

    if(!o){ std::cerr<<"wikigen: write failed for '"<<outPath<<"'\n"; return ""; }
    return outPath;
}

} // namespace sleela
