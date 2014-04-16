// dots.c : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

extern DOTS_API void say_hello() {
	printf("Hello from dll!\n");
}

void RGB_diff(_TCHAR *rgb1, _TCHAR *rgb2) {
	RGB color1 = wchar2RGB(rgb1);
	RGB color2 = wchar2RGB(rgb2);

	double r = color1.red - color2.red;
	double g = color1.green - color2.green;
	double b = color1.blue - color2.blue;

	double cv = sqrt(abs(pow(r, 2) + pow(g, 2) + pow(b, 2)));

	wprintf(L"The distance between %s and %s is %f\n", rgb1, rgb2, cv);
}

void BMP2RGB(_TCHAR *bmp, _TCHAR *au) {
	FILE* fp;
	PALLET p;
	PALLET* pal = &p;

	if (_wfopen_s(&fp, bmp, L"rb") > 0) {
		wprintf(L"Can not open file: %s\n", bmp);
		return;
	}
	getPallet(pal, fp);

	fclose(fp);

	if (_wfopen_s(&fp, au, L"w") > 0) {
		wprintf(L"Can not open file: %s\n", au);
		return;
	}

	rgb2file(pal, fp);

	free(pal);
	fclose(fp);

	printf("Program finished.\n");
}

extern DOTS_API void BMP2DotMatrix(_TCHAR* bmp, _TCHAR* op, _TCHAR* hex, _TCHAR* variation) {
	const unsigned int NAME_LEN = 200;
	PALLET pal;
	Matrix dm, block;
	DotMatrixPot start, corner, end;
	start.r = start.c = 0;
	FontSize fs, *size = &fs; size->h = size->w = 13;
	FILE* fp;
	size_t found = 0;
	DotMatrixRange *head, *node;
	char *mat_hex, name[NAME_LEN];

	wchar_t* stopword;
	double tolerance = wcstod(variation, &stopword);

	if (_wfopen_s(&fp, bmp, L"rb") > 0) {
		wprintf(L"Can not open file: %s\n", bmp);
		return;
	}

	if (getPallet(&pal, fp) > 0) {
		printf("Can not read pallet from bmp\n");
		return;
	}

	thresholding(&pal, &wchar2RGB(hex), tolerance);

	dm = matrix(&pal);

	free(&pal);
	fclose(fp);

	if (_wfopen_s(&fp, op, L"w") > 0) {
		wprintf(L"Can not open file: %s\n", op);
		return;
	}

	head = find(&dm, &start, size, &found);
	fwrite(&found, sizeof(size_t), 1, fp);	// block numbers
	node = head;
	while (node != NULL) {
		block = carve(&dm, node);
		print(&block);
		write(fp, block);
		//mat_hex = mat2hex(&block);
		//printf("%s\n", mat_hex);
		//printf("Please input a name to describe the matrix:\n");
		//gets_s(name, NAME_LEN);
		//write(fp, mat_hex, name);
		free(&block);
		node = node->next;
	}

	freeLink(head);
	free(&dm);
	fclose(fp);

	printf("Program finished.\n");
	return;

ERR:
	printf("Fails to carve.\n");

ERR_CANNOT_OPEN_FILE:
	fprintf(stderr, "Can not open E:\matrix.data\n");
	exit(3);
}

extern DOTS_API void readFontLib(const _TCHAR* lib)
{
	FILE *fp;
	Matrix mat;
	size_t total;

	if (_wfopen_s(&fp, lib, L"rb") > 0)  goto ERR_CANNOT_OPEN_FILE;
	if(fread(&total, sizeof(size_t), 1, fp) < 1) goto ERR_CANNOT_READ_TOTAL;
	while(total > 0) {
		matrix(fp);
		total--;
	}
	fclose(fp);
	return ;

ERR_CANNOT_OPEN_FILE:
	fwprintf(stderr, L"Can not open file: %s\n", lib);
	exit(3);
ERR_CANNOT_READ_TOTAL:
	perror("Can not read total from font lib.");
	exit(4);
}

void print(DotMatrixPot *pot)
{
	printf("start->r = %d, start->c = %d\n", pot->r, pot->c);
}

void print(DotMatrixRange *range)
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

void print(Matrix *mat)
{
	printf("mat->c = %d\n", mat->c);
	printf("mat->r = %d\n", mat->r);

	for (size_t i = 0; i < mat->r; ++i) {
		for (size_t j = 0; j < mat->c; ++j) printf("%d", mat->map[i][j]);
		printf("\n");
	}
}

