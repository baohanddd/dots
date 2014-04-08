#ifndef _TOOLS_
#define _TOOLS_

/* Output rgb value into file */
void rgb2file(PALLET *pal, FILE *fp);

void
thresholding(PALLET* pal,		/* I - pallet consist with many colors */
RGB *base,					/* I - base color */
double variation);			/* I - fault tolerant */

void
dotmatrix(PALLET* pal,
FILE* fp);					/* I - pointer of output file */

// int


#endif