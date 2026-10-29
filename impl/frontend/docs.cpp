// docs.cpp -- `sleela docs` subcommand implementation. See docs.h.
// ===========================================================================
#include "docs.h"
#include "library_index.h"
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
namespace fs = std::filesystem;
namespace sleela {
namespace {

// ---- small text helpers ---------------------------------------------------
std::string lower(std::string s){ for(char& c:s) c=(char)std::tolower((unsigned char)c); return s; }
std::string trim(const std::string& s){
    size_t a=s.find_first_not_of(" \t\r\n");
    if(a==std::string::npos) return "";
    size_t b=s.find_last_not_of(" \t\r\n");
    return s.substr(a,b-a+1);
}
std::string squeeze(const std::string& s){ return std::regex_replace(trim(s), std::regex("\\s+"), " "); }

// A levenshtein-free "nearness": a topic is near a candidate when it is a
// case-insensitive substring of the candidate, or the candidate of it, or they
// share a long common prefix. Returns a score 0..100 (higher = closer); 0 = no.
int nearness(const std::string& topicL, const std::string& candL){
    if(topicL==candL) return 100;
    if(candL.find(topicL)!=std::string::npos) return 85;   // topic inside candidate
    if(topicL.find(candL)!=std::string::npos) return 75;   // candidate inside topic
    size_t n=std::min(topicL.size(),candL.size()), p=0;
    while(p<n && topicL[p]==candL[p]) ++p;
    if(p>=3) return (int)(40 + p);                          // shared prefix
    return 0;
}

// ---- the parsed, rich doc model -------------------------------------------
struct Tag { std::string name, value; };
struct Member {
    std::string kind, name, sig, desc;
    std::vector<Tag> tags;           // @param/@returns/... harvested above it
};
struct ClassDoc {
    std::string kind, name, extends, summary;
    std::vector<Member> fields, methods;
    std::vector<Tag> tags;           // class-level @intent/@thesis/@axiom/...
    bool hasMetadata=false;
};
struct UnitDoc {
    std::string package, name, path, definition, syntax;
    std::vector<ClassDoc> classes;
};

bool isMetaName(const std::string& n){
    static const std::regex r("^(metaVersion|metaId|metaAuthor|metaDate|metaCopyright|metaTrademark|"
        "metaCorporation|metaOrigin|metaBirthDate|metaScienceRating|metaNationalSchedule)$");
    static const std::regex a("^(get|set)(MetaVersion|MetaId|Author|Date|Copyright|Trademark|"
        "Corporation|Origin|BirthDate|ScienceRating|NationalSchedule)$");
    return std::regex_match(n,r) || std::regex_match(n,a);
}

// Pull the plain text out of a single comment line (`<`, `<<`/`>>`, `//`), or
// empty string when the line is a decorative rule, or a sentinel-only line.
std::string commentText(const std::string& raw){
    std::string s=trim(raw);
    if(s.rfind("<<",0)==0){ s=s.substr(2); if(s.size()>=2 && s.substr(s.size()-2)==">>") s=s.substr(0,s.size()-2); }
    else if(!s.empty() && s[0]=='<' && s.rfind("<=",0)!=0){ s=s.substr(1); if(!s.empty() && s.back()=='>') s.pop_back(); }
    else if(s.rfind("//",0)==0){ s=s.substr(2); }
    else if(s.rfind("#",0)==0){ return ""; }
    else { return ""; }
    s=trim(s);
    for(const char* sent : {"@doc","@page","@intent","@title"}) if(s.rfind(sent,0)==0) s=trim(s.substr(std::string(sent).size()));
    bool rule=!s.empty(); for(char c:s) if(c!='-'&&c!=' '){ rule=false; break; }
    if(rule) return "";
    return s;
}

// Collect the raw comment lines (delimiters stripped) of the contiguous run
// ending at `end`, top-to-bottom. Used by both the plain-prose and the tagged
// gatherers so they agree on run boundaries.
std::vector<std::string> gatherRaw(const std::vector<std::string>& lines, int end){
    std::vector<std::string> pieces; int i=end;
    while(i>=0){
        std::string s=trim(lines[i]);
        if(s.empty()) break;
        bool isComment = (!s.empty() && (s[0]=='<' || s.rfind("//",0)==0 || s.rfind("<<",0)==0));
        if(!isComment){
            if(!pieces.empty() && s.find(';')==std::string::npos && s.find('{')==std::string::npos && s.find('(')==std::string::npos){ pieces.push_back(s); --i; continue; }
            break;
        }
        // keep the raw (post-delimiter) text, preserving a leading @tag token
        std::string t=trim(lines[i]);
        if(t.rfind("<<",0)==0){ t=t.substr(2); if(t.size()>=2 && t.substr(t.size()-2)==">>") t=t.substr(0,t.size()-2); }
        else if(!t.empty() && t[0]=='<'){ t=t.substr(1); if(!t.empty() && t.back()=='>') t.pop_back(); }
        else if(t.rfind("//",0)==0){ t=t.substr(2); }
        pieces.push_back(trim(t)); --i;
    }
    std::reverse(pieces.begin(),pieces.end());
    return pieces;
}

// Split a comment run into (free prose, tags). A line whose first token is an
// @word starts a tag; following non-@ lines continue that tag's value; lines
// before any @tag are summary prose. Recognised SleelaDoc tags only.
void splitDoc(const std::vector<std::string>& raw, std::string& proseOut, std::vector<Tag>& tagsOut){
    static const std::regex tagHead("^@([A-Za-z]+)\\b\\s*(.*)$");
    std::vector<std::string> prose; Tag* curTag=nullptr;
    for(auto line : raw){
        std::string s=trim(line);
        bool rule=!s.empty(); for(char c:s) if(c!='-'&&c!=' '){ rule=false; break; }
        if(rule||s.empty()) continue;
        std::smatch m;
        if(std::regex_match(s,m,tagHead)){
            tagsOut.push_back(Tag{lower(m[1].str()), trim(m[2].str())});
            curTag=&tagsOut.back();
        } else if(curTag){
            if(!curTag->value.empty()) curTag->value+=" ";
            curTag->value+=s;
        } else {
            prose.push_back(s);
        }
    }
    std::string p; for(auto& x:prose){ if(x.empty()) continue; if(!p.empty()) p+=" "; p+=x; }
    proseOut=squeeze(p);
}

// Gather the contiguous comment run that ends at line index `end` (inclusive),
// walking upward, and join it as one prose string.
std::string gatherAbove(const std::vector<std::string>& lines, int end){
    std::vector<std::string> pieces; int i=end;
    while(i>=0){
        std::string s=trim(lines[i]);
        if(s.empty()) break;
        std::string t=commentText(lines[i]);
        bool isComment = (!s.empty() && (s[0]=='<' || s.rfind("//",0)==0 || s.rfind("<<",0)==0));
        if(!isComment){
            // allow interior lines of a << >> block (no delimiter) only mid-run
            if(!pieces.empty() && s.find(';')==std::string::npos && s.find('{')==std::string::npos && s.find('(')==std::string::npos){ pieces.push_back(s); --i; continue; }
            break;
        }
        pieces.push_back(t); --i;
    }
    std::reverse(pieces.begin(),pieces.end());
    std::string out; for(const auto& p:pieces){ if(p.empty()) continue; if(!out.empty()) out+=" "; out+=p; }
    return squeeze(out);
}

// Parse one .sleela unit into a rich doc model (mirrors the Python generator's
// grammar closely enough for the terminal rendering). Symbol paths are stored
// RELATIVE to the index root, so resolve against it before opening.
UnitDoc parseUnit(const library::Symbol& sym, const std::string& libRoot){
    UnitDoc u; u.package=sym.package; u.name=sym.name;
    fs::path full = fs::path(sym.path).is_absolute() ? fs::path(sym.path) : (fs::path(libRoot)/sym.path);
    // Display a stable, repo-relative path (lib/<...>), not an absolute build
    // path, so a page is identical on every machine and clean inside the JAR.
    fs::path libp(libRoot);
    std::string libName = libp.filename().string().empty()? "lib" : libp.filename().string();
    u.path = (fs::path(libName)/sym.path).generic_string();
    std::ifstream in(full); if(!in) return u;
    std::stringstream ss; ss<<in.rdbuf(); std::string text=ss.str();
    // header Definition line (within the first block)
    std::smatch m; std::string head=text.substr(0,std::min<size_t>(text.size(),1600));
    if(std::regex_search(head,m,std::regex("Definition:\\s*([^>]+)"))) u.definition=squeeze(m[1].str());
    if(std::regex_search(text,m,std::regex("#sleela\\s+([0-9.]+)"))) u.syntax=m[1].str();

    std::vector<std::string> lines; { std::stringstream ls(text); std::string ln; while(std::getline(ls,ln)) lines.push_back(ln); }
    static const std::regex classRe("^(class|struct|interface)\\s+([A-Za-z_][A-Za-z0-9_]*)(?:\\s+extends\\s+([A-Za-z_][A-Za-z0-9_]*))?");
    static const std::regex fieldRe("^(?:static\\s+)?(?:final\\s+)?([A-Za-z_][A-Za-z0-9_]*(?:\\s*\\[\\s*\\])?)\\s+([A-Za-z_][A-Za-z0-9_]*)\\s*(?:=.*)?;\\s*$");
    static const std::regex methRe("^(?:static\\s+)?(?:final\\s+)?([A-Za-z_][A-Za-z0-9_]*(?:\\s*\\[\\s*\\])?)\\s+([A-Za-z_][A-Za-z0-9_]*)\\s*\\(");
    ClassDoc* cur=nullptr; bool inBlock=false; int depth=0;
    for(int idx=0;idx<(int)lines.size();++idx){
        std::string s=trim(lines[idx]);
        if(inBlock){ if(s.find(">>")!=std::string::npos) inBlock=false; continue; }
        if(s.rfind("<<",0)==0 && s.find(">>")==std::string::npos){ inBlock=true; continue; }
        if(!s.empty() && (s[0]=='<' || s.rfind("//",0)==0)) continue;
        if(s.rfind("#",0)==0) continue;
        int opens=(int)std::count(s.begin(),s.end(),'{');
        int closes=(int)std::count(s.begin(),s.end(),'}');
        std::smatch cm;
        if(std::regex_search(s,cm,classRe)){
            u.classes.push_back(ClassDoc{cm[1].str(),cm[2].str(),cm[3].matched?cm[3].str():"", "", {}, {}, {}, false});
            cur=&u.classes.back();
            { std::string prose; std::vector<Tag> tg; splitDoc(gatherRaw(lines,idx-1),prose,tg); cur->summary=prose; cur->tags=tg; }
            depth += opens-closes;      // usually enters the class body (depth 1)
            continue;
        }
        if(!cur){ depth += opens-closes; continue; }
        // Members live at the class-body level (depth==1). Deeper lines are
        // method-body locals (int i = 0; return this; ...) and must be skipped.
        bool atClassBody = (depth==1);
        std::smatch mm;
        if(atClassBody && std::regex_search(s,mm,fieldRe)){
            std::string nm=mm[2].str();
            if(isMetaName(nm)){ cur->hasMetadata=true; }
            else { std::string prose; std::vector<Tag> tg; splitDoc(gatherRaw(lines,idx-1),prose,tg);
                   cur->fields.push_back(Member{"field",nm,squeeze(s.substr(0,s.size()-1)),prose,tg}); }
        } else if(atClassBody && std::regex_search(s,mm,methRe)){
            std::string nm=mm[2].str();
            if(isMetaName(nm)){ cur->hasMetadata=true; }
            else { std::string sig=s; size_t br=sig.find('{'); if(br!=std::string::npos) sig=trim(sig.substr(0,br));
                   std::string prose; std::vector<Tag> tg; splitDoc(gatherRaw(lines,idx-1),prose,tg);
                   cur->methods.push_back(Member{"method",nm,squeeze(sig),prose,tg}); }
        }
        depth += opens-closes;
        if(depth<0) depth=0;
    }
    return u;
}

// ---- rendering -------------------------------------------------------------
void rule(std::ostream& o,char c='='){ o<<std::string(74,c)<<"\n"; }

void renderClass(std::ostream& o, const UnitDoc& u, const ClassDoc& c){
    rule(o);
    o<<"  "<<c.kind<<"  "<<c.name;
    if(!c.extends.empty()) o<<"  (extends "<<c.extends<<")";
    o<<"\n";
    o<<"  package "<<u.package<<"   |   source "<<u.path;
    if(!u.syntax.empty()) o<<"   |   #sleela "<<u.syntax;
    o<<"\n";
    rule(o);
    // helper: first tag value matching any of the given names (class scope)
    auto classTag=[&](std::initializer_list<const char*> names)->std::string{
        for(const char* nm:names) for(const auto& t:c.tags) if(t.name==nm) return t.value; return std::string(); };
    auto allTags=[&](const char* name)->std::vector<std::string>{
        std::vector<std::string> v; for(const auto& t:c.tags) if(t.name==name) v.push_back(t.value); return v; };

    if(!u.definition.empty()){ o<<"\nDEFINITION\n  "<<u.definition<<"\n"; }

    // DESIGN PURPOSE: prefer an explicit @intent, then @thesis, then the
    // harvested summary prose. Richer when the author tagged intent.
    std::string intent=classTag({"intent"});
    std::string thesis=classTag({"thesis"});
    o<<"\nDESIGN PURPOSE\n";
    if(!intent.empty()) o<<"  "<<intent<<"\n";
    if(!c.summary.empty() && c.summary!=intent) o<<"  "<<c.summary<<"\n";
    if(!thesis.empty()) o<<"  Thesis: "<<thesis<<"\n";
    if(intent.empty() && c.summary.empty() && thesis.empty()) o<<"  (no stated purpose; see the source header)\n";

    // Axioms / invariants: the load-bearing promises, when the author stated them.
    { auto ax=allTags("axiom"); auto inv=allTags("invariant");
      if(!ax.empty()||!inv.empty()){
        o<<"\nPRINCIPLES\n";
        for(auto& a:ax)  o<<"  * axiom: "<<a<<"\n";
        for(auto& i:inv) o<<"  * invariant: "<<i<<"\n";
      } }

    // RELATIONSHIPS: what it builds on, author @see links, and the member tally.
    o<<"\nRELATIONSHIPS\n";
    if(!c.extends.empty()) o<<"  * builds on: "<<c.extends<<"\n";
    for(auto& s:allTags("see")) o<<"  * see also: "<<s<<"\n";
    o<<"  * fields: "<<c.fields.size()<<"   methods: "<<c.methods.size()<<"\n";
    if(c.hasMetadata) o<<"  * carries the standard Metadata Class record (provenance + fiduciary attributes)\n";

    // STRUCTURE: the members, each with its harvested explanation.
    if(!c.fields.empty()){
        o<<"\nSTRUCTURE -- state\n";
        for(const auto& f:c.fields){ o<<"  "<<f.sig<<"\n"; if(!f.desc.empty()) o<<"      "<<f.desc<<"\n"; }
    }
    if(!c.methods.empty()){
        o<<"\nSTRUCTURE -- behaviour\n";
        for(const auto& mth:c.methods){
            o<<"  "<<mth.sig<<"\n";
            if(!mth.desc.empty()) o<<"      "<<mth.desc<<"\n";
            for(const auto& t:mth.tags){
                if(t.name=="param")   o<<"      param   "<<t.value<<"\n";
                else if(t.name=="returns"||t.name=="return") o<<"      returns "<<t.value<<"\n";
                else if(t.name=="throws") o<<"      throws  "<<t.value<<"\n";
            }
        }
    }

    // USER CLAYS: the usage habits a reader brings, and the cues that make a
    // present audience read more carefully. Derived, not invented: it names the
    // assumption a reader trained on other systems will carry in, and points at
    // the careful cue (the class's own stated purpose / metadata).
    o<<"\nUSER CLAYS\n";
    o<<"  * Assumed Be: a reader trained on other peoples' systems will reach for\n"
       "    "<<c.name<<" expecting the familiar shape of its name; read the DESIGN\n"
       "    PURPOSE before assuming behaviour by analogy.\n";
    std::string cue = !intent.empty()? intent : c.summary;
    o<<"  * Careful cue: "<<(cue.empty()? std::string("consult the source header and the member explanations above")
                                        : squeeze(cue).substr(0,std::min<size_t>(cue.size(),180)))<<"\n";
    o<<"  * Audience: INT (the attentive reader) studies the RELATIONSHIPS;\n"
       "    Swarmy (the habitual reader) should slow at the Careful cue.\n";

    // MASTERY LEVELS: a graduated study ladder over this class. Author-supplied
    // @mastery lines (if any) are listed first as the class's own guidance; the
    // standard five-rung ladder always follows so every page teaches the method.
    o<<"\nMASTERY LEVELS\n";
    { auto ms=allTags("mastery"); for(size_t k=0;k<ms.size();++k) o<<"  * "<<ms[k]<<"\n"; if(!ms.empty()) o<<"  --\n"; }
    o<<"  1 Recognise  -- name "<<c.name<<" and say, in one line, what it is for.\n";
    o<<"  2 Use        -- call its "<<c.methods.size()<<" method(s) for the common task.\n";
    o<<"  3 Relate     -- place it among its collaborators"<<(c.extends.empty()?"":" and its base "+c.extends)<<".\n";
    o<<"  4 Extend     -- change or specialise its behaviour without breaking callers.\n";
    o<<"  5 Teach      -- explain its design purpose to the next student.\n";
    o<<"\n";
}

void renderPackage(std::ostream& o, const library::Index& ix, const std::string& pkg){
    rule(o);
    o<<"  package  "<<pkg<<"\n";
    rule(o);
    std::vector<const library::Symbol*> units;
    for(const auto& s:ix.symbols()) if(s.package==pkg) units.push_back(&s);
    std::sort(units.begin(),units.end(),[](auto*a,auto*b){return a->name<b->name;});
    o<<"\n"<<units.size()<<" documentable unit(s) in package '"<<pkg<<"':\n\n";
    for(auto* s:units){
        UnitDoc u=parseUnit(*s,ix.root());
        std::string one = !u.definition.empty()? u.definition
                         : (!u.classes.empty() && !u.classes.front().summary.empty()? u.classes.front().summary : std::string());
        if(one.size()>96) one=one.substr(0,93)+"...";
        o<<"  "<<s->name;
        if(!one.empty()) o<<"\n      "<<one;
        o<<"\n";
    }
    // Facade-only hint: a package whose only /lib unit is the SLPackage marker
    // has its real material elsewhere on disk. Offer the live OS search.
    bool facadeOnly = (units.size()==1 && units.front()->name=="SLPackage");
    if(facadeOnly){
        o<<"\nThis is a FACADE package: its /lib entry is only the package marker.\n"
          <<"Find and document its real files on disk with the live OS search:\n"
          <<"    sleela docs --facade "<<pkg<<"\n"
          <<"(bounded by 6 MB / 6000 files / 6 min; widen with --facade-root <dir>)\n\n";
    } else {
        o<<"\nRead one in full with:  sleela docs "<<pkg<<" <ClassName>\n"
          <<"                   or:  sleela docs <ClassName>\n\n";
    }
}

void listAll(std::ostream& o, const library::Index& ix){
    std::map<std::string,int> pkgCount;
    for(const auto& s:ix.symbols()) ++pkgCount[s.package];
    rule(o);
    o<<"  SleelaDoc -- all documentable topics\n";
    rule(o);
    o<<"\n"<<pkgCount.size()<<" package(s), "<<ix.symbolCount()<<" unit(s). Ask for any by name:\n\n";
    o<<"    sleela docs <package>            e.g. sleela docs crypto\n";
    o<<"    sleela docs <ClassName>          e.g. sleela docs stringbuilder\n";
    o<<"    sleela docs <package> <Class>    the exact unit\n\n";
    o<<"PACKAGES\n";
    for(const auto& kv:pkgCount){ if(kv.first=="__root__") continue; o<<"  "<<kv.first<<"  ("<<kv.second<<")\n"; }
    o<<"\n";
}

void suggestNear(std::ostream& o, const library::Index& ix, const std::string& topic){
    std::string tL=lower(topic);
    struct Hit{ int score; std::string pkg,name; };
    std::vector<Hit> hits;
    std::map<std::string,int> pkgSeen;
    for(const auto& s:ix.symbols()){
        int sc=nearness(tL,lower(s.name));
        if(sc>0) hits.push_back({sc,s.package,s.name});
        int pc=nearness(tL,lower(s.package));
        if(pc>0 && !pkgSeen[s.package]++) hits.push_back({pc-1,s.package,"(whole package)"});
    }
    std::sort(hits.begin(),hits.end(),[](const Hit&a,const Hit&b){return a.score>b.score;});
    o<<"No exact topic '"<<topic<<"'. Near results:\n\n";
    int shown=0;
    for(const auto& h:hits){ if(shown++>=12) break; o<<"  "<<h.pkg<<"  "<<h.name<<"\n"; }
    if(shown==0) o<<"  (nothing close -- try 'sleela docs' for the full list)\n";
    o<<"\n";
}

// ---- pre-generated corpus (deliverable #4) --------------------------------
// The CLI prefers a pre-generated plain-text corpus when present: it is fast,
// offline, and ships inside the SleelaDoc JAR alongside sources (deliverable
// #5). Layout, rooted at the SleelaDoc root (api/sleela/docs by default):
//     <root>/TOPICS.index              one "package<TAB>Unit" line per unit
//     <root>/<package>/<Unit>.txt      the rendered class page
//     <root>/<package>/_package.txt    the rendered package overview
// The corpus is always byte-identical to live output because both go through
// the same renderClass/renderPackage writers.

// Where the corpus lives. Overridable with SLEELA_DOCS_DIR; else <root>/api/
// sleela/docs under the discovered /lib root's parent (the repo/install root).
std::string corpusDir(const library::Index& ix){
    if(const char* e=std::getenv("SLEELA_DOCS_DIR"); e&&*e) return e;
    fs::path lib(ix.root());
    fs::path base = lib.has_parent_path()? lib.parent_path() : lib;   // repo/install root
    return (base/"api"/"sleela"/"docs").generic_string();
}

// Emit one cached page if it exists; returns true when served from cache.
bool serveCached(std::ostream& o, const std::string& dir, const std::string& pkg, const std::string& unit){
    fs::path p = fs::path(dir)/pkg/(unit+".txt");
    std::ifstream in(p); if(!in) return false;
    o<<in.rdbuf();
    return true;
}

// Packages excluded from the COMMITTED corpus (and the JAR). The java compat
// mirror and the early-access trees are enormous (the java package alone is
// ~5k units / 22 MB) and are faithful re-declarations of upstream Java, not
// SLeeLa standard-library design. The live `sleela docs java ...` path still
// documents them on demand; only the pre-generated corpus is scoped to the
// SLeeLa standard library. Override by setting SLEELA_DOCS_ALL=1.
bool corpusIncludes(const std::string& pkg){
    if(const char* e=std::getenv("SLEELA_DOCS_ALL"); e&&*e&&std::string(e)!="0") return true;
    if(pkg=="__root__") return false;
    if(pkg=="java") return false;
    return true;
}

// Generate the entire corpus under `outDir`. Returns 0 on success.
int generateCorpus(const library::Index& ix, const std::string& outDir){
    std::error_code ec;
    fs::create_directories(outDir,ec);
    std::ofstream index((fs::path(outDir)/"TOPICS.index").string());
    if(!index){ std::cerr<<"sleela docs --generate: cannot write under "<<outDir<<"\n"; return 1; }
    // Collect packages in sorted order for stable, reviewable output.
    std::map<std::string,std::vector<const library::Symbol*>> byPkg;
    for(const auto& s:ix.symbols()){
        if(s.path.find("early_access")!=std::string::npos) continue;
        if(!corpusIncludes(s.package)) continue;
        byPkg[s.package].push_back(&s);
    }
    size_t units=0, pages=0;
    for(auto& kv:byPkg){
        const std::string& pkg=kv.first;
        if(pkg=="__root__") continue;
        fs::create_directories(fs::path(outDir)/pkg,ec);
        // package overview
        { std::ostringstream po; renderPackage(po,ix,pkg);
          std::ofstream pf((fs::path(outDir)/pkg/"_package.txt").string()); pf<<po.str(); }
        auto& us=kv.second;
        std::sort(us.begin(),us.end(),[](auto*a,auto*b){return a->name<b->name;});
        for(auto* s:us){
            ++units;
            UnitDoc u=parseUnit(*s,ix.root());
            if(u.classes.empty()) continue;
            std::ostringstream cp;
            for(const auto& c:u.classes) renderClass(cp,u,c);
            std::ofstream cf((fs::path(outDir)/pkg/(s->name+".txt")).string());
            cf<<cp.str();
            index<<pkg<<"\t"<<s->name<<"\n";
            ++pages;
        }
    }
    std::cout<<"SleelaDoc corpus written to "<<outDir<<"\n"
             <<"  "<<byPkg.size()<<" package(s), "<<units<<" unit(s), "<<pages<<" class page(s), plus TOPICS.index\n";
    return 0;
}

// ---- facade live OS search (deliverable #6) -------------------------------
// A facade-only package (e.g. psychiatry, politico) carries just an SLPackage
// marker under /lib; the real subject material lives elsewhere on disk (sibling
// top-level dirs, data, docs, scripts). `sleela docs --facade <topic>` searches
// the filesystem AT RUNTIME for files belonging to the topic and renders a
// facade report. The search is bounded by three user-overridable caps and
// always prints an honest, partial report naming whichever cap stopped it:
//     --facade-max-mb N        working-memory ceiling   (default 6 MB)
//     --facade-max-files N     files examined ceiling    (default 6000)
//     --facade-max-seconds N   wall-clock ceiling        (default 360 = 6 min)
//     --facade-root DIR        where to search (default: the SLeeLa root; may
//                              be widened up to "/" at the user's choice)
struct FacadeCaps { size_t maxBytes=6*1024*1024; size_t maxFiles=6000; long maxSeconds=360; };

struct FacadeHit { std::string path; std::string kind; std::string peek; uintmax_t size=0; };

std::string classifyKind(const fs::path& p){
    std::string e=lower(p.extension().string());
    if(e==".sleela") return "source (.sleela)";
    if(e==".md"||e==".txt"||e==".rst"||e==".adoc") return "document";
    if(e==".c"||e==".h"||e==".cpp"||e==".hpp"||e==".cc"||e==".mm") return "native source";
    if(e==".py"||e==".sh"||e==".ps1"||e==".bat") return "script";
    if(e==".json"||e==".xml"||e==".csv"||e==".yaml"||e==".yml"||e==".sst"||e==".sheet") return "data";
    if(e==".key"||e==".pem"||e==".bin") return "binary/key";
    return "other";
}

bool looksText(const fs::path& p){
    std::string e=lower(p.extension().string());
    static const std::vector<std::string> bin={".key",".pem",".bin",".png",".jpg",".jpeg",".gif",".zip",".jar",".gz",".o",".a",".so",".class"};
    for(const auto& b:bin) if(e==b) return false;
    return true;
}

// First non-empty, non-trivial line of a text file, trimmed to 120 chars.
std::string firstLinePeek(const fs::path& p){
    std::ifstream in(p); if(!in) return "";
    std::string ln; int guard=0;
    while(std::getline(in,ln) && guard++<40){
        std::string s=trim(ln);
        if(s.empty()) continue;
        // skip pure comment/marker lines for a more useful peek
        if(s.size()>120) s=s.substr(0,117)+"...";
        return s;
    }
    return "";
}

// Should this directory be skipped during the walk?
bool skipDir(const std::string& name){
    return name==".git" || name=="node_modules" || name=="build" ||
           name==".cache" || name=="__pycache__" || name==".svn";
}

int renderFacade(std::ostream& o, const library::Index& ix, const std::string& topic,
                 const std::string& rootArg, const FacadeCaps& caps){
    // Resolve the search root: explicit arg, else env, else the SLeeLa root
    // (parent of /lib) so sibling top-level subject dirs are found by default.
    std::string root=rootArg;
    if(root.empty()) if(const char* e=std::getenv("SLEELA_FACADE_PATH"); e&&*e) root=e;
    if(root.empty()){ fs::path lib(ix.root()); root=(lib.has_parent_path()?lib.parent_path():lib).generic_string(); }
    fs::path rootP(root);
    std::error_code ec;
    if(!fs::is_directory(rootP,ec)){ o<<"sleela docs --facade: search root is not a directory: "<<root<<"\n"; return 1; }

    std::string tL=lower(topic);
    rule(o);
    o<<"  facade  "<<topic<<"   (live OS search)\n";
    rule(o);
    o<<"\nsearch root : "<<fs::weakly_canonical(rootP,ec).generic_string();
    if(ec){ ec.clear(); o<<root; }
    o<<"\ncaps        : "<<(caps.maxBytes/(1024*1024))<<" MB working memory, "
      <<caps.maxFiles<<" files, "<<caps.maxSeconds<<" s wall-clock\n\n";

    std::vector<FacadeHit> hits;
    size_t filesSeen=0, bytesHeld=0, dirsSeen=0;
    auto t0=std::chrono::steady_clock::now();
    const char* stopReason=nullptr;
    bool topicDirMatch=false;
    auto elapsedS=[&]{ return (long)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()-t0).count(); };

