/** \file
 * \brief Name space for C++ high level API
 *
 * See Copyright Notice in cd.h
 */
 
#ifndef __CD_PLUS_H
#define __CD_PLUS_H


#include <im_plus.h>

#include "cd.h"
#include "wd.h"
#include "cdiup.h"


/** \brief Name space for C++ high level API
 *
 * \par
 * Defines wrapper classes for all C structures.
 *
 * See \ref cd_plus.h
 */
namespace cd
{
#if 0
  int         cdUseContextPlus(int use);
  void        cdInitContextPlus(void);   /* need an external library */
  void        cdFinishContextPlus(void);   /* need an external library */

  /* context */
  typedef int(*cdCallback)(cdCanvas* canvas, ...);
  int cdContextRegisterCallback(cdContext *context, int cb, cdCallback func);
  unsigned long cdContextCaps(cdContext *context);
  int cdContextIsPlus(cdContext *context);
  int cdContextType(cdContext *context);

  cdImage; // Remover???

  imImage

  cdBitmap* cdCreateBitmap(int w, int h, int type);
  cdBitmap* cdInitBitmap(int w, int h, int type, ...);
  void cdKillBitmap(cdBitmap* bitmap);
  unsigned char* cdBitmapGetData(cdBitmap* bitmap, int dataptr);
  void cdBitmapSetRect(cdBitmap* bitmap, int xmin, int xmax, int ymin, int ymax);
  void cdBitmapRGB2Map(cdBitmap* bitmap_rgb, cdBitmap* bitmap_map);

  missing cdf*(double)
#endif

  class Canvas
  {
    cdCanvas* canvas;

    Canvas() {};

  public:
//    cdCanvas*   cdCreateCanvas(cdContext *context, void *data);

    /* canvas init */
    Canvas(cdCanvas* ref_canvas)
    {
      canvas = ref_canvas;
    }

    ~Canvas()
    {
      if (canvas) cdKillCanvas(canvas);
    }

    cdContext* GetContext()
    {
      return cdCanvasGetContext(canvas);
    }
    int Activate()
    {
      return cdCanvasActivate(canvas);
    }
    void Deactivate()
    {
      cdCanvasDeactivate(canvas);
    }

    /* control */
    int Simulate(int mode)
    {
      return cdCanvasSimulate(canvas, mode);
    }
    void Flush()
    {
      cdCanvasFlush(canvas);
    }
    void Clear()
    {
      cdCanvasClear(canvas);
    }

    cdState* SaveState()
    {
      return cdCanvasSaveState(canvas);
    }
    void RestoreState(cdState* state)
    {
      cdCanvasRestoreState(canvas, state);
    }
    void ReleaseState(cdState* state)
    {
      cdReleaseState(state);
    }

    void SetAttribute(const char* name, char* data)
    {
      cdCanvasSetAttribute(canvas, name, data);
    }
    void SetfAttribute(const char* name, const char* format, ...)
    {
      //char data[10240];
      //va_list arglist;
      //va_start(arglist, format);
      //vsnprintf(data, 10240, format, arglist);
      //va_end(arglist);

      //cdCanvasSetAttribute(canvas, name, data);
    }
    char* GetAttribute(const char* name)
    {
      return cdCanvasGetAttribute(canvas, name);
    }

    /* interpretation */
    int Play(cdContext *context, int xmin, int xmax, int ymin, int ymax, void *data)
    {
      return cdCanvasPlay(canvas, context, xmin, xmax, ymin, ymax, data);
    }

