/** \file
 * \brief Font Properties Estimation
 *
 * See Copyright Notice in cd.h
 */


#include "cd.h"
#include "cd_private.h"
#include <stdlib.h>
#include <string.h>

typedef struct _cd_font_styles
{
  unsigned char s[4];
}cd_font_styles;

static cd_font_styles helv[256] = {
{{0, 0, 0, 0}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{30, 30, 30, 30}},
{{30, 30, 30, 35}},
{{35, 50, 35, 50}},
{{55, 55, 55, 55}},
{{55, 55, 55, 55}},
{{90, 85, 90, 90}},
{{70, 75, 70, 75}},
{{20, 25, 20, 25}},
{{35, 35, 35, 35}},
{{35, 35, 35, 35}},
{{40, 40, 40, 40}},
{{60, 60, 60, 60}},
{{30, 30, 30, 30}},
{{35, 35, 35, 35}},
{{30, 30, 30, 30}},
{{30, 30, 30, 30}},
{{55, 55, 55, 55}},
{{55, 55, 55, 55}},
{{55, 55, 55, 55}},
{{55, 55, 55, 55}},
{{55, 55, 55, 55}},
{{55, 55, 55, 55}},
{{55, 55, 55, 55}},
{{55, 55, 55, 55}},
{{55, 55, 55, 55}},
{{55, 55, 55, 55}},
{{30, 35, 30, 35}},
{{30, 35, 30, 35}},
{{60, 60, 60, 60}},
{{60, 60, 60, 60}},
{{60, 60, 60, 60}},
{{55, 65, 55, 60}},
{{100, 100, 100, 100}},
{{65, 70, 70, 75}},
{{70, 70, 70, 70}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{70, 65, 70, 70}},
{{60, 60, 65, 65}},
{{80, 80, 80, 80}},
{{70, 70, 75, 75}},
{{25, 30, 30, 30}},
{{50, 55, 55, 55}},
{{70, 75, 70, 75}},
{{55, 65, 55, 65}},
{{80, 85, 85, 90}},
{{70, 70, 75, 75}},
{{80, 80, 80, 80}},
{{65, 70, 70, 70}},
{{80, 80, 80, 80}},
{{75, 70, 75, 75}},
{{70, 70, 70, 70}},
{{60, 65, 65, 65}},
{{70, 70, 75, 75}},
{{65, 70, 70, 70}},
{{100, 95, 100, 95}},
{{65, 70, 70, 70}},
{{65, 65, 70, 65}},
{{60, 60, 65, 65}},
{{30, 35, 30, 35}},
{{30, 30, 30, 30}},
{{30, 35, 30, 35}},
{{45, 60, 50, 60}},
{{55, 55, 55, 55}},
{{35, 35, 35, 35}},
{{55, 60, 55, 60}},
{{55, 65, 60, 65}},
{{55, 55, 55, 55}},
{{55, 65, 55, 60}},
{{55, 60, 55, 55}},
{{30, 35, 30, 35}},
{{55, 65, 55, 60}},
{{55, 65, 55, 65}},
{{25, 30, 25, 30}},
{{25, 30, 25, 30}},
{{50, 60, 55, 60}},
{{25, 30, 25, 30}},
{{85, 90, 85, 90}},
{{55, 65, 55, 65}},
{{55, 65, 55, 65}},
{{55, 65, 55, 60}},
{{55, 65, 55, 65}},
{{35, 40, 35, 40}},
{{55, 55, 55, 55}},
{{30, 35, 30, 35}},
{{55, 65, 55, 60}},
{{50, 55, 50, 55}},
{{75, 80, 70, 80}},
{{50, 60, 50, 55}},
{{50, 55, 50, 55}},
{{50, 55, 50, 50}},
{{35, 40, 35, 40}},
{{25, 30, 25, 30}},
{{35, 40, 35, 40}},
{{60, 60, 60, 60}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{25, 30, 25, 30}},
{{55, 55, 55, 55}},
{{35, 55, 30, 55}},
{{100, 100, 100, 100}},
{{55, 55, 55, 55}},
{{55, 55, 55, 55}},
{{35, 35, 35, 35}},
{{100, 105, 100, 100}},
{{70, 70, 70, 70}},
{{35, 35, 35, 35}},
{{100, 105, 100, 100}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{25, 30, 20, 30}},
{{25, 30, 20, 30}},
{{35, 55, 35, 55}},
{{35, 55, 30, 55}},
{{35, 35, 35, 35}},
{{55, 55, 55, 55}},
{{100, 100, 100, 100}},
{{30, 35, 30, 35}},
{{100, 100, 100, 100}},
{{55, 55, 55, 55}},
{{35, 35, 35, 35}},
{{95, 95, 95, 95}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{65, 65, 70, 65}},
{{30, 30, 30, 30}},
{{30, 30, 35, 35}},
{{55, 55, 55, 60}},
{{55, 55, 55, 55}},
{{60, 55, 60, 55}},
{{55, 55, 55, 55}},
{{25, 30, 25, 30}},
{{55, 55, 55, 55}},
{{35, 35, 35, 35}},
{{75, 75, 75, 80}},
{{35, 40, 40, 40}},
{{55, 55, 55, 55}},
{{60, 60, 60, 60}},
{{35, 35, 35, 35}},
{{75, 75, 75, 80}},
{{55, 55, 55, 55}},
{{40, 40, 40, 40}},
{{55, 55, 55, 60}},
{{35, 35, 35, 35}},
{{35, 35, 35, 35}},
{{35, 35, 35, 35}},
{{60, 60, 55, 60}},
{{55, 55, 55, 55}},
{{30, 30, 30, 30}},
{{35, 35, 35, 35}},
{{35, 35, 35, 35}},
{{40, 40, 40, 40}},
{{55, 55, 55, 55}},
{{85, 85, 85, 85}},
{{85, 85, 85, 85}},
{{85, 85, 85, 85}},
{{60, 65, 65, 60}},
{{65, 70, 70, 75}},
{{65, 70, 70, 75}},
{{65, 70, 70, 75}},
{{65, 70, 70, 75}},
{{65, 70, 70, 75}},
{{65, 70, 70, 75}},
{{100, 100, 100, 100}},
{{75, 75, 75, 75}},
{{70, 65, 70, 70}},
{{70, 65, 70, 70}},
{{70, 65, 70, 70}},
{{70, 65, 70, 70}},
{{25, 30, 30, 30}},
{{25, 30, 30, 30}},
{{25, 30, 30, 30}},
{{25, 30, 30, 30}},
{{75, 75, 75, 75}},
{{70, 70, 75, 75}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{60, 60, 60, 60}},
{{80, 80, 80, 80}},
{{70, 70, 75, 75}},
{{70, 70, 75, 75}},
{{70, 70, 75, 75}},
{{70, 70, 75, 75}},
{{65, 65, 70, 65}},
{{70, 70, 70, 70}},
{{65, 65, 65, 65}},
{{55, 60, 55, 60}},
{{55, 60, 55, 60}},
{{55, 60, 55, 60}},
{{55, 60, 55, 60}},
{{55, 60, 55, 60}},
{{55, 60, 55, 60}},
{{90, 90, 90, 90}},
{{55, 55, 55, 55}},
{{55, 60, 55, 55}},
{{55, 60, 55, 55}},
{{55, 60, 55, 55}},
{{55, 60, 55, 55}},
{{25, 30, 25, 30}},
{{25, 30, 25, 30}},
{{25, 30, 25, 30}},
{{25, 30, 25, 30}},
{{55, 65, 55, 60}},
{{55, 65, 55, 65}},
{{55, 65, 55, 65}},
{{55, 65, 55, 65}},
{{55, 65, 55, 65}},
{{55, 65, 55, 65}},
{{55, 65, 55, 65}},
{{55, 55, 55, 55}},
{{60, 65, 65, 60}},
{{55, 65, 55, 60}},
{{55, 65, 55, 60}},
{{55, 65, 55, 60}},
{{55, 65, 55, 60}},
{{50, 55, 50, 55}},
{{55, 65, 55, 65}},
{{50, 55, 50, 55}}
};

