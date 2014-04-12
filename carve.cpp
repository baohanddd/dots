#include "stdafx.h"

Matrix 
carve(const Matrix *dm, const DotMatrixRange *range)
{
	Matrix block;
	size_t *row, n = 0, m = 0;
	block.r = height(range);
	block.c = width(range);
	block.map = arr2d(block.r, block.c);

	for(size_t i = range->cpl.r; i < range->cpr.r; ++i, n = 0) {
		if((row = (size_t*)malloc(sizeof(size_t)*block.c)) == NULL) {
			printf("Can not create matrix in carve().\n");
			return block;
		}
		for(size_t j = range->cpl.c; j < range->cpr.c; ++j) row[n++] = dm->map[i][j];
		block.map[m++] = row;
	}
	return block;
}

DotMatrixRange*
find(Matrix* dm, DotMatrixPot* start, FontSize *size)
{
	DotMatrixRange range, *head = NULL, *current, *last = &range;

	do {
		if (carveRange(dm, &range, start, size) > 0) break;

		current = (DotMatrixRange*)malloc(sizeof(DotMatrixRange));
		current->cpl = range.cpl;
		current->cpr = range.cpr;
		current->next = NULL;

		if (head == NULL) head = current;
		else last->next = current;
		last = current;

	} while(1);

	printf("Output ranges:\n");
	print(head);
	return head;
}

static BOOL
isBottom(const Matrix *dm, const DotMatrixPot *pot, const FontSize *size)
{
	if (dm->r <= pot->r + size->h) return TRUE;
	return FALSE;
}

static int
carveRange(const Matrix* dm, DotMatrixRange *range, DotMatrixPot *start, const FontSize *size)
{
	dmpScanLH(dm, start, size);
	if (!isBottom(dm, start, size)) {
		range->cpl = dmpScanLV(dm, start, size);
		range->cpr = dmpScanRV(dm, start, size);
		range->next = NULL;
		// Set point for next search...
		start->r = range->cpl.r;
		start->c = range->cpr.c;
		return 0;
	}
	return 1;	// already reached the bottom of matrix...
}

static void
dmpScanLH(const Matrix *dm, DotMatrixPot* start, const FontSize *size)	// scan dot matrix to left by horizontal
{
	printf("Start the point to found:\n");
	print(start);

	size_t col = start->c, row_limit;
	if(col > 0) { row_limit = start->r + size->h; if(row_limit > dm->r) row_limit = dm->r; }
	else row_limit = dm->r;

	for (; start->r < row_limit; ++start->r) {
		for (; start->c < dm->c; ++start->c) { 
			if (dm->map[start->r][start->c] == 1)  {
				printf("Found top: \n");
				print(start);
				return; 
			}
		}
		start->c = col;	// Just search the rest of area only...
	}
	if(isBottom(dm, start, size)) { start->c = dm->c; return; }		// Reached end of matrix...
	start->c = 0;
	printf("\n\nNo found anything:\n");
	print(start);
	dmpScanLH(dm, start, size);
}

static DotMatrixPot
dmpScanLV(const Matrix *dm, DotMatrixPot* start, const FontSize* size)
{
	DotMatrixPot pot; pot.c = 0; pot.r = 0;
	size_t i = start->r, j = start->c, stop = size->h + start->r;
	if (dm->r < stop || dm->c == start->c) return pot;

	// printf("i = %d, j = %d, stop = %d\n", i, j, stop);

	for (i = start->r; i < stop; ++i) {
		if (j == 0) break;
		if (dm->map[i][j] == 1) { --j; i = start->r; }
		// printf("i = %d, j = %d, stop = %d\n", i, j, stop);
	}
	// printf("i = %d, j = %d, stop = %d\n", i, j, stop);
	start->c = ++j;
	// printf("start->r = %d, start->c = %d\n", start->r, start->c);
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
			if (dm->map[i][j] == 1) { ++j; i = start->r; limit = 0; }
			// printf("i = %d, j = %d\n", i, j);
		}
		
		++limit; ++j; i = start->r;
		// printf("i = %d, j = %d, limit = %d\n", i, j, limit);
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

static size_t
height(const DotMatrixRange *range)
{
	size_t h = range->cpr.r - range->cpl.r;
	if(h < 0) h = 0;
	return h;
}

static void
print(DotMatrixPot *pot)
{
	printf("start->r = %d, start->c = %d\n", pot->r, pot->c);
}

static void
print(DotMatrixRange *range)
{
	DotMatrixRange *node;
	node = range;
	while (node != NULL) {
		printf("\nRange->cpl:\n");
		print(&node->cpl);
		printf("Range->cpr:\n");
		print(&node->cpr);
		node = node->next;
	}
	printf("\n");
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
freeLink(DotMatrixRange *head)
{
	DotMatrixRange *node, *next;
	node = head;
	while (node != NULL)
	{
		next = node->next;
		free(node);
		node = next;
	}
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

void
write(FILE *fp, const Matrix block)
{
	const Matrix *dm = &block;
	printf("mat->c = %d\n", dm->c);
	printf("mat->r = %d\n", dm->r);
	for (size_t i = 0; i < dm->r; ++i) {
		for (size_t j = 0; j < dm->c; ++j) fprintf(fp, "%d", dm->map[i][j]);
		fprintf(fp, "\n");
	}
}

static size_t** 
arr2d(size_t m, size_t n)
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