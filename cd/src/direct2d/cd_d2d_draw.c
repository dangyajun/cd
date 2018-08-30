#include <math.h>
#include "cd_d2d.h"
#include "cd.h"

#define LINECAP_FLAT 0    /* default */
#define LINEJOIN_MITER 0   /* default */

#define checkSwapCoord(_c1, _c2) { if (_c1 > _c2) { float t = _c2; _c2 = _c1; _c1 = t; } }   /* make sure _c1 is smaller than _c2 */

static void d2d_cd_init_color(dummy_D2D1_COLOR_F* c, long color)
{
  unsigned char red, green, blue, alpha;
  cdDecodeColorAlpha(color, &red, &green, &blue, &alpha);
  c->r = red / 255.0f;
  c->g = green / 255.0f;
  c->b = blue / 255.0f;
  c->a = alpha / 255.0f;
}

static void d2d_cd_setup_arc_segment(dummy_D2D1_ARC_SEGMENT* arc_seg, float cx, float cy, float rx, float ry,
                                     float base_angle, float sweep_angle)
{
  float sweep_rads = (base_angle + sweep_angle) * (PI / 180.0f);

  arc_seg->point.x = cx + rx * cosf(sweep_rads);
  arc_seg->point.y = cy + ry * sinf(sweep_rads);
  arc_seg->size.width = rx;
  arc_seg->size.height = ry;
  arc_seg->rotationAngle = 0.0f;

  if (sweep_angle >= 0.0f)
    arc_seg->sweepDirection = dummy_D2D1_SWEEP_DIRECTION_CLOCKWISE;
  else
    arc_seg->sweepDirection = dummy_D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE;

  if (sweep_angle >= 180.0f)
    arc_seg->arcSize = dummy_D2D1_ARC_SIZE_LARGE;
  else
    arc_seg->arcSize = dummy_D2D1_ARC_SIZE_SMALL;
}

static dummy_ID2D1Geometry* create_arc_geometry(float cx, float cy, float rx, float ry, float base_angle, float sweep_angle, int pie)
{
  dummy_ID2D1PathGeometry* g = NULL;
  dummy_ID2D1GeometrySink* s;
  HRESULT hr;
  float base_rads = base_angle * (PI / 180.0f);
  dummy_D2D1_POINT_2F pt;
  dummy_D2D1_ARC_SEGMENT arc_seg;

  hr = dummy_ID2D1Factory_CreatePathGeometry(d2d_cd_factory, &g);
  if (FAILED(hr)) {
    return NULL;
  }
  hr = dummy_ID2D1PathGeometry_Open(g, &s);
  if (FAILED(hr)) {
    dummy_ID2D1PathGeometry_Release(g);
    return NULL;
  }

  pt.x = cx + rx * cosf(base_rads);
  pt.y = cy + ry * sinf(base_rads);
  dummy_ID2D1GeometrySink_BeginFigure(s, pt, dummy_D2D1_FIGURE_BEGIN_FILLED);

  d2d_cd_setup_arc_segment(&arc_seg, cx, cy, rx, ry, base_angle, sweep_angle);
  dummy_ID2D1GeometrySink_AddArc(s, &arc_seg);

  if (pie == 2)
    dummy_ID2D1GeometrySink_EndFigure(s, dummy_D2D1_FIGURE_END_CLOSED);
  else if (pie == 1)
  {
    pt.x = cx;
    pt.y = cy;
    dummy_ID2D1GeometrySink_AddLine(s, pt);
    dummy_ID2D1GeometrySink_EndFigure(s, dummy_D2D1_FIGURE_END_CLOSED);
  }
  else
    dummy_ID2D1GeometrySink_EndFigure(s, dummy_D2D1_FIGURE_END_OPEN);

  dummy_ID2D1GeometrySink_Close(s);
  dummy_ID2D1GeometrySink_Release(s);

  return (dummy_ID2D1Geometry*)g;
}

