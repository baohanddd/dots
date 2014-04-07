#include "stdafx.h"

float 
matrixCmp(Matrix *mat1, Matrix *mat2)
{
	size_t same = 0;
	float percent = 0.0;
	if (mat1->c != mat2->c || mat1->r != mat2->r) return percent;
	for (size_t i = 0; i < mat1->r; ++i) {
		for (size_t j = 0; j < mat1->c; ++j) {
			if (mat1->map[i][j] == mat2->map[i][j]) same++;
		}
	}
	percent = same / (mat1->c * mat1->r);
	return percent;
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