    // PHASE 1 -- locate directories named <topic> (case-insensitive) anywhere
    // under the root. A facade topic's real material lives in a directory of
    // that name; finding those first lets the file caps apply to the RELEVANT
    // subtree(s) rather than being spent walking unrelated trees. The dir scan
    // is itself bounded by the file/time caps (counting directories visited).
    std::vector<fs::path> topicDirs;
    // If the root directory itself is named the topic, it is a match -- a user
    // who points --facade-root straight at the topic dir expects its contents.
    {
        fs::path canon=fs::weakly_canonical(rootP,ec); if(ec){ ec.clear(); canon=rootP; }
        if(lower(canon.filename().string())==tL) topicDirs.push_back(rootP);
    }
    {
        fs::recursive_directory_iterator dit(rootP, fs::directory_options::skip_permission_denied, ec), dend;
        if(ec){ o<<"(cannot read search root: "<<ec.message()<<")\n"; return 1; }
        for(; dit!=dend; dit.increment(ec)){
            if(ec){ ec.clear(); continue; }
            if(elapsedS()>=caps.maxSeconds){ stopReason="time"; break; }
            std::error_code dec; if(!dit->is_directory(dec)){ dec.clear(); continue; }
            const fs::path& d=dit->path();
            if(skipDir(d.filename().string())){ dit.disable_recursion_pending(); continue; }
            if(++dirsSeen>=caps.maxFiles){ stopReason="files"; break; }   // reuse the files cap for the dir scan
            if(lower(d.filename().string())==tL) topicDirs.push_back(d);
        }
    }