static dummy_ID2D1StrokeStyle* createStrokeStyle(const float* dashes, UINT dashesCount, UINT lineCap, UINT lineJoin)
{
  HRESULT hr;
  dummy_D2D1_STROKE_STYLE_PROPERTIES p;
  dummy_ID2D1StrokeStyle *s;

  p.startCap = (dummy_D2D1_CAP_STYLE)0;
  p.endCap = (dummy_D2D1_CAP_STYLE)lineCap;
  p.dashCap = (dummy_D2D1_CAP_STYLE)lineCap;
  p.lineJoin = (dummy_D2D1_LINE_JOIN)lineJoin;
  p.miterLimit = 1.0f;
  p.dashStyle = dummy_D2D1_DASH_STYLE_CUSTOM;
  p.dashOffset = 0.0f;

  hr = dummy_ID2D1Factory_CreateStrokeStyle(d2d_cd_factory, &p, dashes, dashesCount, &s);
  if (FAILED(hr)) {
    return NULL;
  }

  return s;
}

dummy_ID2D1StrokeStyle *d2dSetLineStyle(int style)
{
  if (style == CD_DASHED)
  {
    float dashes[2] = { 9.0f, 3.0f };
    return createStrokeStyle(dashes, 2, LINECAP_FLAT, LINEJOIN_MITER);
  }
  else if (style == CD_DOTTED)
  {
    float dashes[2] = { 1.0f, 2.0f };
    return createStrokeStyle(dashes, 2, LINECAP_FLAT, LINEJOIN_MITER);
  }
  else if (style == CD_DASH_DOT)
  {
    float dashes[4] = { 7.0f, 3.0f, 1.0f, 3.0f };
    return createStrokeStyle(dashes, 4, LINECAP_FLAT, LINEJOIN_MITER);
  }
  else if (style == CD_DASH_DOT_DOT)
  {
    float dashes[6] = { 7.0f, 3.0f, 1.0f, 3.0f, 1.0f, 3.0f };
    return createStrokeStyle(dashes, 6, LINECAP_FLAT, LINEJOIN_MITER);
  }
  else
    return NULL;
}

dummy_ID2D1Brush* d2dCreateSolidBrush(dummy_ID2D1RenderTarget *target, long color)
 {
   dummy_ID2D1SolidColorBrush* b;
   dummy_D2D1_COLOR_F clr;
   HRESULT hr;

   d2d_cd_init_color(&clr, color);

   hr = dummy_ID2D1RenderTarget_CreateSolidColorBrush(target, &clr, NULL, &b);
   if (FAILED(hr)) {
     return NULL;
   }
   return (dummy_ID2D1Brush*)b;
 }

static void d2dFillEllipse(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float cx, float cy, float rx, float ry)
{
  dummy_D2D1_ELLIPSE e = { { cx, cy }, rx, ry };
  dummy_ID2D1RenderTarget_FillEllipse(target, &e, brush);
}

static void d2dFillEllipsePie(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float cx, float cy, float rx, float ry,
                    float fBaseAngle, float fSweepAngle)
{
  dummy_ID2D1Geometry* g = create_arc_geometry(cx, cy, rx, ry, fBaseAngle, fSweepAngle, 1);
  if (g == NULL)
    return;
  dummy_ID2D1RenderTarget_FillGeometry(target, g, brush, NULL);
  dummy_ID2D1Geometry_Release(g);
}

static void d2dFillEllipseChord(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float cx, float cy, float rx, float ry,
                              float fBaseAngle, float fSweepAngle)
{
  dummy_ID2D1Geometry* g = create_arc_geometry(cx, cy, rx, ry, fBaseAngle, fSweepAngle, 2);
  if (g == NULL)
    return;
  dummy_ID2D1RenderTarget_FillGeometry(target, g, brush, NULL);
  dummy_ID2D1Geometry_Release(g);
}

