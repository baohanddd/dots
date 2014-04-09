#ifndef __CARVE__
#define __CARVE__

typedef struct {
	size_t r;	// row
	size_t c;	// col
} DotMatrixPot;

typedef struct {
	DotMatrixPot top;
	DotMatrixPot bottom;
} DotMatrixRow;

typedef struct {
	DotMatrixPot cpl;	// top left
	DotMatrixPot cpr;	// bottom right
} DotMatrixRange;

typedef struct {
	size_t w;	// width
	size_t h;	// height
} FontSize;

/*
typedef struct {
DOTMATRIX* map;
} Font;
*/
typedef DOTMATRIX Font;

int
carve(DOTMATRIX* dm, FontSize* size, Font* fonts, size_t* found);
static int dmpCmp(DotMatrixPot*, DotMatrixPot*);
static size_t width(DotMatrixRange* range);
static size_t count(DotMatrixRange* range, FontSize *size);
static int carveRange(DOTMATRIX* dm, DotMatrixPot* start, FontSize *size, DotMatrixRange* range);
static int carveFont(DOTMATRIX *dm, DotMatrixRange* range, Font* fonts, FontSize *size, size_t* found);
static int DotMatrix(DotMatrixPot* corner, DotMatrixPot* bottom, const DOTMATRIX* dm, DOTMATRIX* odm);
static void dmpScanLH(DOTMATRIX *dm, DotMatrixPot* start);
static DotMatrixPot dmpScanLV(DOTMATRIX *dm, DotMatrixPot* start, const FontSize* size);
static DotMatrixPot dmpScanRV(DOTMATRIX *dm, const DotMatrixPot* start, const FontSize* size);

#endif