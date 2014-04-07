#include "stdafx.h"

RGB _tchar2RGB(_TCHAR* hexstr) {
	RGB color;
	int hex = _hex2int(hexstr);
	color.red = ((hex >> 16) & 0xFF);    // Extract the RR byte
	color.green = ((hex >> 8) & 0xFF);   // Extract the GG byte
	color.blue = ((hex)& 0xFF);          // Extract the BB byte

	return color;
}

int _hex2int(const TCHAR *value)
{
	struct CHexMap
	{
		TCHAR chr;
		int value;
	};
	const int HexMapL = 16;
	CHexMap HexMap[HexMapL] =
	{
		{ '0', 0 }, { '1', 1 },
		{ '2', 2 }, { '3', 3 },
		{ '4', 4 }, { '5', 5 },
		{ '6', 6 }, { '7', 7 },
		{ '8', 8 }, { '9', 9 },
		{ 'A', 10 }, { 'B', 11 },
		{ 'C', 12 }, { 'D', 13 },
		{ 'E', 14 }, { 'F', 15 }
	};

	TCHAR *mstr = _tcsdup(value);
	// mstr = _wcsupr(mstr);
	TCHAR *s = mstr;
	int result = 0;
	if (*s == '0' && *(s + 1) == 'x') s += 2;
	BOOL firsttime = 1;
	while (*s != '\0')
	{
		BOOL found = 0;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*s == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = 1;
				break;
			}
		}
		if (!found) break;
		s++;
		firsttime = 0;
	}
	free(mstr);
	return result;
}

int						/* O - 0 = Out of fault tolerant, 1 = Inside of fault tolerant */
color_cmp(RGB *one,		/* I - One color */
RGB		*other,		/* I - Other color */
double	variation)  /* I - fault tolerant */
{
	double r = one->red - other->red;
	double g = one->green - other->green;
	double b = one->blue - other->blue;

	return sqrt(abs(pow(r, 2) + pow(g, 2) + pow(b, 2))) <= variation;
}