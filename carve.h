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
	Matrix **mat;
	size_t num;
} Fonts;

// typedef DOTMATRIX Font;

int
carve(Matrix* dm, FontSize* size, Matrix* fonts, size_t* found);
static int dmpCmp(DotMatrixPot*, DotMatrixPot*);
static size_t width(const DotMatrixRange* range);
static size_t count(const DotMatrixRange* range, const FontSize *size);
DotMatrixRange carveRange(Matrix* dm, DotMatrixPot* start, FontSize *size);
Fonts carveFont(const Matrix *dm, DotMatrixRange* range, const FontSize *size);
static int DotMatrix(DotMatrixPot* corner, DotMatrixPot* bottom, const Matrix* dm, Matrix* odm);
static void dmpScanLH(Matrix *dm, DotMatrixPot* start);
static DotMatrixPot dmpScanLV(const Matrix *dm, DotMatrixPot* start, const FontSize* size);
static DotMatrixPot dmpScanRV(const Matrix *dm, const DotMatrixPot* start, const FontSize* size);

void write(FILE*, const Fonts);

static void print(DotMatrixPot *pot);
static void print(DotMatrixRange *range);
static void print(Matrix *mat);

void free(Fonts fonts);

#endif