#include "stdafx.h"

int getPallet(PALLET* pal, FILE* fp) {
	if (getHeader(&pal->header, fp) == 0) return 3;
	if (isBMP(&pal->header) == 0) return 4;
	if (getInfo(pal, fp) == 0) return 5;
	if (getColors(pal, fp) == 0) return 6;
	return 0;
}

int getHeader(BITMAPFILEHEADER *header, FILE* fp) {
	if (fread(header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		printf("Can not read header from bmp...\n");
		return 0;
	}
	return 1;
}

int isBMP(BITMAPFILEHEADER* header) {
	if (header->bfType != 'MB') return 0;
	return 1;
}

int getInfo(PALLET* pal, FILE* fp) {
	size_t info_size = pal->header.bfOffBits - sizeof(BITMAPFILEHEADER);
	if ((pal->info = (BITMAPINFO *)malloc(info_size)) == NULL) {
		printf("Couldn't allocate memory for bitmap info\n");
		return 0;
	}

	if (fread(pal->info, 1, info_size, fp) < info_size) {
		printf("Couldn't read the bitmap header\n");
		return 0;
	}

	return 1;
}

int getColors(PALLET* pal, FILE* fp) {
	size_t bitsize = pal->info->bmiHeader.biSizeImage;
	size_t points = bitsize / sizeof(RGB);

	if ((pal->colors = (RGB *)malloc(bitsize)) == NULL) {
		printf("Couldn't allocate memory\n");
		return 0;
	}

	if (fread(pal->colors, sizeof(RGB), points, fp) < points) {
		printf("Couldn't read bitmap\n");
		return 0;
	}
	return 1;
}

int freePallet(PALLET* pal) {
	free(pal->info);
	free(pal->colors);
	return 0;
}

int										/* O - 0 = success, -1 = failure */
saveBMP2file(const _TCHAR *filename,	/* I - File to load */
PALLET *pal)							/* I - Bitmap information */
{
	FILE             *fp;				/* Open file pointer */
	int              size,				/* Size of file */
		infosize,						/* Size of bitmap info */
		bitsize;						/* Size of bitmap pixels */
	BITMAPFILEHEADER header;			/* File header */
	errno_t			 err;				/* Tag of err */

	/* Try opening the file; use "wb" mode to write this *binary* file. */
	err = _wfopen_s(&fp, filename, L"wb");
	if (err > 0)
		return (-1);

	/* Figure out the bitmap size */
	if (pal->info->bmiHeader.biSizeImage == 0)
		bitsize = (pal->info->bmiHeader.biWidth *
		pal->info->bmiHeader.biBitCount + 7) / 8 *
		abs(pal->info->bmiHeader.biHeight);
	else
		bitsize = pal->info->bmiHeader.biSizeImage;

	/* Figure out the header size */
	infosize = sizeof(BITMAPINFOHEADER);
	switch (pal->info->bmiHeader.biCompression)
	{
	case BI_BITFIELDS:
		infosize += 12; /* Add 3 RGB doubleword masks */
		if (pal->info->bmiHeader.biClrUsed == 0)
			break;
	case BI_RGB:
		if (pal->info->bmiHeader.biBitCount > 8 &&
			pal->info->bmiHeader.biClrUsed == 0)
			break;
	case BI_RLE8:
	case BI_RLE4:
		if (pal->info->bmiHeader.biClrUsed == 0)
			infosize += (1 << pal->info->bmiHeader.biBitCount) * 4;
		else
			infosize += pal->info->bmiHeader.biClrUsed * 4;
		break;
	}

	size = sizeof(BITMAPFILEHEADER)+infosize + bitsize;

	/* Write the file header, bitmap information, and bitmap pixel data... */
	header.bfType = 'MB'; /* Non-portable... sigh */
	header.bfSize = size;
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;
	header.bfOffBits = sizeof(BITMAPFILEHEADER)+infosize;

	if (fwrite(&header, 1, sizeof(BITMAPFILEHEADER), fp) < sizeof(BITMAPFILEHEADER))
	{
		/* Couldn't write the file header - return... */
		printf("Couldn't write the file header\n");
		fclose(fp);
		return (-1);
	}

	if (fwrite(pal->info, 1, infosize, fp) < infosize)
	{
		/* Couldn't write the bitmap header - return... */
		fclose(fp);
		printf("Couldn't write the bitmap header\n");
		return (-1);
	}

	size_t point = bitsize / sizeof(RGB);
	if (fwrite(pal->colors, sizeof(RGB), point, fp) < point)
	{
		/* Couldn't write the bitmap - return... */
		fclose(fp);
		printf("Couldn't write the bitmap\n");
		return (-1);
	}

	/* OK, everything went fine - return... */
	fclose(fp);
	printf("File write successfully\n");
	return (0);
}