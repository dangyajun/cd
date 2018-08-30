/** \file
 * \brief Windows GDI+ Base Driver
 *
 * See Copyright Notice in cd.h
 */

//#define D2D_USE_C_DEFINITIONS
//#include <d2d1.h>
//#define dummy_ 

#include "cdwin_d2d.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <tchar.h>
#include <malloc.h>

#define type2float(_x) ((float)(_x))

TCHAR* cdwStrToSystem(const char* str, int utf8mode);
char* cdwStrFromSystem(const TCHAR* wstr, int utf8mode);
TCHAR* cdwStrToSystemLen(const char* str, int *len, int utf8mode);

static void setClipRect(d2dCanvas *canvas, double x1, double y1, double x2, double y2)
{
  dummy_D2D1_RECT_F rect;

  rect.left = type2float(x1);
  rect.top = type2float(y1);
  rect.right = type2float(x2);
  rect.bottom = type2float(y2);

  d2dSetClip(canvas, &rect);
}

static void set_utf8mode_attrib(cdCtxCanvas* ctxcanvas, char* data)
{
  if (!data || data[0] == '0')
    ctxcanvas->utf8mode = 0;
  else
    ctxcanvas->utf8mode = 1;
}

static char* get_utf8mode_attrib(cdCtxCanvas* ctxcanvas)
{
  if (ctxcanvas->utf8mode)
    return "1";
  else
    return "0";
}

static cdAttribute utf8mode_attrib =
{
  "UTF8MODE",
  set_utf8mode_attrib,
  get_utf8mode_attrib
};


cdCtxCanvas *cdwd2dCreateCanvas(cdCanvas* canvas, HWND hWnd, HDC hDc)
{
  cdCtxCanvas* ctxcanvas;

  ctxcanvas = (cdCtxCanvas*)malloc(sizeof(cdCtxCanvas));
  memset(ctxcanvas, 0, sizeof(cdCtxCanvas));

  /* update canvas context */
  ctxcanvas->canvas = canvas;
  canvas->ctxcanvas = ctxcanvas;
  ctxcanvas->hWnd = hWnd;
  ctxcanvas->hDC = hDc;
  ctxcanvas->d2d_canvas = NULL;

  cdRegisterAttribute(canvas, &utf8mode_attrib);

  return ctxcanvas;
}

void cdwd2dKillCanvas(cdCtxCanvas* ctxcanvas)
{
  if (ctxcanvas->brush)
    dummy_ID2D1Brush_Release(ctxcanvas->brush);

  if (ctxcanvas->stroke_style)
    dummy_ID2D1StrokeStyle_Release(ctxcanvas->stroke_style);

  memset(ctxcanvas, 0, sizeof(cdCtxCanvas));
  free(ctxcanvas);
}

void cdwd2dUpdateCanvas(cdCtxCanvas* ctxcanvas)
{
  if (ctxcanvas->brush)
    dummy_ID2D1Brush_Release(ctxcanvas->brush);

  ctxcanvas->brush = d2dCreateSolidBrush(ctxcanvas->d2d_canvas->target, ctxcanvas->canvas->foreground);

  if (ctxcanvas->stroke_style)
    dummy_ID2D1StrokeStyle_Release(ctxcanvas->stroke_style);

  ctxcanvas->stroke_style = d2dSetLineStyle(ctxcanvas->canvas->line_style);
}

static int cdclip(cdCtxCanvas *ctxcanvas, int mode);

static void cdclear(cdCtxCanvas* ctxcanvas)
{
  dummy_ID2D1Brush *brush = d2dCreateSolidBrush(ctxcanvas->d2d_canvas->target, ctxcanvas->canvas->background);

  if (ctxcanvas->canvas->clip_mode != CD_CLIPOFF)
    cdclip(ctxcanvas, CD_CLIPOFF);
  
  d2dFillRect(ctxcanvas->d2d_canvas->target, ctxcanvas->brush, -0.5f, -0.5f, (float)ctxcanvas->canvas->w - 1 + 0.5f, (float)ctxcanvas->canvas->h - 1 + 0.5f);

  if (ctxcanvas->canvas->clip_mode != CD_CLIPOFF)
    cdclip(ctxcanvas, ctxcanvas->canvas->clip_mode);

  dummy_ID2D1Brush_Release(brush);
}

