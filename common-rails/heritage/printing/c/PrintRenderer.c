#include "PrintRenderer.h"
#include "PrintLayout.h"
#include "PrintGlyphs.h"
void cr_render_square(FILE*o,int f){if(f<0)f=0;if(f>CR_PROGRESS_CELLS)f=CR_PROGRESS_CELLS;for(int r=0;r<CR_PROGRESS_SIDE;r++){for(int c=0;c<CR_PROGRESS_SIDE;c++){int fr=20-r,fc=20-c,idx=fr*21+fc;fputs(idx<f?CR_GLYPH_FULL:CR_GLYPH_EMPTY,o);}fputc('\n',o);}}
