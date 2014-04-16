#ifndef __DOTS_MATRIX__
#define __DOTS_MATRIX__

// Find a font into image
DotMatrixPot compare(Matrix *image, Matrix *font);
static int next(DotMatrixPot *pot, const Matrix *img, const Matrix *font);

// Convert hex string into matrix
Matrix hex2mat(char hex[], const FontSize *size);
static BinaryString hex2sbin(char hex[]);
static char* hex2bin(char hex);

// Convert matrix into hex string
char* mat2hex(const Matrix *mat);
static char bin2hex(int n);


#endif	// __DOTS_MATRIX__