static long int cdforeground(cdCtxCanvas* ctxcanvas, long int color)
{
  if (!ctxcanvas->brush)
    ctxcanvas->brush = (dummy_ID2D1Brush*)d2dCreateSolidBrush(ctxcanvas->d2d_canvas->target, color);
  else if (color != ctxcanvas->canvas->foreground)
  {
    dummy_ID2D1Brush_Release(ctxcanvas->brush);
    ctxcanvas->brush = (dummy_ID2D1Brush*)d2dCreateSolidBrush(ctxcanvas->d2d_canvas->target, color);
  }

  return color;
}

static int cdlinestyle(cdCtxCanvas* ctxcanvas, int style)
{
  if (!ctxcanvas->stroke_style)
    ctxcanvas->stroke_style = d2dSetLineStyle(style);
  else if (style != ctxcanvas->canvas->line_style)
  {
    dummy_ID2D1Brush_Release(ctxcanvas->stroke_style);
    ctxcanvas->stroke_style = d2dSetLineStyle(style);
  }

  return style;
}

static void cdfpixel(cdCtxCanvas* ctxcanvas, double x, double y, long int color)
{
  dummy_ID2D1Brush *brush = d2dCreateSolidBrush(ctxcanvas->d2d_canvas->target, color);

  d2dFillArc(ctxcanvas->d2d_canvas->target, brush, (float)x, (float)y, (float)x, (float)y, 0.0f, 360.0f, 0);

  dummy_ID2D1Brush_Release(brush);
}

static void cdpixel(cdCtxCanvas *ctxcanvas, int x, int y, long color)
{
  cdfpixel(ctxcanvas, (double)x, (double)y, color);
}

static void cdfline(cdCtxCanvas *ctxcanvas, double x1, double y1, double x2, double y2)
{
  d2dDrawLine(ctxcanvas->d2d_canvas->target, ctxcanvas->brush, (float)x1, (float)y1, (float)x2, (float)y2, type2float(ctxcanvas->canvas->line_width), ctxcanvas->stroke_style);
}

static void cdline(cdCtxCanvas *ctxcanvas, int x1, int y1, int x2, int y2)
{
  if (ctxcanvas->canvas->line_width == 1)
  {
    /* compensate Direct2D horizontal and vertical lines when line_width == 1 */
    if (x1 == x2)
      cdfline(ctxcanvas, (double)x1, (double)y1 - 0.5, (double)x2, (double)y2 + 0.5);
    else if (y1 == y2)
      cdfline(ctxcanvas, (double)x1 - 0.5, (double)y1, (double)x2 + 0.5, (double)y2);
    else
      cdfline(ctxcanvas, (double)x1, type2float(y1), type2float(x2), type2float(y2));
  }
  else
    cdfline(ctxcanvas, (double)x1, (double)y1, (double)x2, (double)y2);
}

static void cdfrect(cdCtxCanvas *ctxcanvas, double xmin, double xmax, double ymin, double ymax)
{
  d2dDrawRect(ctxcanvas->d2d_canvas->target, ctxcanvas->brush, (float)xmin, (float)ymin, (float)xmax, (float)ymax, type2float(ctxcanvas->canvas->line_width), ctxcanvas->stroke_style);
}

static void cdrect(cdCtxCanvas *ctxcanvas, int xmin, int xmax, int ymin, int ymax)
{
  if (ctxcanvas->canvas->line_width == 1 && !ctxcanvas->canvas->use_matrix)
  {
    cdline(ctxcanvas, xmin, ymin, xmin, ymax);
    cdline(ctxcanvas, xmin, ymax, xmax, ymax);
    cdline(ctxcanvas, xmax, ymax, xmax, ymin);
    cdline(ctxcanvas, xmax, ymin, xmin, ymin);
  }
  else
    cdfrect(ctxcanvas, (double)xmin, (double)xmax, (double)ymin, (double)ymax);
}

static void cdfbox(cdCtxCanvas *ctxcanvas, double xmin, double xmax, double ymin, double ymax)
{
  d2dFillRect(ctxcanvas->d2d_canvas->target, ctxcanvas->brush, (float)xmin, (float)ymin, (float)xmax, (float)ymax);
}

