#ifndef __CARVE__
#define __CARVE__

DotMatrixRange* find(Matrix* dm, DotMatrixPot* start, FontSize *size, size_t *found);
Matrix carve(const Matrix*, const DotMatrixRange*);

static int carveRange(const Matrix* dm, DotMatrixRange *range, DotMatrixPot *start, const FontSize *size);
static BOOL isBottom(const Matrix *dm, const DotMatrixPot *pot, const FontSize *size);

static void dmpScanLH(const Matrix *dm, DotMatrixPot* start, const FontSize *size);
static DotMatrixPot dmpScanLV(const Matrix *dm, DotMatrixPot* start, const FontSize* size);
static DotMatrixPot dmpScanRV(const Matrix *dm, const DotMatrixPot* start, const FontSize* size);

static size_t width(const DotMatrixRange*);
static size_t height(const DotMatrixRange*);
static size_t count(const DotMatrixRange* range, const FontSize *size);

void freeLink(DotMatrixRange *head);

#endif