    // PHASE 2 -- within each matched topic directory, collect its FOLDERS and
    // its regular files (recursively), honoring the three caps. The folders are
    // the shape of the subject; the files follow as a matter of natural motion.
    // Every directory and file under a topic-named directory belongs to the
    // topic. The matched topic directories themselves are folders of record.
    std::vector<FacadeHit> folders;
    auto addFolder=[&](const fs::path& d){
        FacadeHit f; f.path=d.generic_string(); f.kind="folder"; f.size=0;
        size_t cost=f.path.size()+64; if(bytesHeld+cost>caps.maxBytes){ stopReason="memory"; return false; }
        bytesHeld+=cost; folders.push_back(std::move(f)); return true;
    };
    if(!stopReason)
    for(const auto& base:topicDirs){
        topicDirMatch=true;
        if(!addFolder(base)) break;                     // the topic folder itself
        fs::recursive_directory_iterator it(base, fs::directory_options::skip_permission_denied, ec), end;
        if(ec){ ec.clear(); continue; }
        for(; it!=end; it.increment(ec)){
            if(ec){ ec.clear(); continue; }
            if(elapsedS()>=caps.maxSeconds){ stopReason="time"; break; }
            std::error_code fec;
            if(it->is_directory(fec)){
                if(skipDir(it->path().filename().string())){ it.disable_recursion_pending(); continue; }
                if(++dirsSeen>=caps.maxFiles){ stopReason="files"; break; }
                if(!addFolder(it->path())){ break; }     // folders shown implicitly
                continue;
            }
            if(fec){ fec.clear(); continue; }
            if(!it->is_regular_file(fec)){ fec.clear(); continue; }
            if(++filesSeen>=caps.maxFiles){ stopReason="files"; break; }
            const fs::path& p=it->path();
            FacadeHit h; h.path=p.generic_string(); h.kind=classifyKind(p);
            h.size=fs::file_size(p,fec); if(fec){ fec.clear(); h.size=0; }
            if(looksText(p)) h.peek=firstLinePeek(p);
            size_t cost=h.path.size()+h.peek.size()+64;
            if(bytesHeld+cost>caps.maxBytes){ stopReason="memory"; break; }
            bytesHeld+=cost; hits.push_back(std::move(h));
        }
        if(stopReason) break;
    }

