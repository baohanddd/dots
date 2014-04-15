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
extern DOTS_API void BMP2DotMatrix(_TCHAR* bmp, _TCHAR* op, _TCHAR* hex, _TCHAR* variation);

void RGB_diff(_TCHAR *rgb1, _TCHAR *rgb2);
void BMP2RGB(_TCHAR *, _TCHAR *);

#endif	// __DOTS__