static void d2dDrawEllipse(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float cx, float cy, float rx, float ry,
                       float fStrokeWidth, dummy_ID2D1StrokeStyle *hStrokeStyle)
{
  dummy_D2D1_ELLIPSE e = { { cx, cy }, rx, ry };
  dummy_ID2D1RenderTarget_DrawEllipse(target, &e, brush, fStrokeWidth, hStrokeStyle);
}

static void d2dDrawEllipseArc(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float cx, float cy, float rx, float ry,
                          float fBaseAngle, float fSweepAngle, float fStrokeWidth, dummy_ID2D1StrokeStyle *hStrokeStyle)
{
  dummy_ID2D1Geometry* g = create_arc_geometry(cx, cy, rx, ry, fBaseAngle, fSweepAngle, 0);
  if (g == NULL)
    return;
  dummy_ID2D1RenderTarget_DrawGeometry(target, g, brush, fStrokeWidth, hStrokeStyle);
  dummy_ID2D1Geometry_Release(g);
}

static void d2dDrawEllipseChord(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float cx, float cy, float rx, float ry,
                              float fBaseAngle, float fSweepAngle, float fStrokeWidth, dummy_ID2D1StrokeStyle *hStrokeStyle)
{
  dummy_ID2D1Geometry* g = create_arc_geometry(cx, cy, rx, ry, fBaseAngle, fSweepAngle, 2);
  if (g == NULL)
    return;
  dummy_ID2D1RenderTarget_DrawGeometry(target, g, brush, fStrokeWidth, hStrokeStyle);
  dummy_ID2D1Geometry_Release(g);
}

void d2dFillRect(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float x0, float y0, float x1, float y1)
{
  dummy_D2D1_RECT_F r = { x0, y0, x1, y1 };
  dummy_ID2D1RenderTarget_FillRectangle(target, &r, brush);
}

void d2dDrawRect(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float x0, float y0, float x1, float y1, float fStrokeWidth, dummy_ID2D1StrokeStyle *hStrokeStyle)
{
  dummy_D2D1_RECT_F r = { x0, y0, x1, y1 };
  dummy_ID2D1RenderTarget_DrawRectangle(target, &r, brush, fStrokeWidth, hStrokeStyle);
}

void d2dDrawLine(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float x0, float y0, float x1, float y1, float fStrokeWidth, dummy_ID2D1StrokeStyle *hStrokeStyle)
{
  dummy_D2D1_POINT_2F pt0 = { (float)x0, (float)y0 };
  dummy_D2D1_POINT_2F pt1 = { (float)x1, (float)y1 };
  dummy_ID2D1RenderTarget_DrawLine(target, pt0, pt1, brush, fStrokeWidth, hStrokeStyle);
}

void d2dDrawArc(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float xc, float yc, float w, float h, double a1, double a2, float fStrokeWidth, dummy_ID2D1StrokeStyle *hStrokeStyle)
{
  float baseAngle = (float)(360.0 - a2);
  float sweepAngle = (float)(a2 - a1);

  if (sweepAngle == 360.0f)
    d2dDrawEllipse(target, brush, xc, yc, w / 2.0f, h / 2.0f, fStrokeWidth, hStrokeStyle);
  else
    d2dDrawEllipseArc(target, brush, xc, yc, w / 2.0f, h / 2.0f, baseAngle, sweepAngle, fStrokeWidth, hStrokeStyle);
}

void d2dFillArc(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, float xc, float yc, float w, float h, double a1, double a2, int pie)
{
  float baseAngle = (float)(360.0 - a2);
  float sweepAngle = (float)(a2 - a1);

  if (sweepAngle == 360.0f)
    d2dFillEllipse(target, brush, xc, yc, w / 2.0f, h / 2.0f);
  else if (pie)
    d2dFillEllipsePie(target, brush, xc, yc, w / 2.0f, h / 2.0f, baseAngle, sweepAngle);
  else
    d2dFillEllipseChord(target, brush, xc, yc, w / 2.0f, h / 2.0f, baseAngle, sweepAngle);
}

#define type2float(_x) ((float)(_x))

