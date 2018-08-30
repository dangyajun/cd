/** \file
 * \brief Windows GDI+ Native Window Driver
 *
 * See Copyright Notice in cd.h
 */

#include "cdwin_d2d.h"
#include "cdnative.h"
#include <stdlib.h>
#include <stdio.h>


static d2dCanvas* d2dCreateCanvasWithPaintStruct(HWND hWnd, DWORD dwFlags)
{
  RECT rect;

  GetClientRect(hWnd, &rect);

  dummy_D2D1_RENDER_TARGET_PROPERTIES props = {
    dummy_D2D1_RENDER_TARGET_TYPE_DEFAULT,
    { dummy_DXGI_FORMAT_B8G8R8A8_UNORM, dummy_D2D1_ALPHA_MODE_PREMULTIPLIED },
    0.0f, 0.0f,
    ((dwFlags & CANVAS_NOGDICOMPAT) ?
    0 : dummy_D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE),
    dummy_D2D1_FEATURE_LEVEL_DEFAULT
  };
  dummy_D2D1_HWND_RENDER_TARGET_PROPERTIES props2;
  d2dCanvas* c;
  dummy_ID2D1HwndRenderTarget* target;
  HRESULT hr;

  props2.hwnd = hWnd;
  props2.pixelSize.width = rect.right - rect.left;
  props2.pixelSize.height = rect.bottom - rect.top;
  props2.presentOptions = dummy_D2D1_PRESENT_OPTIONS_NONE;

  /* Note ID2D1HwndRenderTarget is implicitly double-buffered. */
  hr = dummy_ID2D1Factory_CreateHwndRenderTarget(d2d_cd_factory, &props, &props2, &target);
  if (FAILED(hr)) {
    return NULL;
  }

  c = d2dCanvasCreate((dummy_ID2D1RenderTarget*)target, D2D_CANVASTYPE_HWND, (dwFlags & CANVAS_LAYOUTRTL));
  if (c == NULL) {
    return NULL;
  }

  /* make sure text anti-aliasing is clear type */
  dummy_ID2D1RenderTarget_SetTextAntialiasMode(c->target, dummy_D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

  return c;
}

static d2dCanvas* d2dCreateCanvasWithHDC(HDC hDC, const RECT* pRect, DWORD dwFlags)
{
  dummy_D2D1_RENDER_TARGET_PROPERTIES props = {
    dummy_D2D1_RENDER_TARGET_TYPE_DEFAULT,
    { dummy_DXGI_FORMAT_B8G8R8A8_UNORM, dummy_D2D1_ALPHA_MODE_PREMULTIPLIED },
    0.0f, 0.0f,
    ((dwFlags & CANVAS_NOGDICOMPAT) ?
    0 : dummy_D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE),
    dummy_D2D1_FEATURE_LEVEL_DEFAULT
  };
  d2dCanvas* c;
  dummy_ID2D1DCRenderTarget* target;
  HRESULT hr;

  hr = dummy_ID2D1Factory_CreateDCRenderTarget(d2d_cd_factory, &props, &target);
  if (FAILED(hr)) {
    return NULL;
  }

  hr = dummy_ID2D1DCRenderTarget_BindDC(target, hDC, pRect);
  if (FAILED(hr)) {
    dummy_ID2D1RenderTarget_Release((dummy_ID2D1RenderTarget*)target);
    return NULL;
  }

  c = d2dCanvasCreate((dummy_ID2D1RenderTarget*)target, D2D_CANVASTYPE_DC, (dwFlags & CANVAS_LAYOUTRTL));
  if (c == NULL) {
    dummy_ID2D1RenderTarget_Release((dummy_ID2D1RenderTarget*)target);
    return NULL;
  }

  /* make sure text anti-aliasing is clear type */
  dummy_ID2D1RenderTarget_SetTextAntialiasMode(c->target, dummy_D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

  return c;
}

static void cddeactivate(cdCtxCanvas *ctxcanvas)
{
  if (ctxcanvas->d2d_canvas)
  {
    dummy_ID2D1RenderTarget_Release(ctxcanvas->d2d_canvas->target);
    free(ctxcanvas->d2d_canvas);

    ctxcanvas->d2d_canvas = NULL;
  }
}

static int cdactivate(cdCtxCanvas* ctxcanvas)
{
  d2dCanvas *canvas;

  if (!ctxcanvas->hWnd && !ctxcanvas->hDC)
    return CD_ERROR;

  cddeactivate(ctxcanvas);

  if (ctxcanvas->hWnd)
  {
    RECT rect;
    GetClientRect(ctxcanvas->hWnd, &rect);
    ctxcanvas->canvas->w = rect.right - rect.left;
    ctxcanvas->canvas->h = rect.bottom - rect.top;

    ctxcanvas->canvas->w_mm = ((double)ctxcanvas->canvas->w) / ctxcanvas->canvas->xres;
    ctxcanvas->canvas->h_mm = ((double)ctxcanvas->canvas->h) / ctxcanvas->canvas->yres;

    ctxcanvas->canvas->bpp = cdGetScreenColorPlanes();

    canvas = d2dCreateCanvasWithPaintStruct(ctxcanvas->hWnd, CANVAS_DOUBLEBUFFER | CANVAS_NOGDICOMPAT);
  }
  else if (ctxcanvas->hDC)
  {
    RECT rcPaint;
    rcPaint.left = 0;
    rcPaint.top = 0;
    rcPaint.right = ctxcanvas->canvas->w;
    rcPaint.bottom = ctxcanvas->canvas->h;

    canvas = d2dCreateCanvasWithHDC(ctxcanvas->hDC, &rcPaint, CANVAS_DOUBLEBUFFER);
  }
  else
    return CD_ERROR;

  ctxcanvas->d2d_canvas = canvas;

  cdwd2dUpdateCanvas(ctxcanvas);

  dummy_ID2D1RenderTarget_BeginDraw(ctxcanvas->d2d_canvas->target);

  return CD_OK;
}

static void cdflush(cdCtxCanvas *ctxcanvas)
{
  HRESULT hr;

  d2dResetClip(ctxcanvas->d2d_canvas);

  hr = dummy_ID2D1RenderTarget_EndDraw(ctxcanvas->d2d_canvas->target, NULL, NULL);
}

static void cdkillcanvas(cdCtxCanvas* ctxcanvas)
{
  if (ctxcanvas->hDC && ctxcanvas->release_dc)
    ReleaseDC(NULL, ctxcanvas->hDC);  /* to match GetDC(NULL) */

  cddeactivate(ctxcanvas);

  cdwd2dKillCanvas(ctxcanvas);
}

static void cdcreatecanvas(cdCanvas* canvas, void *data)
{
  cdCtxCanvas* ctxcanvas;
  HWND hWnd = NULL;
  HDC hDC = NULL, ScreenDC;
  int release_dc = 0;

  if (!data)
  {
    hDC = GetDC(NULL);
    release_dc = 1;
    canvas->w = GetDeviceCaps(hDC, HORZRES);
    canvas->h = GetDeviceCaps(hDC, VERTRES);
  }
  else if (IsWindow((HWND)data)) 
  {
    RECT rect;
    hWnd = (HWND)data;

    GetClientRect(hWnd, &rect);
    canvas->w = rect.right - rect.left;
    canvas->h = rect.bottom - rect.top;
  }
  else  /* can be a HDC or a string */
  {
    DWORD objtype = GetObjectType((HGDIOBJ)data);
    if (objtype == OBJ_DC || objtype == OBJ_MEMDC || 
        objtype == OBJ_ENHMETADC || objtype == OBJ_METADC)   
    {
      hDC = (HDC)data;
      canvas->w = GetDeviceCaps(hDC, HORZRES);
      canvas->h = GetDeviceCaps(hDC, VERTRES);
    }
    else
      return;
  }

  ScreenDC = GetDC(NULL);
  canvas->bpp = GetDeviceCaps(ScreenDC, BITSPIXEL);
  canvas->xres = ((double)GetDeviceCaps(ScreenDC, LOGPIXELSX)) / 25.4;
  canvas->yres = ((double)GetDeviceCaps(ScreenDC, LOGPIXELSY)) / 25.4;
  ReleaseDC(NULL, ScreenDC);

  canvas->w_mm = ((double)canvas->w) / canvas->xres;
  canvas->h_mm = ((double)canvas->h) / canvas->yres;

  ctxcanvas = cdwd2dCreateCanvas(canvas, hWnd, hDC);
  
  ctxcanvas->hWnd = hWnd;
  ctxcanvas->hDC = hDC;
  ctxcanvas->release_dc = release_dc;
  ctxcanvas->canvas->invert_yaxis = 1;
}

static void cdinittable(cdCanvas* canvas)
{
  cdwd2dInitTable(canvas);

  canvas->cxKillCanvas = cdkillcanvas;
  canvas->cxActivate = cdactivate;
  canvas->cxDeactivate = cddeactivate;
  canvas->cxFlush = cdflush;
}

static cdContext cdNativeContext =
{
  CD_CAP_ALL & ~(CD_CAP_FLUSH | CD_CAP_PLAY | CD_CAP_YAXIS ),
  CD_CTX_WINDOW|CD_CTX_PLUS,
  cdcreatecanvas,
  cdinittable,
  NULL,              
  NULL,
};

cdContext* cdContextNativeWindowPlus(void)
{
  return &cdNativeContext;
}
