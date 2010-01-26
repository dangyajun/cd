/** \file
 * \brief Windows Double Buffer
 *
 * See Copyright Notice in cd.h
 */

#include <stdlib.h>
#include <stdio.h>

#include "cdwin.h"
#include "cddbuf.h"


static void cdkillcanvas(cdCtxCanvas *ctxcanvas)
{
  cdKillImage(ctxcanvas->image_dbuffer);
  cdwKillCanvas(ctxcanvas);
  
  memset(ctxcanvas, 0, sizeof(cdCtxCanvas));
  free(ctxcanvas);
}

static void cddeactivate(cdCtxCanvas* ctxcanvas)
{
  cdCanvas* canvas_dbuffer = ctxcanvas->canvas_dbuffer;
  /* this is done in the canvas_dbuffer context */
  cdCanvasDeactivate(canvas_dbuffer);
}

static void cdflush(cdCtxCanvas *ctxcanvas)
{
  int old_writemode;
  cdImage* image_dbuffer = ctxcanvas->image_dbuffer;
  cdCanvas* canvas_dbuffer = ctxcanvas->canvas_dbuffer;

  GdiFlush();

  /* this is done in the canvas_dbuffer context */
  /* Flush can be affected by Origin and Clipping, but not WriteMode */
  old_writemode = cdCanvasWriteMode(canvas_dbuffer, CD_REPLACE);
  cdCanvasPutImageRect(canvas_dbuffer, image_dbuffer, 0, 0, 0, 0, 0, 0);
  cdCanvasWriteMode(canvas_dbuffer, old_writemode);
}

static void cdcreatecanvas(cdCanvas* canvas, cdCanvas* canvas_dbuffer)
{
  cdCtxCanvas* ctxcanvas;
  cdImage* image_dbuffer;
  cdCtxImage* ctximage;

  /* this is done in the canvas_dbuffer context */
  image_dbuffer = cdCanvasCreateImage(canvas_dbuffer, canvas_dbuffer->w, canvas_dbuffer->h);
  if (!image_dbuffer) 
    return;

  ctximage = image_dbuffer->ctximage;
  
  /* Inicializa driver DBuffer */
  ctxcanvas = cdwCreateCanvas(canvas, NULL, ctximage->hDC, CDW_BMP);

  ctxcanvas->image_dbuffer = image_dbuffer;
  ctxcanvas->canvas_dbuffer = canvas_dbuffer;

  canvas->w = ctximage->w;
  canvas->h = ctximage->h;
  canvas->w_mm = ctximage->w_mm;
  canvas->h_mm = ctximage->h_mm;
  canvas->bpp = ctximage->bpp;
  canvas->xres = ctximage->xres;
  canvas->yres = ctximage->yres;
  ctxcanvas->clip_pnt[2].x = ctxcanvas->clip_pnt[1].x = ctximage->w - 1;
  ctxcanvas->clip_pnt[3].y = ctxcanvas->clip_pnt[2].y = ctximage->h - 1;
}

