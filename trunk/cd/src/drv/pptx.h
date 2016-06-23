/** \file
 * \brief PPTX library
 *
 * See Copyright Notice in cd.h
 */

#ifndef __PPTX_H
#define __PPTX_H

struct _pptxPresentation
{
  FILE* slideFile;
  FILE* slideRelsFile;
  FILE* presentationFile;

  char *baseDir;
  char *pptDir;
  char *pptRelsDir;
  char *layoutsDir;
  char *layoutsRelDir;
  char *mastersDir;
  char *masterRelsDir;
  char *slidesDir;
  char *slidesRelsDir;
  char *themeDir;
  char *mediaDir;
  char *relsDir;

  int slideHeight;
  int slideWidth;
  int slideXRes;
  int slideYRes;

  int slideNum;
  int objectNum;
  int mediaNum;
  int imageId;

  int pixelSize;
};

#define PPTX_PPT_DIR          "ppt"
#define PPTX_PPT_RELS_DIR     "ppt/_rels"
#define PPTX_LAYOUTS_DIR      "ppt/slideLayouts"
#define PPTX_LAYOUTS_RELS_DIR "ppt/slideLayouts/_rels"
#define PPTX_MASTERS_DIR      "ppt/slideMasters"
#define PPTX_MASTERS_RELS_DIR "ppt/slideMasters/_rels"
#define PPTX_SLIDES_DIR       "ppt/slides"
#define PPTX_SLIDES_RELS_DIR  "ppt/slides/_rels"
#define PPTX_THEME_DIR        "ppt/theme"
#define PPTX_MEDIA_DIR        "ppt/media"
#define PPTX_RELS_DIR         "_rels"

#define PPTX_CONTENT_TYPES_FILE     "[Content_Types].xml"
#define PPTX_PRESENTATION_FILE      "ppt\\presentation.xml"
#define PPTX_PRES_PROPS_FILE        "ppt\\presProps.xml"
#define PPTX_IMAGE_FILE             "ppt\\media\\image%d.png"
#define PPTX_SLIDE_LAYOUT_FILE      "ppt\\slideLayouts\\slideLayout1.xml"
#define PPTX_SLIDE_LAYOUT_RELS_FILE "ppt\\slideLayouts\\_rels\\slideLayout1.xml.rels"
#define PPTX_SLIDE_MASTER_FILE       "ppt\\slideMasters\\slideMaster1.xml"
#define PPTX_SLIDE_MASTER_RELS       "ppt\\slideMasters\\_rels\\slideMaster1.xml.rels"
#define PPTX_SLIDE_FILE              "ppt\\slides\\slide%d.xml"
#define PPTX_SLIDE_RELS_FILE         "ppt\\slides\\_rels\\slide%d.xml.rels"
#define PPTX_THEME_FILE              "ppt\\theme\\theme1.xml"
#define PPTX_PRESENTATION_RELS       "ppt\\_rels\\presentation.xml.rels"
#define PPTX_RELS_FILE               "_rels\\.rels"


typedef struct _pptxPresentation pptxPresentation;

pptxPresentation *pptxCreatePresentation(const char *dirname, int width, int height);
int pptxKillPresentation(pptxPresentation *presentation, const char* dirname, const char* filename);

void pptxOpenSlide(pptxPresentation *presentation);
void pptxCloseSlide(pptxPresentation *presentation);

void pptxBeginLine(pptxPresentation *presentation, int xmin, int ymin, int w, int h);
void pptxMoveTo(pptxPresentation *presentation, int x, int y);
void pptxLineTo(pptxPresentation *presentation, int x, int y);
void pptxArcTo(pptxPresentation *presentation, int h, int w, double stAng, double swAng);
void pptxBezierLineTo(pptxPresentation *presentation, int c1x, int c1y, int c2x, int c2y, int c3x, int c3y);
void pptxLineClose(pptxPresentation *presentation);
void pptxNoFill(pptxPresentation *presentation);
void pptxSolidFill(pptxPresentation *presentation, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
void pptxHatchLine(pptxPresentation *presentation, const char* style, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned char bRed, unsigned char bGreen, unsigned char bBlue, unsigned char bAlpha);
void pptxEndLine(pptxPresentation *presentation, int width, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, const char* lineStyle, int nDashes, int *dashes);
void pptxBeginSector(pptxPresentation *presentation, const char *geomType, int xmin, int ymin, int w, int h, double angle1, double angle2);
void pptxEndSector(pptxPresentation *presentation);
void pptxText(pptxPresentation *presentation, int xmin, int ymin, int w, int h, double rotAngle, int bold, int italic, int underline, int strikeout, int size, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, const char* typeface, const char* text);
void pptxPixel(pptxPresentation *presentation, int x, int y, int width, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
void pptxImageMap(pptxPresentation *presentation, int iw, int ih, const unsigned char *index, const long int *colors, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax);
void pptxImageRGBA(pptxPresentation *presentation, int iw, int ih, const unsigned char *red, const unsigned char* green, const unsigned char *blue, const unsigned char *alpha, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax);
void pptxPattern(pptxPresentation *presentation, const unsigned char *red, const unsigned char* green, const unsigned char *blue, int width, int height);
void pptxStipple(pptxPresentation *presentation, unsigned char fRed, unsigned char fGreen, unsigned char fBlue, unsigned char bRed, unsigned char bGreen, unsigned char bBlue, const unsigned char *stipple, int width, int height, int backOpacity);


#endif
