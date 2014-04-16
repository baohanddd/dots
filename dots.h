#ifndef __DOTS__
#define __DOTS__

#ifdef DOTS_EXPORTS
#define DOTS_API __declspec(dllexport) 
#else
#define DOTS_API __declspec(dllimport) 
#endif

typedef struct {
	BYTE blue;
	BYTE green;
	BYTE red;
} RGB;

typedef struct Matrix {
	size_t c;
	size_t r;
	size_t **map;
	char *name;
	Matrix *next;
} Matrix;

typedef struct {
	size_t w;
	size_t h;
} FontSize;

typedef struct {
	char *sbin;
	size_t len;
} BinaryString;

typedef struct {
	size_t r;	// row
	size_t c;	// col
} DotMatrixPot;

typedef struct {
	DotMatrixPot top;
	DotMatrixPot bottom;
} DotMatrixRow;

typedef struct DotMatrixRange {
	DotMatrixPot cpl;	// top left
	DotMatrixPot cpr;	// bottom right
	DotMatrixRange *next;
} DotMatrixRange;

typedef struct {
	BITMAPFILEHEADER header;
	BITMAPINFO *info;
	RGB *colors;
} PALLET;

extern DOTS_API void say_hello();
extern DOTS_API void BMP2DotMatrix(_TCHAR* bmp, _TCHAR* op, _TCHAR* hex, _TCHAR* variation);
extern DOTS_API void readFontLib(const _TCHAR* lib);

void RGB_diff(_TCHAR *rgb1, _TCHAR *rgb2);
void BMP2RGB(_TCHAR *, _TCHAR *);

// Print methods...
void print(DotMatrixPot *pot);
void print(DotMatrixRange *range);
void print(Matrix *mat);

// Save methods...
void write(FILE*, const Matrix);
void write(FILE*, const char *hex, const char *name);	// Record font hex string and font name into lib file

// Init methods...
size_t** arr2d(size_t m, size_t n);
size_t** arrrow(size_t m);
size_t* arrcol(size_t n);
char* string(size_t len);
Matrix matrix(size_t r, size_t c);
Matrix matrix(const PALLET *pal);
Matrix matrix(FILE*);
Matrix matrix(const char *line, size_t len, const FontSize *size);
FontSize fontSize(size_t w, size_t h);


// Free methods...
void free(Matrix *dm);

#endif	// __DOTS__