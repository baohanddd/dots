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

char* mat2hex(const Matrix *mat)
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

/*
int getMatrix(const PALLET *pal, Matrix *dm) {
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
*/

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

Matrix hex2mat(char hex[], const FontSize *size)
{
	size_t idx;
	Matrix mat = matrix(size->h, size->w);
	BinaryString bs = hex2sbin(hex);
	// printf("bs->sbin = %s\n", bs->sbin);
	for (size_t i = 0; i < size->h; ++i) {
		for (size_t j = 0; j < size->w; ++j) {
			idx = i*size->w + j;
			if (idx == bs.len - 1) goto RETURNED;
			mat.map[i][j] = (bs.sbin[idx] == '0') ? 0 : 1;
			// printf("mat.map[%d][%d] = %d\n", i, j, mat.map[i][j]);
		}
	}
RETURNED:
	return mat;
}

static BinaryString hex2sbin(char hex[])
{
	BinaryString bs;
	char c, *tmp;
	bs.len = 0;
	bs.sbin = string(strlen(hex) * 4 + 1);

	while ((c = *(hex++)) != '\0') {
		tmp = hex2bin(c);
		while (*tmp != '\0') bs.sbin[bs.len++] = *tmp++;
	}

	bs.sbin[bs.len++] = '\0';
	// printf("%s\n", bs.sbin);

	return bs;
}

static char* hex2bin(char hex)   /* Function to convert hexadecimal to binary. */
{
	char bin[5], *binary = bin;
	if (hex == '0') binary = "0000\0";
	if (hex == '1') binary = "0001\0";
	if (hex == '2') binary = "0010\0";
	if (hex == '3') binary = "0011\0";
	if (hex == '4') binary = "0100\0";
	if (hex == '5') binary = "0101\0";
	if (hex == '6') binary = "0110\0";
	if (hex == '7') binary = "0111\0";
	if (hex == '8') binary = "1000\0";
	if (hex == '9') binary = "1001\0";
	if (hex == 'A' || hex == 'a') binary = "1010\0";
	if (hex == 'B' || hex == 'b') binary = "1011\0";
	if (hex == 'C' || hex == 'c') binary = "1100\0";
	if (hex == 'D' || hex == 'd') binary = "1101\0";
	if (hex == 'E' || hex == 'e') binary = "1110\0";
	if (hex == 'F' || hex == 'f') binary = "1111\0";

	return binary;
}