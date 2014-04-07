#include "stdafx.h"

extern DOTS_API char* matrix2hex(const Matrix *mat)
{
	char sbin[4], *hex, bin[2];
	int i, j, c = 0, k = 0;
	if ((hex = (char*)malloc(sizeof(char)*mat->r*mat->c / 4 + sizeof(char))) == NULL) return NULL;

	for (i = 0; i < mat->r; ++i) {
		for (j = 0; j < mat->c; ++j) {
			sbin[k++] = mat->map[i][j] == 1 ? '1' : '0';
			if (k % 4 == 0) {
				bin2hex(atoi(sbin), bin);
				hex[c++] = bin[0];
				memset(sbin, 0, 4);
				k = 0;
			}
		}
	}
	hex[c++] = '\0';
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

void bin2hex(int n, char hex[]) /* Function to convert binary to hexadecimal. */
{
	int i = 0, decimal = 0, rem;
	while (n != 0)
	{
		decimal += (n % 10)*pow(2.0, i);
		n /= 10;
		++i;
	}

	/* At this point, variable decimal contains binary number in decimal format. */
	i = 0;
	while (decimal != 0)
	{
		rem = decimal % 16;
		switch (rem)
		{
		case 10:
			hex[i] = 'A';
			break;
		case 11:
			hex[i] = 'B';
			break;
		case 12:
			hex[i] = 'C';
			break;
		case 13:
			hex[i] = 'D';
			break;
		case 14:
			hex[i] = 'E';
			break;
		case 15:
			hex[i] = 'F';
			break;
		default:
			hex[i] = rem + '0';
			break;
		}
		++i;
		decimal /= 16;
	}
	hex[i] = '\0';
	_strrev(hex);       /* Function to reverse string. */
}