    /* coordinates utilities */
    void GetSize(int &width, int &height, double &width_mm, double &height_mm)
    {
      cdCanvasGetSize(canvas, &width, &height, &width_mm, &height_mm);
    }
    int UpdateYAxis(int &y)
    {
      return cdCanvasUpdateYAxis(canvas, &y);
    }
    double UpdateYAxis(double &y)
    {
      return cdfCanvasUpdateYAxis(canvas, &y);
    }
    int InvertYAxis(int y)
    {
      return cdCanvasInvertYAxis(canvas, y);
    }
    double InvertYAxis(double y)
    {
      return cdfCanvasInvertYAxis(canvas, y);
    }
    void MM2Pixel(double mm_dx, double mm_dy, int &dx, int &dy)
    {
      cdCanvasMM2Pixel(canvas, mm_dx, mm_dy, &dx, &dy);
    }
    void MM2Pixel(double mm_dx, double mm_dy, double &dx, double &dy)
    {
      cdfCanvasMM2Pixel(canvas, mm_dx, mm_dy, &dx, &dy);
    }
    void Pixel2MM(int dx, int dy, double &mm_dx, double &mm_dy)
    {
      cdCanvasPixel2MM(canvas, dx, dy, &mm_dx, &mm_dy);
    }
    void Pixel2MM(double dx, double dy, double &mm_dx, double &mm_dy)
    {
      cdfCanvasPixel2MM(canvas, dx, dy, &mm_dx, &mm_dy);
    }
    void Origin(int x, int y)
    {
      cdCanvasOrigin(canvas, x, y);
    }
    void Origin(double x, double y)
    {
      cdfCanvasOrigin(canvas, x, y);
    }
    void GetOrigin(int &x, int &y)
    {
      cdCanvasGetOrigin(canvas, &x, &y);
    }
    void GetOrigin(double &x, double &y)
    {
      cdfCanvasGetOrigin(canvas, &x, &y);
    }

    /* coordinates transformation */
    void Transform(const double* matrix)
    {
      cdCanvasTransform(canvas, matrix);
    }
    double* GetTransform()
    {
      return cdCanvasGetTransform(canvas);
    }
    void TransformMultiply(const double* matrix)
    {
      cdCanvasTransformMultiply(canvas, matrix);
    }
    void TransformRotate(double angle)
    {
      cdCanvasTransformRotate(canvas, angle);
    }
    void TransformScale(double sx, double sy)
    {
      cdCanvasTransformScale(canvas, sx, sy);
    }
    void TransformTranslate(double dx, double dy)
    {
      cdCanvasTransformTranslate(canvas, dx, dy);
    }
    void TransformPoint(int x, int y, int &tx, int &ty)
    {
      cdCanvasTransformPoint(canvas, x, y, &tx, &ty);
    }
    void TransformPoint(double x, double y, double &tx, double &ty)
    {
      cdfCanvasTransformPoint(canvas, x, y, &tx, &ty);
    }

    /* world coordinate transformation */
    void wWindow(double xmin, double xmax, double  ymin, double ymax)
    {
      wdCanvasWindow(canvas, xmin, xmax, ymin, ymax);
    }
    void wGetWindow(double &xmin, double  &xmax, double  &ymin, double &ymax)
    {
      wdCanvasGetWindow(canvas, &xmin, &xmax, &ymin, &ymax);
    }
    void wViewport(int xmin, int xmax, int ymin, int ymax)
    {
      wdCanvasViewport(canvas, xmin, xmax, ymin, ymax);
    }
    void wGetViewport(int &xmin, int  &xmax, int  &ymin, int &ymax)
    {
      wdCanvasGetViewport(canvas, &xmin, &xmax, &ymin, &ymax);
    }
    void wWorld2Canvas(double xw, double yw, int &xv, int &yv)
    {
      wdCanvasWorld2Canvas(canvas, xw, yw, &xv, &yv);
    }
    void wWorld2CanvasSize(double hw, double vw, int &hv, int &vv)
    {
      wdCanvasWorld2CanvasSize(canvas, hw, vw, &hv, &vv);
    }
    void wCanvas2World(int xv, int yv, double &xw, double &yw)
    {
      wdCanvasCanvas2World(canvas, xv, yv, &xw, &yw);
    }
    void wSetTransform(double sx, double sy, double tx, double ty)
    {
      wdCanvasSetTransform(canvas, sx, sy, tx, ty);
    }
    void wGetTransform(double &sx, double &sy, double &tx, double &ty)
    {
      wdCanvasGetTransform(canvas, &sx, &sy, &tx, &ty);
    }
    void wTranslate(double dtx, double dty)
    {
      wdCanvasTranslate(canvas, dtx, dty);
    }
    void wScale(double dsx, double dsy)
    {
      wdCanvasScale(canvas, dsx, dsy);
    }