static void cdbox(cdCtxCanvas *ctxcanvas, int xmin, int xmax, int ymin, int ymax)
{
  /* try to include the last integer line */
  if (!ctxcanvas->canvas->use_matrix)
    cdfbox(ctxcanvas, (double)xmin - 0.5, (double)xmax + 0.5, (double)ymin - 0.5, (double)ymax + 0.5);
  else
    cdfbox(ctxcanvas, (double)xmin, (double)xmax, (double)ymin, (double)ymax);
}

static void cdfarc(cdCtxCanvas *ctxcanvas, double xc, double yc, double w, double h, double a1, double a2)
{
  d2dDrawArc(ctxcanvas->d2d_canvas->target, ctxcanvas->brush, (float)xc, (float)yc, (float)w, (float)h, a1, a2, type2float(ctxcanvas->canvas->line_width), ctxcanvas->stroke_style);
}

static void cdarc(cdCtxCanvas *ctxcanvas, int xc, int yc, int w, int h, double a1, double a2)
{
  cdfarc(ctxcanvas, (double)xc, (double)yc, (double)w, (double)h, a1, a2);
}

static void cdfsector(cdCtxCanvas *ctxcanvas, double xc, double yc, double w, double h, double a1, double a2)
{
  d2dFillArc(ctxcanvas->d2d_canvas->target, ctxcanvas->brush, (float)xc, (float)yc, (float)w, (float)h, a1, a2, 1);
}

static void cdsector(cdCtxCanvas *ctxcanvas, int xc, int yc, int w, int h, double a1, double a2)
{
  cdfsector(ctxcanvas, (double)xc, (double)yc, (double)w, (double)h, a1, a2);
}

static void cdfchord(cdCtxCanvas *ctxcanvas, double xc, double yc, double w, double h, double a1, double a2)
{
  d2dFillArc(ctxcanvas->d2d_canvas->target, ctxcanvas->brush, (float)xc, (float)yc, (float)w, (float)h, a1, a2, 2);
}

static void cdchord(cdCtxCanvas *ctxcanvas, int xc, int yc, int w, int h, double a1, double a2)
{
  cdfchord(ctxcanvas, (double)xc, (double)yc, (double)w, (double)h, a1, a2);
}

static void cdpoly(cdCtxCanvas *ctxcanvas, int mode, cdPoint* poly, int n)
{
  if (mode == CD_CLIP)
    return;

  d2dDrawPolygon(ctxcanvas->d2d_canvas->target, ctxcanvas->brush, (int*)poly, n, mode, type2float(ctxcanvas->canvas->line_width), ctxcanvas->stroke_style);
}

static void cdfpoly(cdCtxCanvas *ctxcanvas, int mode, cdfPoint* poly, int n)
{
  if (mode == CD_CLIP)
    return;

  d2dDrawPolygonF(ctxcanvas->d2d_canvas->target, ctxcanvas->brush, (double*)poly, n, mode, type2float(ctxcanvas->canvas->line_width), ctxcanvas->stroke_style);
}

static int cdfont(cdCtxCanvas* ctxcanvas, const char *type_face, int style, int size)
{
  LOGFONTW lf;
  d2dFont* new_font;
  LONG bold = 0, italic = 0, underline = 0, strikeout = 0, size_pixel;

  if (style&CD_BOLD)
    bold = FW_BOLD;
  else
    bold = FW_NORMAL;

  if (style&CD_ITALIC)
    italic = 1;

  if (style&CD_UNDERLINE)
    underline = 1;

  if (style&CD_STRIKEOUT)
    strikeout = 1;

  if (cdStrEqualNoCase(type_face, "Courier") || cdStrEqualNoCase(type_face, "Monospace"))
    type_face = "Courier New";
  else if (cdStrEqualNoCase(type_face, "Times") || cdStrEqualNoCase(type_face, "Serif"))
    type_face = "Times New Roman";
  else if (cdStrEqualNoCase(type_face, "Helvetica") || cdStrEqualNoCase(type_face, "Sans"))
    type_face = "Arial";

  size_pixel = cdGetFontSizePixels(ctxcanvas->canvas, size);

  wcsncpy(lf.lfFaceName, cdwStrToSystem(type_face, ctxcanvas->utf8mode), LF_FACESIZE);
  lf.lfWeight = bold;
  lf.lfHeight = -size_pixel;
  lf.lfItalic = (BYTE)italic;
  lf.lfUnderline = (BYTE)underline;
  lf.lfStrikeOut = (BYTE)strikeout;
  lf.lfEscapement = ctxcanvas->font_angle;
  lf.lfOrientation = ctxcanvas->font_angle;
  lf.lfWidth = 0;
  lf.lfOutPrecision = OUT_TT_PRECIS;
  lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
  lf.lfQuality = DEFAULT_QUALITY;
  lf.lfPitchAndFamily = FF_DONTCARE | DEFAULT_PITCH;

  new_font = d2dCreateFont(&lf);
  if (!new_font)
    return 0;

  if (ctxcanvas->font)
    d2dDestroyFont(ctxcanvas->font);

  ctxcanvas->font = new_font;

  return 1;
}                                                                           