static cd_font_styles times[256] = {
{{0, 0, 0, 0}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{25, 25, 25, 25}},
{{35, 35, 30, 35}},
{{40, 55, 45, 55}},
{{55, 55, 55, 50}},
{{50, 55, 55, 55}},
{{85, 100, 85, 85}},
{{80, 85, 75, 80}},
{{20, 30, 25, 30}},
{{35, 35, 35, 35}},
{{35, 35, 35, 35}},
{{50, 55, 55, 55}},
{{60, 60, 70, 60}},
{{25, 25, 25, 25}},
{{35, 35, 35, 35}},
{{25, 25, 25, 25}},
{{30, 30, 30, 30}},
{{50, 55, 55, 55}},
{{50, 55, 55, 55}},
{{50, 55, 55, 55}},
{{50, 55, 55, 55}},
{{50, 55, 55, 55}},
{{50, 55, 55, 55}},
{{50, 55, 55, 55}},
{{50, 55, 55, 55}},
{{50, 55, 55, 55}},
{{50, 55, 55, 55}},
{{25, 35, 35, 35}},
{{30, 35, 35, 35}},
{{60, 60, 70, 60}},
{{60, 60, 70, 60}},
{{60, 60, 70, 60}},
{{45, 55, 55, 55}},
{{95, 95, 95, 85}},
{{70, 70, 60, 70}},
{{65, 70, 65, 70}},
{{70, 75, 70, 70}},
{{75, 75, 75, 75}},
{{60, 65, 65, 70}},
{{55, 60, 60, 70}},
{{70, 80, 75, 75}},
{{75, 80, 75, 80}},
{{35, 40, 35, 40}},
{{40, 55, 45, 50}},
{{75, 80, 65, 70}},
{{60, 65, 55, 65}},
{{90, 95, 85, 90}},
{{75, 75, 70, 75}},
{{75, 80, 75, 75}},
{{60, 65, 60, 65}},
{{75, 80, 75, 75}},
{{65, 75, 65, 70}},
{{55, 60, 55, 55}},
{{65, 65, 55, 65}},
{{70, 70, 75, 75}},
{{70, 70, 60, 65}},
{{95, 100, 80, 90}},
{{70, 70, 65, 70}},
{{70, 70, 55, 65}},
{{60, 65, 55, 65}},
{{35, 35, 40, 35}},
{{30, 30, 30, 30}},
{{35, 35, 45, 35}},
{{45, 60, 45, 60}},
{{55, 55, 55, 55}},
{{35, 35, 35, 35}},
{{45, 50, 55, 55}},
{{50, 55, 55, 50}},
{{45, 45, 45, 45}},
{{50, 55, 55, 55}},
{{45, 50, 45, 45}},
{{35, 35, 30, 35}},
{{50, 55, 55, 50}},
{{50, 55, 55, 55}},
{{25, 30, 30, 30}},
{{25, 35, 30, 30}},
{{50, 55, 50, 55}},
{{25, 30, 30, 30}},
{{75, 85, 75, 80}},
{{50, 55, 55, 55}},
{{50, 50, 55, 50}},
{{50, 55, 55, 55}},
{{50, 55, 55, 55}},
{{35, 45, 40, 40}},
{{40, 40, 40, 40}},
{{30, 35, 30, 30}},
{{50, 55, 55, 55}},
{{50, 50, 45, 45}},
{{70, 70, 65, 70}},
{{50, 50, 45, 55}},
{{50, 50, 45, 45}},
{{45, 45, 40, 40}},
{{50, 40, 40, 35}},
{{20, 25, 30, 25}},
{{50, 40, 40, 35}},
{{55, 55, 55, 60}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{35, 35, 35, 35}},
{{50, 55, 50, 55}},
{{45, 55, 55, 55}},
{{100, 100, 90, 100}},
{{50, 50, 55, 55}},
{{50, 55, 55, 55}},
{{35, 35, 35, 35}},
{{100, 100, 100, 105}},
{{55, 60, 55, 55}},
{{35, 35, 35, 35}},
{{90, 100, 95, 95}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{35, 35, 35, 35}},
{{35, 35, 35, 35}},
{{45, 55, 55, 55}},
{{45, 55, 55, 55}},
{{35, 40, 35, 35}},
{{55, 55, 55, 55}},
{{100, 100, 90, 100}},
{{35, 35, 35, 35}},
{{100, 105, 100, 100}},
{{40, 40, 40, 40}},
{{35, 35, 35, 35}},
{{75, 75, 70, 75}},
{{80, 80, 80, 80}},
{{80, 80, 80, 80}},
{{70, 70, 55, 65}},
{{25, 25, 25, 25}},
{{35, 30, 40, 35}},
{{50, 55, 55, 55}},
{{50, 55, 55, 55}},
{{55, 55, 50, 50}},
{{55, 55, 55, 55}},
{{20, 25, 30, 25}},
{{50, 55, 55, 55}},
{{35, 40, 30, 35}},
{{75, 75, 75, 75}},
{{30, 30, 30, 30}},
{{50, 55, 55, 55}},
{{60, 60, 70, 60}},
{{35, 35, 35, 35}},
{{75, 75, 80, 75}},
{{55, 55, 55, 55}},
{{40, 40, 40, 40}},
{{55, 55, 55, 55}},
{{30, 30, 30, 30}},
{{30, 30, 30, 30}},
{{30, 35, 35, 35}},
{{55, 60, 60, 60}},
{{45, 55, 55, 55}},
{{25, 25, 25, 25}},
{{30, 35, 35, 35}},
{{30, 30, 30, 30}},
{{30, 35, 35, 30}},
{{50, 55, 55, 55}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{75, 75, 75, 75}},
{{45, 55, 50, 55}},
{{70, 70, 60, 70}},
{{70, 70, 60, 70}},
{{70, 70, 60, 70}},
{{70, 70, 60, 70}},
{{70, 70, 60, 70}},
{{70, 70, 60, 70}},
{{90, 100, 90, 95}},
{{70, 75, 70, 70}},
{{60, 65, 65, 70}},
{{60, 65, 65, 70}},
{{60, 65, 65, 70}},
{{60, 65, 65, 70}},
{{35, 40, 35, 40}},
{{35, 40, 35, 40}},
{{35, 40, 35, 40}},
{{35, 40, 35, 40}},
{{75, 75, 75, 75}},
{{75, 75, 70, 75}},
{{75, 80, 75, 75}},
{{75, 80, 75, 75}},
{{75, 80, 75, 75}},
{{75, 80, 75, 75}},
{{75, 80, 75, 75}},
{{60, 60, 70, 60}},
{{75, 80, 75, 75}},
{{75, 70, 75, 75}},
{{75, 70, 75, 75}},
{{75, 70, 75, 75}},
{{75, 70, 75, 75}},
{{70, 70, 55, 65}},
{{60, 65, 65, 65}},
{{50, 55, 55, 55}},
{{45, 50, 55, 55}},
{{45, 50, 55, 55}},
{{45, 50, 55, 55}},
{{45, 50, 55, 55}},
{{45, 50, 55, 55}},
{{45, 50, 55, 55}},
{{70, 75, 70, 75}},
{{45, 45, 45, 45}},
{{45, 50, 45, 45}},
{{45, 50, 45, 45}},
{{45, 50, 45, 45}},
{{45, 50, 45, 45}},
{{30, 30, 30, 30}},
{{30, 30, 30, 30}},
{{30, 30, 30, 30}},
{{30, 30, 30, 30}},
{{55, 50, 55, 55}},
{{50, 55, 55, 55}},
{{50, 50, 55, 50}},
{{50, 50, 55, 50}},
{{50, 50, 55, 50}},
{{50, 50, 55, 50}},
{{50, 50, 55, 50}},
{{55, 55, 55, 55}},
{{55, 50, 55, 55}},
{{50, 55, 55, 55}},
{{50, 55, 55, 55}},
{{50, 55, 55, 55}},
{{50, 55, 55, 55}},
{{50, 50, 45, 45}},
{{55, 55, 50, 50}},
{{50, 50, 45, 45}}
};


