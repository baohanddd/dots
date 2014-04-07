#include "stdafx.h"

/*
int _tmain(int argc, _TCHAR* argv[])
{
	char hex[100];
	FontSize size;
	Matrix mat;
	BinaryString bs;

	size.w = size.h = 12;

	scanf_s("%s", hex, _countof(hex));
	printf("hex: %s\n", hex);
	bs = hex2sbin(hex);
	mat = sbin2mat(&bs, &size);
	printMatrix(mat);
	free(bs.sbin);
	free(mat.map);

	return 0;
}
*/

extern DOTS_API Matrix hex2mat(char hex[], FontSize *size)
{
	BinaryString bs = hex2sbin(hex);
	return sbin2mat(&bs, size);
}

Matrix sbin2mat(BinaryString *bs, FontSize *size)
{
	size_t idx;
	Matrix mat = matrix(size->h, size->w);
	// printf("bs->sbin = %s\n", bs->sbin);
	for (size_t i = 0; i < size->h; ++i) {
		for (size_t j = 0; j < size->w; ++j) {
			idx = i*size->w + j;
			if (idx == bs->len - 1) goto RETURNED;
			mat.map[i][j] = (bs->sbin[idx] == '0') ? 0 : 1;
			// printf("mat.map[%d][%d] = %d\n", i, j, mat.map[i][j]);
		}
	}
RETURNED:
	return mat;
}

BinaryString hex2sbin(char hex[])
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

char* hex2bin(char hex)   /* Function to convert hexadecimal to binary. */
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

char* string(size_t len)
{
	char *s;
	if ((s = (char*)malloc(sizeof(char)*len)) == NULL) return NULL;
	return s;
}

Matrix matrix(size_t r, size_t c)
{
	Matrix mat;

	mat.r = r;
	mat.c = c;
	mat.map = arr2d(r, c);

	return mat;
}

extern DOTS_API void printMatrix(Matrix mat)
{
	for (size_t i = 0; i < mat.r; ++i) {
		for (size_t j = 0; j < mat.c; ++j) printf("%d", mat.map[i][j]);
		printf("\n");
	}
}

size_t** arr2d(size_t m, size_t n)
{
	size_t **map, *row;

	if ((map = (size_t**)malloc(sizeof(size_t*)*m)) == NULL) return NULL;
	for (size_t i = 0; i < m; ++i) {
		if ((row = (size_t*)malloc(sizeof(size_t)*n)) == NULL) return NULL;
		memset(row, 0, sizeof(size_t)*n);
		map[i] = row;
	}
	return map;
}