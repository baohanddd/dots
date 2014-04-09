#ifndef _BMP_
#define _BMP_

typedef struct {
	BYTE blue;
	BYTE green;
	BYTE red;
} RGB;

typedef struct {
	BITMAPFILEHEADER header;
	BITMAPINFO *info;
	RGB *colors;
} PALLET;

FILE* fopen(wchar_t *filename);

int getPallet(PALLET* pal, FILE* fp);

int getHeader(BITMAPFILEHEADER *header, FILE* fp);

int isBMP(BITMAPFILEHEADER* header);

int getInfo(PALLET* pal, FILE* fp);

int getColors(PALLET* pal, FILE* fp);

int freePallet(PALLET* pal);

int	/* O - 0 = success, -1 = failure */
saveBMP2file(const _TCHAR *filename, PALLET *pal);



RGB _tchar2RGB(_TCHAR*);
int _hex2int(const TCHAR *value);

int						/* O - 0 = Out of fault tolerant, 1 = Inside of fault tolerant */
color_cmp(RGB *one,		/* I - One color */
RGB	*other,				/* I - Other color */
double variation);		/* I - fault tolerant */

int getMatrix(const PALLET *pal, Matrix *dm);
void freeMatrix(Matrix *dm);
void dotmat2File(const Matrix *dm, FILE* fp);

int matrixFromFile(wchar_t *filename, Matrix *mat);

#endif