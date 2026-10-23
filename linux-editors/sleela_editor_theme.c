#include "sleela_editor_theme.h"
#include <stdio.h>
#include <string.h>

#define COLOR(n,h,r) {n,h,r}
static const SleelaThemeColor american[] = {
 COLOR("background","101820","Deep navy background"), COLOR("foreground","F4F7FA","Soft white foreground"),
 COLOR("keyword","7FB3FF","American blue keywords"), COLOR("type","73DACA","Cool teal types"),
 COLOR("string","F2C078","Warm amber strings"), COLOR("number","C4A7E7","Violet numeric literals"),
 COLOR("comment","8A96A3","Quiet slate comments"), COLOR("function","82C91E","Readable green functions"),
 COLOR("operator","D8DEE9","Neutral operators"), COLOR("constant","FF8F8F","Coral constants"),
 COLOR("preprocessor","F0A6CA","Rose preprocessor directives"), COLOR("selection","233D55","Blue selection surface"),
 COLOR("diagnostic","FF6B6B","High-contrast errors")
};
static const SleelaThemeColor eurasia[] = {
 COLOR("background","111827","Ink navy background"), COLOR("foreground","F0F2F5","Silk white foreground"),
 COLOR("keyword","7AA7E8","Sapphire keywords"), COLOR("type","69C9B3","Jade types"),
 COLOR("string","D7B56D","Silk gold strings"), COLOR("number","C5A6E8","Amethyst numeric literals"),
 COLOR("comment","8995A5","Slate comments"), COLOR("function","8FCB8A","Jade-green functions"),
 COLOR("operator","D0D8E2","Soft silver operators"), COLOR("constant","D98A9A","Burgundy rose constants"),
 COLOR("preprocessor","D98A9A","Burgundy preprocessor directives"), COLOR("selection","30435A","Sapphire selection surface"),
 COLOR("diagnostic","FF7777","Clear diagnostic red")
};
static const SleelaThemeColor russian_slavic[] = {
 COLOR("background","202733","Balanced charcoal-slate background"), COLOR("foreground","E8EAF0","Muted porcelain foreground"),
 COLOR("keyword","A9C0DD","Medium imperial blue keywords"), COLOR("type","8FC4B2","Quiet green-blue types"),
 COLOR("string","D6BD8B","Antique gold strings"), COLOR("number","C2B0D8","Muted lilac numeric literals"),
 COLOR("comment","9AA3AF","Soft graphite comments"), COLOR("function","A7C69C","Restrained sage functions"),
 COLOR("operator","D1D6DE","Cool silver operators"), COLOR("constant","D99198","Medium ruby constants"),
 COLOR("preprocessor","D5A6B4","Dusty rose preprocessor directives"), COLOR("selection","3B4655","Slate selection surface"),
 COLOR("diagnostic","E58B91","Readable muted red diagnostics")
};
static const SleelaThemeColor silk_road[] = {
 COLOR("background","1C2529","Charcoal-teal background"), COLOR("foreground","F5F0E5","Warm textile ivory foreground"),
 COLOR("keyword","7FADE0","Lapis lazuli keywords"), COLOR("type","45C6C2","Turquoise types"),
 COLOR("string","D9B56D","Desert gold strings"), COLOR("number","C4A4DE","Dye-violet numeric literals"),
 COLOR("comment","92A3A0","Weathered stone comments"), COLOR("function","A7C982","Oasis green functions"),
 COLOR("operator","D5DEDA","Pale mineral operators"), COLOR("constant","E58B79","Terracotta constants"),
 COLOR("preprocessor","E5A9A0","Woven rose preprocessor directives"), COLOR("selection","315052","Turquoise selection surface"),
 COLOR("diagnostic","FF7777","Clear diagnostic red")
};
static const SleelaThemeColor east_asian[] = {
 COLOR("background","171A1C","Ink-black background"), COLOR("foreground","F2EFE7","Rice-paper ivory foreground"),
 COLOR("keyword","D97878","Lacquer red keywords"), COLOR("type","72C7A5","Jade types"),
 COLOR("string","D8B76A","Ceremonial gold strings"), COLOR("number","BDA6DE","Soft amethyst numeric literals"),
 COLOR("comment","969F9B","Ink-wash gray comments"), COLOR("function","9FCB9A","Garden green functions"),
 COLOR("operator","D6DBD7","Pale ink operators"), COLOR("constant","E59B83","Vermilion constants"),
 COLOR("preprocessor","D97878","Lacquer preprocessor directives"), COLOR("selection","3A4541","Jade-charcoal selection surface"),
 COLOR("diagnostic","FF7777","Clear diagnostic red")
};
typedef struct ThemeEntry { const char *id; const SleelaThemeColor *colors; unsigned int count; } ThemeEntry;
#define ENTRY(id, arr) {id, arr, (unsigned int)(sizeof(arr)/sizeof((arr)[0]))}
static const ThemeEntry themes[] = {
 ENTRY("american", american), ENTRY("eurasia", eurasia), ENTRY("russian-slavic", russian_slavic),
 ENTRY("silk-road", silk_road), ENTRY("east-asian", east_asian)
};
const char *sleela_theme_names(unsigned int *count) {
 static const char *names[] = {"american", "eurasia", "russian-slavic", "silk-road", "east-asian"};
 if (count) *count = (unsigned int)(sizeof names / sizeof names[0]);
 return names[0];
}
const SleelaThemeColor *sleela_theme_palette_for(const char *theme, unsigned int *count) {
 unsigned int i;
 if (!theme || !*theme) theme = "american";
 for (i=0; i<(unsigned int)(sizeof themes/sizeof themes[0]); ++i) {
  if (strcmp(theme, themes[i].id)==0) {
   if (count) *count=themes[i].count;
   return themes[i].colors;
  }
 }
 if (count) *count=0;
 return NULL;
}
const SleelaThemeColor *sleela_theme_find_for(const char *theme, const char *name) {
 unsigned int i,n;
 const SleelaThemeColor *p;
 if (!name || !*name) return NULL;
 p=sleela_theme_palette_for(theme,&n);
 for(i=0;p && i<n;i++) if(strcmp(name,p[i].name)==0) return &p[i];
 return NULL;
}
const SleelaThemeColor *sleela_theme_palette(unsigned int *count) { return sleela_theme_palette_for("american",count); }
const SleelaThemeColor *sleela_theme_find(const char *name) { return sleela_theme_find_for("american",name); }
int sleela_theme_print_terminal_for(const char *theme) {
 unsigned int i,n; const SleelaThemeColor *p=sleela_theme_palette_for(theme,&n);
 if (!p) { fprintf(stderr,"Unknown SLeeLa theme: %s\n",theme ? theme : "(null)"); return 2; }
 for(i=0;i<n;i++){ unsigned int r,g,b;
  if(sscanf(p[i].hex,"%2x%2x%2x",&r,&g,&b)!=3) return 1;
  printf("\033[38;2;%u;%u;%um%-14s  #%s  %s\033[0m\n",r,g,b,p[i].name,p[i].hex,p[i].role);
 }
 return 0;
}
int sleela_theme_print_terminal(void) { return sleela_theme_print_terminal_for("american"); }