    /* clipping */
    int Clip(int mode)
    {
      return cdCanvasClip(canvas, mode);
    }
    void ClipArea(int xmin, int xmax, int ymin, int ymax)
    {
      cdCanvasClipArea(canvas, xmin, xmax, ymin, ymax);
    }
    void ClipArea(double xmin, double xmax, double ymin, double ymax)
    {
      cdfCanvasClipArea(canvas, xmin, xmax, ymin, ymax);
    }
    void wClipArea(double xmin, double xmax, double  ymin, double ymax)
    {
      wdCanvasClipArea(canvas, xmin, xmax, ymin, ymax);
    }
    int GetClipArea(int &xmin, int &xmax, int &ymin, int &ymax)
    {
      return cdCanvasGetClipArea(canvas, &xmin, &xmax, &ymin, &ymax);
    }
    int GetClipArea(double &xmin, double &xmax, double &ymin, double &ymax)
    {
      return cdfCanvasGetClipArea(canvas, &xmin, &xmax, &ymin, &ymax);
    }
    int wGetClipArea(double &xmin, double &xmax, double &ymin, double &ymax)
    {
      return wdCanvasGetClipArea(canvas, &xmin, &xmax, &ymin, &ymax);
    }

    /* clipping region */
    int IsPointInRegion(int x, int y)
    {
      return cdCanvasIsPointInRegion(canvas, x, y);
    }
    int wIsPointInRegion(double x, double y)
    {
      return wdCanvasIsPointInRegion(canvas, x, y);
    }
    void OffsetRegion(int x, int y)
    {
      cdCanvasOffsetRegion(canvas, x, y);
    }
    void wOffsetRegion(double x, double y)
    {
      wdCanvasOffsetRegion(canvas, x, y);
    }
    void GetRegionBox(int &xmin, int &xmax, int &ymin, int &ymax)
    {
      cdCanvasGetRegionBox(canvas, &xmin, &xmax, &ymin, &ymax);
    }
    void wGetRegionBox(double &xmin, double &xmax, double &ymin, double &ymax)
    {
      wdCanvasGetRegionBox(canvas, &xmin, &xmax, &ymin, &ymax);
    }
    int RegionCombineMode(int mode)
    {
      return cdCanvasRegionCombineMode(canvas, mode);
    }

    /* primitives */
    void Pixel(int x, int y, long color)
    {
      cdCanvasPixel(canvas, x, y, color);
    }
    void wPixel(double x, double y, long color)
    {
      wdCanvasPixel(canvas, x, y, color);
    }
    void Mark(int x, int y)
    {
      cdCanvasMark(canvas, x, y);
    }
    void wMark(double x, double y)
    {
      wdCanvasMark(canvas, x, y);
    }

    void Begin(int mode)
    {
      cdCanvasBegin(canvas, mode);
    }
    void PathSet(int action)
    {
      cdCanvasPathSet(canvas, action);
    }
    void End()
    {
      cdCanvasEnd(canvas);
    }

