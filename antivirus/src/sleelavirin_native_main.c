#include "sleelavirin_native.h"
#include <stdio.h>
#include <string.h>

static void usage(const char *program)
{
    fprintf(stderr,"Usage: %s --database FILE <file> [file ...]\n",program);
    fprintf(stderr,"Native engine compares SHA-256 digests with a local signature list.\n");
    fprintf(stderr,"NO_MATCH is not proof that a file is safe.\n");
}
int main(int argc,char **argv)
{
    const char *database=NULL; int i=1, status=0, scanned=0;
    if(argc<2){usage(argv[0]);return 2;}
    while(i<argc){
        if(strcmp(argv[i],"--database")==0){
            if(i+1>=argc){usage(argv[0]);return 2;}
            database=argv[i+1]; i+=2; break;
        }
        if(strcmp(argv[i],"--help")==0){usage(argv[0]);return 0;}
        usage(argv[0]); return 2;
    }
    if(!database||i>=argc){usage(argv[0]);return 2;}
    for(;i<argc;i++){
        char name[512]; int result=sleelavirin_native_scan_file(argv[i],database,name,sizeof(name));
        scanned++;
        if(result==1){printf("DETECTED %s%s%s\n",argv[i],name[0]?" : ":"",name);if(status==0)status=1;}
        else if(result==0){printf("NO_MATCH %s\n",argv[i]);}
        else {fprintf(stderr,"ERROR %s (file or signature database unavailable/unreadable)\n",argv[i]);status=2;}
    }
    return scanned?status:2;
}