    // Group by kind for a legible report.
    std::sort(hits.begin(),hits.end(),[](const FacadeHit&a,const FacadeHit&b){
        if(a.kind!=b.kind) return a.kind<b.kind; return a.path<b.path; });

    std::sort(folders.begin(),folders.end(),[](const FacadeHit&a,const FacadeHit&b){return a.path<b.path;});

    if(hits.empty() && folders.empty()){
        o<<"No folders or files found for topic '"<<topic<<"' under the search root.\n";
        if(!topicDirMatch) o<<"  (no path component named '"<<topic<<"' was seen; try --facade-root <dir> to widen)\n";
    } else {
        // Folders first -- the shape of the subject, shown implicitly.
        if(!folders.empty()){
            o<<folders.size()<<" folder(s) of '"<<topic<<"':\n\n[folder]\n";
            for(const auto& f:folders) o<<"  "<<f.path<<"/\n";
            o<<"\n";
        }
        o<<hits.size()<<" file(s) belonging to '"<<topic<<"' (the files follow as natural motion):\n";
        std::string lastKind;
        for(const auto& h:hits){
            if(h.kind!=lastKind){ o<<"\n["<<h.kind<<"]\n"; lastKind=h.kind; }
            o<<"  "<<h.path<<"  ("<<h.size<<" bytes)\n";
            if(!h.peek.empty()) o<<"      "<<h.peek<<"\n";
        }
        // For any .sleela sources found, render their full class pages too.
        bool anySrc=false;
        for(const auto& h:hits) if(h.kind=="source (.sleela)"){ anySrc=true; break; }
        if(anySrc){
            o<<"\n"<<std::string(74,'-')<<"\nRENDERED SOURCE PAGES\n"<<std::string(74,'-')<<"\n";
            for(const auto& h:hits){
                if(h.kind!="source (.sleela)") continue;
                library::Symbol sym{topic, fs::path(h.path).stem().string(), h.path};
                UnitDoc u=parseUnit(sym, ix.root());
                u.path=h.path;  // show the real discovered path
                for(const auto& c:u.classes) renderClass(o,u,c);
            }
        }
    }

