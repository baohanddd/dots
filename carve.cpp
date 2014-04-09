#include "stdafx.h"

int
carve(Matrix* dm, FontSize* size, Fonts *fonts, size_t* found)
{
	DotMatrixPot     pot = { 0, 0 }, prev = pot, *start = &pot;
	DotMatrixRange  dotMatrixRange;
	DotMatrixRange **ranges;
	size_t n_range = 100, c_range = 0;

	if ((ranges = (DotMatrixRange**)malloc(sizeof(DotMatrixRange*)*n_range)) == NULL) return 3;

	do {
		prev = *start;
		dotMatrixRange = carveRange(dm, start, size);
		if (n_range == c_range) { printf("The number of blocks of range has reached maximum\n"); return 4; }
		ranges[c_range++] = &dotMatrixRange;
	} while (dmpCmp(&prev, start));

	for (size_t i = 0; i < c_range; ++i) {
		fonts = &carveFont(dm, ranges[i], size);
		free(ranges[i]);
	}

	free(ranges);
	return 0;
}

static int
dmpCmp(DotMatrixPot* p1, DotMatrixPot* p2) {
	return (p1->r != p2->r) || (p2->c != p2->c);
}

DotMatrixRange
carveRange(Matrix* dm, DotMatrixPot* start, FontSize *size)
{
	DotMatrixRange range;

	dmpScanLH(dm, start);
	range.cpl = dmpScanLV(dm, start, size);
	range.cpr = dmpScanRV(dm, start, size);
	return range;
}

Fonts
carveFont(const Matrix *dm, DotMatrixRange* range, const FontSize *size)
{
	Fonts fonts; fonts.num = 0;
	Matrix matrix, *font = &matrix;
	DotMatrixPot pot;
	size_t n = count(range, size), c;
	
	if (n <= 0) return fonts;
	if ((fonts.mat = (Matrix**)malloc(sizeof(Matrix*)*n)) == NULL) return fonts;
	pot.c = range->cpl.c;
	pot.r = range->cpr.r;

	for (c = 0; pot.c < range->cpr.c; ++c, pot.c += size->w) {
		if (DotMatrix(&pot, &(range->cpr), dm, font) != 0) goto CREATE_DOTMATRIX;
		fonts.mat[c] = font;
		++fonts.num;
	}

	return fonts;

CREATE_DOTMATRIX:
	printf("Fails to create dot matrix.\n");
	return fonts;
}

static int
DotMatrix(DotMatrixPot* corner, DotMatrixPot* bottom, const Matrix* dm, Matrix* odm)
{
	odm->r = bottom->r - corner->r;
	odm->c = bottom->c - corner->c;
	size_t* rows;
	size_t i, j = 0, m = 0, n, stop = corner->r + odm->r;

	if ((odm->map = (size_t**)malloc(sizeof(size_t*)* odm->r)) == NULL) return 3;

	for (i = corner->r, n = 0; i < stop; ++i, n = 0) {
		if ((rows = (size_t*)malloc(sizeof(size_t)* odm->c)) == NULL) return 3;
		for (j = corner->c; j < bottom->c; ++j) rows[n++] = dm->map[i][j];
		odm->map[m++] = rows;
	}
	return 0;
}

static void
dmpScanLH(Matrix *dm, DotMatrixPot* start)	// scan dot matrix to left by horizontal
{
	for (; start->r < dm->r; ++start->r) {
		for (; start->c < dm->c; ++start->c) { if (dm->map[start->r][start->c] == 1)  return; }
		start->c = 0;
	}
}

static DotMatrixPot
dmpScanLV(const Matrix *dm, DotMatrixPot* start, const FontSize* size)
{
	DotMatrixPot pot; pot.c = 0; pot.r = 0;
	size_t i = start->r, j = start->c, stop = size->h + start->r;
	if (dm->r < stop || dm->c == start->c) return pot;

	printf("i = %d, j = %d, stop = %d\n", i, j, stop);

	for (i = start->r; i < stop; ++i) {
		if (j == 0) break;
		if (dm->map[i][j] == 1) { --j; i = start->r; }
		printf("i = %d, j = %d, stop = %d\n", i, j, stop);
	}
	printf("i = %d, j = %d, stop = %d\n", i, j, stop);
	start->c = ++j;
	printf("start->r = %d, start->c = %d\n", start->r, start->c);
	pot.r = start->r;
	pot.c = start->c;
	return pot;
}

static DotMatrixPot
dmpScanRV(const Matrix *dm, const DotMatrixPot* start, const FontSize* size)
{
	DotMatrixPot pot;
	size_t limit = 0;
	pot.c = pot.r = 0;
	size_t i, j = start->c, stop = size->h + start->r;
	if (dm->r < stop || dm->c == start->c) return pot;

	do {
		for (i = start->r; i < stop; ++i) {
			if (j == dm->c - 1) goto ENDMAT;
			if (dm->map[i][j] == 1) { ++j; i = start->r; }
		}
		++limit; ++j; i = start->r;
	} while (limit < 3);

ENDMAT:
	pot.r = start->r + size->h;
	pot.c = j - limit;
	return pot;
}

static size_t
count(const DotMatrixRange* range, const FontSize *size)
{
	size_t c = width(range) / size->w;
	return c;
}

static size_t
width(const DotMatrixRange* range)
{
	size_t w = range->cpr.c - range->cpl.c;
	if (w < 0) w = 0;
	return w;
}


static void
print(DotMatrixPot *pot)
{
	printf("start->r = %d, start->c = %d\n", pot->r, pot->c);
}

static void
print(DotMatrixRange *range)
{
	printf("Range->cpl:\n");
	print(&range->cpl);
	printf("Range->cpr:\n");
	print(&range->cpr);
}

static void
print(Matrix *mat)
{
	printf("mat->c = %d\n", mat->c);
	printf("mat->r = %d\n", mat->r);

	for (size_t i = 0; i < mat->r; ++i) {
		for (size_t j = 0; j < mat->c; ++j) printf("%d", mat->map[i][j]);
		printf("\n");
	}
}

void
free(Fonts fonts)
{
	for (size_t i = 0; i < fonts.num; ++i) free(fonts.mat[i]);
	free(fonts.mat);
}

void 
write(FILE* fp, const Fonts fonts)
{
	Matrix *dm;

	for (size_t m = 0; m < fonts.num; ++m) {
		dm = fonts.mat[m];
		for (size_t i = 0; i < dm->r; ++i) {
			for (size_t j = 0; j < dm->c; ++j) fprintf(fp, "%d", dm->map[i][j]);
			fprintf(fp, "\n");
		}
	}
}