static int cdnativefont(cdCtxCanvas* ctxcanvas, const char* nativefont)
{
  LOGFONT lf;
  int size = 12, bold = FW_NORMAL, italic = 0,
    style = CD_PLAIN, underline = 0, strikeout = 0,
    size_pixel;
  char type_face[1024];

  if (nativefont[0] == '-' && nativefont[1] == 'd')
  {
    COLORREF rgbColors;
    CHOOSEFONT cf;

    ZeroMemory(&cf, sizeof(CHOOSEFONT));

    cf.lStructSize = sizeof(CHOOSEFONT);
    cf.hwndOwner = GetForegroundWindow();
    cf.lpLogFont = &lf;
    cf.Flags = CF_SCREENFONTS | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT;
    rgbColors = cf.rgbColors = ctxcanvas->canvas->foreground;

    size_pixel = cdGetFontSizePixels(ctxcanvas->canvas, ctxcanvas->canvas->font_size);

    if (style&ctxcanvas->canvas->font_style)
      bold = FW_BOLD;
    else
      bold = FW_NORMAL;

    if (style&ctxcanvas->canvas->font_style)
      italic = 1;

    if (style&ctxcanvas->canvas->font_style)
      underline = 1;

    if (style&ctxcanvas->canvas->font_style)
      strikeout = 1;

    lstrcpy(lf.lfFaceName, cdwStrToSystem(ctxcanvas->canvas->font_type_face, ctxcanvas->utf8mode));
    lf.lfWeight = bold;
    lf.lfHeight = -size_pixel;
    lf.lfItalic = (BYTE)italic;
    lf.lfUnderline = (BYTE)underline;
    lf.lfStrikeOut = (BYTE)strikeout;
    lf.lfEscapement = ctxcanvas->font_angle;
    lf.lfOrientation = ctxcanvas->font_angle;
    lf.lfWidth = 0;
    lf.lfOutPrecision = OUT_TT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfPitchAndFamily = FF_DONTCARE | DEFAULT_PITCH;

    if (ChooseFont(&cf))
    {
      if (rgbColors != cf.rgbColors)
        cdCanvasSetForeground(ctxcanvas->canvas, cdEncodeColor(GetRValue(cf.rgbColors), GetGValue(cf.rgbColors), GetBValue(cf.rgbColors)));
    }
    else
      return 0;

    bold = lf.lfWeight;
    italic = lf.lfItalic;
    size = lf.lfHeight;
    strcpy(type_face, cdwStrFromSystem(lf.lfFaceName, ctxcanvas->utf8mode));
    underline = lf.lfUnderline;
    strikeout = lf.lfStrikeOut;

    if (bold != FW_NORMAL) style |= CD_BOLD;
    if (italic) style |= CD_ITALIC;
    if (underline) style |= CD_UNDERLINE;
    if (strikeout) style |= CD_STRIKEOUT;
  }
  else
  {
    if (!cdParseIupWinFont(nativefont, type_face, &style, &size))
    {
      if (!cdParsePangoFont(nativefont, type_face, &style, &size))
        return 0;
    }

    if (style&CD_BOLD)
      bold = FW_BOLD;
    if (style&CD_ITALIC)
      italic = 1;
    if (style&CD_UNDERLINE)
      underline = 1;
    if (style&CD_STRIKEOUT)
      strikeout = 1;

    size_pixel = cdGetFontSizePixels(ctxcanvas->canvas, size);

    lstrcpy(lf.lfFaceName, cdwStrToSystem(type_face, ctxcanvas->utf8mode));
    lf.lfWeight = bold;
    lf.lfHeight = -size_pixel;
    lf.lfItalic = (BYTE)italic;
    lf.lfUnderline = (BYTE)underline;
    lf.lfStrikeOut = (BYTE)strikeout;
    lf.lfEscapement = ctxcanvas->font_angle;
    lf.lfOrientation = ctxcanvas->font_angle;
    lf.lfWidth = 0;
    lf.lfOutPrecision = OUT_TT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfPitchAndFamily = FF_DONTCARE | DEFAULT_PITCH;
  }

  if (ctxcanvas->font)
    d2dDestroyFont(ctxcanvas->font);

  ctxcanvas->font = d2dCreateFont(&lf);

  return 1;
}

