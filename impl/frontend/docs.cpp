// docs.cpp -- `sleela docs` subcommand implementation. See docs.h.
// ===========================================================================
#include "docs.h"
#include "library_index.h"
#include <algorithm>
#include <cctype>
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
struct Member { std::string kind, name, sig, desc; };
struct ClassDoc {
    std::string kind, name, extends, summary;
    std::vector<Member> fields, methods;
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
            u.classes.push_back(ClassDoc{cm[1].str(),cm[2].str(),cm[3].matched?cm[3].str():"", "", {}, {}, false});
            cur=&u.classes.back();
            cur->summary=gatherAbove(lines, idx-1);
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
            else cur->fields.push_back(Member{"field",nm,squeeze(s.substr(0,s.size()-1)),gatherAbove(lines,idx-1)});
        } else if(atClassBody && std::regex_search(s,mm,methRe)){
            std::string nm=mm[2].str();
            if(isMetaName(nm)){ cur->hasMetadata=true; }
            else { std::string sig=s; size_t br=sig.find('{'); if(br!=std::string::npos) sig=trim(sig.substr(0,br));
                   cur->methods.push_back(Member{"method",nm,squeeze(sig),gatherAbove(lines,idx-1)}); }
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
    if(!u.definition.empty()){ o<<"\nDEFINITION\n  "<<u.definition<<"\n"; }
    if(!c.summary.empty()){ o<<"\nDESIGN PURPOSE\n  "<<c.summary<<"\n"; }

    // RELATIONSHIPS: what it builds on, and (shallow) what types it names.
    o<<"\nRELATIONSHIPS\n";
    if(!c.extends.empty()) o<<"  * builds on: "<<c.extends<<"\n";
    o<<"  * fields: "<<c.fields.size()<<"   methods: "<<c.methods.size()<<"\n";
    if(c.hasMetadata) o<<"  * carries the standard Metadata Class record (provenance + fiduciary attributes)\n";

    // STRUCTURE: the members, each with its harvested explanation.
    if(!c.fields.empty()){
        o<<"\nSTRUCTURE -- state\n";
        for(const auto& f:c.fields){ o<<"  "<<f.sig<<"\n"; if(!f.desc.empty()) o<<"      "<<f.desc<<"\n"; }
    }
    if(!c.methods.empty()){
        o<<"\nSTRUCTURE -- behaviour\n";
        for(const auto& mth:c.methods){ o<<"  "<<mth.sig<<"\n"; if(!mth.desc.empty()) o<<"      "<<mth.desc<<"\n"; }
    }

    // USER CLAYS: the usage habits a reader brings, and the cues that make a
    // present audience read more carefully. Derived, not invented: it names the
    // assumption a reader trained on other systems will carry in, and points at
    // the careful cue (the class's own stated purpose / metadata).
    o<<"\nUSER CLAYS\n";
    o<<"  * Assumed Be: a reader trained on other peoples' systems will reach for\n"
       "    "<<c.name<<" expecting the familiar shape of its name; read the DESIGN\n"
       "    PURPOSE before assuming behaviour by analogy.\n";
    o<<"  * Careful cue: "<<(c.summary.empty()? std::string("consult the source header and the member explanations above")
                                              : squeeze(c.summary).substr(0,std::min<size_t>(c.summary.size(),160)))<<"\n";
    o<<"  * Audience: INT (the attentive reader) studies the RELATIONSHIPS;\n"
       "    Swarmy (the habitual reader) should slow at the Careful cue.\n";

    // MASTERY LEVELS: a graduated study ladder over this class.
    o<<"\nMASTERY LEVELS\n";
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
    o<<"\nRead one in full with:  sleela docs "<<pkg<<" <ClassName>\n"
      <<"                   or:  sleela docs <ClassName>\n\n";
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