void write(FILE *fp, const Matrix block)
{
	const Matrix *dm = &block;
	size_t *row;
	fwrite(&dm->r, sizeof(size_t), 1, fp);
	fwrite(&dm->c, sizeof(size_t), 1, fp);
	for (size_t i = 0; i < dm->r; ++i) {
		row = dm->map[i];
		fwrite(row, sizeof(size_t), dm->c, fp);
		//for (size_t j = 0; j < dm->c; ++j) fprintf(fp, "%d", dm->map[i][j]);
		//fprintf(fp, "\n");
	}
}

void write(FILE *fp, const char *hex, const char *name)
{
	fprintf(fp, "%s|%s\n", hex, name);
}

/* Initialize methods below... */
char* string(size_t len)
{
	char *s;
	if ((s = (char*)malloc(sizeof(char)*len)) == NULL) return NULL;
	return s;
}

Matrix matrix(FILE *fp)
{
	Matrix mat;
	size_t *row;

	if(fread(&mat.r, sizeof(size_t), 1, fp) < 1) goto ERR_CANNOT_READ_MATRIX;
	if(fread(&mat.c, sizeof(size_t), 1, fp) < 1) goto ERR_CANNOT_READ_MATRIX;
	mat.map = arrrow(mat.r);
	for(size_t i = 0; i < mat.r; ++i) {
		row = arrcol(mat.c);
		if(fread(row, sizeof(size_t), mat.c, fp) < mat.c) goto ERR_CANNOT_READ_MATRIX;
		mat.map[i] = row;
	}

	print(&mat);

	return mat;

ERR_CANNOT_READ_MATRIX:
	fclose(fp);
	fprintf(stderr, "Read matrix from file fails.\n");
	exit(3);
}

Matrix matrix(const PALLET *pal) {
	RGB *color;
	size_t points = pal->info->bmiHeader.biSizeImage / sizeof(RGB);
	size_t rows = points / pal->info->bmiHeader.biWidth;
	Matrix mat = matrix(rows, pal->info->bmiHeader.biWidth), *dm = &mat;
	size_t i;

	for (size_t m = rows; m != 0; --m) {
		for (size_t n = 0; n < pal->info->bmiHeader.biWidth; ++n) {
			i = (m - 1) * pal->info->bmiHeader.biWidth + n;
			color = &pal->colors[i];
			if (color->blue == 0) dm->map[rows - m][n] = 1;
		}
	}
	return mat;
}

Matrix matrix(const char line[], size_t len, const FontSize *size)
{
	Matrix mat;
	size_t c = 0, i = 0;
	char *hex = string(len), *name = string(len);
	int second = 0;

	for(i = 0; i < len; ++i, ++c) {
		if(line[i] == '|') break;
		hex[c] = line[i];
	}
	hex[c] = '\0';
	for(i += 1, c = 0; i < len; ++i, ++c) {
		if(line[i] == '\0') break;
		name[c] = line[i];
	}
	name[c] = '\0';

	mat = hex2mat(hex, size);
	mat.name = name;

	return mat;
}

Matrix matrix(size_t r, size_t c)
{
	Matrix mat;

	mat.r = r;
	mat.c = c;
	mat.map = arr2d(r, c);
	mat.name = NULL;
	mat.next = NULL;

	return mat;
}

size_t** arr2d(size_t m, size_t n)
{
	size_t **map, *row;

	if ((map = (size_t**)malloc(sizeof(size_t*)*m)) == NULL) goto ERR_CAN_NOT_ALLOCATE_MEM;
	for (size_t i = 0; i < m; ++i) {
		if ((row = (size_t*)malloc(sizeof(size_t)*n)) == NULL) goto ERR_CAN_NOT_ALLOCATE_MEM;
		memset(row, 0, sizeof(size_t)*n);
		map[i] = row;
	}
	return map;

ERR_CAN_NOT_ALLOCATE_MEM:
	fprintf(stderr, "Can not allocate any memory for create new matrix...");
	exit(3);
}

size_t** arrrow(size_t m)
{
	size_t ** map;
	if ((map = (size_t**)malloc(sizeof(size_t*)*m)) == NULL) goto ERR_CAN_NOT_ALLOCATE_MEM;
	return map;

ERR_CAN_NOT_ALLOCATE_MEM:
	fprintf(stderr, "Can not allocate any memory for create row...");
	exit(3);
}

size_t* arrcol(size_t n)
{
	size_t *row;
	if ((row = (size_t*)malloc(sizeof(size_t)*n)) == NULL) goto ERR_CAN_NOT_ALLOCATE_MEM;
	return row;

ERR_CAN_NOT_ALLOCATE_MEM:
	fprintf(stderr, "Can not allocate any memory for create column...");
	exit(3);
}

FontSize fontSize(size_t w, size_t h)
{
	FontSize size;
	size.w = w; size.h = h;
	return size;
}

/* Free methods below... */
void free(Matrix *dm)
{
	for (size_t r = 0; r < dm->r; ++r) free(dm->map[r]);
	free(dm->map);
}