    void Line(int x1, int y1, int x2, int y2)
    {
      cdCanvasLine(canvas, x1, y1, x2, y2);
    }
    void Line(double x1, double y1, double x2, double y2)
    {
      cdfCanvasLine(canvas, x1, y1, x2, y2);
    }
    void wLine(double x1, double y1, double x2, double y2)
    {
      wdCanvasLine(canvas, x1, y1, x2, y2);
    }
    void Vertex(int x, int y)
    {
      cdCanvasVertex(canvas, x, y);
    }
    void wVertex(double x, double y)
    {
      wdCanvasVertex(canvas, x, y);
    }
    void Vertex(double x, double y)
    {
      cdfCanvasVertex(canvas, x, y);
    }
    void Rect(int xmin, int xmax, int ymin, int ymax)
    {
      cdCanvasRect(canvas, xmin, xmax, ymin, ymax);
    }
    void Rect(double xmin, double xmax, double ymin, double ymax)
    {
      cdfCanvasRect(canvas, xmin, xmax, ymin, ymax);
    }
    void wRect(double xmin, double xmax, double ymin, double ymax)
    {
      wdCanvasRect(canvas, xmin, xmax, ymin, ymax);
    }
    void Box(int xmin, int xmax, int ymin, int ymax)
    {
      cdCanvasBox(canvas, xmin, xmax, ymin, ymax);
    }
    void Box(double xmin, double xmax, double ymin, double ymax)
    {
      cdfCanvasBox(canvas, xmin, xmax, ymin, ymax);
    }
    void wBox(double xmin, double xmax, double ymin, double ymax)
    {
      wdCanvasBox(canvas, xmin, xmax, ymin, ymax);
    }
    void Arc(int xc, int yc, int w, int h, double angle1, double angle2)
    {
      cdCanvasArc(canvas, xc, yc, w, h, angle1, angle2);
    }
    void Arc(double xc, double yc, double w, double h, double angle1, double angle2)
    {
      cdfCanvasArc(canvas, xc, yc, w, h, angle1, angle2);
    }
    void wArc(double xc, double yc, double w, double h, double angle1, double angle2)
    {
      wdCanvasArc(canvas, xc, yc, w, h, angle1, angle2);
    }
    void Sector(int xc, int yc, int w, int h, double angle1, double angle2)
    {
      cdCanvasSector(canvas, xc, yc, w, h, angle1, angle2);
    }
    void Sector(double xc, double yc, double w, double h, double angle1, double angle2)
    {
      cdfCanvasSector(canvas, xc, yc, w, h, angle1, angle2);
    }
    void wSector(double xc, double yc, double w, double h, double angle1, double angle2)
    {
      wdCanvasSector(canvas, xc, yc, w, h, angle1, angle2);
    }
    void Chord(double xc, double yc, double w, double h, double angle1, double angle2)
    {
      cdfCanvasChord(canvas, xc, yc, w, h, angle1, angle2);
    }
    void Chord(int xc, int yc, int w, int h, double angle1, double angle2)
    {
      cdCanvasChord(canvas, xc, yc, w, h, angle1, angle2);
    }
    void wChord(double xc, double yc, double w, double h, double angle1, double angle2)
    {
      wdCanvasChord(canvas, xc, yc, w, h, angle1, angle2);
    }
    void Text(int x, int y, const char* s)
    {
      cdCanvasText(canvas, x, y, s);
    }
    void Text(double x, double y, const char* s)
    {
      cdfCanvasText(canvas, x, y, s);
    }
    void wText(double x, double y, const char* s)
    {
      wdCanvasText(canvas, x, y, s);
    }