static double cdtextorientation(cdCtxCanvas* ctxcanvas, double angle)
{
  int font_angle = (int)(angle * 10);
  if (ctxcanvas->font_angle == font_angle) /* first time angle=0, do not create font twice */
    return angle;

  ctxcanvas->font_angle = font_angle;

  /* recreate the font, because angle changed */
  cdfont(ctxcanvas, ctxcanvas->canvas->font_type_face, ctxcanvas->canvas->font_style, ctxcanvas->canvas->font_size);

  return angle;
}

static void cdgetfontdim(cdCtxCanvas* ctxcanvas, int *max_width, int *line_height, int *ascent, int *descent)
{
  d2dFontMetrics metrics;
  d2dFontGetMetrics(ctxcanvas->font, &metrics);

  if (max_width)
    d2dFontMeasureString(ctxcanvas->font, L"W", 1, max_width, NULL);

  if (line_height)
    *line_height = (int)metrics.fLeading;

  if (ascent)
    *ascent = (int)metrics.fAscent;

  if (descent)
    *descent = (int)metrics.fDescent;

  return;
}

static void cdgettextsize(cdCtxCanvas* ctxcanvas, const char *s, int len, int *width, int *height)
{                         
  d2dFontMeasureString(ctxcanvas->font, cdwStrToSystem(s, ctxcanvas->utf8mode), len, width, height);
}

static void cdfcliparea(cdCtxCanvas *ctxcanvas, double xmin, double xmax, double ymin, double ymax)
{
  if (ctxcanvas->canvas->clip_mode == CD_CLIPAREA)
    setClipRect(ctxcanvas->d2d_canvas, xmin, ymin, xmax, ymax);
}

static int cdclip(cdCtxCanvas *ctxcanvas, int mode)
{
  if (mode == CD_CLIPAREA)
    setClipRect(ctxcanvas->d2d_canvas, ctxcanvas->canvas->clip_frect.xmin, ctxcanvas->canvas->clip_frect.ymin, ctxcanvas->canvas->clip_frect.xmax, ctxcanvas->canvas->clip_frect.ymax);
  else if (mode == CD_CLIPOFF)
    d2dSetClip(ctxcanvas->d2d_canvas, NULL);
  else if (mode == CD_CLIPPOLYGON || mode == CD_CLIPREGION)
    return ctxcanvas->canvas->clip_mode;
  return mode;
}

