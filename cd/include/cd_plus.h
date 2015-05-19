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
#include "cddbuf.h"
#include "cdprint.h"
#include "cdnative.h"
#include "cdgdiplus.h"
#include "cdcgm.h"
#include "cddgn.h"
#include "cddxf.h"
#include "cdclipbd.h"
#include "cdemf.h"
#include "cdirgb.h"
#include "cdmf.h"
#include "cdps.h"
#include "cdpdf.h"
#include "cdsvg.h"
#include "cdwmf.h"
#include "cddebug.h"
#include "cdgl.h"
#include "cdpicture.h"



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
  NativeBitmap(const im:Image& image);
  ~NativeBitmap();
  PutBitmap

   cdfCanvasPlay
   wdCanvasPlay 
   cdfCanvasIsPointInRegion 
   cdfCanvasOffsetRegion 
   cdfCanvasGetRegionBox 
   cdfCanvasPixel 
   cdfCanvasMark 
   cdfCanvasGetTextBox 
   cdfCanvasGetTextBounds 
   cdfCanvasPutImageRectRGBA 
   cdfCanvasPutImageRectRGB 
   cdfCanvasPutImageRectMap 
   cdfCanvasGetImageRGB 
   wdCanvasGetImageRGB 

   defines
#endif

  char* Version()
  {
    return cdVersion();
  }
  char* VersionDate()
  {
    return cdVersionDate();
  }
  int VersionNumber()
  {
    return cdVersionNumber();
  }



  long ColorEncode(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255)
  {
    alpha = ~alpha;
    return (((long)alpha) << 24) | (((long)red) << 16) | (((long)green) << 8) | ((long)blue);
  }
  unsigned char ColorAlpha(long color)
  {
    unsigned char alpha = (unsigned char)(color >> 24);
    alpha = ~alpha;
    return alpha;
  }
  unsigned char ColorRed(long color)
  {
    return (unsigned char)(color >> 16);
  }
  unsigned char ColorGreen(long color)
  {
    return (unsigned char)(color >> 8);
  }
  unsigned char ColorBlue(long color)
  {
    return (unsigned char)color;
  }


  class Context
  {
    cdContext* cd_context;

    friend class Canvas;

  public:
    Context(cdContext* ref_context)
    {
      cd_context = ref_context;
    }

    unsigned long Caps()
    {
      return cdContextCaps(cd_context);
    }
    bool IsPlus()
    {
      return cdContextIsPlus(cd_context) != 0;
    }
    int Type()
    {
      return cdContextType(cd_context);
    }

    static void UsePlus(bool use)
    {
      cdUseContextPlus(use);
    }
    static void InitPlus()
    {
      cdInitContextPlus();
    }
    static void FinishPlus()
    {
      cdFinishContextPlus();
    }

    enum ExtraPolyModes
    {
      SPLINE = CD_SPLINE,
      FILLSPLINE = CD_FILLSPLINE,
      FILLGRADIENT = CD_FILLGRADIENT
    };


    static void GetScreenSize(int &width, int &height, double &width_mm, double &height_mm)
    {
      cdGetScreenSize(&width, &height, &width_mm, &height_mm);
    }
    static int GetScreenColorPlanes()
    {
      return cdGetScreenColorPlanes();
    }

    static Context NativeWindowImageRGBDoubleBufferIup()
    {
      return Context(cdContextIupDBufferRGB());
    }
    static Context NativeWindowDoubleBufferIup()
    {
      return Context(cdContextIupDBuffer());
    }
    static Context NativeWindowIup()
    {
      return Context(cdContextIup());
    }
    static Context NativeOpenGL()
    {
      return Context(cdContextGL());
    }
    static Context NativePrinter()
    {
      return Context(cdContextPrinter());
    }
    static Context NativeClipboard()
    {
      return Context(cdContextClipboard());
    }
    static Context ImageRGB()
    {
      return Context(cdContextImageRGB());
    }
    static Context ImageRGBDoubleBuffer()
    {
      return Context(cdContextDBufferRGB());
    }
    static Context Picture()
    {
      return Context(cdContextPicture());
    }
    static Context MetafileDebug()
    {
      return Context(cdContextDebug());
    }
    static Context MetafileWMF()
    {
      return Context(cdContextWMF());
    }
    static Context MetafileEMF()
    {
      return Context(cdContextEMF());
    }
    static Context MetafilePDF()
    {
      return Context(cdContextPDF());
    }
    static Context MetafileMF()
    {
      return Context(cdContextMetafile());
    }
    static Context MetafileDGN()
    {
      return Context(cdContextDGN());
    }
    static Context MetafileDXF()
    {
      return Context(cdContextDXF());
    }
    static Context MetafileSVG()
    {
      return Context(cdContextSVG());
    }
    static Context MetafilePS()
    {
      return Context(cdContextPS());
    }
    static Context MetafileCGM()
    {
      return Context(cdContextCGM());
    }

#if 0
#define CD_SIZECB 0
    typedef int(*cdSizeCB)(cdCanvas *canvas, int w, int h, double w_mm, double h_mm);
#define CD_ABORT 1
#define CD_CONTINUE 0

#define CD_CGMCOUNTERCB 1
#define CD_CGMSCLMDECB 2
#define CD_CGMVDCEXTCB 3
#define CD_CGMBEGPICTCB 4
#define CD_CGMBEGPICTBCB 5
#define CD_CGMBEGMTFCB 6
static int cgm_countercb(cdCanvas *canvas, double percent);
static int cgm_sclmdecb(cdCanvas *canvas, short scl_mde, short *draw_mode_i, double *factor_f);
static int cgm_vdcextcb(cdCanvas *canvas, short type, void *xmn, void *ymn, void *xmx, void *ymx);
static int cgm_begpictcb(cdCanvas *canvas, char *pict);
static int cgm_begpictbcb(cdCanvas *canvas);
static int cgm_begmtfcb(cdCanvas *canvas, int *xmn, int *ymn, int *xmx, int *ymx);

    typedef int(*cdCallback)(cdCanvas* canvas, ...);
    int cdContextRegisterCallback(cdContext *context, int cb, cdCallback func);

    unsigned char* cdRedImage(cdCanvas* cnv);
    unsigned char* cdGreenImage(cdCanvas* cnv);
    unsigned char* cdBlueImage(cdCanvas* cnv);
    unsigned char* cdAlphaImage(cdCanvas* cnv);
#endif
  };


  class Canvas
  {
  protected:
    cdCanvas* canvas;

    /* forbidden */
    Canvas() { canvas = 0; }
    Canvas(const Canvas&) {}

  public:
    Canvas(cdCanvas* ref_canvas)
    {
      canvas = ref_canvas;
    }
    ~Canvas()
    {
      if (canvas) 
        cdKillCanvas(canvas);
    }

    Context GetContext()
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
    char* GetAttribute(const char* name)
    {
      return cdCanvasGetAttribute(canvas, name);
    }

    /* interpretation */
    int Play(const Context& context, int xmin, int xmax, int ymin, int ymax, void *data)
    {
      return cdCanvasPlay(canvas, context.cd_context, xmin, xmax, ymin, ymax, data);
    }
    int Play(const Context& context, double xmin, double xmax, double ymin, double ymax, void *data)
    {
      return cdfCanvasPlay(canvas, context.cd_context, xmin, xmax, ymin, ymax, data);
    } 
    int wPlay(const Context& context, double xmin, double xmax, double ymin, double ymax, void *data)
    {
      return wdCanvasPlay(canvas, context.cd_context, xmin, xmax, ymin, ymax, data);
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
    int IsPointInRegion(double x, double y)
    {
      return cdfCanvasIsPointInRegion(canvas, x, y);
    }
    int wIsPointInRegion(double x, double y)
    {
      return wdCanvasIsPointInRegion(canvas, x, y);
    }
    void OffsetRegion(int x, int y)
    {
      cdCanvasOffsetRegion(canvas, x, y);
    }
    void OffsetRegion(double x, double y)
    {
      cdfCanvasOffsetRegion(canvas, x, y);
    }
    void wOffsetRegion(double x, double y)
    {
      wdCanvasOffsetRegion(canvas, x, y);
    }
    void GetRegionBox(int &xmin, int &xmax, int &ymin, int &ymax)
    {
      cdCanvasGetRegionBox(canvas, &xmin, &xmax, &ymin, &ymax);
    }
    void GetRegionBox(double &xmin, double &xmax, double &ymin, double &ymax)
    {
      cdfCanvasGetRegionBox(canvas, &xmin, &xmax, &ymin, &ymax);
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
    void Pixel(double x, double y, long color)
    {
      cdfCanvasPixel(canvas, x, y, color);
    }
    void wPixel(double x, double y, long color)
    {
      wdCanvasPixel(canvas, x, y, color);
    }
    void Mark(int x, int y)
    {
      cdCanvasMark(canvas, x, y);
    }
    void Mark(double x, double y)
    {
      cdfCanvasMark(canvas, x, y);
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
    void GetTextBox(double x, double y, const char* s, double &xmin, double &xmax, double &ymin, double &ymax)
    {
      cdfCanvasGetTextBox(canvas, x, y, s, &xmin, &xmax, &ymin, &ymax);
    }
    void wGetTextBox(double x, double y, const char* s, double &xmin, double &xmax, double &ymin, double &ymax)
    {
      wdCanvasGetTextBox(canvas, x, y, s, &xmin, &xmax, &ymin, &ymax);
    }
    void GetTextBounds(int x, int y, const char* s, int *rect)
    {
      cdCanvasGetTextBounds(canvas, x, y, s, rect);
    }
    void GetTextBounds(double x, double y, const char* s, double *rect)
    {
      cdfCanvasGetTextBounds(canvas, x, y, s, rect);
    }
    void wGetTextBounds(double x, double y, const char* s, double *rect)
    {
      wdCanvasGetTextBounds(canvas, x, y, s, rect);
    }
    int GetColorPlanes()
    {
      return cdCanvasGetColorPlanes(canvas);
    }


    /* client images */
    void PutImage(const im::Image& image, int x, int y, int w, int h)
    {
      const imImage* im_image = image.im_image;
      if (im_image->color_space == IM_RGB)
      {
        if (im_image->has_alpha)
          cdCanvasPutImageRectRGBA(canvas, im_image->width, im_image->height,
                                   (unsigned char*)im_image->data[0],
                                   (unsigned char*)im_image->data[1],
                                   (unsigned char*)im_image->data[2],
                                   (unsigned char*)im_image->data[3],
                                   x, y, w, h, 0, 0, 0, 0);
        else
          cdCanvasPutImageRectRGB(canvas, im_image->width, im_image->height,
                                  (unsigned char*)im_image->data[0],
                                  (unsigned char*)im_image->data[1],
                                  (unsigned char*)im_image->data[2],
                                  x, y, w, h, 0, 0, 0, 0);
      }
      else
        cdCanvasPutImageRectMap(canvas, im_image->width, im_image->height,
                                (unsigned char*)im_image->data[0], im_image->palette,
                                x, y, w, h, 0, 0, 0, 0);
    }
    void PutImage(const im::Image& image, double x, double y, double w, double h)
    {
      const imImage* im_image = image.im_image;
      if (im_image->color_space == IM_RGB)
      {
        if (im_image->has_alpha)
          cdfCanvasPutImageRectRGBA(canvas, im_image->width, im_image->height,
                                    (unsigned char*)im_image->data[0],
                                    (unsigned char*)im_image->data[1],
                                    (unsigned char*)im_image->data[2],
                                    (unsigned char*)im_image->data[3],
                                    x, y, w, h, 0, 0, 0, 0);
        else
          cdfCanvasPutImageRectRGB(canvas, im_image->width, im_image->height,
                                   (unsigned char*)im_image->data[0],
                                   (unsigned char*)im_image->data[1],
                                   (unsigned char*)im_image->data[2],
                                   x, y, w, h, 0, 0, 0, 0);
      }
      else
        cdfCanvasPutImageRectMap(canvas, im_image->width, im_image->height,
                                 (unsigned char*)im_image->data[0], im_image->palette,
                                 x, y, w, h, 0, 0, 0, 0);
    }
    void wPutImage(const im::Image& image, double x, double y, double w, double h)
    {
      const imImage* im_image = image.im_image;
      if (im_image->color_space == IM_RGB)
      {
        if (im_image->has_alpha)
          wdCanvasPutImageRectRGBA(canvas, im_image->width, im_image->height,
                                   (unsigned char*)im_image->data[0],
                                   (unsigned char*)im_image->data[1],
                                   (unsigned char*)im_image->data[2],
                                   (unsigned char*)im_image->data[3],
                                   x, y, w, h, 0, 0, 0, 0);
        else
          wdCanvasPutImageRectRGB(canvas, im_image->width, im_image->height,
                                  (unsigned char*)im_image->data[0],
                                  (unsigned char*)im_image->data[1],
                                  (unsigned char*)im_image->data[2],
                                  x, y, w, h, 0, 0, 0, 0);
      }
      else
        wdCanvasPutImageRectMap(canvas, im_image->width, im_image->height,
                                (unsigned char*)im_image->data[0], im_image->palette,
                                x, y, w, h, 0, 0, 0, 0);
    }
    void GetImage(im::Image& image, int x, int y)
    {
      const imImage* im_image = image.im_image;
      cdCanvasGetImageRGB(canvas,
                          (unsigned char*)im_image->data[0],
                          (unsigned char*)im_image->data[1],
                          (unsigned char*)im_image->data[2],
                          x, y, im_image->width, im_image->height);
    }
    void GetImage(im::Image& image, double x, double y)
    {
      const imImage* im_image = image.im_image;
      cdfCanvasGetImageRGB(canvas,
                           (unsigned char*)im_image->data[0],
                           (unsigned char*)im_image->data[1],
                           (unsigned char*)im_image->data[2],
                           x, y, im_image->width, im_image->height);
    }
    void wGetImage(im::Image& image, double x, double y)
    {
      const imImage* im_image = image.im_image;
      wdCanvasGetImageRGB(canvas,
                          (unsigned char*)im_image->data[0],
                          (unsigned char*)im_image->data[1],
                          (unsigned char*)im_image->data[2],
                          x, y, im_image->width, im_image->height);
    }
  };

  class CanvasImageRGB : public Canvas
  {
  public:
    CanvasImageRGB(int width, int height, bool has_alpha = false, double res = 0)
      : Canvas()
    {
      const char *alpha = "";
      if (has_alpha)
        alpha = "-a";

      if (res)
        canvas = cdCreateCanvasf(CD_IMAGERGB, "%dx%d %g %s", width, height, res, alpha);
      else
        canvas = cdCreateCanvasf(CD_IMAGERGB, "%dx%d %s", width, height, alpha);
    }
    CanvasImageRGB(im::Image& image, double res = 0)
      : Canvas()
    {
      imImage* im_image = image.im_image;

      if (im_image->color_space != IM_RGB || im_image->data_type != IM_BYTE)
        return;

      if (!res)
      {
        const char* res_unit = image.GetAttribString("ResolutionUnit");
        if (res_unit)
        {
          double xres = image.GetAttribReal("XResolution");
          if (xres)
          {
            /* to DPM */
            if (res_unit[0] == 'D' &&
                res_unit[1] == 'P' &&
                res_unit[2] == 'I')
              res = xres / (10. * 2.54);
            else  /* DPC */
              res = xres / 10.0;
          }
        }
      }

      if (res)
      {
        if (im_image->has_alpha)
          canvas = cdCreateCanvasf(CD_IMAGERGB, "%dx%d %p %p %p %p -a", im_image->width, im_image->height,
                                   im_image->data[0], im_image->data[1], im_image->data[2], im_image->data[3]);
        else
          canvas = cdCreateCanvasf(CD_IMAGERGB, "%dx%d %p %p %p", im_image->width, im_image->height,
                                   im_image->data[0], im_image->data[1], im_image->data[2]);
      }
      else
      {
        if (im_image->has_alpha)
          canvas = cdCreateCanvasf(CD_IMAGERGB, "%dx%d %p %p %p %p -r%g -a", im_image->width, im_image->height,
                                                 im_image->data[0], im_image->data[1], im_image->data[2], im_image->data[3], res);
        else
          canvas = cdCreateCanvasf(CD_IMAGERGB, "%dx%d %p %p %p -r%g", im_image->width, im_image->height,
                                                 im_image->data[0], im_image->data[1], im_image->data[2], res);
      }
    }
  };
  class CanvasMetafileEMF : public Canvas
  {
  public:
    CanvasMetafileEMF(const char* filename, int width, int height, double res = 0)
      : Canvas()
    {
      if (res)
        canvas = cdCreateCanvasf(CD_EMF, "\"%s\" %dx%d %g", filename, width, height, res);
      else
        canvas = cdCreateCanvasf(CD_EMF, "\"%s\" %dx%d", filename, width, height);
    }
  };
  class CanvasMetafileWMF : public Canvas
  {
  public:
    CanvasMetafileWMF(const char* filename, int width, int height, double res = 0)
      : Canvas()
    {
      if (res)
        canvas = cdCreateCanvasf(CD_WMF, "\"%s\" %dx%d %g", filename, width, height, res);
      else
        canvas = cdCreateCanvasf(CD_WMF, "\"%s\" %dx%d", filename, width, height);
    }
  };
  class CanvasMetafileMF : public Canvas
  {
  public:
    CanvasMetafileMF(const char* filename, double res = 0)
      : Canvas()
    {
      if (res)
        canvas = cdCreateCanvasf(CD_METAFILE, "\"%s\" %g", filename, res);
      else
        canvas = cdCreateCanvasf(CD_METAFILE, "\"%s\"", filename);
    }
    CanvasMetafileMF(const char* filename, double width_mm, double height_mm, double res = 0)
      : Canvas()
    {
      if (res)
        canvas = cdCreateCanvasf(CD_METAFILE, "\"%s\" %gx%g %g", filename, width_mm, height_mm, res);
      else
        canvas = cdCreateCanvasf(CD_METAFILE, "\"%s\" %gx%g", filename, width_mm, height_mm);
    }
  };
  class CanvasMetafileSVG : public Canvas
  {
  public:
    CanvasMetafileSVG(const char* filename, double res = 0)
      : Canvas()
    {
      if (res)
        canvas = cdCreateCanvasf(CD_SVG, "\"%s\" %g", filename, res);
      else
        canvas = cdCreateCanvasf(CD_SVG, "\"%s\"", filename);
    }
    CanvasMetafileSVG(const char* filename, double width_mm, double height_mm, double res = 0)
      : Canvas()
    {
      if (res)
        canvas = cdCreateCanvasf(CD_SVG, "\"%s\" %gx%g %g", filename, width_mm, height_mm, res);
      else
        canvas = cdCreateCanvasf(CD_SVG, "\"%s\" %gx%g", filename, width_mm, height_mm);
    }
  };
  class CanvasMetafileDebug : public Canvas
  {
  public:
    CanvasMetafileDebug(const char* filename, double res = 0)
      : Canvas()
    {
      if (res)
        canvas = cdCreateCanvasf(CD_DEBUG, "\"%s\" %g", filename, res);
      else
        canvas = cdCreateCanvasf(CD_DEBUG, "\"%s\"", filename);
    }
    CanvasMetafileDebug(const char* filename, double width_mm, double height_mm, double res = 0)
      : Canvas()
    {
      if (res)
        canvas = cdCreateCanvasf(CD_DEBUG, "\"%s\" %gx%g %g", filename, width_mm, height_mm, res);
      else
        canvas = cdCreateCanvasf(CD_DEBUG, "\"%s\" %gx%g", filename, width_mm, height_mm);
    }
  };
  class NativePrinter : public Canvas
  {
  public:
    NativePrinter(const char* name, bool show_dialog = false)
      : Canvas()
    {
      if (show_dialog)
        canvas = cdCreateCanvasf(CD_PRINTER, "%s -d", name);
      else
        canvas = cdCreateCanvasf(CD_PRINTER, "%s", name);
    }
  };
  class CanvasOpenGL : public Canvas
  {
  public:
    CanvasOpenGL(int width, int height, double res = 0)
      : Canvas()
    {
      if (res)
        canvas = cdCreateCanvasf(CD_GL, "%dx%d %g", width, height, res);
      else
        canvas = cdCreateCanvasf(CD_GL, "%dx%d", width, height);
    }
  };
  class CanvasPicture : public Canvas
  {
  public:
    CanvasPicture(double res = 0)
      : Canvas()
    {
      if (res)
        canvas = cdCreateCanvasf(CD_PICTURE, "%g", res);
      else
        canvas = cdCreateCanvas(CD_PICTURE, "");
    }
  };
#if 0
static Context NativeWindowImageRGBDoubleBufferIup()
static Context NativeWindowDoubleBufferIup()
static Context NativeWindowIup()
  Ihandle*
static Context NativeClipboardIup()
  "widthxheight [resolution] [-b]" //or in C "%dx%d %g -b"
  "[width_mmxheight_mm] [resolution] -m"  //or in C "%gx%g %g"
static Context MetafileDGN()
  "filename [widthxheight] [resolution] [-f] [-sseedfile]"   //or in C "%s %gx%g %g %s"
static Context MetafileDXF()
  "filename [widthxheight] [resolution] [-ac2000] [-limits xmin ymin xmax ymax]"    //or in C "%s %gx%g %g %s %s %g %g %g %g"
static Context MetafileCGM()
  "filename [widthxheight] [resolution] [-t] [-pprec] -d[description]" //or in C style "%s %gx%g %g %s"
static Context MetafilePDF()
  "filename -p[paper] -w[width] -h[height] -s[resolution] [-o]" //or in C "%s -p%d -w%g -h%g -s%d -o"
static Context MetafilePS()
  "filename -p[paper] -w[width] -h[height] -l[left] -r[right] -b[bottom] -t[top] -s[resolution] [-e] [-g] [-o] [-1] d[margin]" //"%s -p%d -w%g -h%g -l%g -r%g -b%g -t%g -s%d -e -o -1 -g -d%g"
#endif
}

#endif

