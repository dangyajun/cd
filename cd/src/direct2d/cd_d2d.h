/** \file
 * \brief Windows direct2d Base Driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_D2D_H
#define __CD_D2D_H

#define COBJMACROS
#define UNICODE
#define _UNICODE

#include <windows.h>                                                                                                     
#include <wincodec.h>

#include "dummy/d2d1.h"
#include "dummy/dwrite.h"

#define D2D_CANVASTYPE_DC           0
#define D2D_CANVASTYPE_HWND         1

#define CANVAS_DOUBLEBUFFER      0x0001
#define CANVAS_NOGDICOMPAT       0x0002
#define CANVAS_LAYOUTRTL         0x0004

#define D2D_CANVASFLAG_RECTCLIP     0x1
#define D2D_CANVASFLAG_RTL          0x2

#define D2D_BASEDELTA_X             0.5f
#define D2D_BASEDELTA_Y             0.5f

#define PI 3.14159265358979323846f

extern HMODULE d2d_cd_dll;
extern HMODULE wic_cd_dll;
extern HMODULE dwrite_cd_dll;

extern dummy_ID2D1Factory* d2d_cd_factory;
extern IWICImagingFactory* wic_cd_factory;
extern dummy_IDWriteFactory* dwrite_cd_factory;

typedef struct _d2dCanvas {
  WORD type;    /* D2D_CANVASTYPE_* */
  WORD flags;    /* D2D_CANVASFLAG_* */
  dummy_ID2D1RenderTarget* target;
  dummy_ID2D1Layer* clip_layer;
} d2dCanvas;


typedef struct _d2dFont {
  dummy_IDWriteTextFormat* tf;
  dummy_DWRITE_FONT_METRICS metrics;
} d2dFont;

typedef struct _d2dFontMetrics {
  float fEmHeight;        /* Typically height of letter 'M' or 'H' */
  float fAscent;          /* Height of char cell above the base line. */
  float fDescent;         /* Height of char cell below the base line. */
  float fLeading;         /* Distance of two base lines in multi-line text. */
                          /* Usually: fEmHeight < fAscent + fDescent <= fLeading */
} d2dFontMetrics;


void d2dStartup(void);
void d2dShutdown(void);

d2dCanvas* d2dCanvasCreate(dummy_ID2D1RenderTarget* target, WORD type, BOOL rtl);
void d2dResetTransform(dummy_ID2D1RenderTarget* target);
void d2dRotateWorld(dummy_ID2D1RenderTarget *target, float cx, float cy, float fAngle);
void d2dApplyTransform(dummy_ID2D1RenderTarget* target, const dummy_D2D1_MATRIX_3X2_F* matrix);
void d2dSetClip(d2dCanvas *canvas, const dummy_D2D1_RECT_F* pRect);
void d2dResetClip(d2dCanvas* c);

IWICBitmap* d2dCreateImageFromBufferRGB(UINT uWidth, UINT uHeight, const unsigned char *red, const unsigned char *green, const unsigned char *blue, const unsigned char *alpha);
IWICBitmap* d2dCreateImageFromBufferMap(UINT uWidth, UINT uHeight, const unsigned char *map, const long* cPalette);
void d2dDestroyImage(IWICBitmap *image);
void d2dBitBltImage(dummy_ID2D1RenderTarget *target, IWICBitmap *bitmap, const dummy_D2D1_RECT_F* pDestRect, const dummy_D2D1_RECT_F* pSourceRect);

d2dFont* d2dCreateFont(const LOGFONTW* pLogFont);
void d2dDestroyFont(d2dFont* font);
void d2dFontGetMetrics(d2dFont *font, d2dFontMetrics *pMetrics);
void d2dFontMeasureString(d2dFont *hFont, const WCHAR* pszText, int iTextLength, int *w, int *h);

dummy_ID2D1Brush* d2dCreateSolidBrush(dummy_ID2D1RenderTarget *target, long color);
dummy_ID2D1StrokeStyle *d2dSetLineStyle(int line_style, int line_cap, int line_join);
void d2dInitColor(dummy_D2D1_COLOR_F* c, long color);

void d2dDrawText(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float x, float y, float w, float h, const WCHAR* pszText, int iTextLength, d2dFont *font);
void d2dDrawLine(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float x0, float y0, float x1, float y1, float fStrokeWidth, dummy_ID2D1StrokeStyle *hStrokeStyle);
void d2dDrawRect(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float x0, float y0, float x1, float y1, float fStrokeWidth, dummy_ID2D1StrokeStyle *hStrokeStyle);
void d2dFillRect(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float x0, float y0, float x1, float y1);
void d2dDrawArc(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float xc, float yc, float w, float h, double a1, double a2, float fStrokeWidth, dummy_ID2D1StrokeStyle *hStrokeStyle);
void d2dFillArc(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float xc, float yc, float w, float h, double a1, double a2, int pie);
void d2dDrawPolygon(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, int* points, int count, int mode, float fStrokeWidth, dummy_ID2D1StrokeStyle *hStrokeStyle);
void d2dDrawPolygonF(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, double* points, int count, int mode, float fStrokeWidth, dummy_ID2D1StrokeStyle *hStrokeStyle);


#endif