    /* attributes */
    void SetBackground(long color)
    {
      cdCanvasSetBackground(canvas, color);
    }
    void SeForeground(long color)
    {
      cdCanvasSetForeground(canvas, color);
    }
    long Background(long color)
    {
      return cdCanvasBackground(canvas, color);
    }
    long Foreground(long color)
    {
      return cdCanvasForeground(canvas, color);
    }
    int BackOpacity(int opacity)
    {
      return cdCanvasBackOpacity(canvas, opacity);
    }
    int WriteMode(int mode)
    {
      return cdCanvasWriteMode(canvas, mode);
    }
    int LineStyle(int style)
    {
      return cdCanvasLineStyle(canvas, style);
    }
    void LineStyleDashes(const int* dashes, int count)
    {
      cdCanvasLineStyleDashes(canvas, dashes, count);
    }
    int LineWidth(int width)
    {
      return cdCanvasLineWidth(canvas, width);
    }
    double wLineWidth(double width)
    {
      return wdCanvasLineWidth(canvas, width);
    }
    int LineJoin(int join)
    {
      return cdCanvasLineJoin(canvas, join);
    }
    int LineCap(int cap)
    {
      return cdCanvasLineCap(canvas, cap);
    }
    int InteriorStyle(int style)
    {
      return cdCanvasInteriorStyle(canvas, style);
    }
    int Hatch(int style)
    {
      return cdCanvasHatch(canvas, style);
    }
    void Stipple(int w, int h, const unsigned char* stipple)
    {
      cdCanvasStipple(canvas, w, h, stipple);
    }
    void wStipple(int w, int h, const unsigned char* stipple, double w_mm, double h_mm)
    {
      wdCanvasStipple(canvas, w, h, stipple, w_mm, h_mm);
    }
    unsigned char* GetStipple(int &n, int &m)
    {
      return cdCanvasGetStipple(canvas, &n, &m);
    }
    void Pattern(int w, int h, long const int *pattern)
    {
      cdCanvasPattern(canvas, w, h, pattern);
    }
    void wPattern(int w, int h, const long *color, double w_mm, double h_mm)
    {
      wdCanvasPattern(canvas, w, h, color, w_mm, h_mm);
    }
    long* GetPattern(int& n, int& m)
    {
      return cdCanvasGetPattern(canvas, &n, &m);
    }
    int FillMode(int mode)
    {
      return cdCanvasFillMode(canvas, mode);
    }
    int Font(const char* type_face, int style, int size)
    {
      return cdCanvasFont(canvas, type_face, style, size);
    }
    int wFont(const char* type_face, int style, double size)
    {
      return wdCanvasFont(canvas, type_face, style, size);
    }
    void GetFont(char *type_face, int &style, int &size)
    {
      cdCanvasGetFont(canvas, type_face, &style, &size);
    }
    void wGetFont(char *type_face, int &style, double &size)
    {
      wdCanvasGetFont(canvas, type_face, &style, &size);
    }
    char* NativeFont(const char* font)
    {
      return cdCanvasNativeFont(canvas, font);
    }
    int TextAlignment(int alignment)
    {
      return cdCanvasTextAlignment(canvas, alignment);
    }
    double TextOrientation(double angle)
    {
      return cdCanvasTextOrientation(canvas, angle);
    }
    int MarkType(int type)
    {
      return cdCanvasMarkType(canvas, type);
    }
    int MarkSize(int size)
    {
      return cdCanvasMarkSize(canvas, size);
    }
    double wMarkSize(double size)
    {
      return wdCanvasMarkSize(canvas, size);
    }

    /* vector text */
    void VectorText(int x, int y, const char* s)
    {
      cdCanvasVectorText(canvas, x, y, s);
    }
    void VectorText(double x, double y, const char* s)
    {
      cdfCanvasVectorText(canvas, x, y, s);
    }
    void wVectorText(double x, double y, const char* s)
    {
      wdCanvasVectorText(canvas, x, y, s);
    }
    void MultiLineVectorText(int x, int y, const char* s)
    {
      cdCanvasMultiLineVectorText(canvas, x, y, s);
    }
    void MultiLineVectorText(double x, double y, const char* s)
    {
      cdfCanvasMultiLineVectorText(canvas, x, y, s);
    }
    void wMultiLineVectorText(double x, double y, const char* s)
    {
      wdCanvasMultiLineVectorText(canvas, x, y, s);
    }

