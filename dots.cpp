// dots.c : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

extern DOTS_API void say_hello() {
	printf("Hello from dll!\n");
}

extern DOTS_API int ocrFromBMP(wchar_t *filename)
{
	FILE file, *fp = &file;
	PALLET pallet, *pal = &pallet;
	Matrix matrix, *mat = &matrix;
	if (_wfopen_s(&fp, filename, L"rb") == 0) {
		wprintf(L"Can not open %s", filename);
		return 3;
	}
	if (getPallet(pal, fp) > 0) {
		wprintf(L"Can not get pallet from %s", filename);
		return 3;
	}
	fclose(fp); 
	getMatrix(pal, mat);
	freePallet(pal);



	return 0;
}