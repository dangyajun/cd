/** \file
 * \brief Windows direct2d Base Driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CDWIN_D2D_H
#define __CDWIN_D2D_H

#define COBJMACROS

#include "cd.h"
#include "cd_private.h"
#include "cd_d2d.h"

struct _cdCtxCanvas
{
  cdCanvas* canvas;

  d2dCanvas *d2d_canvas;
  d2dFont *font;

  dummy_ID2D1Brush *brush;
  dummy_ID2D1StrokeStyle *stroke_style;

  HWND hWnd; /* CDW_WIN handle */
  HDC hDC;   /* GDI Device Context handle */            
  int release_dc;

  int font_angle;
  int utf8mode;
};

cdCtxCanvas *cdwd2dCreateCanvas(cdCanvas* canvas, HWND hWnd, HDC hDc);
void cdwd2dKillCanvas(cdCtxCanvas* ctxcanvas);
void cdwd2dUpdateCanvas(cdCtxCanvas* ctxcanvas);

void cdwd2dInitTable(cdCanvas* canvas);

#endif 
