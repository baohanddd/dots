#ifndef __DOTS__
#define __DOTS__

#ifdef DOTS_EXPORTS
#define DOTS_API __declspec(dllexport) 
#else
#define DOTS_API __declspec(dllimport) 
#endif

typedef struct {
	size_t c;
	size_t r;
	size_t** map;
} Matrix;

typedef struct {
	size_t w;
	size_t h;
} FontSize;

extern DOTS_API void say_hello();
extern DOTS_API int initMatrix(Matrix *mat, size_t m, size_t n);
extern DOTS_API Matrix hex2mat(char hex[], FontSize *size);
extern DOTS_API void printMatrix(Matrix mat);

extern DOTS_API int ocrFromBMP(wchar_t *filename);
extern DOTS_API void BMP2DotMatrix(_TCHAR* bmp, _TCHAR* op, _TCHAR* hex, _TCHAR* variation);

#endif	// __DOTS__