static void cdftext(cdCtxCanvas *ctxcanvas, double x, double y, const char *s, int len)
{
  d2dFontMetrics fm;
  int w, h, desc;
  double ox = x, oy = y;

  /* cxFText method is called for single lines only */

  WCHAR* wstr = cdwStrToSystemLen(s, &len, ctxcanvas->utf8mode);

  d2dFontGetMetrics(ctxcanvas->font, &fm);
  desc = (int)fm.fDescent;
  d2dFontMeasureString(ctxcanvas->font, wstr, len, &w, &h);

  /* move to top-left corner of the text */
  switch (ctxcanvas->canvas->text_alignment)
  {
    case CD_BASE_RIGHT:
    case CD_NORTH_EAST:
    case CD_EAST:
    case CD_SOUTH_EAST:
      x = x - w;
      break;
    case CD_BASE_CENTER:
    case CD_CENTER:
    case CD_NORTH:
    case CD_SOUTH:
      x = x - w / 2;
      break;
    case CD_BASE_LEFT:
    case CD_NORTH_WEST:
    case CD_WEST:
    case CD_SOUTH_WEST:
      x = x;
      break;
  }

  switch (ctxcanvas->canvas->text_alignment)
  {
    case CD_BASE_LEFT:
    case CD_BASE_CENTER:
    case CD_BASE_RIGHT:
      y = y - h + desc;
      break;
    case CD_SOUTH_EAST:
    case CD_SOUTH_WEST:
    case CD_SOUTH:
      y = y - h;
      break;
    case CD_NORTH_EAST:
    case CD_NORTH:
    case CD_NORTH_WEST:
      y = y;
      break;
    case CD_CENTER:
    case CD_EAST:
    case CD_WEST:
      y = y - h / 2;
      break;
  }

  if (ctxcanvas->canvas->text_orientation)
  {
    double cos_angle = cos(ctxcanvas->canvas->text_orientation*CD_DEG2RAD);
    double sin_angle = sin(ctxcanvas->canvas->text_orientation*CD_DEG2RAD);
    cdfRotatePoint(ctxcanvas->canvas, x, y, ox, oy, &x, &y, sin_angle, cos_angle);
  }

  if (ctxcanvas->canvas->text_orientation)
    d2dRotateWorld(ctxcanvas->d2d_canvas->target, (float)x, (float)y, (float)-ctxcanvas->canvas->text_orientation);  /* counterclockwise */

  d2dDrawText(ctxcanvas->d2d_canvas->target, ctxcanvas->brush, (float)x, (float)y, (float)w, (float)h, wstr, len, ctxcanvas->font);

  /* restore settings */
  if (ctxcanvas->canvas->text_orientation)
    d2dResetTransform(ctxcanvas->d2d_canvas->target);
}

static void cdtext(cdCtxCanvas *ctxcanvas, int x, int y, const char *s, int len)
{
  cdftext(ctxcanvas, (double)x, (double)y, s, len);
}

static void cdfputimagerectrgba(cdCtxCanvas* ctxcanvas, int width, int height, const unsigned char *red,
                                const unsigned char *green, const unsigned char *blue, const unsigned char *alpha, double x, double y, double w, double h, int xmin, int xmax, int ymin, int ymax)
{
  dummy_D2D1_RECT_F destRect;
  dummy_D2D1_RECT_F srcRect;

  IWICBitmap* bitmap = d2dCreateImageFromBufferRGB(width, height, red, green, blue, alpha);

  destRect.left = type2float(x);
  destRect.top = type2float(y - h);
  destRect.right = type2float(x + w);
  destRect.bottom = type2float(y);

  srcRect.left = type2float(xmin);
  srcRect.top = type2float(h - 1 - ymax);
  srcRect.right = type2float(xmax);
  srcRect.bottom = type2float(h - 1 - ymin);

  d2dBitBltImage(ctxcanvas->d2d_canvas->target, bitmap, &destRect, &srcRect);

  d2dDestroyImage(bitmap);
}

static void cdfputimagerectrgb(cdCtxCanvas* ctxcanvas, int width, int height, const unsigned char *red,
                               const unsigned char *green, const unsigned char *blue, double x, double y, double w, double h, int xmin, int xmax, int ymin, int ymax)
{
  dummy_D2D1_RECT_F destRect;
  dummy_D2D1_RECT_F srcRect;

  IWICBitmap* bitmap = d2dCreateImageFromBufferRGB(width, height, red, green, blue, NULL);

  destRect.left = type2float(x);
  destRect.top = type2float(y - h);
  destRect.right = type2float(x + w);
  destRect.bottom = type2float(y);

  srcRect.left = type2float(xmin);
  srcRect.top = type2float(h - 1 - ymax);
  srcRect.right = type2float(xmax);
  srcRect.bottom = type2float(h - 1 - ymin);

  d2dBitBltImage(ctxcanvas->d2d_canvas->target, bitmap, &destRect, &srcRect);

  d2dDestroyImage(bitmap);
}

