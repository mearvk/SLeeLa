#include "PrintRenderer.h"
#include "PrintGlyphs.h"
void cr_render_square(FILE*o,int f){if(f<0)f=0;if(f>441)f=441;for(int r=0;r<21;r++){for(int c=0;c<21;c++){int i=(20-r)*21+(20-c);fputs(i<f?CR_GLYPH_FULL:CR_GLYPH_EMPTY,o);}fputc('\n',o);}}
