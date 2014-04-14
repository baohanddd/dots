#ifndef __DOTS_MATRIX__
#define __DOTS_MATRIX__

typedef struct {
	char *sbin;
	size_t len;
} BinaryString;

float matrixCmp(Matrix*, Matrix*);
void bin2hex(int n, char hex[]);
int createMatrix(Matrix *mat);

BinaryString hex2sbin(char hex[]);
char* hex2bin(char hex);
char* string(size_t len);
Matrix sbin2mat(BinaryString *bs, FontSize *size);
Matrix matrix(size_t r, size_t c);
size_t** arr2d(size_t m, size_t n);

char* matrix2hex(const Matrix *mat);
static char bin2hex(int n);

void write(FILE*, const char *hex, const wchar_t *name);


#endif	// __DOTS_MATRIX__