    /* vector text attributes */
    char *VectorFont(const char *filename)
    {
      return cdCanvasVectorFont(canvas, filename);
    }
    void VectorTextDirection(int x1, int y1, int x2, int y2)
    {
      cdCanvasVectorTextDirection(canvas, x1, y1, x2, y2);
    }
    void VectorTextDirection(double x1, double y1, double x2, double y2)
    {
      cdfCanvasVectorTextDirection(canvas, x1, y1, x2, y2);
    }
    void wVectorTextDirection(double x1, double y1, double x2, double y2)
    {
      wdCanvasVectorTextDirection(canvas, x1, y1, x2, y2);
    }
    double* VectorTextTransform(const double* matrix)
    {
      return cdCanvasVectorTextTransform(canvas, matrix);
    }
    void VectorTextSize(int size_x, int size_y, const char* s)
    {
      cdCanvasVectorTextSize(canvas, size_x, size_y, s);
    }
    void VectorTextSize(double size_x, double size_y, const char* s)
    {
      cdfCanvasVectorTextSize(canvas, size_x, size_y, s);
    }
    void wVectorTextSize(double size_x, double size_y, const char* s)
    {
      wdCanvasVectorTextSize(canvas, size_x, size_y, s);
    }
    int VectorCharSize(int size)
    {
      return cdCanvasVectorCharSize(canvas, size);
    }
    double VectorCharSize(double size)
    {
      return cdfCanvasVectorCharSize(canvas, size);
    }
    double wVectorCharSize(double size)
    {
      return wdCanvasVectorCharSize(canvas, size);
    }
    void VectorFontSize(double size_x, double size_y)
    {
      cdCanvasVectorFontSize(canvas, size_x, size_y);
    }
    void GetVectorFontSize(double &size_x, double &size_y)
    {
      cdCanvasGetVectorFontSize(canvas, &size_x, &size_y);
    }


    /* vector text properties */
    void GetVectorTextSize(const char* s, int &x, int &y)
    {
      cdCanvasGetVectorTextSize(canvas, s, &x, &y);
    }
    void GetVectorTextSize(const char* s, double &x, double &y)
    {
      cdfCanvasGetVectorTextSize(canvas, s, &x, &y);
    }
    void wGetVectorTextSize(const char* s, double &x, double &y)
    {
      wdCanvasGetVectorTextSize(canvas, s, &x, &y);
    }
    void GetVectorTextBounds(const char* s, int x, int y, int *rect)
    {
      cdCanvasGetVectorTextBounds(canvas, s, x, y, rect);
    }
    void GetVectorTextBounds(const char* s, double x, double y, double *rect)
    {
      cdfCanvasGetVectorTextBounds(canvas, s, x, y, rect);
    }
    void wGetVectorTextBounds(const char* s, double x, double y, double *rect)
    {
      wdCanvasGetVectorTextBounds(canvas, s, x, y, rect);
    }
    void GetVectorTextBox(int x, int y, const char *s, int &xmin, int &xmax, int &ymin, int &ymax)
    {
      cdCanvasGetVectorTextBox(canvas, x, y, s, &xmin, &xmax, &ymin, &ymax);
    }
    void GetVectorTextBox(double x, double y, const char *s, double &xmin, double &xmax, double &ymin, double &ymax)
    {
      cdfCanvasGetVectorTextBox(canvas, x, y, s, &xmin, &xmax, &ymin, &ymax);
    }
    void wGetVectorTextBox(double x, double y, const char *s, double &xmin, double &xmax, double &ymin, double &ymax)
    {
      wdCanvasGetVectorTextBox(canvas, x, y, s, &xmin, &xmax, &ymin, &ymax);
    }