void d2dDrawPolygon(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, int* points, int count, int mode, float fStrokeWidth, dummy_ID2D1StrokeStyle *hStrokeStyle)
{
  dummy_ID2D1PathGeometry* g;
  dummy_ID2D1GeometrySink* sink;
  dummy_D2D1_POINT_2F pt;
  HRESULT hr;
  int i;

  hr = dummy_ID2D1Factory_CreatePathGeometry(d2d_cd_factory, &g);
  if (FAILED(hr)) {
    return;
  }
  hr = dummy_ID2D1PathGeometry_Open(g, &sink);

  pt.x = type2float(points[0]);
  pt.y = type2float(points[1]);
  dummy_ID2D1GeometrySink_BeginFigure(sink, pt, dummy_D2D1_FIGURE_BEGIN_FILLED);

  for (i = 2; i < count * 2; i = i + 2)
  {
    pt.x = type2float(points[i]);
    pt.y = type2float(points[i + 1]);
    dummy_ID2D1GeometrySink_AddLine(sink, pt);
  }

  if (mode == CD_CLOSED_LINES || mode == CD_FILL)
  {
    pt.x = type2float(points[0]);
    pt.y = type2float(points[1]);
    dummy_ID2D1GeometrySink_AddLine(sink, pt);
  }

  dummy_ID2D1GeometrySink_EndFigure(sink, (0 ? dummy_D2D1_FIGURE_END_CLOSED : dummy_D2D1_FIGURE_END_OPEN));
  dummy_ID2D1GeometrySink_Close(sink);
  dummy_ID2D1GeometrySink_Release(sink);

  if (mode == CD_FILL)
    dummy_ID2D1RenderTarget_FillGeometry(target, (dummy_ID2D1Geometry*)g, brush, NULL);
  else
    dummy_ID2D1RenderTarget_DrawGeometry(target, (dummy_ID2D1Geometry*)g, brush, fStrokeWidth, hStrokeStyle);
}

void d2dDrawPolygonF(dummy_ID2D1RenderTarget *target, dummy_ID2D1Brush *brush, double* points, int count, int mode, float fStrokeWidth, dummy_ID2D1StrokeStyle *hStrokeStyle)
{
  dummy_ID2D1PathGeometry* g;
  dummy_ID2D1GeometrySink* sink;
  dummy_D2D1_POINT_2F pt;
  HRESULT hr;
  int i;

  hr = dummy_ID2D1Factory_CreatePathGeometry(d2d_cd_factory, &g);
  if (FAILED(hr)) {
    return;
  }
  hr = dummy_ID2D1PathGeometry_Open(g, &sink);

  pt.x = type2float(points[0]);
  pt.y = type2float(points[1]);
  dummy_ID2D1GeometrySink_BeginFigure(sink, pt, dummy_D2D1_FIGURE_BEGIN_FILLED);

  for (i = 2; i < count * 2; i = i + 2)
  {
    pt.x = type2float(points[i]);
    pt.y = type2float(points[i + 1]);
    dummy_ID2D1GeometrySink_AddLine(sink, pt);
  }

  if (mode == CD_CLOSED_LINES || mode == CD_FILL)
  {
    pt.x = type2float(points[0]);
    pt.y = type2float(points[1]);
    dummy_ID2D1GeometrySink_AddLine(sink, pt);
  }

  dummy_ID2D1GeometrySink_EndFigure(sink, (0 ? dummy_D2D1_FIGURE_END_CLOSED : dummy_D2D1_FIGURE_END_OPEN));
  dummy_ID2D1GeometrySink_Close(sink);
  dummy_ID2D1GeometrySink_Release(sink);

  if (mode == CD_FILL)
    dummy_ID2D1RenderTarget_FillGeometry(target, (dummy_ID2D1Geometry*)g, brush, NULL);
  else
    dummy_ID2D1RenderTarget_DrawGeometry(target, (dummy_ID2D1Geometry*)g, brush, fStrokeWidth, hStrokeStyle);
}
