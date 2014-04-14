#include "stdafx.h"

DotMatrixPot 
compare(Matrix *img, Matrix *font)
{
	DotMatrixPot pot = { 0, 0 };
	size_t same = 0;
	float percent = 0.0;

	int notMatch = 0;

	size_t row_limit;
	size_t col_limit;

	if (img->c < font->c || img->r < font->r) return pot;

	while (next(&pot, img, font)) {
		notMatch = 0;
		row_limit = pot.r + font->r;
		col_limit = pot.c + font->c;
		for (size_t i = pot.r; i < row_limit; ++i) {
			if (notMatch == 1) break;
			for (size_t j = pot.c; j < col_limit; ++j) {
				if (img->map[i][j] != font->map[i][j]) { notMatch = 1; same = 0; break; }
				else same++;
			}
		}
	}

	percent = same / (font->c * font->r);
	if (percent < 0.9) pot.c = pot.r = 0;
	return pot;
}

static int
next(DotMatrixPot *pot, const Matrix *img, const Matrix *font)
{
	if ((pot->c + font->c) > img->c) { pot->c = 0; pot->r++; }
	else pot->c++;
	if ((pot->r + font->r) > img->r) return 0;
	return 1;
}

char* matrix2hex(const Matrix *mat)
{
	char sbin[4], *hex;
	size_t i, j, c = 0, k = 0;
	size_t len = mat->r * mat->c / 4 + 1;
	if ((hex = (char*)malloc(sizeof(char)*len)) == NULL) return NULL;

	for (i = 0; i < mat->r; ++i) {
		for (j = 0; j < mat->c; ++j) {
			sbin[k++] = mat->map[i][j] == 1 ? '1' : '0';
			if (k % 4 == 0) {
				hex[c++] = bin2hex(atoi(sbin));
				memset(sbin, 0, 4);
				k = 0;
			}
		}
	}
	hex[c++] = '\0';
	return hex;
}

int 
getMatrix(const PALLET *pal, Matrix *dm) {
	RGB *color;
	size_t points = pal->info->bmiHeader.biSizeImage / sizeof(RGB);
	size_t rows = points / pal->info->bmiHeader.biWidth;
	size_t i;
	size_t r = 0;
	size_t* row;
	if ((dm->map = (size_t**)malloc(sizeof(size_t*)* rows)) == NULL) return 3;
	dm->r = rows;
	dm->c = pal->info->bmiHeader.biWidth;

	for (size_t m = rows; m != 0; --m) {
		// init row...
		if ((row = (size_t*)malloc(sizeof(size_t)*pal->info->bmiHeader.biWidth)) == NULL) return 3;
		memset(row, 0, sizeof(size_t)* pal->info->bmiHeader.biWidth);

		for (size_t n = 0; n < pal->info->bmiHeader.biWidth; ++n) {
			i = (m - 1) * pal->info->bmiHeader.biWidth + n;
			color = &pal->colors[i];
			if (color->blue == 0) row[n] = 1;
		}
		dm->map[r++] = row;
	}
	return 0;
}

void
freeMatrix(Matrix *dm)
{
	for (size_t r = 0; r < dm->r; ++r) free(dm->map[r]);
	free(dm->map);
}

void
dotmat2File(const Matrix *dm, FILE* fp)
{
	for (size_t i = 0; i < dm->r; ++i) {
		for (size_t j = 0; j < dm->c; ++j) fprintf(fp, "%d", dm->map[i][j]);
		fprintf(fp, "\n");
	}
}

static char bin2hex(int n) /* Function to convert binary to hexadecimal. */
{
	char hex;
	int i = 0, decimal = 0, rem;
	while (n != 0)
	{
		decimal += (n % 10)*pow(2.0, i);
		n /= 10;
		++i;
	}

	/* At this point, variable decimal contains binary number in decimal format. */
	rem = decimal % 16;
	switch (rem)
	{
	case 0:
		hex = '0';
		break;
	case 1:
		hex = '1';
		break;
	case 2:
		hex = '2';
		break;
	case 3:
		hex = '3';
		break;
	case 4:
		hex = '4';
		break;
	case 5:
		hex = '5';
		break;
	case 6:
		hex = '6';
		break;
	case 7:
		hex = '7';
		break;
	case 8:
		hex = '8';
		break;
	case 9:
		hex = '9';
		break;
	case 10:
		hex = 'A';
		break;
	case 11:
		hex = 'B';
		break;
	case 12:
		hex = 'C';
		break;
	case 13:
		hex = 'D';
		break;
	case 14:
		hex = 'E';
		break;
	case 15:
		hex = 'F';
		break;
	}
	return hex;
}

extern DOTS_API int initMatrix(Matrix *mat, size_t m, size_t n)
{
	int i, j;
	size_t *row;
	mat->r = m; mat->c = n;
	if ((mat->map = (size_t**)malloc(sizeof(size_t*)* m)) == NULL) return 3;
	for (i = 0; i < m; ++i) {
		if ((row = (size_t*)malloc(sizeof(size_t)*n)) == NULL) return 3;
		memset(row, 0, sizeof(int)*n);
		for (j = 0; j < n; ++j) { row[j] = j % 2; }
		mat->map[i] = row;
	}
	return 0;
}

void write(FILE *fp, const char *hex, const char *name)
{
	fprintf(fp, "%s|%s\n", hex, name);
}