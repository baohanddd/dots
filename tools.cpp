#include "stdafx.h"

void
rgb2file(PALLET* pal, FILE* fp) {
	RGB* rgb;
	size_t points = pal->info->bmiHeader.biSizeImage / sizeof(RGB);
	for (size_t i = 0; i < points; ++i) {
		rgb = &pal->colors[i];
		fprintf(fp, "FFAddColor(0x%02x%02x%02x)\n", rgb->red, rgb->green, rgb->blue);
	}
}

void
thresholding(PALLET* pal,		/* I - pallet consist with many colors */
RGB *base,						/* I - base color */
double variation)				/* I - fault tolerant */
{
	RGB *other;
	printf("variation = %f\n", variation);
	size_t points = pal->info->bmiHeader.biSizeImage / sizeof(RGB);
	for (size_t i = 0; i < points; ++i) {
		other = &pal->colors[i];
		color_cmp(base, other, variation) == 1 ?
			other->blue = other->green = other->red = 0 :
			other->blue = other->green = other->red = 255;
	}
}

void
dotmatrix(PALLET* pal, FILE* fp)
{
	RGB *color;
	size_t points = pal->info->bmiHeader.biSizeImage / sizeof(RGB);
	size_t rows = points / pal->info->bmiHeader.biWidth;
	size_t i;
	size_t r = 0;

	for (size_t m = rows; m != 0; --m) {
		for (size_t n = 0; n < pal->info->bmiHeader.biWidth; ++n) {
			i = (m - 1) * pal->info->bmiHeader.biWidth + n;
			color = &pal->colors[i];
			if (color->blue == 0) fprintf(fp, "1");
			else fprintf(fp, "0");
			if (++r % pal->info->bmiHeader.biWidth == 0) fprintf(fp, "\n");
		}
	}
}