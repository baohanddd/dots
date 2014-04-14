// OCRT.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


/*
int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2) {
		printf("Usage: OCRT.exe command [param1]...\n");
		printf("diff...\n");
		return 1;
	}

	if (_tcscmp(argv[1], L"diff") == 0 && argc == 4) {
		RGB_diff(argv[2], argv[3]);
		return 0;
	}

	if (_tcscmp(argv[1], L"2rgb") == 0 && argc == 4) {
		BMP2RGB(argv[2], argv[3]);
		return 0;
	}


	if (_tcscmp(argv[1], L"matrix") == 0 && argc == 6) {
		BMP2DotMatrix(argv[2], argv[3], argv[4], argv[5]);
		return 0;
	}

	printf("Usage: OCRT.exe command [param1]...\n");
	return 0;
}
*/

void RGB_diff(_TCHAR *rgb1, _TCHAR *rgb2) {
	RGB color1 = _tchar2RGB(rgb1);
	RGB color2 = _tchar2RGB(rgb2);

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
		wprintf(L"Can not open file: %s", bmp);
		return;
	}
	getPallet(pal, fp);

	fclose(fp);

	if (_wfopen_s(&fp, au, L"w") > 0) {
		wprintf(L"Can not open file: %s", au);
		return;
	}

	rgb2file(pal, fp);

	freePallet(pal);
	fclose(fp);

	printf("Program finished.\n");
}

extern DOTS_API void BMP2DotMatrix(_TCHAR* bmp, _TCHAR* op, _TCHAR* hex, _TCHAR* variation) {
	const unsigned int NAME_LEN = 200;
	PALLET pal;
	Matrix dm, odm, block;
	Fonts fonts;
	DotMatrixPot start, corner, end;
	start.r = start.c = 0;
	FontSize fs, *size = &fs; size->h = size->w = 13;
	FILE* fp;
	size_t found;
	DotMatrixRange range, *head, *node;
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

	thresholding(&pal, &_tchar2RGB(hex), tolerance);

	if (getMatrix(&pal, &dm) != 0) {
		printf("Can not read dot matrix from pallet\n");
		return;
	}

	freePallet(&pal);
	fclose(fp);

	if (_wfopen_s(&fp, op, L"w") > 0) {
		wprintf(L"Can not open file: %s\n", op);
		return;
	}

	head = find(&dm, &start, size);
	node = head;
	while (node != NULL) {
		block = carve(&dm, node);
		print(&block);
		mat_hex = matrix2hex(&block);
		printf("%s\n", mat_hex);
		printf("Please input a name to describe the matrix:\n");
		gets_s(name, NAME_LEN);
		write(fp, mat_hex, name);
		freeMatrix(&block);
		node = node->next;
	}

	freeLink(head);
	freeMatrix(&dm);
	fclose(fp);

	printf("Program finished.\n");
	return;

ERR:
	printf("Fails to carve.\n");
}