    long elapsed=elapsedS();
    o<<"\n"<<std::string(74,'=')<<"\n";
    o<<"scanned "<<dirsSeen<<" dir(s), collected "<<filesSeen<<" file(s) in "<<elapsed<<" s; held ~"<<(bytesHeld/1024)<<" KB.\n";
    if(stopReason) o<<"NOTE: stopped early -- "<<stopReason<<" cap reached ("
        <<(std::string(stopReason)=="memory"? std::to_string(caps.maxBytes/(1024*1024))+" MB"
          : std::string(stopReason)=="files"? std::to_string(caps.maxFiles)+" files"
          : std::to_string(caps.maxSeconds)+" s")
        <<"). Raise it with --facade-max-"<<(std::string(stopReason)=="memory"?"mb":stopReason==std::string("files")?"files":"seconds")<<" N, or narrow --facade-root.\n";
    else o<<"complete (no cap reached).\n";
    return 0;
}

} // namespace

int docsCmd(int argc, char** argv){
    // Collect the topic words that follow the "docs" command token. The driver
    // may or may not have resliced argv (it only does so when global options
    // preceded the command), so locate "docs" and take everything after it.
    std::vector<std::string> words;
    int docsAt=-1;
    for(int i=0;i<argc;++i) if(std::string(argv[i])=="docs"){ docsAt=i; break; }
    int firstWord=(docsAt>=0)?docsAt+1:1;
    for(int i=firstWord;i<argc;++i) words.push_back(argv[i]);
    library::Index ix = library::Index::discover();
    if(ix.empty()){
        std::cerr<<"sleela docs: could not locate the /lib source tree.\n"
                   "  Set SLEELA_LIB or run from the SLeeLa checkout.\n";
        return 1;
    }
    std::ostream& o=std::cout;

    // Facade live OS search: `sleela docs --facade <topic> [--facade-root DIR]
    // [--facade-max-mb N] [--facade-max-files N] [--facade-max-seconds N]`.
    // Also auto-offered when a plain topic resolves to a facade-only package.
    {
        bool facade=false; std::string facadeRoot; FacadeCaps caps; std::string facadeTopic;
        std::vector<std::string> rest;
        for(size_t i=0;i<words.size();++i){
            const std::string& w=words[i];
            auto next=[&](const char* flag)->std::string{ if(i+1<words.size()){ return words[++i]; } std::cerr<<"sleela docs: "<<flag<<" needs a value\n"; return ""; };
            if(w=="--facade"||w=="-f") facade=true;
            else if(w=="--facade-root") facadeRoot=next("--facade-root");
            else if(w.rfind("--facade-root=",0)==0) facadeRoot=w.substr(14);
            else if(w=="--facade-max-mb"){ std::string v=next("--facade-max-mb"); if(!v.empty()) caps.maxBytes=(size_t)std::strtoull(v.c_str(),nullptr,10)*1024*1024; }
            else if(w=="--facade-max-files"){ std::string v=next("--facade-max-files"); if(!v.empty()) caps.maxFiles=(size_t)std::strtoull(v.c_str(),nullptr,10); }
            else if(w=="--facade-max-seconds"){ std::string v=next("--facade-max-seconds"); if(!v.empty()) caps.maxSeconds=std::strtol(v.c_str(),nullptr,10); }
            else rest.push_back(w);
        }
        if(facade){
            if(rest.empty()){ std::cerr<<"sleela docs --facade: name a topic, e.g. 'sleela docs --facade psychiatry'\n"; return 2; }
            return renderFacade(o,ix,rest.back(),facadeRoot,caps);
        }
    }

    // Corpus generation: `sleela docs --generate [dir]`.
    if(!words.empty() && (words[0]=="--generate"||words[0]=="-g")){
        std::string dir = (words.size()>=2)? words[1] : corpusDir(ix);
        return generateCorpus(ix,dir);
    }
    // Prefer a pre-generated corpus unless the caller forces live parsing with
    // `--live`. When forced, drop the flag and continue.
    bool forceLive=false;
    if(!words.empty() && words[0]=="--live"){ forceLive=true; words.erase(words.begin()); }
    std::string cdir=corpusDir(ix);
    bool haveCorpus = !forceLive && fs::is_directory(cdir);

    if(words.empty()){ listAll(o,ix); return 0; }

    // Two-word form: <package> <Class>
    if(words.size()>=2){
        const std::string& pkg=words[0]; const std::string& cls=words[1];
        for(const auto& s:ix.symbols()) if(lower(s.package)==lower(pkg) && lower(s.name)==lower(cls)){
            if(haveCorpus && serveCached(o,cdir,s.package,s.name)) return 0;
            UnitDoc u=parseUnit(s,ix.root());
            if(u.classes.empty()){ o<<"(unit '"<<s.name<<"' declares no class)\n"; return 0; }
            for(const auto& c:u.classes) renderClass(o,u,c);
            return 0;
        }
        // fall through to single-topic resolution on the second word
    }

    const std::string topic=words.size()>=2?words[1]:words[0];
    std::string tL=lower(topic);

    // 1) exact package?
    if(ix.hasPackage(topic) || [&]{ for(const auto& s:ix.symbols()) if(lower(s.package)==tL) return true; return false; }()){
        std::string pkg; for(const auto& s:ix.symbols()) if(lower(s.package)==tL){ pkg=s.package; break; }
        if(pkg.empty()) pkg=topic;
        renderPackage(o,ix,pkg);
        return 0;
    }
    // 2) exact class (case-insensitive)? Collect EVERY match so a name shared
    //    across packages disambiguates rather than silently picking one. Prefer
    //    units that actually declare a class, and rank SL* standard-library
    //    units ahead of the large java compat mirror.
    std::vector<library::Symbol> exact;
    std::string slForm = "sl"+tL;   // standard-library classes are SL-prefixed
    for(const auto& s:ix.symbols()){ std::string nL=lower(s.name); if(nL==tL || nL==slForm) exact.push_back(s); }
    if(!exact.empty()){
        auto rank=[](const library::Symbol& s){
            int r=0;
            if(s.package=="java") r+=100;                         // compat mirror: deprioritise
            if(s.path.find("early_access")!=std::string::npos) r+=50;
            if(s.name.rfind("SL",0)==0) r-=10;                    // SL* standard library: prefer
            return r;
        };
        std::sort(exact.begin(),exact.end(),[&](const library::Symbol&a,const library::Symbol&b){return rank(a)<rank(b);});
        // Keep only matches that declare a class; if none do, fall back to all.
        std::vector<std::pair<library::Symbol,UnitDoc>> good;
        for(const auto& s:exact){ UnitDoc u=parseUnit(s,ix.root()); if(!u.classes.empty()) good.push_back({s,u}); }
        if(good.empty()){ o<<"(no class declaration found for '"<<topic<<"')\n\n"; suggestNear(o,ix,topic); return 0; }
        if(good.size()>1){
            o<<"'"<<topic<<"' names "<<good.size()<<" units across packages. Pick one:\n\n";
            for(const auto& g:good) o<<"  sleela docs "<<g.first.package<<" "<<g.first.name<<"\n";
            o<<"\nShowing the best standard-library match ("<<good.front().first.package<<"):\n\n";
        }
        const auto& g=good.front();
        if(haveCorpus && serveCached(o,cdir,g.first.package,g.first.name)) return 0;
        for(const auto& c:g.second.classes) renderClass(o,g.second,c);
        return 0;
    }
    // 3) near results
    suggestNear(o,ix,topic);
    return 0;
}

} // namespace sleela
