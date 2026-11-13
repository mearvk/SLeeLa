#include <algorithm>
#include "semantic.h"
#include "unsigned_integer.h"
#include "wide_integer.h"
#include "annotation_pipeline.h"
#include <map>
#include "java_references.h"
#include <set>
#include <string>
#include <functional>
#include <vector>
#include <utility>
namespace sleela {
namespace {
enum class Kind { Void, Int, Unsigned, Double, Bool, String, Null, Any, Struct, Array, Unknown, Error };
// For Kind::Array, `name` carries the full array type text (e.g. "int[]") and
// `elem` the element type name (e.g. "int").
struct Type { Kind kind=Kind::Unknown; std::string name{}; std::string elem{}; };
// Unsigned (positive-only) type names: the arbitrary-width U<width> family and
// the base-2 uint<width> series (uint32, uint64, uint128, ... uint16777216,
// syntax 2.8). Both carry Kind::Unsigned and the same positive-only, width-
// strict discipline; they are distinguished only by their (different) name
// strings, so a `U256` and a `uint256` are separate unsigned types.
static bool unsignedTypeName(const std::string& n){return UnsignedInteger::isValidTypeName(n)||UnsignedWideInteger::isValidTypeName(n);}
// The base-2 signed-integer series (syntax 2.7): int32, int64, int128, int256,
// ... up to int16777216 (powers of two, 2^5..2^24). Recognized as identifiers
// like the U<width> unsigned family; type-checks as an integer (see typeOf).
// int64/int128 are also keyword-spelled but resolve to the same integer kind.
static bool signedWideTypeName(const std::string& n){return SignedWideInteger::isValidTypeName(n);}
static Type typeOf(const std::string& n){
 // Accept the Java-style keyword names the lexer produces (int, double,
 // boolean, String, void) as well as the lowercase aliases (bool, string).
 // A trailing "[]" (one or more) denotes an array of the inner type.
 if(n.size()>=2 && n.compare(n.size()-2,2,"[]")==0){
  Type t; t.kind=Kind::Array; t.name=n; t.elem=n.substr(0,n.size()-2); return t;
 }
 // Generic arguments are signature metadata in syntax 2.2+; the VM type is
 // the raw reference type, so Class<?> has the same runtime type as Class.
 const std::size_t generic = n.find('<');
 if(generic != std::string::npos && n.back() == '>')
  return{Kind::Struct,n.substr(0,generic),{}};
 if(unsignedTypeName(n)){return{Kind::Unsigned,n,{}};}
 if(n=="void"){return{Kind::Void,{},{}};}
 if(n=="int"){return{Kind::Int,{},{}};}
 // Base-2 signed series int32..int16777216 (syntax 2.7) type-checks as the
 // integer kind -- identical checking to `int`. The runtime backs widths <=64
 // on the 64-bit slot and widths >=128 on the 128-bit slot (see compiler.cpp
 // runtimeTypeOperand); arithmetic beyond 128 bits is a documented follow-up.
 if(signedWideTypeName(n)){return{Kind::Int,{},{}};}
 // Wide signed integer primitives (syntax 2.6). long and int64 are true 64-bit
 // aliases of int (the runtime SLValue already stores int as int64_t). int128
 // is accepted and type-checks as an integer; its runtime backing is currently
 // 64-bit (true 128-bit arithmetic is a documented follow-up).
 if(n=="long"||n=="int64"||n=="int128"){return{Kind::Int,{},{}};}
 if(n=="double"){return{Kind::Double,{},{}};}
 // Java primitives without a distinct VM slot (syntax 3.0 Java signature
 // surface): char, byte and short ride the integer slot, float the double slot.
 if(n=="char"){return{Kind::Int,"char",{}};}   // syntax 3.2: the name marks the static type char (an integer for every check)
 if(n=="byte"||n=="short"){return{Kind::Int,{},{}};}
 if(n=="float"){return{Kind::Double,{},{}};}
 if(n=="bool"||n=="boolean"){return{Kind::Bool,{},{}};}
 if(n=="string"||n=="String"){return{Kind::String,{},{}};}
 // The Java UTF-16 String kind, a String at type-check time (instanceof utf16).
 if(n=="utf16"){return{Kind::String,{},{}};}
  if(n=="Any"){return{Kind::Any,"Any",{}};}
 return{Kind::Struct,n,{}};
}
static std::string nameOf(const Type&t){
 switch(t.kind){case Kind::Void:return"void";case Kind::Int:return"int";case Kind::Unsigned:return t.name;case Kind::Double:return"double";case Kind::Bool:return"bool";case Kind::String:return"string";case Kind::Null:return"null";case Kind::Any:return"Any";case Kind::Struct:return t.name;case Kind::Array:return t.name.empty()?"array":t.name;case Kind::Unknown:return"unknown";default:return"error";}
}
static bool same(const Type&a,const Type&b){return a.kind==b.kind&&((a.kind!=Kind::Struct&&a.kind!=Kind::Array&&a.kind!=Kind::Unsigned)||a.name==b.name);}
static bool numeric(const Type&t){return t.kind==Kind::Int||t.kind==Kind::Double||t.kind==Kind::Unsigned;}
// Loaded classes for the Java subtype rule (SLEELA.syntax 43.6); set by
// Analyzer::run for the duration of one analysis (syntax 1.10+), else null.
static const javaref::ClassIndex* g_javaClasses=nullptr;
static bool assignable(const Type&to,const Type&from){
 if(to.kind==Kind::Struct&&to.name=="Object") return true;
 // Java widening reference conversion: a subclass / implementing class is
 // assignable to its superclass or interface.
 if(g_javaClasses&&to.kind==Kind::Struct&&from.kind==Kind::Struct&&to.name!=from.name
    &&javaref::isSubtypeOf(*g_javaClasses,from.name,to.name)) return true;
 return to.kind==Kind::Unknown||from.kind==Kind::Unknown||to.kind==Kind::Any||(to.kind==Kind::Double&&from.kind==Kind::Int)||(from.kind==Kind::Null&&(to.kind==Kind::Any||to.kind==Kind::Struct||to.kind==Kind::String||to.kind==Kind::Array))||same(to,from);
}
static bool builtin(const std::string&n){
 static const std::set<std::string> s={
 "javaStringFromCodeUnits","javaStringFromUtf8","javaStringToUtf8","javaStringFromCodeUnit","javaStringLength","javaStringCharAt","javaStringCodePointAt","javaStringCodePointCount","javaStringOffsetByCodePoints","javaStringSubstring","javaStringConcat","javaStringHashCode","javaStringCompareTo","spawn","join","lock","unlock","send","recv","structPack","structUnpack","listen","accept","connect","sockread","sockwrite","sockclose",
 "pipe","pipePeer","fifoCreate","npfsCreate","openFile","read","write","close","unlinkFile",
 "timeUtcMillis","timeUtcNanos","timeMonotonicNanos","timePrecisionMillis","timeLocation","timeHttpDate","timeJson","timeNtp","timeSetLocation","sleep",
 "Munction.start","synchroOpen","synchroDispatch","synchroReport","synchroClose","synchroSent","synchroReceived","synchroMean","synchroMin","synchroMax","synchroP95","synchroLoss",
 "bestOfNew","bestOfWeight","bestOfMinVersion","bestOfCostBudget","bestOfCandidate","bestOfRecord","bestOfScore","bestOfBest","bestOfChoice","bestOfReport","bestOfClose",
 "bestOfMean","bestOfLoss","bestOfJitter","bestOfCertainty","bestOfCandidateArch","bestOfArchRealized","bestOfArch","bestOfArchParam","bestOfArchState",
 "audioNew","audioAdd","audioControls","audioValidate","audioRender","audioClose","audioPlatform",
 "arrayNew","arrayLength","arrayGet","arraySet","arrayPush",
 "osPlatform","osCapability","osGetEnv","osSetEnv","osCurrentDir","osChangeDir","osHostName","osUserName","osTempDir","osProcessId",
 "osExists","osIsDir","osFileSize","osMakeDir","osRemove","osRename","osRun","osSpawn","osWait","osKill","osProcessClose",
 "oslsSystemInfo","oslsProcessInfo","oslsEnumerateProcesses","oslsGetEnvironmentValue","oslsEnumerateEnvironment","oslsGetFileInfo","oslsListDirectory","oslsGetCPUInfo","oslsGetTimezoneInfo","oslsGetNetworkInterfaces","oslsCronValidate","oslsCronNextAfter",
 "oslsGetMemoryInfo","oslsGetAvailableMemory","oslsGetDiskSpace","oslsGetVolumeInfo","oslsGetSystemLoad","oslsGetCapabilities","oslsGetRuntimeInfo","oslsGetNativeError","oslsPathNormalize","oslsPathJoin","oslsPathResolve","oslsGetPathInfo","oslsNetworkResolve",
 "conduct","role","insight","congruent","route","sysdepth","degreemax",
 "ideaCanvasCreate","ideaCanvasDestroy","ideaCanvasCols","ideaCanvasRows","ideaCanvasCount",
 "ideaCanvasSetParams","ideaCanvasExcite","ideaCanvasStep","ideaCanvasAdvance","ideaCanvasTimestep",
 "ideaCanvasIntensityAt","ideaCanvasHasRenderer","ideaCanvasOpenWindow","ideaCanvasShouldClose",
 "ideaCanvasPresent","ideaCanvasRun",
 "ideaCanvasSetBackendPolicy","ideaCanvasPolicy","ideaCanvasActiveBackend","ideaCanvasGpuAvailable","ideaCanvasMeasuredHz","ideaCanvasMigrate",
 "bigmul","bigadd","bigdivmod","bigsub","bigdiv","bigisqrt",
 "nativeOpen","nativeCall","nativeClose"};
 if(s.count(n)!=0) return true;
 // SleelaUI (ui*) bridge built-ins: any ui<Capital>... name is a native UI
 // call lowered by the compiler to OP_UI_CALL (see sleela_ui_ids.h / the
 // tryEmitUiBuiltin table). Accept the whole family here so the analyzer does
 // not reject a valid toolkit call as an unknown method.
 if(n.size()>2 && n[0]=='u' && n[1]=='i' && n[2]>='A' && n[2]<='Z') return true;
 return false;
}
struct MethodSig{const Method* m=nullptr;std::string owner;};
class Analyzer{
 const Program&p; const SyntaxVersion&syntax; const bool requireMain; SemanticResult r;
 std::set<std::string> structs; std::set<std::string> classNames; std::map<std::string,std::map<std::string,Type>> fields;
 std::map<std::string,MethodSig> methods; std::map<std::string,std::vector<MethodSig>> overloads; std::map<std::string,Type> globals; std::map<std::string,std::string> owners;
 std::map<std::string,std::string> superOf; // derived class -> resolved base (syntax 1.10)
 struct Scope{std::map<std::string,Type> vars;}; std::vector<Scope> scopes; const Method* cur=nullptr; std::string cls;
 void err(const std::string&s){r.errors.push_back("Semantic error: "+s);}
 // A type is known if it is a scalar keyword (Java or lowercase alias), a
 // declared struct, or a declared class/interface name (so a constructor's
 // return type and object-typed parameters resolve).
 bool known(const std::string&t)const{
  // An array type "T[]" is known when its element type T is known. Nested
  // arrays ("T[][]") recurse. Element type must not be void.
  if(t.size()>=2 && t.compare(t.size()-2,2,"[]")==0){std::string inner=t.substr(0,t.size()-2);return inner!="void"&&known(inner);}
  const std::size_t generic = t.find('<');
  if(generic != std::string::npos){
   if(syntax<SyntaxVersion{2,2} || t.empty() || t.back()!='>') return false;
   return known(t.substr(0,generic));
  }
  if(t=="char"||t=="byte"||t=="short"||t=="float") return true;
  if(t=="utf16") return true;
  return t=="void"||t=="int"||t=="long"||t=="int64"||t=="int128"||unsignedTypeName(t)||signedWideTypeName(t)||t=="double"||t=="bool"||t=="boolean"||t=="string"||t=="String"||t=="Any"||structs.count(t)||classNames.count(t);
 }
 Type tn(const std::string&t)const{return known(t)?typeOf(t):Type{Kind::Error,t};}
 void collect(){
  for(const auto&s:p.structs)if(!structs.insert(s.name).second)err("duplicate struct '"+s.name+"'");
  for(const auto&s:p.structs){std::map<std::string,Type> f;if(s.fields.size()>64)err("struct '"+s.name+"' declares "+std::to_string(s.fields.size())+" fields, but the VM admits at most 64 fields per struct/class layout. Reduce the field count (for example, group related fields into an array or a nested struct).");
   for(const auto&x:s.fields){if(f.count(x.name))err("duplicate field '"+x.name+"' in struct '"+s.name+"'");
    Type t=tn(x.type);if(t.kind==Kind::Error)err("unknown type '"+x.type+"' for field '"+x.name+"'");if(t.kind==Kind::Void)err("field '"+x.name+"' cannot be void");f[x.name]=t;}fields[s.name]=f;}
  for(const auto&c:p.classes)classNames.insert(c.name);
  // Nested classes (syntax 1.11): the parser hoists each lexically nested class
  // into p.classes as "Outer__Inner" (flagged isNested) and records the count.
  // Gate the feature on the declared syntax version and re-assert the
  // per-document cap of 42 (the parser's rule-of-two already bounded depth).
  {
   bool anyNested=false; for(const auto&c:p.classes) if(c.isNested){anyNested=true;break;}
   if((anyNested||p.nestedClassCount>0)&&syntax<SyntaxVersion{1,11})
    err("nested classes require #sleela 1.11");
   if(p.nestedClassCount>42)
    err("a single Sleela document may contain at most 42 nested classes");
  }
  // Author marker (syntax 2.4): the bare `author`/`authorial` document word
  // requires #sleela 2.4. When set it relaxes the duplicate-member rejections
  // below (each duplicate keeps a distinct declaration-order id + SHA-256
  // brander, emitted into the originated source document). Below 2.4 the words
  // never parse as a marker, so this only fires on a genuine 2.4-feature use in
  // an older-declared document.
  if(p.authorMarked&&syntax<SyntaxVersion{2,4})
   err("the author/authorial document marker requires #sleela 2.4");
  // Material marker (syntax 2.5): declarative only. Below 2.5 the word never
  // parses as a marker, so this fires only on a genuine 2.5-feature use in an
  // older-declared document. There is deliberately no behavioral effect.
  if(p.materialMarked&&syntax<SyntaxVersion{2,5})
   err("the material document marker requires #sleela 2.5");
  // Memory marker (syntax 2.5): declarative only. Below 2.5 the word never
  // parses as a marker, so this fires only on a genuine 2.5-feature use in an
  // older-declared document. There is deliberately no behavioral effect.
  if(p.memoryMarked&&syntax<SyntaxVersion{2,5})
   err("the memory document marker requires #sleela 2.5");
  // Determine marker (syntax 2.6): declarative only. Below 2.6 the word never
  // parses as a marker, so this fires only on a genuine 2.6-feature use in an
  // older-declared document. There is deliberately no behavioral effect.
  if(p.determineMarked&&syntax<SyntaxVersion{2,6})
   err("the determine document marker requires #sleela 2.6");
  // Object marker (syntax 2.6): declarative only; gate-check only, no effect.
  if(p.objectMarked&&syntax<SyntaxVersion{2,6})
   err("the object document marker requires #sleela 2.6");
  // Field marker (syntax 2.6): declarative only; gate-check only, no effect.
  if(p.fieldMarked&&syntax<SyntaxVersion{2,6})
   err("the field document marker requires #sleela 2.6");
  // Pure marker (syntax 3.0): declarative only. Below 3.0 the word never parses
  // as a marker, so this fires only on a genuine 3.0-feature use in an older-
  // declared document. There is deliberately no behavioral effect.
  if(p.pureMarked&&syntax<SyntaxVersion{3,0})
   err("the pure document marker requires #sleela 3.0");
  // Gasoline marker (syntax 3.0): declarative only; near-synonym of pure.
  if(p.gasolineMarked&&syntax<SyntaxVersion{3,0})
   err("the gasoline document marker requires #sleela 3.0");
  // Artifact marker (syntax 3.0): declarative only; declares the document an
  // enacted, legislature-grade artifact graded unto time and space.
  if(p.artifactMarked&&syntax<SyntaxVersion{3,0})
   err("the artifact document marker requires #sleela 3.0");
  // Syntax 3.1 surface (section 44): every use the parser met must be declared.
  if(syntax<SyntaxVersion{3,1}){
   for(const auto&u:p.syntax31Uses)err(u+" requires #sleela 3.1");
  }
  // Syntax 3.2 surface (sections 45 and 46).
  if(syntax<SyntaxVersion{3,2}){
   for(const auto&u:p.syntax32Uses)err(u+" requires #sleela 3.2");
  }
  // Spark marker (syntax 3.0, section 43): indicates Java style declarative.
  if(javaref::sparkDocument(p)&&syntax<SyntaxVersion{3,0})
   err("the spark document marker requires #sleela 3.0");
  // @mix consequent fields (syntax 2.9): a field annotated `@mix` may repeat
  // its name; the group is understood as consequent (array/list). Top-down
  // order gives indexes 1,2,3,... unless `@mix(1,2,3,...)` assigns them, in
  // which case the index count must match the number of same-named @mix fields.
  const bool mixEnabled = !(syntax<SyntaxVersion{2,9});
  auto fieldHasMix=[](const Field&f,const annotation::Annotation**out)->bool{
   for(const auto&a:f.annotations){
    if(a.name=="mix"){ if(out){*out=&a;} return true; }
   }
   if(out){*out=nullptr;}
   return false;};
  // Count comma-separated index tokens in an @mix(...) value (parser joins
  // tokens with spaces, e.g. "1 , 2 , 3"); a bare marker @mix has no value.
  auto mixIndexCount=[](const std::string&v)->int{
   if(v.empty()){return 0;}
   int n=1;
   for(char ch:v){ if(ch==','){++n;} }
   return n;};
  // Java API declarations may overload a method name (for example,
  // java.lang.Object.wait). Reject duplicate parameter signatures, not
  // distinct overloads. Expression lookup remains name-keyed in this release.
  std::map<std::string,std::vector<const Method*>> methodDeclarations;
  for(const auto&c:p.classes){
   std::map<std::string,Type> instanceFields;
   // First, for 2.9, tally @mix groups (name -> count) and remember any manual
   // index annotation seen on that name, so we can validate the counts match.
   std::map<std::string,int> mixGroupCount; std::map<std::string,int> mixManualCount; std::map<std::string,bool> mixManualSeen;
   if(mixEnabled){for(const auto&f:c.fields){
    if(f.isStatic){continue;}
    const annotation::Annotation*ma=nullptr;
    if(fieldHasMix(f,&ma)){
     ++mixGroupCount[f.name];
     if(ma&&!ma->value.empty()){
      mixManualSeen[f.name]=true;
      int cnt=mixIndexCount(ma->value);
      if(mixManualCount.count(f.name)&&mixManualCount[f.name]!=cnt){ err("@mix index lists for field '"+f.name+"' in class '"+c.name+"' disagree on count"); }
      mixManualCount[f.name]=cnt;
     }
    }
   }}
   if(mixEnabled){for(const auto&kv:mixManualSeen){
    if(kv.second){
     const std::string&nm=kv.first;
     if(mixManualCount[nm]!=mixGroupCount[nm]){ err("@mix index count ("+std::to_string(mixManualCount[nm])+") does not match the number of '"+nm+"' fields ("+std::to_string(mixGroupCount[nm])+") in class '"+c.name+"'"); }
    }
   }}
   for(const auto&f:c.fields){Type t=tn(f.type);if(t.kind==Kind::Error)err("unknown type '"+f.type+"' for field '"+f.name+"'");if(t.kind==Kind::Void)err("field '"+f.name+"' cannot be void");
    const bool isMix = mixEnabled && fieldHasMix(f,nullptr);
    if(f.isStatic){const std::string key=c.name+"::"+f.name;if(globals.count(key)&&!p.authorMarked)err("duplicate static field '"+key+"'");globals[key]=t;owners[key]=c.name;}
    else {if(instanceFields.count(f.name)&&!p.authorMarked&&!isMix)err("duplicate field '"+f.name+"' in class '"+c.name+"'");instanceFields[f.name]=t;}}
   // The VM admits at most 64 named fields per class/struct layout (the struct
   // path above enforces the same cap). Catch an over-wide class HERE, at
   // semantic analysis, so `sleela check` reports it exactly as `run`/`compile`
   // do -- rather than passing check and only failing later at VM layout
   // registration with a message that looks like a layout-COUNT overflow.
   // (Inherited-field overflow from `extends` is still caught downstream when
   // the full base+derived layout is registered.)
   if(instanceFields.size()>64)err("class '"+c.name+"' has "+std::to_string(instanceFields.size())+" instance fields, but the VM admits at most 64 fields per class/struct layout. Reduce the field count -- for example, pack a group of related fields into a single array field reached through accessor methods (methods cost no layout slot), or split the class.");
   fields[c.name]=instanceFields;
   for(const auto&m:c.methods){const std::string key=c.name+"::"+m.name;
    bool duplicateSignature=false;
    for(const Method* prior:methodDeclarations[key]){
     if(prior->params.size()!=m.params.size()) continue;
     bool sameParameters=true;
     for(std::size_t i=0;i<m.params.size();++i)
      if(prior->params[i].type!=m.params[i].type){sameParameters=false;break;}
     if(sameParameters){duplicateSignature=true;break;}
    }
    if(duplicateSignature&&!p.authorMarked)err("duplicate method '"+key+"'");
    methodDeclarations[key].push_back(&m);
    if(!known(m.retType))err("unknown return type '"+m.retType+"' for method '"+m.name+"'");
    if(m.isProtected&&!m.isStatic)err("protected method '"+m.name+"' must also be static");
    methods[key]={&m,c.name};overloads[key].push_back({&m,c.name});if(m.name=="main")methods["main"]={&m,c.name};}
  }
  // --- OOD inheritance (syntax 1.10): resolve `class D extends B`. ----------
  // A derived class inherits the base class's instance fields and methods.
  // A same-name method in the derived class overrides the base's. Resolution
  // is version-gated so that <=1.9 documents keep their historical behavior
  // (extends was retained metadata, not resolved). This runs after every
  // class's own members are collected, and walks each extends chain base-first.
  if(!(syntax<SyntaxVersion{1,10})){
   // index classes by name for superclass lookup
   std::map<std::string,const ClassDecl*> byName;
   for(const auto&c:p.classes) byName[c.name]=&c;
   // Java superclass references resolve through java_references.cpp (shared with
   // the compiler so check and run agree).
   auto resolveBaseName=[&](const std::string& declared)->std::string{ return javaref::resolveBaseName(byName,declared); };
   // validate: known base, no cycles
   for(const auto&c:p.classes){
    const std::string sup=resolveBaseName(c.java.superclass);
    if(sup.empty()) continue;
    if(!byName.count(sup)){ if(classNames.count(sup)==0 && structs.count(sup)==0) err("class '"+c.name+"' extends unknown type '"+c.java.superclass+"'"); continue; }
    // cycle detection: walk the extends chain from c; if we return to a class
    // already on the path, the inheritance is cyclic.
    { std::set<std::string> seen; std::string cur2=c.name;
      while(byName.count(cur2)){ seen.insert(cur2); const std::string s2=resolveBaseName(byName[cur2]->java.superclass);
        if(s2.empty()) break; if(seen.count(s2)){ err("cyclic inheritance involving class '"+c.name+"'"); break; } cur2=s2; } }
   }
   // merge base members into each derived class, resolving the chain base-first
   std::set<std::string> done; std::set<std::string> inProgress;
   std::function<void(const std::string&)> resolve=[&](const std::string& name){
    if(done.count(name)||!byName.count(name)) return;
    if(!inProgress.insert(name).second) return; // cycle guard (reported above)
    const ClassDecl* c=byName[name];
    const std::string sup=resolveBaseName(c->java.superclass);
    if(!sup.empty()&&byName.count(sup)&&!inProgress.count(sup)){
     resolve(sup); // ensure base is fully resolved first
     // inherit instance fields the derived class does not redeclare (the base's
     // field set is already fully resolved, so this carries grandparent fields).
     auto bf=fields.find(sup); if(bf!=fields.end()){ auto&df=fields[name]; for(const auto&kv:bf->second) if(!df.count(kv.first)) df[kv.first]=kv.second; }
     // inherit methods the derived class does not override. Walk the base's
     // RESOLVED method set (its own + what it inherited), so multi-level chains
     // carry grandparent methods down. A same-name derived method overrides.
     const std::string pfx=sup+"::";
     std::vector<std::pair<std::string,MethodSig>> toAdd;
     for(const auto&kv:methods){ const std::string&bk=kv.first; if(bk.rfind(pfx,0)!=0) continue; std::string mname=bk.substr(pfx.size()); if(mname=="main") continue; const std::string dk=name+"::"+mname; if(!methods.count(dk)) toAdd.push_back({dk,kv.second}); }
     for(auto&e:toAdd){ methods[e.first]=e.second; auto ob=overloads.find(sup+e.first.substr(e.first.find("::"))); if(ob!=overloads.end()&&!overloads.count(e.first)) overloads[e.first]=ob->second; }
     superOf[name]=sup;
    }
    inProgress.erase(name);
    done.insert(name);
   };
   for(const auto&c:p.classes) resolve(c.name);
  }
  // --- Document extension (syntax 1.10): `extends to ... <grouper>;` --------
  for(const auto&de:p.documentExtensions){
   if(syntax<SyntaxVersion{1,10}){err("document extension 'extends to ...' requires syntax 1.10");break;}
   if(de.targets.empty()){err("document extension names no target document");continue;}
   if(de.grouper==Grouper::Linear){
    // linear reals: a congruent-linear chain, bounded by the catalog's
    // congruent-linear-systems-max (3024). The chain degree is targets+self.
    if((long)de.targets.size()+1>3024) err("linear document extension exceeds the congruent-linear maximum (3024)");
   } else {
    // grouped structures are bounded by the complexity-degree-max (4): a
    // grouper may organize up to 4 member documents in one degree.
    if(de.targets.size()>4) err("grouped document extension exceeds the complexity-degree maximum (4 members per group)");
    if(de.grouper==Grouper::Services&&de.server.empty()) err("the 'services' grouper requires a Server-of-Services document name");
   }
  }
 }
 void push(){scopes.push_back({});} void pop(){scopes.pop_back();}
 std::vector<std::string> labels_;   // syntax 3.1: labels enclosing the statement being checked
 bool declare(const std::string&n,const Type&t){auto&v=scopes.back().vars;if(v.count(n)){err("duplicate local variable '"+n+"'");return false;}v[n]=t;return true;}
 Type lookup(const std::string&n)const{for(auto i=scopes.rbegin();i!=scopes.rend();++i){auto x=i->vars.find(n);if(x!=i->vars.end())return x->second;}auto cf=fields.find(cls);if(cf!=fields.end()){auto x=cf->second.find(n);if(x!=cf->second.end())return x->second;}auto g=globals.find(cls+"::"+n);if(g!=globals.end())return g->second;g=globals.find(n);if(g!=globals.end())return g->second;if(n=="next")return{Kind::Int,{}};return{Kind::Error,n};}
 void declarations(){
  if(requireMain&&!methods.count("main"))err("no 'main' method found");
  for(const auto&c:p.classes){for(const auto&f:c.fields){if(f.isProtected&&!f.isStatic)err("protected field '"+f.name+"' must also be static");}
   for(const auto&m:c.methods){std::set<std::string> seen;for(const auto&x:m.params){if(!known(x.type))err("unknown parameter type '"+x.type+"'");if(x.type=="void")err("parameter '"+x.name+"' cannot be void");if(!seen.insert(x.name).second)err("duplicate parameter '"+x.name+"' in method '"+m.name+"'");}}}
 }
 // A field initializer that constructs an object (`static Flag YES = new Flag(true);`)
 // is analyzed so constructor overload resolution picks the same constructor it
 // would in a method body; without this the compiler fell back to the first
 // constructor of matching arity.
 void fieldInit(const ClassDecl&owner,const Field&f){
  auto ne=dynamic_cast<const NewExpr*>(f.init.get());
  if(!ne||ne->args.empty()||(ne->typeName.size()>=2&&ne->typeName.compare(ne->typeName.size()-2,2,"[]")==0))return;
  cur=nullptr;cls=owner.name;scopes.clear();push();expr(*f.init);pop();
 }
 void method(const ClassDecl&owner,const Method&m){cur=&m;cls=owner.name;scopes.clear();push();if(!m.isStatic)declare("this",Type{Kind::Struct,owner.name,{}});for(const auto&x:m.params)declare(x.name,tn(x.type));block(*m.body);if(m.name=="main"&&m.retType!="void")err("main must return void");pop();}
 void block(const Block&b){for(const auto&s:b.stmts)stmt(*s);}
 void stmt(const Stmt&s){
  if(auto b=dynamic_cast<const Block*>(&s)){push();block(*b);pop();return;}
  if(auto d=dynamic_cast<const VarDecl*>(&s)){
   if(d->type=="let"){
    if(syntax<SyntaxVersion{1,8}){err("'let' inferred declarations require syntax 1.8");return;}
    if(!d->init){err("'let' declaration '"+d->name+"' requires an initializer");return;}
    Type inferred=expr(*d->init);
    if(inferred.kind==Kind::Unknown||inferred.kind==Kind::Error||inferred.kind==Kind::Null||inferred.kind==Kind::Void){
     err("cannot infer a usable type for 'let' declaration '"+d->name+"'");
     return;
    }
    declare(d->name,inferred);return;
   }
   Type t=tn(d->type);if(t.kind==Kind::Error){err("unknown variable type '"+d->type+"'");return;}if(t.kind==Kind::Void){err("variable '"+d->name+"' cannot be void");return;}if(d->init){Type g=expr(*d->init);if(!assignable(t,g))err("initializer for variable '"+d->name+"' has type "+nameOf(g)+", expected "+nameOf(t));}declare(d->name,t);return;
  }
  if(auto a=dynamic_cast<const Assign*>(&s)){Type t=lookup(a->name);if(t.kind==Kind::Error){err("assignment to undeclared variable '"+a->name+"'");return;}Type g=expr(*a->value);if(!assignable(t,g))err("cannot assign "+nameOf(g)+" to '"+a->name+"' of type "+nameOf(t));return;}
  if(auto f=dynamic_cast<const FieldAssign*>(&s)){if(isImplicitRef(*f->base,f->field)){err("'ref' is the implicit reference of an object and is read-only");return;}Type t=member(*f->base,f->field),g=expr(*f->value);if(t.kind!=Kind::Error&&!assignable(t,g))err("cannot assign "+nameOf(g)+" to field '"+f->field+"' of type "+nameOf(t));return;}
  if(auto gh=dynamic_cast<const GcHintStmt*>(&s)){
   // GC hint (syntax 1.9): `x = gc N;` / `x = mem N;`. The target must be an
   // in-scope value the developer is releasing, and the aggressiveness must be
   // within the 0..100 band. Both constraints are enforced rather than silently
   // narrowed, so a mistaken target or an out-of-range level is a clear error.
   if(syntax<SyntaxVersion{1,9}){err("GC hint 'x = "+gh->spelling+" N;' requires syntax 1.9");return;}
   Type t=lookup(gh->target);
   if(t.kind==Kind::Error){err("GC hint target '"+gh->target+"' is not an in-scope variable");return;}
   if(gh->aggressiveness<0||gh->aggressiveness>100){err("GC hint aggressiveness must be 0..100");return;}
   return;
  }
  if(auto e=dynamic_cast<const ExprStmt*>(&s)){expr(*e->expr);return;} if(auto p=dynamic_cast<const PrintStmt*>(&s)){Type t=expr(*p->expr);if(syntax>=SyntaxVersion{3,2}&&isCharType(t))p->charArg=true;if(t.kind==Kind::Void)err("print() cannot print void");return;}
  if(auto x=dynamic_cast<const ReturnStmt*>(&s)){Type want=tn(cur->retType);
   if(want.kind==Kind::Void){if(x->value){expr(*x->value);err("void method '"+cur->name+"' cannot return a value");}return;}
   if(!x->value){err("non-void method '"+cur->name+"' must return a value");return;}
   Type g=expr(*x->value);if(!assignable(want,g))err("method '"+cur->name+"' returns "+nameOf(g)+", expected "+nameOf(want));return;}
  if(auto i=dynamic_cast<const IfStmt*>(&s)){requireBool(expr(*i->cond),"if condition");stmt(*i->thenS);if(i->elseS)stmt(*i->elseS);return;}
  if(auto w=dynamic_cast<const WhileStmt*>(&s)){checkLoopSpill(w->spill);requireBool(expr(*w->cond),"while condition");stmt(*w->body);return;}
  if(auto f=dynamic_cast<const ForStmt*>(&s)){checkLoopSpill(f->spill);push();if(f->init)stmt(*f->init);if(f->cond)requireBool(expr(*f->cond),"for condition");if(f->threadStable)requireBool(expr(*f->threadStable),"for thread-stability guard");if(f->heapHealthy)requireBool(expr(*f->heapHealthy),"for VM heap-health guard");if(f->update)stmt(*f->update);stmt(*f->body);pop();return;}
  if(auto fe=dynamic_cast<const ForEachStmt*>(&s)){forEach(*fe);return;}
  if(auto d=dynamic_cast<const DoStmt*>(&s)){checkLoopSpill(d->spill);stmt(*d->body);requireBool(expr(*d->cond),"do-while condition");return;}
  if(auto b=dynamic_cast<const BreakStmt*>(&s)){if(!b->label.empty()&&std::find(labels_.begin(),labels_.end(),b->label)==labels_.end())err("break to undefined label '"+b->label+"'");return;}
  if(auto c=dynamic_cast<const ContinueStmt*>(&s)){if(!c->label.empty()&&std::find(labels_.begin(),labels_.end(),c->label)==labels_.end())err("continue to undefined label '"+c->label+"'");return;}
  if(auto ls=dynamic_cast<const LabeledStmt*>(&s)){
   if(std::find(labels_.begin(),labels_.end(),ls->label)!=labels_.end())err("label '"+ls->label+"' is already in use");
   labels_.push_back(ls->label);stmt(*ls->body);labels_.pop_back();return;}
  if(auto t=dynamic_cast<const ThrowStmt*>(&s)){expr(*t->value);return;}
  if(auto a=dynamic_cast<const AssertStmt*>(&s)){requireBool(expr(*a->cond),"assert condition");if(a->message)expr(*a->message);return;}
  if(auto y=dynamic_cast<const YieldStmt*>(&s)){expr(*y->value);return;}
  if(auto sy=dynamic_cast<const SynchronizedStmt*>(&s)){expr(*sy->monitor);push();block(*sy->body);pop();return;}
  if(auto tr=dynamic_cast<const TryStmt*>(&s)){push();block(*tr->body);pop();for(const auto&c:tr->catches){push();
   // Typed / multi-catch (43.6): each alternative must name a loaded class
   // (or Any); a multi-catch variable is typed by its single alternative only.
   Type ct{Kind::Unknown,{}};
   if(c.types.size()==1){ const std::string t=g_javaClasses?javaref::resolveBaseName(*g_javaClasses,c.types[0]):c.types[0]; ct=t=="Any"?Type{Kind::Any,"Any",{}}:tn(t); }
   if(!(syntax<SyntaxVersion{3,0})){ bool shadows=false; for(const auto& sc:scopes) if(sc.vars.count(c.variable)) shadows=true; if(shadows) err("catch variable '"+c.variable+"' shadows a local variable in scope"); }
   if(!(syntax<SyntaxVersion{3,0})) for(const auto& a:c.types){ const std::string t=g_javaClasses?javaref::resolveBaseName(*g_javaClasses,a):a; if(t!="Any"&&!classNames.count(t)) err("catch type '"+a+"' is not a loaded class"); }
   declare(c.variable,ct.kind==Kind::Error?Type{Kind::Unknown,{}}:ct);block(*c.body);pop();}if(tr->finallyBlock){push();block(*tr->finallyBlock);pop();}return;}
  if(auto sw=dynamic_cast<const SwitchStmt*>(&s)){expr(*sw->selector);push();for(const auto&c:sw->cases){for(const auto&l:c.labels)expr(*l);for(const auto&st:c.statements)stmt(*st);}pop();return;}
  err("unknown statement kind");
 }
 void requireBool(const Type&t,const char*w){if(t.kind!=Kind::Bool&&t.kind!=Kind::Unknown)err(std::string(w)+" must be bool, got "+nameOf(t));}
 // Validate a loop's @spill annotation. The parser already enforces the surface
 // grammar (a single @spill, at most one judges and one witnesses subject, a
 // non-negative pause). This is the semantic backstop: the pause must not be
 // negative, and a spill may name at most two subjects -- the judges and the
 // witnesses of the loop's performs.
 void checkLoopSpill(const LoopSpill& spill){
  if(!spill.hasSpill)return;
  if(spill.spillPauseMs<0)err("@spill pause must be non-negative");
  int subjects=(spill.spillJudges.empty()?0:1)+(spill.spillWitnesses.empty()?0:1);
  if(subjects>2)err("@spill may name at most two subjects (judges and witnesses)");
 }
 // Syntax 1.8 constructor invocation `new T(args...)`. Supported: a single
 // declared constructor whose arity matches and whose parameter types accept
 // the supplied arguments. NOT supported in 1.8: overload resolution (more than
 // one constructor) and this()/base delegation -- those are rejected explicitly
 // rather than silently narrowing behavior. Argument subexpressions are always
 // analyzed (even on the error paths) so nested type errors are still reported.
 void checkConstructorArgs(const NewExpr&n){
  if(n.args.empty()&&syntax<SyntaxVersion{1,8}) return;
  if(syntax<SyntaxVersion{1,8}){for(const auto&a:n.args)expr(*a);err("constructor arguments for '"+n.typeName+"' require syntax 1.8");return;}
  std::vector<const Method*> ctors;
  for(const auto&c:p.classes) if(c.name==n.typeName)
   for(const auto&m:c.methods) if(m.java.constructor) ctors.push_back(&m);
  std::vector<Type> at=argTypesOf(n.args);
  if(n.args.empty()){ for(const Method* m:ctors) if(m->params.empty()){ n.resolved=m; break; } return; }
  if(ctors.empty()){err("class '"+n.typeName+"' declares no constructor to accept "+std::to_string(n.args.size())+" argument(s)");return;}
  // Constructor overload resolution (syntax 3.0): same rule as methods.
  const Method* ctor=nullptr; int bestScore=-1;
  for(const Method* m:ctors){
   if(m->params.size()!=n.args.size()) continue;
   int score=0; bool ok=true;
   for(size_t i=0;i<at.size();++i){Type w=tn(m->params[i].type); if(!assignable(w,at[i])){ok=false;break;} if(w.kind==at[i].kind&&(w.kind!=Kind::Struct||w.name==at[i].name)) score+=2; else if(w.kind!=Kind::Any) score+=1;}
   if(ok&&score>bestScore){ctor=m;bestScore=score;}
  }
  if(!ctor) for(const Method* m:ctors) if(m->params.size()==n.args.size()){ctor=m;break;}
  if(!ctor){err("class '"+n.typeName+"' declares no constructor to accept "+std::to_string(n.args.size())+" argument(s)");return;}
  if(ctors.size()>1&&syntax<SyntaxVersion{3,0}){err("class '"+n.typeName+"' declares multiple constructors; constructor overload resolution requires syntax 3.0");return;}
  n.resolved=ctor;
  for(size_t i=0;i<n.args.size();++i){Type w=tn(ctor->params[i].type);if(!assignable(w,at[i]))err("constructor argument "+std::to_string(i+1)+" to '"+n.typeName+"' has type "+nameOf(at[i])+", expected "+nameOf(w));}
 }
 // The `next` / `next.next` / `next.next.next.next` system-degree idiom (see
 // compiler emitVar/emitMember): `next` is Degree 1 and each `.next` step is a
 // bounded symbolic relation, not a struct field access. Recognize an all-`next`
 // chain so the analyzer treats it as an integer degree rather than a member.
 static bool isNextChain(const Expr&e){
  if(auto v=dynamic_cast<const VarExpr*>(&e))return v->name=="next";
  if(auto m=dynamic_cast<const MemberAccess*>(&e))return m->field=="next"&&isNextChain(*m->base);
  return false;
 }
 // True when `base.ref` names the implicit object reference: the base is a
 // class/struct value whose declared fields do not include a field named ref.
 // Evaluated quietly so the caller reports any diagnostics exactly once.
 bool isImplicitRef(const Expr&base,const std::string&f){
  if(f!="ref")return false;
  const size_t keep=r.errors.size();
  Type b=expr(base);
  r.errors.resize(keep);
  if(b.kind!=Kind::Struct)return false;
  auto s0=fields.find(b.name);
  return s0!=fields.end()&&!s0->second.count("ref");
 }
 const MemberAccess* curMA_=nullptr;   // the MemberAccess node whose member() call is in progress
 static bool isCharType(const Type&t){return t.kind==Kind::Int&&t.name=="char";}
 Type member(const Expr&e,const std::string&f){
  if(auto rv=dynamic_cast<const VarExpr*>(&e)){
   if(classNames.count(rv->name) && lookup(rv->name).kind==Kind::Error){
    auto g=globals.find(rv->name+"::"+f);
    if(g!=globals.end()) return g->second;
    err("class '"+rv->name+"' has no static field '"+f+"'");return{Kind::Error,{}};
   }
  }
  Type b=expr(e);if(b.kind==Kind::Unknown)return b;
  if(f=="length"&&b.kind==Kind::Array){   // syntax 3.2: a.length
   if(syntax<SyntaxVersion{3,2})err("the array length member requires #sleela 3.2 (use arrayLength(a) below 3.2)");
   if(curMA_)curMA_->arrayLength=true;
   return{Kind::Int,{},{}};
  }
  if(f=="ref"&&b.kind==Kind::Struct){auto s0=fields.find(b.name);if(s0!=fields.end()&&!s0->second.count("ref"))return{Kind::Int,{}};}if(b.kind!=Kind::Struct){err("member '"+f+"' requires a struct value, got "+nameOf(b));return{Kind::Error,{}};}auto s=fields.find(b.name);if(s==fields.end()){err("unknown struct '"+b.name+"'");return{Kind::Error,{}};}auto x=s->second.find(f);if(x==s->second.end()){err("struct '"+b.name+"' has no field '"+f+"'");return{Kind::Error,{}};}return x->second;}
 Type expr(const Expr&e){
  if(auto il=dynamic_cast<const IntLit*>(&e)){if(il->isChar)return{Kind::Int,"char",{}};return{Kind::Int,{}};}
  if(dynamic_cast<const DoubleLit*>(&e)){return{Kind::Double,{}};}
  if(dynamic_cast<const BoolLit*>(&e)){return{Kind::Bool,{}};}
  if(dynamic_cast<const StrLit*>(&e)){return{Kind::String,{}};}
  if(dynamic_cast<const NullLit*>(&e)){return{Kind::Null,{}};}
  if(auto rr=dynamic_cast<const RanRefExpr*>(&e)){
   // `ran::<word>` -- the System / already-Ran-in-RAM reference. Requires 1.9.
   // `ran::mem` is the current live system memory as an int. The GC words
   // `ran::gc` / `ran::mem N` are statement hints (handled in parseSimpleStatement),
   // not value expressions, so only the value names resolve here.
   if(syntax<SyntaxVersion{1,9}){err("the 'ran::' system namespace requires syntax 1.9");return{Kind::Error,{}};}
   if(rr->word=="mem"){return{Kind::Int,{}};}
   err("unknown system reference 'ran::"+rr->word+"' (known value: ran::mem)");
   return{Kind::Error,{}};
  }
  if(auto v=dynamic_cast<const VarExpr*>(&e)){Type t=lookup(v->name);if(t.kind==Kind::Error)err("use of undeclared variable '"+v->name+"'");return t;}
  if(auto n=dynamic_cast<const NewExpr*>(&e)){
   // `new T[n]` arrives as a NewExpr whose typeName ends in "[]" and whose
   // first arg is the length. `new T()` is a struct instantiation.
   if(n->typeName.size()>=2 && n->typeName.compare(n->typeName.size()-2,2,"[]")==0){
    if(!known(n->typeName)){err("new of unknown array type '"+n->typeName+"'");return{Kind::Error,{}};}
    if(n->hasInit){
     const std::string et=n->typeName.substr(0,n->typeName.size()-2);
     Type want=typeOf(et);
     if(known(et))for(const auto&a:n->init){Type g=expr(*a);if(!assignable(want,g))err("array initializer element has type "+nameOf(g)+", expected "+nameOf(want));}
     else err("array initializer element type '"+et+"' is not a known type");
     return typeOf(n->typeName);
    }
    for(const auto&a:n->args){Type s=expr(*a);if(s.kind!=Kind::Int&&s.kind!=Kind::Unknown)err("array size must be int, got "+nameOf(s));}
    return typeOf(n->typeName);
   }
   if(!structs.count(n->typeName)&&!classNames.count(n->typeName)){err("new of unknown class or struct '"+n->typeName+"'");return{Kind::Error,{}};}
   checkConstructorArgs(*n);
   return{Kind::Struct,n->typeName};}
  if(auto m=dynamic_cast<const MemberAccess*>(&e)){if(m->field=="next"&&isNextChain(*m->base))return{Kind::Int,{}};const MemberAccess* savedMA=curMA_;curMA_=m;Type mt=member(*m->base,m->field);curMA_=savedMA;return mt;}
  if(auto u=dynamic_cast<const Unary*>(&e)){Type t=expr(*u->operand);if(u->op=="++"||u->op=="--"||u->op=="post++"||u->op=="post--"){if(syntax<SyntaxVersion{3,0})err("'"+u->op.substr(u->op.size()-2)+"' requires #sleela 3.0");else if(!numeric(t)&&t.kind!=Kind::Unknown)err("'"+u->op.substr(u->op.size()-2)+"' requires a numeric variable, got "+nameOf(t));else if(auto lm=dynamic_cast<const MemberAccess*>(u->operand.get()); lm&&lm->arrayLength)err("the array length is read-only");else if(!dynamic_cast<const VarExpr*>(u->operand.get())&&!dynamic_cast<const MemberAccess*>(u->operand.get())&&!dynamic_cast<const ArrayAccess*>(u->operand.get()))err("'"+u->op.substr(u->op.size()-2)+"' needs a variable, field or array element");return t;}if(u->op=="-"&&(!numeric(t)||t.kind==Kind::Unsigned)&&t.kind!=Kind::Unknown)err("unary '-' requires signed numeric operand, got "+nameOf(t));if(u->op=="!"&&t.kind!=Kind::Bool&&t.kind!=Kind::Unknown)err("unary '!' requires bool operand, got "+nameOf(t));if(u->op=="~"){if(t.kind!=Kind::Int&&t.kind!=Kind::Unknown)err("unary '~' requires an integer operand, got "+nameOf(t));return{Kind::Int,{}};}if(isCharType(t))return{Kind::Int,{},{}};return t;}
  if(auto b=dynamic_cast<const Binary*>(&e)){return binary(*b);}
  if(auto c=dynamic_cast<const Call*>(&e)){return call(*c);}
  if(auto m=dynamic_cast<const MethodCall*>(&e)){return fluent(*m);}
  if(auto a=dynamic_cast<const AssignmentExpr*>(&e)){
   Type g=expr(*a->value);
   const bool strAppendChar=(a->op=="+="&&syntax>=SyntaxVersion{3,2}&&isCharType(g));
   if(strAppendChar)a->charMask=2u;
   if(auto v=dynamic_cast<const VarExpr*>(a->target.get())){Type t=lookup(v->name);if(t.kind==Kind::Error){err("assignment to undeclared variable '"+v->name+"'");return{Kind::Error,{}};}if(!assignable(t,g)&&!(strAppendChar&&t.kind==Kind::String))err("cannot assign "+nameOf(g)+" to '"+v->name+"' of type "+nameOf(t));return t;}
   if(auto mm=dynamic_cast<const MemberAccess*>(a->target.get())){if(isImplicitRef(*mm->base,mm->field)){err("'ref' is the implicit reference of an object and is read-only");return{Kind::Error,{}};}const MemberAccess* savedMA2=curMA_;curMA_=mm;Type t=member(*mm->base,mm->field);curMA_=savedMA2;if(mm->arrayLength){err("the array length is read-only");return{Kind::Int,{},{}};}if(t.kind!=Kind::Error&&!assignable(t,g)&&!(strAppendChar&&t.kind==Kind::String))err("cannot assign "+nameOf(g)+" to field '"+mm->field+"'");return t;}
   if(auto aa=dynamic_cast<const ArrayAccess*>(a->target.get())){Type base=expr(*aa->base);Type idx=expr(*aa->index);if(idx.kind!=Kind::Int&&idx.kind!=Kind::Unknown)err("array index must be int, got "+nameOf(idx));if(base.kind==Kind::Array){Type el=typeOf(base.elem);if(syntax>=SyntaxVersion{3,2}&&isCharType(el))a->narrowChar=true;if(!assignable(el,g)&&!(strAppendChar&&el.kind==Kind::String))err("cannot assign "+nameOf(g)+" to element of "+nameOf(base));return el;}if(base.kind!=Kind::Unknown)err("index assignment requires an array value, got "+nameOf(base));return g;}
   err("assignment target must be a variable, struct field, or array element");return{Kind::Error,{}};
  }
  if(auto c=dynamic_cast<const ConditionalExpr*>(&e)){requireBool(expr(*c->cond),"conditional '?:' condition");Type a=expr(*c->thenE),b=expr(*c->elseE);if(assignable(a,b))return a;if(assignable(b,a))return b;return{Kind::Unknown,{}};}
  if(auto io=dynamic_cast<const InstanceOfExpr*>(&e)){expr(*io->value);Type target=tn(io->typeName);if(target.kind==Kind::Error)err("instanceof uses unknown type "+io->typeName);if(target.kind==Kind::Any||target.kind==Kind::Void)err("instanceof target must be a concrete runtime type");if(target.kind==Kind::Array)err("instanceof on array types requires runtime array element contracts, which are not yet available");return{Kind::Bool,{}};}
  if(auto ca=dynamic_cast<const CastExpr*>(&e)){expr(*ca->operand);Type target=tn(ca->typeName);if(target.kind==Kind::Error)err("cast uses unknown type "+ca->typeName);if(target.kind==Kind::Array)err("casts to array types require runtime array element contracts, which are not yet available");return target;}
  if(dynamic_cast<const SuperExpr*>(&e)){
   // `super` is `this` viewed as the base class (43.6).
   auto so=superOf.find(cls);
   if(cur&&!cur->isStatic&&so!=superOf.end())return{Kind::Struct,so->second,{}};
   return{Kind::Unknown,{}};
  }
  if(dynamic_cast<const LineageExpr*>(&e))return{Kind::String,{}};
  if(dynamic_cast<const ThisExpr*>(&e)){if(cls.empty()||!cur||cur->isStatic){err("'this' is only available in an instance method");return{Kind::Error,{}};}return{Kind::Struct,cls,{}};}
  if(auto aa=dynamic_cast<const ArrayAccess*>(&e)){Type base=expr(*aa->base);Type idx=expr(*aa->index);if(idx.kind!=Kind::Int&&idx.kind!=Kind::Unknown)err("array index must be int, got "+nameOf(idx));if(base.kind==Kind::Array)return typeOf(base.elem);if(base.kind==Kind::Unknown)return{Kind::Unknown,{}};err("index access requires an array value, got "+nameOf(base));return{Kind::Error,{}};}
  if(auto mr=dynamic_cast<const MethodReferenceExpr*>(&e)){expr(*mr->base);return{Kind::Unknown,{}};}
  if(auto g=dynamic_cast<const GuideChainExpr*>(&e)){return guideChain(*g);}
  err("unknown expression kind");return{Kind::Error,{}};
 }
 // Guide chain `new T(...) :: G1 :: G2 ...` (syntax 2.3). The head must be a
 // known class; each Guide must name a zero-argument method on that class. The
 // chain is fluent: it evaluates to the head's instance type.
 Type guideChain(const GuideChainExpr&g){
  if(syntax<SyntaxVersion{2,3}){expr(*g.head);err("Guide chains 'new T(...) :: Guide ...' require syntax 2.3");return{Kind::Error,{}};}
  Type h=expr(*g.head);
  if(h.kind!=Kind::Struct){if(h.kind!=Kind::Unknown&&h.kind!=Kind::Error)err("a Guide chain head must be a constructed class, got "+nameOf(h));return h;}
  const std::string owner=h.name;
  for(const std::string&gn:g.guides){
   auto it=methods.find(owner+"::"+gn);
   if(it==methods.end()){err("Guide '"+gn+"' is not a method of class '"+owner+"'");continue;}
   const Method&m=*it->second.m;
   if(!m.params.empty())err("Guide '"+gn+"' must be a zero-argument method of class '"+owner+"'");
   if(m.isProtected&&it->second.owner!=cls)err("protected Guide access denied for '"+gn+"'");
  }
  return{Kind::Struct,owner,{}};
 }
 Type binary(const Binary&b){Type l=expr(*b.lhs),r=expr(*b.rhs);if(b.op=="+"&&syntax>=SyntaxVersion{3,2})b.charMask=(isCharType(l)?1u:0u)|(isCharType(r)?2u:0u);if(b.op=="&&"||b.op=="||"){if(l.kind!=Kind::Bool&&l.kind!=Kind::Unknown)err("operator '"+b.op+"' requires bool operands");if(r.kind!=Kind::Bool&&r.kind!=Kind::Unknown)err("operator '"+b.op+"' requires bool operands");return{Kind::Bool,{}};}
  if(b.op=="+"||b.op=="-"||b.op=="*"||b.op=="/"||b.op=="%"){
   // Java-style: `+` with a String operand is string concatenation; the VM's
   // OP_ADD already renders either operand to text. Only `+` concatenates.
   if(b.op=="+"&&(l.kind==Kind::String||r.kind==Kind::String))return{Kind::String,{}};
   if(l.kind==Kind::Unsigned || r.kind==Kind::Unsigned){
    if(l.kind!=Kind::Unsigned || r.kind!=Kind::Unsigned || l.name!=r.name){
     err("operator '"+b.op+"' requires unsigned operands of the same width");
     return{Kind::Error,{},{}};
    }
    return{Kind::Unsigned,l.name,{}};
   }
   if(!numeric(l)&&l.kind!=Kind::Unknown){err("operator '"+b.op+"' requires numeric operands");}
   if(!numeric(r)&&r.kind!=Kind::Unknown){err("operator '"+b.op+"' requires numeric operands");}
   return{(l.kind==Kind::Double||r.kind==Kind::Double)?Kind::Double:Kind::Int,{}};}
  if(b.op=="&"||b.op=="|"||b.op=="^"){
   const bool lk=(l.kind==Kind::Int||l.kind==Kind::Bool),rk=(r.kind==Kind::Int||r.kind==Kind::Bool);
   const bool lu=(l.kind==Kind::Unknown),ru=(r.kind==Kind::Unknown);
   if((!lk&&!lu)||(!rk&&!ru)||(lk&&rk&&(l.kind==Kind::Bool)!=(r.kind==Kind::Bool)))
    err("operator '"+b.op+"' requires two integer operands or two bool operands, got "+nameOf(l)+" and "+nameOf(r));
   return{(l.kind==Kind::Bool||r.kind==Kind::Bool)?Kind::Bool:Kind::Int,{}};}
  if(b.op=="<<"||b.op==">>"||b.op==">>>"){
   if((l.kind!=Kind::Int&&l.kind!=Kind::Unknown)||(r.kind!=Kind::Int&&r.kind!=Kind::Unknown))err("shift operator '"+b.op+"' requires integer operands, got "+nameOf(l)+" and "+nameOf(r));
   return{Kind::Int,{}};}
  if(b.op=="<"||b.op=="<="||b.op==">"||b.op==">="){if(l.kind==Kind::Unsigned||r.kind==Kind::Unsigned){if(l.kind!=Kind::Unsigned||r.kind!=Kind::Unsigned||l.name!=r.name)err("comparison '"+b.op+"' requires unsigned operands of the same width");}else if((!numeric(l)||!numeric(r))&&l.kind!=Kind::Unknown&&r.kind!=Kind::Unknown)err("comparison '"+b.op+"' requires numeric operands");return{Kind::Bool,{}};}
  if(b.op=="=="||b.op=="!="){if(!assignable(l,r)&&!assignable(r,l)&&l.kind!=Kind::Unknown&&r.kind!=Kind::Unknown)err("equality operands have incompatible types "+nameOf(l)+" and "+nameOf(r));return{Kind::Bool,{}};}
  // ::: structural-offload send: the left operand is the String data source;
  // the right evaluates to the receiver handle the VM frames the elements into.
  // The expression yields the SOURCE (left), so the same String can keep
  // flowing to further receivers -- `x ::: a ::: b` sends x to a, then x to b
  // (fan-out), which is how String functions feed data directly to each other.
  if(b.op==":::"){
   if(l.kind!=Kind::String&&l.kind!=Kind::Unknown)err("operator ':::' requires a String data source on the left");
   return l;
  }
  err("unknown binary operator '"+b.op+"'");return{Kind::Error,{}};
 }
 // Overload resolution (syntax 3.0): among the declarations of key with the
 // call's arity, choose the one whose parameter types accept the arguments,
 // preferring exact type matches; ties keep the first declaration. Returns the
 // single declaration (or the last-declared one) when there is no choice.
 const MethodSig* pickOverload(const std::string& key,const std::vector<Type>& argTypes){
  auto ov=overloads.find(key);
  if(ov==overloads.end()||ov->second.size()<2){auto it=methods.find(key);return it==methods.end()?nullptr:&it->second;}
  const MethodSig* best=nullptr; int bestScore=-1;
  for(const auto& cand:ov->second){
   const Method& m=*cand.m;
   if(m.params.size()!=argTypes.size()) continue;
   int score=0; bool ok=true;
   for(size_t i=0;i<argTypes.size();++i){
    Type w=tn(m.params[i].type); const Type& g=argTypes[i];
    if(!assignable(w,g)){ok=false;break;}
    if(w.kind==g.kind&&(w.kind!=Kind::Struct||w.name==g.name)) score+=2;
    else if(w.kind!=Kind::Any&&!(w.kind==Kind::Struct&&w.name=="Object")) score+=1;
   }
   if(ok&&score>bestScore){best=&cand;bestScore=score;}
  }
  if(best) return best;
  for(const auto& cand:ov->second) if(cand.m->params.size()==argTypes.size()) return &cand;
  auto it=methods.find(key); return it==methods.end()?nullptr:&it->second;
 }
 std::vector<Type> argTypesOf(const std::vector<ExprP>& args){std::vector<Type> t;for(const auto&a:args)t.push_back(expr(*a));return t;}
 Type call(const Call&c){auto it=methods.find(cls+"::"+c.callee);if(it==methods.end())it=methods.find(c.callee);if(it==methods.end()&&c.callee.rfind("__native_",0)==0){
   // Synthesized native helpers are stored as qualified class methods. Match
   // the complete helper symbol across native owners, not just chemistry.
   for(const auto& entry:methods){
    const auto sep=entry.first.rfind("::");
    if(sep!=std::string::npos&&entry.first.compare(sep+2,std::string::npos,c.callee)==0&&entry.first.substr(0,sep).rfind("__Native",0)==0){it=methods.find(entry.first);break;}
   }
  }if(it!=methods.end()){std::vector<Type> at=argTypesOf(c.args);if(const MethodSig* pk=pickOverload(it->first,at)){it=methods.find(it->first);c.resolved=pk->m;}const Method&m=c.resolved?*c.resolved:*it->second.m;if(c.args.size()!=m.params.size())err("method '"+c.callee+"' expects "+std::to_string(m.params.size())+" argument(s), got "+std::to_string(c.args.size()));size_t n=c.args.size()<m.params.size()?c.args.size():m.params.size();for(size_t i=0;i<n;i++){Type g=at[i],w=tn(m.params[i].type);if(!assignable(w,g))err("argument "+std::to_string(i+1)+" to '"+c.callee+"' has type "+nameOf(g)+", expected "+nameOf(w));}if(m.isProtected&&it->second.owner!=cls)err("protected method access denied for '"+c.callee+"'");return tn(m.retType);}
  // `spawn(method)` names a zero-arg method to run on a new thread; its single
  // argument is a method name, not a value, so it is not resolved as a variable.
  if(c.callee=="spawn"){if(c.args.size()!=1){err("spawn(method) takes exactly one argument");return{Kind::Unknown,{}};}auto v=dynamic_cast<const VarExpr*>(c.args[0].get());if(!v||(!methods.count(v->name)&&!methods.count(cls+"::"+v->name)))err("spawn(method) argument must be a declared method name");return{Kind::Unknown,{}};}
  // `structUnpack(TypeName, json)` -- the first argument is a struct type name,
  // not a value; the result is an instance of that struct type.
  if(c.callee=="structUnpack"){if(c.args.size()!=2){err("structUnpack(TypeName, json) takes exactly two arguments");return{Kind::Error,{}};}auto v=dynamic_cast<const VarExpr*>(c.args[0].get());if(!v||!structs.count(v->name)){err("structUnpack first argument must be a declared struct type name");return{Kind::Error,{}};}expr(*c.args[1]);return{Kind::Struct,v->name};}
  std::vector<Type> argTypes; argTypes.reserve(c.args.size());
  for(const auto&a:c.args) argTypes.push_back(expr(*a));
  if(!builtin(c.callee)){err("call to unknown method '"+c.callee+"'");}
  const std::string&n=c.callee;
  if(n=="javaStringFromCodeUnits") return{Kind::String,{}};
  if(n=="javaStringFromUtf8"){
   if(argTypes.size()!=1)err("javaStringFromUtf8(value) takes exactly one argument");
   if(!argTypes.empty() && argTypes[0].kind!=Kind::String && argTypes[0].kind!=Kind::Unknown)
    err("javaStringFromUtf8 requires a legacy UTF-8 String argument");
   return{Kind::String,{}};
  }
  if(n=="javaStringToUtf8"){
   if(argTypes.size()!=1)err("javaStringToUtf8(value) takes exactly one argument");
   if(!argTypes.empty() && argTypes[0].kind!=Kind::String && argTypes[0].kind!=Kind::Unknown)
    err("javaStringToUtf8 requires an SL_UTF16 Java String argument");
   return{Kind::String,{}};
  }
  if(n=="javaStringFromCodeUnit"){
   if(argTypes.size()!=1)err("javaStringFromCodeUnit(codeUnit) takes exactly one argument");
   if(!argTypes.empty() && argTypes[0].kind!=Kind::Int && argTypes[0].kind!=Kind::Unsigned && argTypes[0].kind!=Kind::Unknown)
    err("javaStringFromCodeUnit requires an integer code unit");
   return{Kind::String,{}};
  }
  const std::set<std::string> javaStringUnary={"javaStringLength","javaStringHashCode"};
  const std::set<std::string> javaStringIndexed={"javaStringCharAt","javaStringCodePointAt"};
  const std::set<std::string> javaStringTriple={"javaStringCodePointCount","javaStringOffsetByCodePoints","javaStringSubstring"};
  if(javaStringUnary.count(n)||javaStringIndexed.count(n)||javaStringTriple.count(n)){
   if(argTypes.empty() || (argTypes[0].kind!=Kind::String && argTypes[0].kind!=Kind::Unknown))
    err(n+" requires an SL_UTF16 Java String as its first argument");
   for(size_t i=1;i<argTypes.size();++i)
    if(argTypes[i].kind!=Kind::Int && argTypes[i].kind!=Kind::Unsigned && argTypes[i].kind!=Kind::Unknown)
     err(n+" index/range arguments must be integers");
  }
  if(n=="javaStringConcat"||n=="javaStringCompareTo"){
   if(argTypes.size()>0 && argTypes[0].kind!=Kind::String && argTypes[0].kind!=Kind::Unknown)err(n+" requires a Java String as its first argument");
   if(argTypes.size()>1 && argTypes[1].kind!=Kind::String && argTypes[1].kind!=Kind::Unknown)err(n+" requires a Java String as its second argument");
  }
  if(n=="javaStringLength"||n=="javaStringCharAt"||n=="javaStringCodePointAt"||
     n=="javaStringCodePointCount"||n=="javaStringOffsetByCodePoints"||
     n=="javaStringHashCode"||n=="javaStringCompareTo") return{Kind::Int,{}};
  if(n=="javaStringSubstring"||n=="javaStringConcat") return{Kind::String,{}};
  if(n=="arrayNew")return{Kind::Array,"array",{}};
  // Native Drive (syntax 3.0): host-authorized in-process native modules. The
  // compiler lowers these to OP_NATIVE_OPEN/CALL/CLOSE; the checker validates
  // version and shape so check/run agree with compile.
  if(n=="nativeOpen"||n=="nativeCall"||n=="nativeClose"){
   if(syntax<SyntaxVersion{3,0})err("Native Drive requires #sleela 3.0");
   if(n=="nativeOpen"){
    if(argTypes.size()!=1)err("nativeOpen(path) takes exactly one String argument");
    else if(argTypes[0].kind!=Kind::String&&argTypes[0].kind!=Kind::Unknown)err("nativeOpen(path) requires a String path");
    return{Kind::Int,{}};
   }
   if(n=="nativeClose"){
    if(argTypes.size()!=1)err("nativeClose(handle) takes exactly one argument");
    return{Kind::Int,{}};
   }
   if(argTypes.size()<2||argTypes.size()>4096)err("nativeCall(handle, method, ...args) requires a handle, method String, and at most 4094 arguments");
   else if(argTypes[1].kind!=Kind::String&&argTypes[1].kind!=Kind::Unknown)err("nativeCall method name must be a String");
   return{Kind::Unknown,{}};
  }
  if(n=="arrayLength")return{Kind::Int,{}};
  if(n=="arrayPush" || n=="arraySet"){
   const size_t valueArg=(n=="arrayPush"?1:2);
   if(argTypes.size()>valueArg){
    const Type& at=argTypes[0];
    if(at.kind==Kind::Array){
     Type expected=typeOf(at.elem);
     if(!assignable(expected,argTypes[valueArg])) err(n+" cannot store "+nameOf(argTypes[valueArg])+" in "+nameOf(at));
    } else if(at.kind!=Kind::Unknown && at.kind!=Kind::Error) err(n+" requires an array as its first argument, got "+nameOf(at));
   }
   return n=="arrayPush"?Type{Kind::Int,{},{}}:(argTypes.size()>2?argTypes[2]:Type{Kind::Unknown,{},{}});
  }
  if(n=="arrayGet"){
   if(!argTypes.empty() && argTypes[0].kind==Kind::Array) return typeOf(argTypes[0].elem);
   return{Kind::Unknown,{}};
  }
  if(n=="conduct"||n=="congruent"){return{Kind::Bool,{}};}
  if(n=="role"||n=="insight"||n=="route"||n=="timeLocation"||n=="timeHttpDate"||n=="timeJson"){return{Kind::String,{}};}
  if(n=="osPlatform"||n=="osGetEnv"||n=="osCurrentDir"||n=="osHostName"||n=="osUserName"||n=="osTempDir"){return{Kind::String,{}};}
  if(n=="osCapability"||n=="osSetEnv"||n=="osChangeDir"||n=="osProcessId"||n=="osExists"||n=="osIsDir"||n=="osFileSize"||n=="osMakeDir"||n=="osRemove"||n=="osRename"||n=="osRun"||n=="osSpawn"||n=="osWait"||n=="osKill"){return{Kind::Int,{}};}
  if(n=="sysdepth"||n=="degreemax"||n=="timeUtcMillis"||n=="timeUtcNanos"||n=="timeMonotonicNanos"||n=="timePrecisionMillis"){return{Kind::Int,{}};}
  if(n=="bigmul"||n=="bigadd"||n=="bigdivmod"||n=="bigsub"||n=="bigdiv"||n=="bigisqrt"){return{Kind::Int,{}};}
  if(n=="synchroMean"||n=="synchroMin"||n=="synchroMax"||n=="synchroP95"||n=="synchroLoss"||n=="bestOfMean"||n=="bestOfLoss"||n=="bestOfJitter"||n=="bestOfCertainty"){return{Kind::Double,{}};}
  if(n=="Munction.start"||n=="read"||n=="recv"||n=="sockread"||n=="timeNtp"||n=="sleep"){return{Kind::Unknown,{}};}
  // SleelaUI (ui*) bridge return types: string getters, one double getter, the
  // rest return int handles/counts/status.
  if(n=="uiVersion"||n=="uiBackendName"||n=="uiWidgetGetText"||n=="uiBallumStatusName"||n=="uiBallumDisplayName"||n=="uiBallumEventTypeName"||n=="uiBallumFxName"||n=="uiBallumBackendName"||n=="uiWifiStatusName"||n=="uiWifiNetworkSsid"||n=="uiWifiNetworkBssid"||n=="uiWifiNetworkSecurity"){return{Kind::String,{}};}
  if(n=="uiWidgetGetValue"){return{Kind::Double,{}};}
  if(n.size()>2 && n[0]=='u' && n[1]=='i' && n[2]>='A' && n[2]<='Z'){return{Kind::Int,{}};}
  return{Kind::Unknown,{}};
 }
 // Enhanced for (SLEELA.syntax 43.6).
 const Method* instanceMethod(const std::string& cls,const std::string& name,size_t argc){
  auto it=methods.find(cls+"::"+name); if(it==methods.end()) return nullptr;
  auto ov=overloads.find(it->first);
  if(ov!=overloads.end()) for(const auto& sig:ov->second) if(sig.m&&!sig.m->isStatic&&sig.m->params.size()==argc) return sig.m;
  return (it->second.m&&!it->second.m->isStatic&&it->second.m->params.size()==argc)?it->second.m:nullptr;
 }
 void forEach(const ForEachStmt& f){
  if(syntax<SyntaxVersion{3,0}){err("the enhanced for 'for (T v : xs)' requires #sleela 3.0");return;}
  Type it=expr(*f.iterable); Type want=tn(f.type);
  if(want.kind==Kind::Error){err("unknown loop variable type '"+f.type+"'");return;}
  Type elem{Kind::Unknown,{}};
  if(it.kind==Kind::Array){ elem=typeOf(it.elem); f.overIterable=false; }
  else if(it.kind==Kind::Struct){
   const Method* im=instanceMethod(it.name,"iterator",0);
   if(!im){err("enhanced for over '"+it.name+"' needs an array or an object with iterator()");return;}
   Type rt=tn(im->retType);
   if(rt.kind!=Kind::Struct){err("'"+it.name+".iterator()' must return an iterator object, got "+nameOf(rt));return;}
   const Method* hn=instanceMethod(rt.name,"hasNext",0); const Method* nx=instanceMethod(rt.name,"next",0);
   if(!hn||!nx){err("iterator type '"+rt.name+"' needs hasNext() and next()");return;}
   f.overIterable=true; f.iterableType=it.name; f.iteratorType=rt.name; f.iteratorM=im; f.hasNextM=hn; f.nextM=nx;
   elem=tn(nx->retType);
  }
  else if(it.kind!=Kind::Unknown&&it.kind!=Kind::Error){err("enhanced for needs an array or an iterable object, got "+nameOf(it));return;}
  // Element types Any/Unknown (erased generics) bind without a cast, as Java's
  // compiler-inserted checkcast does; otherwise the element must be assignable.
  if(elem.kind!=Kind::Any&&elem.kind!=Kind::Unknown&&!assignable(want,elem))
   err("enhanced for variable '"+f.name+"' has type "+nameOf(want)+", elements are "+nameOf(elem));
  push(); declare(f.name,want); stmt(*f.body); pop();
 }
 // super(args) / this(args) constructor invocation (SLEELA.syntax 43.6).
 Type constructorInvocation(const MethodCall&m){
  if(!cur||!cur->java.constructor){err(std::string(m.superConstructor?"super":"this")+"(...) is only allowed in a constructor");return{Kind::Void,{}};}
  std::vector<Type> at=argTypesOf(m.args);
  if(syntax<SyntaxVersion{3,0}) return{Kind::Void,{}};   // below 3.0: lineage-only, arguments not used
  std::string target=cls;
  if(m.superConstructor){
   auto so=superOf.find(cls);
   if(so==superOf.end()){ if(!m.args.empty()) err("super(...) with arguments in '"+cls+"' needs a loaded base class"); return{Kind::Void,{}}; }
   target=so->second;
  }
  const Method* best=nullptr; int bestScore=-1; bool any=false;
  for(const auto&c:p.classes) if(c.name==target) for(const auto&mm:c.methods){
   if(!mm.java.constructor) continue; any=true;
   if(mm.params.size()!=at.size()) continue;
   if(m.thisConstructor&&&mm==cur) continue;
   int score=0; bool ok=true;
   for(size_t i=0;i<at.size();++i){Type w=tn(mm.params[i].type); if(!assignable(w,at[i])){ok=false;break;} if(same(w,at[i])) score+=2; else score+=1;}
   if(ok&&score>bestScore){best=&mm;bestScore=score;}
  }
  if(!best){
   if(m.args.empty()&&!any) return{Kind::Void,{}};   // implicit default constructor
   err("no constructor of '"+target+"' takes "+std::to_string(at.size())+" argument(s) of these types");
   return{Kind::Void,{}};
  }
  m.resolved=best; return{Kind::Void,{}};
 }
 Type fluent(const MethodCall&m){
  if(m.superConstructor||m.thisConstructor) return constructorInvocation(m);
  if(m.method=="start"){if(auto r=dynamic_cast<const VarExpr*>(m.receiver.get())){if(r->name=="Munction"){if(m.args.size()!=1)err("Munction.start(name) takes one argument");else{Type t=expr(*m.args[0]);if(t.kind!=Kind::String&&t.kind!=Kind::Unknown)err("Munction.start name must be string");}return{Kind::Unknown,{}};}}}
  // The builtin `Ptr` namespace: Ptr.valid(p), Ptr.id(p), ... operate on an
  // object reference (an int obtained from `obj.ref`). Like `Munction`, it is
  // special only when no class/struct/variable named Ptr is in scope.
  if(auto rf=dynamic_cast<const VarExpr*>(m.receiver.get())){
   if(rf->name=="Ptr" && !classNames.count("Ptr") && !structs.count("Ptr") && lookup("Ptr").kind==Kind::Error){
    static const std::map<std::string,std::pair<int,Kind>> refFns={
     {"valid",{1,Kind::Bool}},{"old",{1,Kind::Bool}},{"id",{1,Kind::Int}},{"gen",{1,Kind::Int}},
     {"type",{1,Kind::Int}},{"fields",{1,Kind::Int}},{"bytes",{1,Kind::Int}},{"age",{1,Kind::Int}},
     {"live",{0,Kind::Int}},{"limit",{0,Kind::Int}}};
    auto fn=refFns.find(m.method);
    if(fn==refFns.end()){err("Ptr has no function '"+m.method+"' (valid, old, id, gen, type, fields, bytes, age, live, limit)");return{Kind::Error,{}};}
    if((int)m.args.size()!=fn->second.first){err("Ptr."+m.method+"() expects "+std::to_string(fn->second.first)+" argument(s), got "+std::to_string(m.args.size()));return{Kind::Error,{}};}
    for(const auto&a:m.args){Type t=expr(*a);if(t.kind!=Kind::Int&&t.kind!=Kind::Unknown&&t.kind!=Kind::Null)err("Ptr."+m.method+"() takes a ref (int), got "+nameOf(t));}
    return{fn->second.second,{}};
   }
  }
  // Static access through the class name (Java): Class.method(args), when the
  // receiver names a class and is not a variable in scope.
  if(auto rv=dynamic_cast<const VarExpr*>(m.receiver.get())){
   if(classNames.count(rv->name) && lookup(rv->name).kind==Kind::Error){
    auto st=methods.find(rv->name+"::"+m.method);
    if(st==methods.end() && m.method=="deref"){
     // Implicit `Class.deref(ref)`: the object if the ref is valid and an
     // instance of this class, else null. Never raises on a stale ref.
     if(m.args.size()!=1){err("'"+rv->name+".deref' expects 1 argument, got "+std::to_string(m.args.size()));return{Kind::Error,{}};}
     Type t=expr(*m.args[0]);if(t.kind!=Kind::Int&&t.kind!=Kind::Unknown&&t.kind!=Kind::Null)err("'"+rv->name+".deref' takes a ref (int), got "+nameOf(t));
     return{Kind::Struct,rv->name};
    }
    if(st==methods.end()){err("class '"+rv->name+"' has no method '"+m.method+"'");return{Kind::Error,{}};}
    std::vector<Type> at=argTypesOf(m.args);
    const MethodSig* pk=pickOverload(st->first,at); m.resolved=pk->m;
    const Method& target=*pk->m;
    if(!target.isStatic){err("instance method '"+rv->name+"::"+m.method+"' must be called on an instance");return{Kind::Error,{}};}
    if(m.args.size()!=target.params.size())err("method '"+m.method+"' expects "+std::to_string(target.params.size())+" argument(s), got "+std::to_string(m.args.size()));
    for(size_t i=0;i<m.args.size()&&i<target.params.size();++i){Type g=at[i],w=tn(target.params[i].type);if(!assignable(w,g))err("argument to '"+m.method+"' has type "+nameOf(g)+", expected "+nameOf(w));}
    if(target.isProtected&&pk->owner!=cls)err("protected method access denied for '"+m.method+"'");
    return tn(target.retType);
   }
  }
  Type rcv=expr(*m.receiver);
  static const std::set<std::string> v={"connect","enable","send","thatch","consume","latch","closeWithReceipt","close","reception"};
  if(v.count(m.method)){if(m.method=="consume"||m.method=="latch"||m.method=="close"||m.method=="reception"||m.method=="closeWithReceipt"){if(!m.args.empty())err("Munction "+m.method+"() takes no arguments");}else{if(m.args.size()!=1)err("Munction "+m.method+"() takes exactly one argument");if(!m.args.empty()){Type t=expr(*m.args[0]);if(t.kind!=Kind::String&&t.kind!=Kind::Unknown)err("Munction "+m.method+" argument must be string");}}return m.method=="closeWithReceipt"?Type{Kind::String,{}}:rcv;}
  // Java String instance methods (syntax 3.0): `s.length()`, `"x".equals(y)`.
  // The call binds to the static JavaStringMethods.<method>(String self, ...)
  // in /lib, overloads resolved with the receiver as the first argument.
  if(rcv.kind==Kind::String && !(syntax<SyntaxVersion{3,0})){
   if(!classNames.count("JavaStringMethods")){
    r.needsStringMethods=true;
    err("method '"+m.method+"' on a String needs the java.lang.String method library (JavaStringMethods), which is not loaded");
    return{Kind::Error,{}};
   }
   auto sm=methods.find("JavaStringMethods::"+m.method);
   if(sm==methods.end()){err("String has no method '"+m.method+"'");return{Kind::Error,{}};}
   std::vector<Type> at=argTypesOf(m.args);
   at.insert(at.begin(),Type{Kind::String,{},{}});
   const MethodSig* pk=pickOverload(sm->first,at); m.resolved=pk->m; m.stringReceiver=true;
   const Method& target=*pk->m;
   if(!target.isStatic){err("String method '"+m.method+"' must be a static JavaStringMethods method");return{Kind::Error,{}};}
   if(at.size()!=target.params.size())err("String method '"+m.method+"' expects "+std::to_string(target.params.size()>0?target.params.size()-1:0)+" argument(s), got "+std::to_string(m.args.size()));
   for(size_t i=1;i<at.size()&&i<target.params.size();++i){Type g=at[i],w=tn(target.params[i].type);if(!assignable(w,g))err("argument to String method '"+m.method+"' has type "+nameOf(g)+", expected "+nameOf(w));}
   return tn(target.retType);
  }
  if(rcv.kind!=Kind::Struct){err("method '"+m.method+"' requires a class or struct receiver");return{Kind::Error,{}};}
  auto it=methods.find(rcv.name+"::"+m.method);if(it==methods.end()){err("class '"+rcv.name+"' has no method '"+m.method+"'");return{Kind::Error,{}};}
  std::vector<Type> at=argTypesOf(m.args);
  const MethodSig* pk=pickOverload(it->first,at); m.resolved=pk->m;
  const Method& target=*pk->m;if(target.isStatic)err("static method '"+rcv.name+"::"+m.method+"' must be called through its class");
  if(m.args.size()!=target.params.size())err("method '"+m.method+"' expects "+std::to_string(target.params.size())+" argument(s), got "+std::to_string(m.args.size()));
  for(size_t i=0;i<m.args.size()&&i<target.params.size();++i){Type g=at[i],w=tn(target.params[i].type);if(!assignable(w,g))err("argument to '"+m.method+"' has type "+nameOf(g)+", expected "+nameOf(w));}
  if(target.isProtected&&pk->owner!=cls)err("protected method access denied for '"+m.method+"'");
  return tn(target.retType);
}
public:
 Analyzer(const Program&x,const SyntaxVersion&s,bool needMain):p(x),syntax(s),requireMain(needMain){}
 SemanticResult run(){
  javaref::ClassIndex javaClasses; for(const auto&c:p.classes) javaClasses[c.name]=&c;
  struct ClassesGuard{ ~ClassesGuard(){ g_javaClasses=nullptr; } } classesGuard;
  if(!(syntax<SyntaxVersion{1,10})) g_javaClasses=&javaClasses;
  collect();
  declarations();
  for(const auto&d:frontend::validateLanguageAnnotations(p.annotations)){
    if(d.severity==frontend::AnnotationDiagnostic::Severity::Error) err(d.message);
    else r.warnings.push_back("Semantic warning: "+d.message);
  }
  for(const auto&c:p.classes)for(const auto&f:c.fields)fieldInit(c,f);
  for(const auto&c:p.classes)for(const auto&m:c.methods)method(c,m);
  return r;
}
};
}
SemanticResult analyzeSemantics(const Program&p,const SyntaxVersion&s){return analyzeSemantics(p,s,true);}
SemanticResult analyzeSemantics(const Program&p,const SyntaxVersion&s,bool requireMain){return Analyzer(p,s,requireMain).run();}
}
