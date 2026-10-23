#include "sleela_editor_theme.h"
#include <iostream>
#include <string>
static void usage(const char *program) {
 std::cerr << "Usage: " << program << " [--themes | --theme ID --palette | --theme ID --hex COLOR]\n"
           << "Theme IDs: american, eurasia, russian-slavic, silk-road, east-asian\n";
}
int main(int argc,char **argv) {
 std::string theme="american";
 if(argc==1) return sleela_theme_print_terminal();
 if(std::string(argv[1])=="--themes") {
  std::cout << "american\neurasia\nrussian-slavic\nsilk-road\neast-asian\n";
  return 0;
 }
 if(std::string(argv[1])=="--palette" && argc==2) return sleela_theme_print_terminal();
 if(std::string(argv[1])=="--theme" && argc>=3) {
  theme=argv[2];
  if(argc==4 && std::string(argv[3])=="--palette") return sleela_theme_print_terminal_for(theme.c_str());
  if(argc==5 && std::string(argv[3])=="--hex") {
   const SleelaThemeColor *c=sleela_theme_find_for(theme.c_str(),argv[4]);
   if(!c){std::cerr<<"Unknown theme or color: "<<theme<<" / "<<argv[4]<<'\n';return 2;}
   std::cout<<'#'<<c->hex<<"  "<<c->role<<'\n';return 0;
  }
 }
 if(std::string(argv[1])=="--hex" && argc==3) {
  const SleelaThemeColor *c=sleela_theme_find(argv[2]);
  if(!c){std::cerr<<"Unknown American palette color: "<<argv[2]<<'\n';return 2;}
  std::cout<<'#'<<c->hex<<"  "<<c->role<<'\n';return 0;
 }
 usage(argv[0]); return 64;
}
