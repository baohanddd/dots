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

/*
typedef struct {
DOTMATRIX* map;
} Font;
*/
// typedef DOTMATRIX Font;

int
carve(Matrix* dm, FontSize* size, Matrix* fonts, size_t* found);
static int dmpCmp(DotMatrixPot*, DotMatrixPot*);
static size_t width(DotMatrixRange* range);
static size_t count(DotMatrixRange* range, FontSize *size);
DotMatrixRange carveRange(Matrix* dm, DotMatrixPot* start, FontSize *size);
Matrix* carveFont(Matrix *dm, DotMatrixRange* range, FontSize *size, size_t* found);
static int DotMatrix(DotMatrixPot* corner, DotMatrixPot* bottom, const Matrix* dm, Matrix* odm);
static void dmpScanLH(Matrix *dm, DotMatrixPot* start);
static DotMatrixPot dmpScanLV(const Matrix *dm, DotMatrixPot* start, const FontSize* size);
static DotMatrixPot dmpScanRV(Matrix *dm, const DotMatrixPot* start, const FontSize* size);

static void print(DotMatrixPot *pot);
static void print(DotMatrixRange *range);
static void print(Matrix *mat);

#endif