    /* properties */
    void GetFontDim(int &max_width, int &height, int &ascent, int &descent)
    {
      cdCanvasGetFontDim(canvas, &max_width, &height, &ascent, &descent);
    }
    void wGetFontDim(double &max_width, double &height, double &ascent, double &descent)
    {
      wdCanvasGetFontDim(canvas, &max_width, &height, &ascent, &descent);
    }
    void GetTextSize(const char* s, int &width, int &height)
    {
      cdCanvasGetTextSize(canvas, s, &width, &height);
    }
    void wGetTextSize(const char* s, double &width, double &height)
    {
      wdCanvasGetTextSize(canvas, s, &width, &height);
    }
    void GetTextBox(int x, int y, const char* s, int &xmin, int &xmax, int &ymin, int &ymax)
    {
      cdCanvasGetTextBox(canvas, x, y, s, &xmin, &xmax, &ymin, &ymax);
    }
    void wGetTextBox(double x, double y, const char* s, double &xmin, double &xmax, double &ymin, double &ymax)
    {
      wdCanvasGetTextBox(canvas, x, y, s, &xmin, &xmax, &ymin, &ymax);
    }
    void GetTextBounds(int x, int y, const char* s, int *rect)
    {
      cdCanvasGetTextBounds(canvas, x, y, s, rect);
    }
    void wGetTextBounds(double x, double y, const char* s, double *rect)
    {
      wdCanvasGetTextBounds(canvas, x, y, s, rect);
    }
    int GetColorPlanes()
    {
      return cdCanvasGetColorPlanes(canvas);
    }

    /* color */
    void Palette(int n, const long *palette, int mode)
    {
      cdCanvasPalette(canvas, n, palette, mode);
    }

    /* client images */
    void GetImageRGB(im::Image& image, int x, int y)
    {
//      cdCanvasGetImageRGB(canvas, r, g, b, x, y, w, h);
    }
    void PutImageRectRGB(int iw, int ih, const unsigned char* r, const unsigned char* g, const unsigned char* b, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
    {
      cdCanvasPutImageRectRGB(canvas, iw, ih, r, g, b, x, y, w, h, xmin, xmax, ymin, ymax);
    }
    void wPutImageRectRGB(int iw, int ih, const unsigned char* r, const unsigned char* g, const unsigned char* b, double x, double y, double w, double h, int xmin, int xmax, int ymin, int ymax)
    {
      wdCanvasPutImageRectRGB(canvas, iw, ih, r, g, b, x, y, w, h, xmin, xmax, ymin, ymax);
    }
    void PutImageRectRGBA(int iw, int ih, const unsigned char* r, const unsigned char* g, const unsigned char* b, const unsigned char* a, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
    {
      cdCanvasPutImageRectRGBA(canvas, iw, ih, r, g, b, a, x, y, w, h, xmin, xmax, ymin, ymax);
    }
    void wPutImageRectRGBA(int iw, int ih, const unsigned char* r, const unsigned char* g, const unsigned char* b, const unsigned char* a, double x, double y, double w, double h, int xmin, int xmax, int ymin, int ymax)
    {
      wdCanvasPutImageRectRGBA(canvas, iw, ih, r, g, b, a, x, y, w, h, xmin, xmax, ymin, ymax);
    }
    void PutImageRectMap(int iw, int ih, const unsigned char* index, const long* colors, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
    {
      cdCanvasPutImageRectMap(canvas, iw, ih, index, colors, x, y, w, h, xmin, xmax, ymin, ymax);
    }
    void wPutImageRectMap(int iw, int ih, const unsigned char* index, const long* colors, double x, double y, double w, double h, int xmin, int xmax, int ymin, int ymax)
    {
      wdCanvasPutImageRectMap(canvas, iw, ih, index, colors, x, y, w, h, xmin, xmax, ymin, ymax);
    }

    void PutBitmap(cdBitmap* bitmap, int x, int y, int w, int h)
    {
      cdCanvasPutBitmap(canvas, bitmap, x, y, w, h);
    }
    void wPutBitmap(cdBitmap* bitmap, double x, double y, double w, double h)
    {
      wdCanvasPutBitmap(canvas, bitmap, x, y, w, h);
    }
    void GetBitmap(cdBitmap* bitmap, int x, int y)
    {
      cdCanvasGetBitmap(canvas, bitmap, x, y);
    }
  };
}

#endif