typedef struct _cdFontType
{
  int max_width, line_height, ascent, descent, style, size;
  double sizex;
  int (*CharWidth)(char c);
}cdFontType;


static cdFontType font;


static int CharWidthCourier(char c)
{
  (void)c;
  return (int)(0.60 * font.sizex + 0.5);
}


static int CharWidthTimesRoman(char c)
{
  return (int)(times[(int)c].s[font.style] * font.sizex / 100 + 0.5);
}


static int CharWidthHelvetica(char c)
{
  return (int)(helv[(int)c].s[font.style] * font.sizex / 100 + 0.5);
}


static void cdFontEx(cdCanvas* canvas, const char* type_face, int style, int size)
{
  double mm_dx, mm_dy;
  double sizey, sizex;

  font.style = style;

  if (size < 0)
  {
    double size_mm;
    cdCanvasPixel2MM(canvas, -size, 0, &size_mm, NULL);
    size = (int)(size_mm * CD_MM2PT + 0.5);
  }

  font.size = size;

  cdCanvasPixel2MM(canvas, 1, 1, &mm_dx, &mm_dy);

  sizey = ((25.4 / 72) / mm_dy) * size;
  sizex = ((25.4 / 72) / mm_dx) * size;

  font.sizex = sizex;

  font.line_height = (int)(1.2 * sizey + 0.5);
  font.ascent = (int)(0.75 * font.line_height + 0.5);
  font.descent = (int)(0.20 * font.line_height + 0.5);

  if (strcmp(type_face, "Times")==0)
  {
    if (style == CD_PLAIN || style == CD_BOLD)
      font.max_width = (int)(1.05 * sizex + 0.5);
    else
      font.max_width = (int)(1.15 * sizex + 0.5);

    font.CharWidth = CharWidthTimesRoman;
  }
  else if (strcmp(type_face, "Helvetica")==0)
  {
    if (style == CD_PLAIN || style == CD_BOLD)
      font.max_width = (int)(1.05 * sizex + 0.5);
    else
      font.max_width = (int)(1.15 * sizex + 0.5);

    font.CharWidth = CharWidthHelvetica;
  }
  else
  {
    if (style == CD_PLAIN || style == CD_ITALIC)
      font.max_width = (int)(0.65 * sizex + 0.5);
    else
      font.max_width = (int)(0.80 * sizex + 0.5);

    font.CharWidth = CharWidthCourier;
  }
}

void cdgetfontdimEX(cdCtxCanvas* ctxcanvas, int *max_width, int *line_height, int *ascent, int *descent)
{
  cdCanvas* canvas = ((cdCtxCanvasBase*)ctxcanvas)->canvas;
  cdFontEx(canvas, canvas->font_type_face, canvas->font_style, canvas->font_size);
  if (line_height) *line_height = font.line_height;
  if (max_width) *max_width = font.max_width;
  if (ascent) *ascent = font.ascent;
  if (descent) *descent = font.descent;
}

void cdgettextsizeEX(cdCtxCanvas* ctxcanvas, const char *s, int len, int *width, int *height)
{
  int i = 0, w = 0;
  cdCanvas* canvas = ((cdCtxCanvasBase*)ctxcanvas)->canvas;
  cdFontEx(canvas, canvas->font_type_face, canvas->font_style, canvas->font_size);
  while (i < len)
  {
    w += font.CharWidth(s[i]);
    i++;
  }

  if (height) *height = font.line_height;
  if (width) *width = w;
}