static int cdactivate(cdCtxCanvas *ctxcanvas)
{
  cdCanvas* canvas_dbuffer = ctxcanvas->canvas_dbuffer;

  /* this is done in the canvas_dbuffer context */
  /* this will update canvas size */
  cdCanvasActivate(canvas_dbuffer);

  /* check if the size changed */
  if (canvas_dbuffer->w != ctxcanvas->image_dbuffer->w ||
      canvas_dbuffer->h != ctxcanvas->image_dbuffer->h)
  {
    cdCanvas* canvas = ctxcanvas->canvas;
    /* save the current, if the rebuild fail */
    cdImage* old_image_dbuffer = ctxcanvas->image_dbuffer;
    cdCtxCanvas* old_ctxcanvas = ctxcanvas;

    /* if the image is rebuild, the canvas that uses the image must be also rebuild */

    /* rebuild the image and the canvas */
    canvas->ctxcanvas = NULL;
    cdcreatecanvas(canvas, canvas_dbuffer);
    if (!canvas->ctxcanvas)
    {
      canvas->ctxcanvas = old_ctxcanvas;
      return CD_ERROR;
    }

    /* remove the old image and canvas */
    cdKillImage(old_image_dbuffer);
    cdwKillCanvas(old_ctxcanvas);
    free(old_ctxcanvas);

    ctxcanvas = canvas->ctxcanvas;

    /* update canvas attributes */
    canvas->cxBackground(ctxcanvas, canvas->background);
    canvas->cxForeground(ctxcanvas, canvas->foreground);
    canvas->cxBackOpacity(ctxcanvas, canvas->back_opacity);
    canvas->cxWriteMode(ctxcanvas, canvas->write_mode);
    canvas->cxLineStyle(ctxcanvas, canvas->line_style);
    canvas->cxLineWidth(ctxcanvas, canvas->line_width);
    canvas->cxLineCap(ctxcanvas, canvas->line_cap);
    canvas->cxLineJoin(ctxcanvas, canvas->line_join);
    canvas->cxHatch(ctxcanvas, canvas->hatch_style);
    if (canvas->stipple) canvas->cxStipple(ctxcanvas, canvas->stipple_w, canvas->stipple_h, canvas->stipple);
    if (canvas->pattern) canvas->cxPattern(ctxcanvas, canvas->pattern_w, canvas->pattern_h, canvas->pattern);
    canvas->cxInteriorStyle(ctxcanvas, canvas->interior_style);
    if (canvas->native_font[0] == 0) canvas->cxFont(ctxcanvas, canvas->font_type_face, canvas->font_style, canvas->font_size);
    else canvas->cxNativeFont(ctxcanvas, canvas->native_font);
    canvas->cxTextAlignment(ctxcanvas, canvas->text_alignment);
    canvas->cxTextOrientation(ctxcanvas, canvas->text_orientation);
    if (canvas->use_matrix && canvas->cxTransform) canvas->cxTransform(ctxcanvas, canvas->matrix);
    if (canvas->clip_mode == CD_CLIPAREA && canvas->cxClipArea) canvas->cxClipArea(ctxcanvas, canvas->clip_rect.xmin, canvas->clip_rect.xmax, canvas->clip_rect.ymin, canvas->clip_rect.ymax);
/*    if (canvas->clip_mode == CD_CLIPAREA && canvas->cxFClipArea) canvas->cxFClipArea(ctxcanvas, canvas->clip_frect.xmin, canvas->clip_frect.xmax, canvas->clip_frect.ymin, canvas->clip_frect.ymax); */
    if (canvas->clip_mode == CD_CLIPPOLYGON && canvas->clip_poly) canvas->cxPoly(ctxcanvas, CD_CLIP, canvas->clip_poly, canvas->clip_poly_n);
/*    if (canvas->clip_mode == CD_CLIPPOLYGON && canvas->clip_fpoly) canvas->cxFPoly(ctxcanvas, CD_CLIP, canvas->clip_fpoly, canvas->clip_poly_n); */
    if (canvas->clip_mode != CD_CLIPOFF) canvas->cxClip(ctxcanvas, canvas->clip_mode);
  }

  return CD_OK;
}

static void cdinittable(cdCanvas* canvas)
{
  cdwInitTable(canvas);

  canvas->cxActivate = cdactivate;
  canvas->cxDeactivate = cddeactivate;
  canvas->cxFlush = cdflush;
  canvas->cxKillCanvas = cdkillcanvas;
}

static cdContext cdDBufferContext =
{
  CD_CAP_ALL & ~(CD_CAP_PLAY | CD_CAP_YAXIS | 
                 CD_CAP_FPRIMTIVES ),
  0,
  cdcreatecanvas,  
  cdinittable,
  NULL,             
  NULL, 
};

cdContext* cdContextDBuffer(void)
{
  if (cdUseContextPlus(CD_QUERY))
  {
    cdContext* ctx = cdGetContextPlus(CD_CTX_DBUFFER);
    if (ctx != NULL)
      return ctx;
  }

  return &cdDBufferContext;
}