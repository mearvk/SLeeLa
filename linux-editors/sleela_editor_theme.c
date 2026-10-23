#include "sleela_editor_theme.h"
#include <stdio.h>
#include <string.h>
static const SleelaThemeColor palette[] = {
 {"background","101820","Deep navy background"}, {"foreground","F4F7FA","Soft white foreground"},
 {"keyword","7FB3FF","Clear American blue"}, {"type","73DACA","Cool teal types"},
 {"string","F2C078","Warm amber strings"}, {"number","C4A7E7","Violet numeric literals"},
 {"comment","8A96A3","Quiet slate comments"}, {"function","82C91E","Readable green functions"},
 {"operator","D8DEE9","Neutral operators"}, {"constant","FF8F8F","Coral constants"},
 {"preprocessor","F0A6CA","Rose preprocessor directives"}, {"selection","233D55","Blue selection surface"},
 {"diagnostic","FF6B6B","High-contrast errors"}
};
const SleelaThemeColor *sleela_theme_palette(unsigned int *count) { if (count) *count=(unsigned int)(sizeof palette/sizeof palette[0]); return palette; }
const SleelaThemeColor *sleela_theme_find(const char *name) { unsigned int i,n; if (!name || !*name) return NULL; sleela_theme_palette(&n); for(i=0;i<n;i++) if(strcmp(name,palette[i].name)==0) return &palette[i]; return NULL; }
int sleela_theme_print_terminal(void) { unsigned int i,n; const SleelaThemeColor *p=sleela_theme_palette(&n); for(i=0;i<n;i++){ unsigned int r,g,b; if(sscanf(p[i].hex,"%2x%2x%2x",&r,&g,&b)!=3)return 1; printf("\033[38;2;%u;%u;%um%-14s  #%s  %s\033[0m\n",r,g,b,p[i].name,p[i].hex,p[i].role); } return 0; }
