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

int free(PALLET* pal);

/* O - 0 = success, -1 = failure */
int	saveBMP2file(const _TCHAR *filename, PALLET *pal);

RGB wchar2RGB(const _TCHAR*);

static int str2hex(const TCHAR *value);

/* O - 0 = Out of fault tolerant, 1 = Inside of fault tolerant */
int	colorCmp(RGB *one, RGB	*other, double variation);

/* Convert bmp into an .au file used to searching in FastFind */
void rgb2file(PALLET *pal, FILE *fp);

void thresholding(PALLET* pal, RGB *base, double variation);

void dotmatrix(PALLET* pal, FILE* fp);	

#endif