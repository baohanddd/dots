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

typedef struct DotMatrixRange {
	DotMatrixPot cpl;	// top left
	DotMatrixPot cpr;	// bottom right
	DotMatrixRange *next;
} DotMatrixRange;


typedef struct {
	Matrix **mat;
	size_t num;
} Fonts;

int carve(Matrix* dm, FontSize* size, Matrix* fonts, size_t* found);
Matrix carve(const Matrix*, const DotMatrixRange*);

static int dmpCmp(DotMatrixPot*, DotMatrixPot*);

DotMatrixRange* find(Matrix* dm, DotMatrixPot* start, FontSize *size);
DotMatrixRange carveRange(Matrix* dm, DotMatrixPot* start, FontSize *size);
Fonts carveFont(const Matrix *dm, DotMatrixRange* range, const FontSize *size);
static int DotMatrix(DotMatrixPot* corner, DotMatrixPot* bottom, const Matrix* dm, Matrix* odm);
static void dmpScanLH(const Matrix *dm, DotMatrixPot* start);
static DotMatrixPot dmpScanLV(const Matrix *dm, DotMatrixPot* start, const FontSize* size);
static DotMatrixPot dmpScanRV(const Matrix *dm, const DotMatrixPot* start, const FontSize* size);



void write(FILE*, const Fonts);
void write(FILE*, const Matrix);

static void print(DotMatrixPot *pot);
static void print(DotMatrixRange *range);
static void print(Matrix *mat);

static size_t width(const DotMatrixRange*);
static size_t height(const DotMatrixRange*);
static size_t count(const DotMatrixRange* range, const FontSize *size);

void free(Fonts fonts);

static size_t** arr2d(size_t m, size_t n);

#endif