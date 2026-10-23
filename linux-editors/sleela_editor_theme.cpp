#include "sleela_editor_theme.h"
#include <iostream>
#include <string>
int main(int argc,char **argv){ if(argc==1 || std::string(argv[1])=="--palette") return sleela_theme_print_terminal(); if(std::string(argv[1])=="--hex" && argc==3){ const SleelaThemeColor *c=sleela_theme_find(argv[2]); if(!c){std::cerr<<"Unknown SLeeLa theme color: "<<argv[2]<<'\n';return 2;} std::cout<<'#'<<c->hex<<"  "<<c->role<<'\n';return 0;} std::cerr<<"Usage: "<<argv[0]<<" [--palette | --hex COLOR]\n";return 64; }
