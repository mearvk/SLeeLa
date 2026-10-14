/*
 * registry/sleela_registry_tool.cpp -- a terminal reviewer for the registry.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Seeds (or loads) a registry, records a provenance entry with an originality
 * seal, and reviews it — the hands-on demo of what a new install's reviewable
 * registry looks like.
 *   build/sleela-registry seed <out.sleelareg>     # seed + save
 *   build/sleela-registry review <file.sleelareg>  # load + review
 */
#include "sleela_registry.hpp"

#include <cstdio>
#include <cstring>
#include <string>

static sl_reg_provenance makeProvenance(const char* file,const char* author,
                                        const char* version,const char* hexid,
                                        const std::string& content){
    sl_reg_provenance p; std::memset(&p,0,sizeof p);
    std::snprintf(p.name,sizeof p.name,"%s",file);
    std::snprintf(p.date,sizeof p.date,"2026-10-14T00:00:00Z");
    std::snprintf(p.author,sizeof p.author,"%s",author);
    std::snprintf(p.version,sizeof p.version,"%s",version);
    std::snprintf(p.hex_id,sizeof p.hex_id,"%s",hexid);
    sl_reg_sha256_hex(content.data(),content.size(),p.hash);
    sl_reg_seal(&p);  // originality seal over hexid|version|hash|author
    return p;
}

int main(int argc,char**argv){
    std::string cmd = (argc>=2)?argv[1]:"review";

    if(cmd=="seed"){
        const char* out = (argc>=3)?argv[2]:"registry.sleelareg";
        reg::Registry r;
        r.seed();
        // record a provenance entry (what a tracked artifact looks like)
        sl_reg_provenance p = makeProvenance(
            "sleela_registry.c","MEARVK LLC / the Author","1.0.0","1dea0005ec0de001",
            "the registry core source");
        r.provenanceSet("Artifacts/registry-core", p);
        if(r.save(out)==SL_REG_OK) std::printf("seeded + saved %s\n",out);
        else { std::printf("save failed\n"); return 1; }
        std::printf("%s", r.review().c_str());
        return 0;
    }

    if(cmd=="review"){
        const char* in = (argc>=3)?argv[2]:"registry.sleelareg";
        reg::Registry r;
        if(r.load(in)!=SL_REG_OK){ std::printf("cannot load %s (seeding fresh)\n",in); r.seed(); }
        std::printf("%s", r.review().c_str());
        return 0;
    }

    std::printf("usage: sleela-registry <seed|review> <file.sleelareg>\n");
    return 2;
}