static void cdfputimagerectmap(cdCtxCanvas* ctxcanvas, int width, int height, const unsigned char *index, 
                               const long int *colors, double x, double y, double w, double h, int xmin, int xmax, int ymin, int ymax)
{
  dummy_D2D1_RECT_F destRect;
  dummy_D2D1_RECT_F srcRect;

  IWICBitmap* bitmap = d2dCreateImageFromBufferMap(width, height, index, colors);

  destRect.left = type2float(x);
  destRect.top = type2float(y - h);
  destRect.right = type2float(x + w);
  destRect.bottom = type2float(y);

  srcRect.left = type2float(xmin);
  srcRect.top = type2float(h - 1 - ymax);
  srcRect.right = type2float(xmax);
  srcRect.bottom = type2float(h - 1 - ymin);

  d2dBitBltImage(ctxcanvas->d2d_canvas->target, bitmap, &destRect, &srcRect);

  d2dDestroyImage(bitmap);
}

static void cdputimagerectrgb(cdCtxCanvas *ctxcanvas, int iw, int ih, const unsigned char *r, const unsigned char *g, const unsigned char *b,
                              int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  cdfputimagerectrgb(ctxcanvas, iw, ih, r, g, b, (double)x, (double)y, (double)w, (double)h, xmin, xmax, ymin, ymax);
}

static void cdputimagerectrgba(cdCtxCanvas *ctxcanvas, int iw, int ih, const unsigned char *r, const unsigned char *g, const unsigned char *b, const unsigned char *a,
                               int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  cdfputimagerectrgba(ctxcanvas, iw, ih, r, g, b, a, (double)x, (double)y, (double)w, (double)h, xmin, xmax, ymin, ymax);
}

static void cdputimagerectmap(cdCtxCanvas *ctxcanvas, int iw, int ih, const unsigned char *index, const long *colors,
                              int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  cdfputimagerectmap(ctxcanvas, iw, ih, index, colors, (double)x, (double)y, (double)w, (double)h, xmin, xmax, ymin, ymax);
}

void cdwd2dInitTable(cdCanvas* canvas)
{
  canvas->cxClear = cdclear;

  canvas->cxPixel = cdpixel;
  canvas->cxLine = cdline;
  canvas->cxPoly = cdpoly;
  canvas->cxRect = cdrect;
  canvas->cxBox = cdbox;
  canvas->cxArc = cdarc;
  canvas->cxSector = cdsector;
  canvas->cxChord = cdchord;
  canvas->cxText = cdtext;

  canvas->cxFPixel = cdfpixel;
  canvas->cxFLine = cdfline;
  canvas->cxFPoly = cdfpoly;
  canvas->cxFRect = cdfrect;
  canvas->cxFBox = cdfbox;
  canvas->cxFArc = cdfarc;
  canvas->cxFSector = cdfsector;
  canvas->cxFChord = cdfchord;
  canvas->cxFText = cdftext;

  canvas->cxFont = cdfont;
  canvas->cxGetFontDim = cdgetfontdim;
  canvas->cxGetTextSize = cdgettextsize; 

  canvas->cxPutImageRectRGBA = cdputimagerectrgba;
  canvas->cxPutImageRectRGB = cdputimagerectrgb;
  canvas->cxPutImageRectMap = cdputimagerectmap;
  canvas->cxFPutImageRectRGBA = cdfputimagerectrgba;
  canvas->cxFPutImageRectRGB = cdfputimagerectrgb;
  canvas->cxFPutImageRectMap = cdfputimagerectmap;

  canvas->cxClip = cdclip;
  canvas->cxFClipArea = cdfcliparea;
  canvas->cxForeground = cdforeground;
  canvas->cxLineStyle = cdlinestyle;
}

//TODO (ver cairo ou pdf)
// canvas->cxTransform = cdtransform;
// 1) line join, line cap
// 2) polygon - CD_PATH, CD_BEZIER, FillRule?, holes
// 3) clip - CD_CLIPPOLYGON
// hatch, pattern e stipple?
// LINEGRADIENT, RADIALGRADIENT, IMGINTERP, ROTATE
// regions?
