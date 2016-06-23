/** \file
 * \brief PPTX library
 *
 * See Copyright Notice in cd.h
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include "cd.h"
#include "cd_private.h"

#include "pptx.h"

#define to_angle(_)   (int)((_)*60000)

struct _pptxPresentation
{
  char baseDir[10240];

  FILE* slideFile;
  FILE* slideRelsFile;
  FILE* presentationFile;

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
#define PPTX_PRESENTATION_FILE      "ppt/presentation.xml"
#define PPTX_PRES_PROPS_FILE        "ppt/presProps.xml"
#define PPTX_IMAGE_FILE             "ppt/media/image%d.png"
#define PPTX_SLIDE_LAYOUT_FILE      "ppt/slideLayouts/slideLayout1.xml"
#define PPTX_SLIDE_LAYOUT_RELS_FILE "ppt/slideLayouts/_rels/slideLayout1.xml.rels"
#define PPTX_SLIDE_MASTER_FILE      "ppt/slideMasters/slideMaster1.xml"
#define PPTX_SLIDE_MASTER_RELS      "ppt/slideMasters/_rels/slideMaster1.xml.rels"
#define PPTX_SLIDE_FILE             "ppt/slides/slide%d.xml"
#define PPTX_SLIDE_RELS_FILE        "ppt/slides/_rels/slide%d.xml.rels"
#define PPTX_THEME_FILE             "ppt/theme/theme1.xml"
#define PPTX_PRESENTATION_RELS      "ppt/_rels/presentation.xml.rels"
#define PPTX_RELS_FILE              "_rels/.rels"


int minizip(const char *filename, const char *temDir, const char **files, const int nFiles);


static int createImageFileRGBA(char *filename, int width, int height, const unsigned char *red, const unsigned char *green, const unsigned char *blue, const unsigned char *alpha)
{
#if 0
  int error;
  LodePNGState state;

  imImage *image = imImageCreate(width, height, IM_RGB, IM_BYTE);

  if (alpha != NULL)
  {
    imImageAddAlpha(image);
  }
  memcpy(image->data[0], red, width*height);
  memcpy(image->data[1], green, width*height);
  memcpy(image->data[2], blue, width*height);
  if (alpha != NULL)
  {
    memcpy(image->data[3], alpha, width*height);
  }

  imFile* ifile = imFileNew(filename, "PNG", &error);
  if (error)
    return error;

  error = imFileSaveImage(ifile, image);

  imFileClose(ifile);

  if (error)
    return error;
#endif

  return 0;
}

static int createImageFileMap(char *filename, int width, int height, const unsigned char *index, const long int *colors)
{
#if 0
  imImage *image = imImageInit(width, height, colorSpace, IM_BYTE, index, colors, 256);

  imFile* ifile = imFileNew(filename, "PNG", &error);
  if (error)
    return error;

  error = imFileSaveImage(ifile, image);

  imFileClose(ifile);

  if (error)
    return error;
#endif

  return 0;
}



/************************************  PRINT  ******************************************************/


static void printOpenSlide(FILE* slideFile, int objectNum)
{
  const char *prefix =
  {
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
    "<p:sld xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:mv=\"urn:schemas-microsoft-com:mac:vml\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\" xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" xmlns:dgm=\"http://schemas.openxmlformats.org/drawingml/2006/diagram\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:pvml=\"urn:schemas-microsoft-com:office:powerpoint\" xmlns:com=\"http://schemas.openxmlformats.org/drawingml/2006/compatibility\" xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\">\n"
    "\t<p:cSld>\n"
    "\t\t<p:spTree>\n"
    "\t\t\t<p:nvGrpSpPr>\n"
    "\t\t\t\t<p:cNvPr id=\"%d\" name=\"Shape %d\"/>\n"
    "\t\t\t\t<p:cNvGrpSpPr/>\n"
    "\t\t\t\t<p:nvPr/>\n"
    "\t\t\t</p:nvGrpSpPr>\n"
    "\t\t\t<p:grpSpPr>\n"
    "\t\t\t\t<a:xfrm>\n"
    "\t\t\t\t\t<a:off x=\"0\" y=\"0\"/>\n"
    "\t\t\t\t\t<a:ext cx=\"0\" cy=\"0\"/>\n"
    "\t\t\t\t\t<a:chOff x=\"0\" y=\"0\"/>\n"
    "\t\t\t\t\t<a:chExt cx=\"0\" cy=\"0\"/>\n"
    "\t\t\t\t</a:xfrm>\n"
    "\t\t\t</p:grpSpPr>\n"
  };

  fprintf(slideFile, prefix, objectNum, objectNum);
}

static void printOpenSlideRels(FILE* slideRelsFile)
{
  const char *rels =
  {
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
    "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\n"
    "\t<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout\" Target=\"../slideLayouts/slideLayout1.xml\"/>\n"
  };

  fprintf(slideRelsFile, rels);
}

static void printPresProps(FILE* presPropsFile)
{
  const char *presProps =
  {
    "<?xml version = \"1.0\" encoding = \"UTF-8\" standalone = \"yes\"?>\n"
    "<p:presentationPr xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" "
    "xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:mv=\"urn:schemas-microsoft-com:mac:vml\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\" "
    "xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" xmlns:dgm=\"http://schemas.openxmlformats.org/drawingml/2006/diagram\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" "
    "xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:pvml=\"urn:schemas-microsoft-com:office:powerpoint\" xmlns:com=\"http://schemas.openxmlformats.org/drawingml/2006/compatibility\" "
    "xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\"/>\n"
  };

  fprintf(presPropsFile, presProps);
}

static void printRels(FILE* relsFile)
{
  const char *rels =
  {
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
    "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\n"
    "\t<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"ppt/presentation.xml\"/>\n"
    "</Relationships>\n"
  };

  fprintf(relsFile, rels);
}

static void printLayoutRelsFile(FILE* layoutRelsFile)
{
  const char *rels =
  {
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
    "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\n"
    "\t<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster\" Target=\"../slideMasters/slideMaster1.xml\"/>\n"
    "</Relationships>\n"
  };

  fprintf(layoutRelsFile, rels);
}

static void printLayoutFile(FILE* layoutFile)
{
  const char *rels =
  {
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
    "\t<p:sldLayout xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" "
    "xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:mv=\"urn:schemas-microsoft-com:mac:vml\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\" "
    "xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" xmlns:dgm=\"http://schemas.openxmlformats.org/drawingml/2006/diagram\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" "
    "xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:pvml=\"urn:schemas-microsoft-com:office:powerpoint\" xmlns:com=\"http://schemas.openxmlformats.org/drawingml/2006/compatibility\" "
    "xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\" type=\"blank\">\n"
    "\t<p:cSld name=\"Blank\">\n"
    "\t\t<p:spTree>\n"
    "\t\t\t<p:nvGrpSpPr>\n"
    "\t\t\t\t<p:cNvPr id=\"48\" name=\"Shape 48\"/>\n"
    "\t\t\t\t<p:cNvGrpSpPr/>\n"
    "\t\t\t\t<p:nvPr/>\n"
    "\t\t\t</p:nvGrpSpPr>\n"
    "\t\t\t<p:grpSpPr>\n"
    "\t\t\t\t<a:xfrm>\n"
    "\t\t\t\t\t<a:off x=\"0\" y=\"0\"/>\n"
    "\t\t\t\t\t<a:ext cx=\"0\" cy=\"0\"/>\n"
    "\t\t\t\t\t<a:chOff x=\"0\" y=\"0\"/>\n"
    "\t\t\t\t\t<a:chExt cx=\"0\" cy=\"0\"/>\n"
    "\t\t\t\t</a:xfrm>\n"
    "\t\t\t</p:grpSpPr>\n"
    "\t\t\t</p:spTree>\n"
    "\t</p:cSld>\n"
    "\t<p:clrMapOvr>\n"
    "\t\t<a:masterClrMapping/>\n"
    "\t</p:clrMapOvr>\n"
    "</p:sldLayout>"
  };

  fprintf(layoutFile, rels);
}

static void printMasterRelsFile(FILE* masterRelsFile)
{
  const char *rels =
  {
    "<?xml version = \"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
    "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\n"
    "\t<Relationship Id=\"rId11\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout\" Target=\"../slideLayouts/slideLayout1.xml\"/>\n"
    "\t<Relationship Id=\"rId12\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"../theme/theme1.xml\"/>\n"
    "</Relationships>"
  };

  fprintf(masterRelsFile, rels);
}

static void printMasterFile(FILE* masterFile)
{
  const char *rels =
  {
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
    "<p:sldMaster xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" "
    "xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:mv=\"urn:schemas-microsoft-com:mac:vml\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\" "
    "xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" xmlns:dgm=\"http://schemas.openxmlformats.org/drawingml/2006/diagram\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" "
    "xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:pvml=\"urn:schemas-microsoft-com:office:powerpoint\" xmlns:com=\"http://schemas.openxmlformats.org/drawingml/2006/compatibility\" "
    "xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\">\n"
    "\t<p:cSld>\n"
    "\t\t<p:bg>\n"
    "\t\t\t<p:bgPr>\n"
    "\t\t\t\t<a:solidFill>\n"
    "\t\t\t\t\t<a:schemeClr val=\"lt1\"/>\n"
    "\t\t\t\t</a:solidFill>\n"
    "\t\t\t</p:bgPr>\n"
    "\t\t</p:bg>\n"
    "\t\t<p:spTree>\n"
    "\t\t\t<p:nvGrpSpPr>\n"
    "\t\t\t\t<p:cNvPr id=\"5\" name=\"Shape 5\"/>\n"
    "\t\t\t\t<p:cNvGrpSpPr/>\n"
    "\t\t\t\t<p:nvPr/>\n"
    "\t\t\t</p:nvGrpSpPr>\n"
    "\t\t\t<p:grpSpPr>\n"
    "\t\t\t\t<a:xfrm>\n"
    "\t\t\t\t\t<a:off x=\"0\" y=\"0\"/>\n"
    "\t\t\t\t\t<a:ext cx=\"0\" cy=\"0\"/>\n"
    "\t\t\t\t\t<a:chOff x=\"0\" y=\"0\"/>\n"
    "\t\t\t\t\t<a:chExt cx=\"0\" cy=\"0\"/>\n"
    "\t\t\t\t</a:xfrm>\n"
    "\t\t\t</p:grpSpPr>\n"
    "\t\t</p:spTree>\n"
    "\t</p:cSld>\n"
    "\t<p:clrMap accent1=\"accent4\" accent2=\"accent2\" accent3=\"accent3\" accent4=\"accent4\" accent5=\"accent5\" accent6=\"accent6\" bg1=\"lt1\" bg2=\"dk2\" tx1=\"dk1\" tx2=\"lt2\" folHlink=\"folHlink\" "
    "hlink=\"hlink\"/>\n"
    "\t<p:sldLayoutIdLst>\n"
    "\t\t<p:sldLayoutId id=\"2147483658\" r:id=\"rId11\"/>\n"
    "\t</p:sldLayoutIdLst>\n"
    "\t<p:hf dt=\"0\" ftr=\"0\" hdr=\"0\" sldNum=\"0\"/>\n"
    "</p:sldMaster>"
  };

  fprintf(masterFile, rels);
}

static void printThemeFile(FILE* themeFile)
{
  const char *rels =
  {
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
    "<a:theme xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" name=\"simple-light-2\">\n"
    "\t<a:themeElements>\n"
    "\t\t<a:clrScheme name=\"Simple Light\">\n"
    "\t\t\t<a:dk1>\n"
    "\t\t\t\t<a:srgbClr val=\"000000\"/>\n"
    "\t\t\t</a:dk1>\n"
    "\t\t\t<a:lt1>\n"
    "\t\t\t\t<a:srgbClr val=\"FFFFFF\"/>\n"
    "\t\t\t</a:lt1>\n"
    "\t\t\t<a:dk2>\n"
    "\t\t\t\t<a:srgbClr val=\"595959\"/>\n"
    "\t\t\t</a:dk2>\n"
    "\t\t\t<a:lt2>\n"
    "\t\t\t\t<a:srgbClr val=\"EEEEEE\"/>\n"
    "\t\t\t</a:lt2>\n"
    "\t\t\t<a:accent1>\n"
    "\t\t\t\t<a:srgbClr val=\"FFAB40\"/>\n"
    "\t\t\t</a:accent1>\n"
    "\t\t\t<a:accent2>\n"
    "\t\t\t\t<a:srgbClr val=\"212121\"/>\n"
    "\t\t\t</a:accent2>\n"
    "\t\t\t<a:accent3>\n"
    "\t\t\t\t<a:srgbClr val=\"78909C\"/>\n"
    "\t\t\t</a:accent3>\n"
    "\t\t\t<a:accent4>\n"
    "\t\t\t\t<a:srgbClr val=\"FFAB40\"/>\n"
    "\t\t\t</a:accent4>\n"
    "\t\t\t<a:accent5>\n"
    "\t\t\t\t<a:srgbClr val=\"0097A7\"/>\n"
    "\t\t\t</a:accent5>\n"
    "\t\t\t<a:accent6>\n"
    "\t\t\t\t<a:srgbClr val=\"EEFF41\"/>\n"
    "\t\t\t</a:accent6>\n"
    "\t\t\t<a:hlink>\n"
    "\t\t\t\t<a:srgbClr val=\"0097A7\"/>\n"
    "\t\t\t</a:hlink>\n"
    "\t\t\t<a:folHlink>\n"
    "\t\t\t\t<a:srgbClr val=\"0097A7\"/>\n"
    "\t\t\t</a:folHlink>\n"
    "\t\t</a:clrScheme>\n"
    "\t\t<a:fontScheme name=\"Office\">\n"
    "\t\t\t<a:majorFont>\n"
    "\t\t\t\t<a:latin typeface=\"Arial\"/>\n"
    "\t\t\t\t\t<a:ea typeface=\"\"/>\n"
    "\t\t\t\t\t<a:cs typeface=\"\"/>\n"
    "\t\t\t\t\t<a:font script=\"Arab\" typeface=\"Times New Roman\"/>\n"
    "\t\t\t\t\t<a:font script=\"Hebr\" typeface=\"Times New Roman\"/>\n"
    "\t\t\t\t\t<a:font script=\"Thai\" typeface=\"Angsana New\"/>\n"
    "\t\t\t\t\t<a:font script=\"Ethi\" typeface=\"Nyala\"/>\n"
    "\t\t\t\t\t<a:font script=\"Beng\" typeface=\"Vrinda\"/>\n"
    "\t\t\t\t\t<a:font script=\"Gujr\" typeface=\"Shruti\"/>\n"
    "\t\t\t\t\t<a:font script=\"Khmr\" typeface=\"MoolBoran\"/>\n"
    "\t\t\t\t\t<a:font script=\"Knda\" typeface=\"Tunga\"/>\n"
    "\t\t\t\t\t<a:font script=\"Guru\" typeface=\"Raavi\"/>\n"
    "\t\t\t\t\t<a:font script=\"Cans\" typeface=\"Euphemia\"/>\n"
    "\t\t\t\t\t<a:font script=\"Cher\" typeface=\"Plantagenet Cherokee\"/>\n"
    "\t\t\t\t\t<a:font script=\"Yiii\" typeface=\"Microsoft Yi Baiti\"/>\n"
    "\t\t\t\t\t<a:font script=\"Tibt\" typeface=\"Microsoft Himalaya\"/>\n"
    "\t\t\t\t\t<a:font script=\"Thaa\" typeface=\"MV Boli\"/>\n"
    "\t\t\t\t\t<a:font script=\"Deva\" typeface=\"Mangal\"/>\n"
    "\t\t\t\t\t<a:font script=\"Telu\" typeface=\"Gautami\"/>\n"
    "\t\t\t\t\t<a:font script=\"Taml\" typeface=\"Latha\"/>\n"
    "\t\t\t\t\t<a:font script=\"Syrc\" typeface=\"Estrangelo Edessa\"/>\n"
    "\t\t\t\t\t<a:font script=\"Orya\" typeface=\"Kalinga\"/>\n"
    "\t\t\t\t\t<a:font script=\"Mlym\" typeface=\"Kartika\"/>\n"
    "\t\t\t\t\t<a:font script=\"Laoo\" typeface=\"DokChampa\"/>\n"
    "\t\t\t\t\t<a:font script=\"Sinh\" typeface=\"Iskoola Pota\"/>\n"
    "\t\t\t\t\t<a:font script=\"Mong\" typeface=\"Mongolian Baiti\"/>\n"
    "\t\t\t\t\t<a:font script=\"Viet\" typeface=\"Times New Roman\"/>\n"
    "\t\t\t\t\t<a:font script=\"Uigh\" typeface=\"Microsoft Uighur\"/>\n"
    "\t\t\t\t\t<a:font script=\"Geor\" typeface=\"Sylfaen\"/>\n"
    "\t\t\t</a:majorFont>\n"
    "\t\t\t<a:minorFont>\n"
    "\t\t\t\t<a:latin typeface=\"Arial\"/>\n"
    "\t\t\t\t<a:ea typeface=\"\"/>\n"
    "\t\t\t\t<a:cs typeface=\"\"/>\n"
    "\t\t\t\t<a:font script=\"Arab\" typeface=\"Arial\"/>\n"
    "\t\t\t\t<a:font script=\"Hebr\" typeface=\"Arial\"/>\n"
    "\t\t\t\t<a:font script=\"Thai\" typeface=\"Cordia New\"/>\n"
    "\t\t\t\t<a:font script=\"Ethi\" typeface=\"Nyala\"/>\n"
    "\t\t\t\t<a:font script=\"Beng\" typeface=\"Vrinda\"/>\n"
    "\t\t\t\t<a:font script=\"Gujr\" typeface=\"Shruti\"/>\n"
    "\t\t\t\t<a:font script=\"Khmr\" typeface=\"DaunPenh\"/>\n"
    "\t\t\t\t<a:font script=\"Knda\" typeface=\"Tunga\"/>\n"
    "\t\t\t\t<a:font script=\"Guru\" typeface=\"Raavi\"/>\n"
    "\t\t\t\t<a:font script=\"Cans\" typeface=\"Euphemia\"/>\n"
    "\t\t\t\t<a:font script=\"Cher\" typeface=\"Plantagenet Cherokee\"/>\n"
    "\t\t\t\t<a:font script=\"Yiii\" typeface=\"Microsoft Yi Baiti\"/>\n"
    "\t\t\t\t<a:font script=\"Tibt\" typeface=\"Microsoft Himalaya\"/>\n"
    "\t\t\t\t<a:font script=\"Thaa\" typeface=\"MV Boli\"/>\n"
    "\t\t\t\t<a:font script=\"Deva\" typeface=\"Mangal\"/>\n"
    "\t\t\t\t<a:font script=\"Telu\" typeface=\"Gautami\"/>\n"
    "\t\t\t\t<a:font script=\"Taml\" typeface=\"Latha\"/>\n"
    "\t\t\t\t<a:font script=\"Syrc\" typeface=\"Estrangelo Edessa\"/>\n"
    "\t\t\t\t<a:font script=\"Orya\" typeface=\"Kalinga\"/>\n"
    "\t\t\t\t<a:font script=\"Mlym\" typeface=\"Kartika\"/>\n"
    "\t\t\t\t<a:font script=\"Laoo\" typeface=\"DokChampa\"/>\n"
    "\t\t\t\t<a:font script=\"Sinh\" typeface=\"Iskoola Pota\"/>\n"
    "\t\t\t\t<a:font script=\"Mong\" typeface=\"Mongolian Baiti\"/>\n"
    "\t\t\t\t<a:font script=\"Viet\" typeface=\"Arial\"/>\n"
    "\t\t\t\t<a:font script=\"Uigh\" typeface=\"Microsoft Uighur\"/>\n"
    "\t\t\t\t<a:font script=\"Geor\" typeface=\"Sylfaen\"/>\n"
    "\t\t\t</a:minorFont>\n"
    "\t\t</a:fontScheme>\n"
    "\t\t<a:fmtScheme name=\"Offce\">\n"
    "\t\t\t<a:fillStyleLst>\n"
    "\t\t\t\t<a:solidFill>\n"
    "\t\t\t\t\t<a:schemeClr val=\"phClr\"/>\n"
    "\t\t\t\t</a:solidFill>\n"
    "\t\t\t\t<a:gradFill rotWithShape=\"1\">\n"
    "\t\t\t\t\t<a:gsLst>\n"
    "\t\t\t\t\t\t<a:gs pos=\"0\">\n"
    "\t\t\t\t\t\t\t<a:schemeClr val=\"phClr\">\n"
    "\t\t\t\t\t\t\t\t<a:tint val=\"50000\"/>\n"
    "\t\t\t\t\t\t\t\t<a:satMod val=\"300000\"/>\n"
    "\t\t\t\t\t\t\t</a:schemeClr>\n"
    "\t\t\t\t\t\t</a:gs>\n"
    "\t\t\t\t\t\t<a:gs pos=\"35000\">\n"
    "\t\t\t\t\t\t\t<a:schemeClr val=\"phClr\">\n"
    "\t\t\t\t\t\t\t\t<a:tint val=\"37000\"/>\n"
    "\t\t\t\t\t\t\t\t<a:satMod val=\"300000\"/>\n"
    "\t\t\t\t\t\t\t</a:schemeClr>\n"
    "\t\t\t\t\t\t</a:gs>\n"
    "\t\t\t\t\t\t<a:gs pos=\"100000\">\n"
    "\t\t\t\t\t\t\t<a:schemeClr val=\"phClr\">\n"
    "\t\t\t\t\t\t\t\t<a:tint val=\"15000\"/>\n"
    "\t\t\t\t\t\t\t\t<a:satMod val=\"350000\"/>\n"
    "\t\t\t\t\t\t\t</a:schemeClr>\n"
    "\t\t\t\t\t\t</a:gs>\n"
    "\t\t\t\t\t</a:gsLst>\n"
    "\t\t\t\t\t<a:lin ang=\"16200000\" scaled=\"1\"/>\n"
    "\t\t\t\t</a:gradFill>\n"
    "\t\t\t\t<a:gradFill rotWithShape=\"1\">\n"
    "\t\t\t\t\t<a:gsLst>\n"
    "\t\t\t\t\t\t<a:gs pos=\"0\">\n"
    "\t\t\t\t\t\t\t<a:schemeClr val=\"phClr\">\n"
    "\t\t\t\t\t\t\t\t<a:tint val=\"100000\"/>\n"
    "\t\t\t\t\t\t\t\t<a:shade val=\"100000\"/>\n"
    "\t\t\t\t\t\t\t\t<a:satMod val=\"130000\"/>\n"
    "\t\t\t\t\t\t\t</a:schemeClr>\n"
    "\t\t\t\t\t\t</a:gs>\n"
    "\t\t\t\t\t\t<a:gs pos=\"100000\">\n"
    "\t\t\t\t\t\t\t<a:schemeClr val=\"phClr\">\n"
    "\t\t\t\t\t\t\t\t<a:tint val=\"50000\"/>\n"
    "\t\t\t\t\t\t\t\t<a:shade val=\"100000\"/>\n"
    "\t\t\t\t\t\t\t\t<a:satMod val=\"350000\"/>\n"
    "\t\t\t\t\t\t\t</a:schemeClr>\n"
    "\t\t\t\t\t\t</a:gs>\n"
    "\t\t\t\t\t</a:gsLst>\n"
    "\t\t\t\t\t<a:lin ang=\"16200000\" scaled=\"0\"/>\n"
    "\t\t\t\t</a:gradFill>\n"
    "\t\t\t</a:fillStyleLst>\n"
    "\t\t\t<a:lnStyleLst>\n"
    "\t\t\t\t<a:ln w=\"9525\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\">\n"
    "\t\t\t\t\t<a:solidFill>\n"
    "\t\t\t\t\t\t<a:schemeClr val=\"phClr\">\n"
    "\t\t\t\t\t\t\t<a:shade val=\"95000\"/>\n"
    "\t\t\t\t\t\t\t<a:satMod val=\"105000\"/>\n"
    "\t\t\t\t\t\t</a:schemeClr>\n"
    "\t\t\t\t\t</a:solidFill>\n"
    "\t\t\t\t\t<a:prstDash val=\"solid\"/>\n"
    "\t\t\t\t</a:ln>\n"
    "\t\t\t\t<a:ln w=\"25400\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\">\n"
    "\t\t\t\t\t<a:solidFill>\n"
    "\t\t\t\t\t\t<a:schemeClr val=\"phClr\"/>\n"
    "\t\t\t\t\t</a:solidFill>\n"
    "\t\t\t\t\t<a:prstDash val=\"solid\"/>\n"
    "\t\t\t\t</a:ln>\n"
    "\t\t\t\t<a:ln w=\"38100\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\">\n"
    "\t\t\t\t\t<a:solidFill>\n"
    "\t\t\t\t\t\t<a:schemeClr val=\"phClr\"/>\n"
    "\t\t\t\t\t</a:solidFill>\n"
    "\t\t\t\t\t<a:prstDash val=\"solid\"/>\n"
    "\t\t\t\t</a:ln>\n"
    "\t\t\t</a:lnStyleLst>\n"
    "\t\t\t<a:effectStyleLst>\n"
    "\t\t\t\t<a:effectStyle>\n"
    "\t\t\t\t\t<a:effectLst>\n"
    "\t\t\t\t\t\t<a:outerShdw blurRad=\"40000\" dist=\"20000\" dir=\"5400000\" rotWithShape=\"0\">\n"
    "\t\t\t\t\t\t\t<a:srgbClr val=\"000000\">\n"
    "\t\t\t\t\t\t\t\t<a:alpha val=\"38000\"/>\n"
    "\t\t\t\t\t\t\t</a:srgbClr>\n"
    "\t\t\t\t\t\t</a:outerShdw>\n"
    "\t\t\t\t\t</a:effectLst>\n"
    "\t\t\t\t</a:effectStyle>\n"
    "\t\t\t\t<a:effectStyle>\n"
    "\t\t\t\t\t<a:effectLst>\n"
    "\t\t\t\t\t\t<a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\">\n"
    "\t\t\t\t\t\t\t<a:srgbClr val=\"000000\">\n"
    "\t\t\t\t\t\t\t\t<a:alpha val=\"35000\"/>\n"
    "\t\t\t\t\t\t\t</a:srgbClr>\n"
    "\t\t\t\t\t\t</a:outerShdw>\n"
    "\t\t\t\t\t</a:effectLst>\n"
    "\t\t\t\t</a:effectStyle>\n"
    "\t\t\t\t<a:effectStyle>\n"
    "\t\t\t\t\t<a:effectLst>\n"
    "\t\t\t\t\t\t<a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\">\n"
    "\t\t\t\t\t\t\t<a:srgbClr val=\"000000\">\n"
    "\t\t\t\t\t\t\t\t<a:alpha val=\"35000\"/>\n"
    "\t\t\t\t\t\t\t</a:srgbClr>\n"
    "\t\t\t\t\t\t</a:outerShdw>\n"
    "\t\t\t\t\t</a:effectLst>\n"
    "\t\t\t\t\t<a:scene3d>\n"
    "\t\t\t\t\t\t<a:camera prst=\"orthographicFront\">\n"
    "\t\t\t\t\t\t\t<a:rot lat=\"0\" lon=\"0\" rev=\"0\"/>\n"
    "\t\t\t\t\t\t</a:camera>\n"
    "\t\t\t\t\t\t<a:lightRig rig=\"threePt\" dir=\"t\">\n"
    "\t\t\t\t\t\t\t<a:rot lat=\"0\" lon=\"0\" rev=\"1200000\"/>\n"
    "\t\t\t\t\t\t</a:lightRig>\n"
    "\t\t\t\t\t</a:scene3d>\n"
    "\t\t\t\t\t<a:sp3d>\n"
    "\t\t\t\t\t<a:bevelT w=\"63500\" h=\"25400\"/>\n"
    "\t\t\t\t\t</a:sp3d>\n"
    "\t\t\t\t</a:effectStyle>\n"
    "\t\t\t</a:effectStyleLst>\n"
    "\t\t\t<a:bgFillStyleLst>\n"
    "\t\t\t\t<a:solidFill>\n"
    "\t\t\t\t\t<a:schemeClr val=\"phClr\"/>\n"
    "\t\t\t\t</a:solidFill>\n"
    "\t\t\t\t<a:gradFill rotWithShape=\"1\">\n"
    "\t\t\t\t\t<a:gsLst>\n"
    "\t\t\t\t\t\t<a:gs pos=\"0\">\n"
    "\t\t\t\t\t\t\t<a:schemeClr val=\"phClr\">\n"
    "\t\t\t\t\t\t\t\t<a:tint val=\"40000\"/>\n"
    "\t\t\t\t\t\t\t\t<a:satMod val=\"350000\"/>\n"
    "\t\t\t\t\t\t\t</a:schemeClr>\n"
    "\t\t\t\t\t\t</a:gs>\n"
    "\t\t\t\t\t\t<a:gs pos=\"40000\">\n"
    "\t\t\t\t\t\t\t<a:schemeClr val=\"phClr\">\n"
    "\t\t\t\t\t\t\t\t<a:tint val=\"45000\"/>\n"
    "\t\t\t\t\t\t\t\t<a:shade val=\"99000\"/>\n"
    "\t\t\t\t\t\t\t\t<a:satMod val=\"350000\"/>\n"
    "\t\t\t\t\t\t\t</a:schemeClr>\n"
    "\t\t\t\t\t\t</a:gs>\n"
    "\t\t\t\t\t\t<a:gs pos=\"100000\">\n"
    "\t\t\t\t\t\t\t<a:schemeClr val=\"phClr\">\n"
    "\t\t\t\t\t\t\t\t<a:shade val=\"20000\"/>\n"
    "\t\t\t\t\t\t\t\t<a:satMod val=\"255000\"/>\n"
    "\t\t\t\t\t\t\t</a:schemeClr>\n"
    "\t\t\t\t\t\t</a:gs>\n"
    "\t\t\t\t\t</a:gsLst>\n"
    "\t\t\t\t\t<a:path path=\"circle\">\n"
    "\t\t\t\t\t\t<a:fillToRect l=\"50000\" t=\"-80000\" r=\"50000\" b=\"180000\"/>\n"
    "\t\t\t\t\t</a:path>\n"
    "\t\t\t\t</a:gradFill>\n"
    "\t\t\t\t<a:gradFill rotWithShape=\"1\">\n"
    "\t\t\t\t\t<a:gsLst>\n"
    "\t\t\t\t\t\t<a:gs pos=\"0\">\n"
    "\t\t\t\t\t\t\t<a:schemeClr val=\"phClr\">\n"
    "\t\t\t\t\t\t\t\t<a:tint val=\"80000\"/>\n"
    "\t\t\t\t\t\t\t\t<a:satMod val=\"300000\"/>\n"
    "\t\t\t\t\t\t\t</a:schemeClr>\n"
    "\t\t\t\t\t\t</a:gs>\n"
    "\t\t\t\t\t\t<a:gs pos=\"100000\">\n"
    "\t\t\t\t\t\t\t<a:schemeClr val=\"phClr\">\n"
    "\t\t\t\t\t\t\t\t<a:shade val=\"30000\"/>\n"
    "\t\t\t\t\t\t\t\t<a:satMod val=\"200000\"/>\n"
    "\t\t\t\t\t\t\t</a:schemeClr>\n"
    "\t\t\t\t\t\t</a:gs>\n"
    "\t\t\t\t\t</a:gsLst>\n"
    "\t\t\t\t\t<a:path path=\"circle\">\n"
    "\t\t\t\t\t\t<a:fillToRect l=\"50000\" t=\"50000\" r=\"50000\" b=\"50000\"/>\n"
    "\t\t\t\t\t</a:path>\n"
    "\t\t\t\t</a:gradFill>\n"
    "\t\t\t</a:bgFillStyleLst>\n"
    "\t\t</a:fmtScheme>\n"
    "\t</a:themeElements>\n"
    "</a:theme>\n"
  };

  fprintf(themeFile, rels);
}

static void printPresentation(FILE* presentationFile, int nSlides, int height, int width)
{
  int i;

  const char *presentationPrefix =
  {
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
    "<p:presentation xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:mv=\"urn:schemas-microsoft-com:mac:vml\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\" xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" xmlns:dgm=\"http://schemas.openxmlformats.org/drawingml/2006/diagram\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:pvml=\"urn:schemas-microsoft-com:office:powerpoint\" xmlns:com=\"http://schemas.openxmlformats.org/drawingml/2006/compatibility\" xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\" autoCompressPictures=\"0\" strictFirstAndLastChars=\"0\" saveSubsetFonts=\"1\">\n"
    "\t<p:sldMasterIdLst>\n"
    "\t\t<p:sldMasterId id=\"2147483659\" r:id=\"rId3\"/>\n"
    "\t\t\t</p:sldMasterIdLst>\n"
    "\t\t\t\t<p:sldIdLst>\n"
  };

  const char *slides =
  {
    "\t\t\t\t\t<p:sldId id=\"%d\" r:id=\"rId%d\"/>\n"
  };

  const char *presentationSuffix =
  {
    "\t\t\t\t</p:sldIdLst>\n"
    "\t\t\t\t<p:sldSz cy=\"%d\" cx=\"%d\"/>\n"
    "\t\t\t\t<p:notesSz cx=\"6858000\" cy=\"9144000\"/>\n"
    "</p:presentation>"
  };

  fprintf(presentationFile, presentationPrefix);
  for (i = 0; i < nSlides; i++)
    fprintf(presentationFile, slides, i + 256, i + 4);
  fprintf(presentationFile, presentationSuffix, height, width);
}

static void printContentTypes(FILE* ctFile, int nSlides)
{
  int i;

  const char *contentTypesPrefix =
  {
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
    "<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">\n"
    "\t<Default Extension=\"png\" ContentType=\"image/png\"/>\n"
    "\t<Default Extension=\"jpeg\" ContentType=\"image/jpeg\"/>\n"
    "\t<Default ContentType=\"application/xml\" Extension = \"xml\"/>\n"
    "\t<Default ContentType=\"application/vnd.openxmlformats-package.relationships+xml\" Extension=\"rels\"/>\n"
    "\t<Override ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slideLayout+xml\" PartName=\"/ppt/slideLayouts/slideLayout1.xml\"/>\n"
    "\t<Override ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slideMaster+xml\" PartName=\"/ppt/slideMasters/slideMaster1.xml\"/>\n"
  };

  const char *slide =
  {
    "\t<Override ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slide+xml\" PartName=\"/ppt/slides/slide%d.xml\"/>\n"
  };

  const char *contentTypesSuffix =
  {
    "\t<Override ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml\" PartName=\"/ppt/presentation.xml\"/>\n"
    "\t<Override ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.presProps+xml\" PartName=\"/ppt/presProps.xml\"/>\n"
    "\t<Override ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\" PartName=\"/ppt/theme/theme1.xml\"/>\n"
    "</Types>\n"
  };

  fprintf(ctFile, contentTypesPrefix);
  for (i = 0; i < nSlides; i++)
    fprintf(ctFile, slide, i + 1);
  fprintf(ctFile, contentTypesSuffix);
}

static void printPptRelsFile(FILE* pptRelsFile, int nSlides)
{
  int i;

  const char *relsPrefix =
  {
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
    "<Relationships xmlns = \"http://schemas.openxmlformats.org/package/2006/relationships\">\n"
    "\t<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme1.xml\"/>\n"
    "\t<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/presProps\" Target=\"presProps.xml\"/>\n"
    "\t<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster\" Target=\"slideMasters/slideMaster1.xml\"/>\n"
  };

  const char *slides =
  {
    "\t<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide\" Target=\"slides/slide%d.xml\"/>\n"
  };

  const char *relsSuffix =
  {
    "</Relationships>\n"
  };

  fprintf(pptRelsFile, relsPrefix);
  for (i = 0; i < nSlides; i++)
    fprintf(pptRelsFile, slides, i + 4, i + 1);
  fprintf(pptRelsFile, relsSuffix);
}

static void printCloseSlide(FILE* slideFile)
{
  const char *suffix =
  {
    "\t\t</p:spTree>\n"
    "\t</p:cSld>\n"
    "</p:sld>"
  };

  fprintf(slideFile, suffix);
}

static void printCloseSlideRels(FILE* slideRelsFile)
{
  const char *rels =
  {
    "</Relationships>"
  };

  fprintf(slideRelsFile, rels);
}

static void printSlideRels(pptxPresentation *presentation)
{
  const char *rels =
  {
    "\t<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"../media/image%d.png\"/>\n"
  };

  fprintf(presentation->slideRelsFile, rels, presentation->imageId, presentation->mediaNum);
}


/**************************************  FILE OPEN/CLOSE   ******************************************************/


int pptxOpenSlide(pptxPresentation *presentation)
{
  char filename[10240];

  sprintf(filename, "%s/"PPTX_SLIDE_FILE, presentation->baseDir, presentation->slideNum);

  presentation->slideFile = fopen(filename, "w");
  if (!presentation->slideFile)
    return 0;

  sprintf(filename, "%s/"PPTX_SLIDE_RELS_FILE, presentation->baseDir, presentation->slideNum);

  presentation->slideRelsFile = fopen(filename, "w");
  if (!presentation->slideRelsFile)
    return 0;

  printOpenSlide(presentation->slideFile, presentation->objectNum);

  printOpenSlideRels(presentation->slideRelsFile);

  presentation->objectNum++;
  return 1;
}

void pptxCloseSlide(pptxPresentation *presentation)
{
  fflush(presentation->slideFile);

  printCloseSlide(presentation->slideFile);

  printCloseSlideRels(presentation->slideRelsFile);

  fclose(presentation->slideFile);

  fclose(presentation->slideRelsFile);

  presentation->slideNum++;
}

static FILE* openFile(pptxPresentation *presentation, const char* subfile)
{
  char filename[10240];

  sprintf(filename, "%s/%s", presentation->baseDir, subfile);

  return fopen(filename, "w");
}

void pptxWritePresProps(pptxPresentation *presentation)
{
  FILE* presPropsFile = openFile(presentation, PPTX_PRES_PROPS_FILE);
  if (!presPropsFile)
    return;

  printPresProps(presPropsFile);

  fclose(presPropsFile);
}

void pptxWriteRels(pptxPresentation *presentation)
{
  FILE *relsFile = openFile(presentation, PPTX_RELS_FILE);
  if (!relsFile)
    return;

  printRels(relsFile);

  fclose(relsFile);
}

void pptxWriteLayoutRels(pptxPresentation *presentation)
{
  FILE *pptLayoutRels = openFile(presentation, PPTX_SLIDE_LAYOUT_RELS_FILE);
  if (!pptLayoutRels)
    return;

  printLayoutRelsFile(pptLayoutRels);

  fclose(pptLayoutRels);
}

void pptxWriteLayout(pptxPresentation *presentation)
{
  FILE *layoutRels = openFile(presentation, PPTX_SLIDE_LAYOUT_FILE);
  if (!layoutRels)
    return;

  printLayoutFile(layoutRels);

  fclose(layoutRels);
}

void pptxWriteMasterRels(pptxPresentation *presentation)
{
  FILE *masterRels = openFile(presentation, PPTX_SLIDE_MASTER_RELS);
  if (!masterRels)
    return;

  printMasterRelsFile(masterRels);

  fclose(masterRels);
}

void pptxWriteMaster(pptxPresentation *presentation)
{
  FILE *master = openFile(presentation, PPTX_SLIDE_MASTER_FILE);
  if (!master)
    return;

  printMasterFile(master);

  fclose(master);
}

void pptxWriteTheme(pptxPresentation *presentation)
{
  FILE *theme = openFile(presentation, PPTX_THEME_FILE);
  if (!theme)
    return;

  printThemeFile(theme);

  fclose(theme);
}

void pptxWritePresentation(pptxPresentation *presentation)
{
  presentation->presentationFile = openFile(presentation, PPTX_PRESENTATION_FILE);
  if (!presentation->presentationFile)
    return;

  printPresentation(presentation->presentationFile, presentation->slideNum, presentation->slideHeight, presentation->slideWidth);

  fclose(presentation->presentationFile);
}

void pptxWriteContentTypes(pptxPresentation *presentation)
{
  FILE *ctFile = openFile(presentation, PPTX_CONTENT_TYPES_FILE);
  if (!ctFile)
    return;

  printContentTypes(ctFile, presentation->slideNum);

  fclose(ctFile);
}

void pptxWritePptRels(pptxPresentation *presentation)
{
  FILE *pptRels = openFile(presentation, PPTX_PRESENTATION_RELS);
  if (!pptRels)
    return;

  printPptRelsFile(pptRels, presentation->slideNum);

  fclose(pptRels);
}


/**************************************  PRIMITIVES   ******************************************************/


void pptxBeginLine(pptxPresentation *presentation, int xmin, int ymin, int w, int h)
{
  const char *linePrefix =
  {
    "\t\t\t<p:sp>\n"
    "\t\t\t\t<p:nvSpPr>\n"
    "\t\t\t\t\t<p:cNvPr id=\"%d\" name=\"Shape %d\"/>\n"
    "\t\t\t\t\t<p:cNvSpPr/>\n"
    "\t\t\t\t\t<p:nvPr/>\n"
    "\t\t\t\t</p:nvSpPr>\n"
    "\t\t\t\t<p:spPr>\n"
    "\t\t\t\t\t<a:xfrm>\n"
    "\t\t\t\t\t\t<a:off x=\"%d\" y=\"%d\"/>\n"
    "\t\t\t\t\t\t<a:ext cx=\"%d\" cy=\"%d\"/>\n"
    "\t\t\t\t\t</a:xfrm>\n"
    "\t\t\t\t\t<a:custGeom>\n"
    "\t\t\t\t\t\t<a:pathLst>\n"
    "\t\t\t\t\t\t\t<a:path extrusionOk=\"0\" w=\"%d\" h=\"%d\">\n"
  };

  fprintf(presentation->slideFile, linePrefix, presentation->objectNum, presentation->objectNum, xmin*presentation->slideXRes, ymin*presentation->slideYRes,
          w*presentation->slideXRes, h*presentation->slideYRes, w*presentation->slideXRes, h*presentation->slideYRes);
}

void pptxMoveTo(pptxPresentation *presentation, int x, int y)
{
  const char *line =
  {
    "\t\t\t\t\t\t\t\t<a:moveTo>\n"
    "\t\t\t\t\t\t\t\t\t<a:pt x=\"%d\" y=\"%d\"/>\n"
    "\t\t\t\t\t\t\t\t</a:moveTo>\n"
  };

  fprintf(presentation->slideFile, line, x*presentation->slideXRes, y*presentation->slideYRes);
}

void pptxLineTo(pptxPresentation *presentation, int x, int y)
{
  const char *line =
  {
    "\t\t\t\t\t\t\t\t<a:lnTo>\n"
    "\t\t\t\t\t\t\t\t\t<a:pt x=\"%d\" y=\"%d\"/>\n"
    "\t\t\t\t\t\t\t\t</a:lnTo>\n"
  };

  fprintf(presentation->slideFile, line, x*presentation->slideXRes, y*presentation->slideYRes);
}

void pptxArcTo(pptxPresentation *presentation, int h, int w, double stAng, double swAng)
{
  const char *arc =
  {
    "\t\t\t\t\t\t\t\t<a:arcTo hR=\"%d\" wR=\"%d\" stAng=\"%d\" swAng=\"%d\"/>\n"
  };

  fprintf(presentation->slideFile, arc, h*presentation->slideYRes, w*presentation->slideXRes, to_angle(stAng), to_angle(swAng));
}

void pptxBezierLineTo(pptxPresentation *presentation, int c1x, int c1y, int c2x, int c2y, int c3x, int c3y)
{
  const char *line =
  {
    "\t\t\t\t\t\t\t\t<a:cubicBezTo>\n"
    "\t\t\t\t\t\t\t\t\t<a:pt x=\"%d\" y=\"%d\"/>\n"
    "\t\t\t\t\t\t\t\t\t<a:pt x=\"%d\" y=\"%d\"/>\n"
    "\t\t\t\t\t\t\t\t\t<a:pt x=\"%d\" y=\"%d\"/>\n"
    "\t\t\t\t\t\t\t\t</a:cubicBezTo>\n"
  };

  fprintf(presentation->slideFile, line, c1x*presentation->slideXRes, c1y*presentation->slideYRes, c2x*presentation->slideXRes, c2y*presentation->slideYRes,
          c3x*presentation->slideXRes, c3y*presentation->slideYRes);
}

void pptxLineClose(pptxPresentation *presentation)
{
  const char *lineSuffix =
  {
    "\t\t\t\t\t\t\t</a:path>\n"
    "\t\t\t\t\t\t</a:pathLst>\n"
    "\t\t\t\t\t</a:custGeom>\n"
  };

  fprintf(presentation->slideFile, lineSuffix);
}

void pptxNoFill(pptxPresentation *presentation)
{
  const char *noFill =
  {
    "\t\t\t\t\t<a:noFill/>\n"
  };

  fprintf(presentation->slideFile, noFill);
}

void pptxSolidFill(pptxPresentation *presentation, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
  int alphaPct = (int)(((double)alpha / 255.) * 100 * 1000);

  const char *fill =
  {
    "\t\t\t\t\t<a:solidFill>\n"
    "\t\t\t\t\t\t<a:srgbClr val=\"%02X%02X%02X\">\n"
    "\t\t\t\t\t\t\t<a:alpha val=\"%d\"/>\n"
    "\t\t\t\t\t\t</a:srgbClr>\n"
    "\t\t\t\t\t</a:solidFill>\n"
  };

  fprintf(presentation->slideFile, fill, red, green, blue, alphaPct);
}

void pptxHatchLine(pptxPresentation *presentation, const char* style, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha,
                   unsigned char bRed, unsigned char bGreen, unsigned char bBlue, unsigned char bAlpha)
{
  int alphaPct = (int)(((double)alpha / 255.) * 100 * 1000);
  int bAlphaPct = (int)(((double)bAlpha / 255.) * 100 * 1000);

  const char *patt =
  {
    "\t\t\t\t\t<a:pattFill prst=\"%s\">\n"
    "\t\t\t\t\t\t<a:fgClr>\n"
    "\t\t\t\t\t\t\t<a:srgbClr val=\"%02X%02X%02X\">\n"
    "\t\t\t\t\t\t\t\t<a:alpha val=\"%d\"/>\n"
    "\t\t\t\t\t\t</a:srgbClr>\n"
    "\t\t\t\t\t\t</a:fgClr>\n"
    "\t\t\t\t\t\t\t<a:bgClr>\n"
    "\t\t\t\t\t\t\t\t<a:srgbClr val=\"%02X%02X%02X\">\n"
    "\t\t\t\t\t\t\t\t\t<a:alpha val=\"0\"/>\n"
    "\t\t\t\t\t\t\t\t</a:srgbClr>\n"
    "\t\t\t\t\t\t\t</a:bgClr>\n"
    "</a:pattFill>\n"
  };

  fprintf(presentation->slideFile, patt, style, red, green, blue, alphaPct, bRed, bGreen, bBlue, bAlphaPct);
}

void pptxPattern(pptxPresentation *presentation, const unsigned char *red, const unsigned char* green, const unsigned char *blue, int width, int height)
{
  const char *img =
  {
    "\t\t\t\t\t<a:blipFill dpi=\"0\" rotWithShape=\"1\">\n"
    "\t\t\t\t\t\t<a:blip r:embed=\"rId%d\"/>\n"
    "\t\t\t\t\t\t<a:srcRect/>\n"
    "\t\t\t\t\t\t<a:tile tx=\"0\" ty=\"0\" sx=\"100000\" sy=\"100000\" flip=\"none\" algn=\"tl\"/>\n"
    "\t\t\t\t\t</a:blipFill>\n"
  };
  char filename[10240];

  sprintf(filename, "%s/"PPTX_IMAGE_FILE, presentation->baseDir, presentation->mediaNum);

  createImageFileRGBA(filename, width, height, red, green, blue, NULL);

  fprintf(presentation->slideFile, img, presentation->imageId);

  printSlideRels(presentation);

  presentation->mediaNum++;
  presentation->imageId++;
}

static void createStippleImage(pptxPresentation *presentation, unsigned char fRed, unsigned char fGreen, unsigned char fBlue, unsigned char bRed, unsigned char bGreen, unsigned char bBlue,
                            const unsigned char *stipple, int width, int height, int backOpacity)
{
  int lin, col;
  char filename[10240];
  int plane_size = width*height;
  unsigned char* red = (unsigned char*)malloc(plane_size * 4);
  unsigned char* green = red + plane_size;
  unsigned char* blue = green + plane_size;
  unsigned char* alpha = blue + plane_size;

  for (lin = 0; lin < width; lin++)
  {
    for (col = 0; col < height; col++)
    {
      int ind = (height - lin - 1)*width + col;
      if (stipple[width*lin + col] == 0)
      {
        red[ind] = bRed;
        green[ind] = bGreen;
        blue[ind] = bBlue;
        if (backOpacity == 1)
          alpha[ind] = 255;
        else
          alpha[ind] = 0;
      }
      else
      {
        red[ind] = fRed;
        green[ind] = fGreen;
        blue[ind] = fBlue;
        alpha[ind] = 255;
      }
    }
  }

  sprintf(filename, "%s/"PPTX_IMAGE_FILE, presentation->baseDir, presentation->mediaNum);

  createImageFileRGBA(filename, width, height, red, green, blue, alpha);

  free(red);
}

void pptxStipple(pptxPresentation *presentation, unsigned char fRed, unsigned char fGreen, unsigned char fBlue, unsigned char bRed, unsigned char bGreen, unsigned char bBlue,
                 const unsigned char *stipple, int width, int height, int backOpacity)
{
  const char *img =
  {
    "\t\t\t\t\t<a:blipFill dpi=\"0\" rotWithShape=\"1\">\n"
    "\t\t\t\t\t\t<a:blip r:embed=\"rId%d\"/>\n"
    "\t\t\t\t\t\t<a:srcRect/>\n"
    "\t\t\t\t\t\t<a:tile tx=\"0\" ty=\"0\" sx=\"100000\" sy=\"100000\" flip=\"none\" algn=\"tl\"/>\n"
    "\t\t\t\t\t</a:blipFill>\n"
  };

  createStippleImage(presentation, fRed, fGreen, fBlue, bRed, bGreen, bBlue, stipple, width, height, backOpacity);

  fprintf(presentation->slideFile, img, presentation->imageId);

  printSlideRels(presentation);

  presentation->mediaNum++;
  presentation->imageId++;
}

void pptxEndLine(pptxPresentation *presentation, int width, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, const char* lineStyle, int nDashes, int *dashes)
{
  int i;
  int alphaPct = (int)(((double)alpha / 255.) * 100 * 1000);

  const char *lineEndPrefix =
  {
    "\t\t\t\t\t<a:ln cap=\"flat\" cmpd=\"sng\" w=\"%d\">\n"
    "\t\t\t\t\t\t<a:solidFill>\n"
    "\t\t\t\t\t\t\t<a:srgbClr val=\"%02X%02X%02X\">\n"
    "\t\t\t\t\t\t\t\t<a:alpha val=\"%d\"/>\n"
    "\t\t\t\t\t\t\t</a:srgbClr>\n"
    "\t\t\t\t\t\t</a:solidFill>\n"
  };

  const char *style =
  {
    "\t\t\t\t\t\t<a:prstDash val = \"%s\"/>\n"
  };

  const char *lineEndSuffix =
  {
    "\t\t\t\t\t\t<a:round/>\n"
    "\t\t\t\t\t\t<a:headEnd len=\"lg\" w=\"lg\" type=\"none\"/>\n"
    "\t\t\t\t\t\t<a:tailEnd len=\"lg\" w=\"lg\" type=\"none\"/>\n"
    "\t\t\t\t\t</a:ln>\n"
    "\t\t\t\t</p:spPr>\n"
    "\t\t\t</p:sp>\n"
  };

  fprintf(presentation->slideFile, lineEndPrefix, width*presentation->pixelSize, red, green, blue, alphaPct, lineStyle);

  if (strcmp(lineStyle, "custom") != 0)
    fprintf(presentation->slideFile, style, lineStyle);
  else
  {
    fprintf(presentation->slideFile, "\t\t\t\t\t<a:custDash>\n");
    for (i = 0; i < nDashes; i += 2)
      fprintf(presentation->slideFile, "\t\t\t\t\t\t<a:ds d=\"%d%%\" sp=\"%d%%\"/>\n", dashes[i], dashes[i + 1]);
    fprintf(presentation->slideFile, "\t\t\t\t\t</a:custDash>\n");
  }

  fprintf(presentation->slideFile, lineEndSuffix, lineStyle);

  presentation->objectNum++;
}

void pptxBeginSector(pptxPresentation *presentation, const char *geomType, int xmin, int ymin, int w, int h, double angle1, double angle2)
{
  const char *arc =
  {
    "\t\t\t<p:sp>\n"
    "\t\t\t\t<p:nvSpPr>\n"
    "\t\t\t\t\t<p:cNvPr id=\"%d\" name=\"Shape %d\"/>\n"
    "\t\t\t\t\t<p:cNvSpPr/>\n"
    "\t\t\t\t\t<p:nvPr/>\n"
    "\t\t\t\t</p:nvSpPr>\n"
    "\t\t\t\t<p:spPr>\n"
    "\t\t\t\t\t<a:xfrm>\n"
    "\t\t\t\t\t\t<a:off x=\"%d\" y=\"%d\"/>\n"
    "\t\t\t\t\t\t<a:ext cx=\"%d\" cy=\"%d\"/>\n"
    "\t\t\t\t\t</a:xfrm>\n"
    "\t\t\t\t\t\t<a:prstGeom prst=\"%s\">\n"
    "\t\t\t\t\t\t\t<a:avLst>\n"
    "\t\t\t\t\t\t\t\t<a:gd fmla=\"val %d\" name=\"adj1\"/>\n"
    "\t\t\t\t\t\t\t\t<a:gd fmla=\"val %d\" name=\"adj2\"/>\n"
    "\t\t\t\t\t\t</a:avLst>\n"
    "\t\t\t\t\t\t</a:prstGeom>\n"
  };

  fprintf(presentation->slideFile, arc, presentation->objectNum, presentation->objectNum, xmin*presentation->slideXRes, ymin*presentation->slideYRes, w*presentation->slideXRes, h*presentation->slideYRes,
          geomType, to_angle(angle1), to_angle(angle2));
}

void pptxEndSector(pptxPresentation *presentation)
{
  const char *lineSuffix =
  {
    "\t\t\t\t\t\t\t</a:path>\n"
    "\t\t\t\t\t\t</a:pathLst>\n"
    "\t\t\t\t\t</a:custGeom>\n"
  };

  fprintf(presentation->slideFile, lineSuffix);

  presentation->objectNum++;
}

void pptxText(pptxPresentation *presentation, int xmin, int ymin, int w, int h, double rotAngle, int bold, int italic, int underline, int strikeout, int size, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha,
              const char* typeface, const char* text)
{
  int alphaPct = (int)(((double)alpha / 255.) * 100 * 1000);

  const char *textInit =
  {
    "\t\t\t<p:sp>\n"
    "\t\t\t\t<p:nvSpPr>\n"
    "\t\t\t\t\t<p:cNvPr id=\"%d\" name=\"Shape %d\"/>\n"
    "\t\t\t\t\t<p:cNvSpPr txBox=\"0\"/>\n"
    "\t\t\t\t\t<p:nvPr/>\n"
    "\t\t\t\t</p:nvSpPr>\n"
    "\t\t\t\t<p:spPr>\n"
    "\t\t\t\t\t<a:xfrm rot=\"%d\">\n"
    "\t\t\t\t\t\t<a:off x=\"%d\" y=\"%d\"/>\n"
    "\t\t\t\t\t\t<a:ext cx=\"%d\" cy=\"%d\"/>\n"
    "\t\t\t\t\t</a:xfrm>\n"
    "\t\t\t\t\t<a:prstGeom prst=\"rect\">\n"
    "\t\t\t\t\t\t<a:avLst/>\n"
    "\t\t\t\t\t</a:prstGeom>\n"
    "\t\t\t\t\t<a:noFill/>\n"
    "\t\t\t\t\t<a:ln>\n"
    "\t\t\t\t\t\t<a:noFill/>\n"
    "\t\t\t\t\t</a:ln>\n"
    "\t\t\t\t</p:spPr>\n"
    "\t\t\t\t<p:txBody>\n"
    "\t\t\t\t\t<a:bodyPr wrap=\"none\" anchorCtr=\"1\" anchor=\"ctr\" bIns=\"0\" lIns=\"0\" rIns=\"0\" tIns=\"0\">\n"
    "\t\t\t\t\t\t<a:noAutofit/>\n"
    "\t\t\t\t\t</a:bodyPr>\n"
    "\t\t\t\t\t<a:lstStyle/>\n"
    "\t\t\t\t\t<a:p>\n"
    "\t\t\t\t\t\t<a:pPr lvl=\"0\">\n"
    "\t\t\t\t\t\t\t<a:spcBef>\n"
    "\t\t\t\t\t\t\t\t<a:spcPts val=\"0\"/>\n"
    "\t\t\t\t\t\t\t</a:spcBef>\n"
    "\t\t\t\t\t\t\t<a:buNone/>\n"
    "\t\t\t\t\t\t</a:pPr>\n"
    "\t\t\t\t\t\t<a:r>\n"
    "\t\t\t\t\t\t\t<a:rPr b=\"%d\" i=\"%d\" strike=\"%s\" lang=\"en\" u=\"%s\" sz=\"%d\">\n"
    "\t\t\t\t\t\t\t\t<a:solidFill>\n"
    "\t\t\t\t\t\t\t\t\t<a:srgbClr val=\"%02X%02X%02X\">\n"
    "\t\t\t\t\t\t\t\t\t\t<a:alpha val=\"%d\"/>\n"
    "\t\t\t\t\t\t\t\t\t</a:srgbClr>\n"
    "\t\t\t\t\t\t\t\t</a:solidFill>\n"
    "\t\t\t\t\t\t\t\t<a:latin typeface=\"%s\"/>\n"
    "\t\t\t\t\t\t\t\t<a:ea typeface=\"%s\"/>\n"
    "\t\t\t\t\t\t\t\t<a:cs typeface=\"%s\"/>\n"
    "\t\t\t\t\t\t\t\t<a:sym typeface=\"%s\"/>\n"
    "\t\t\t\t\t\t\t</a:rPr>\n"
    "\t\t\t\t\t\t\t<a:t>%s</a:t>\n"
    "\t\t\t\t\t\t</a:r>\n"
    "\t\t\t\t\t</a:p>\n"
    "\t\t\t\t</p:txBody>\n"
    "\t\t\t</p:sp>\n"
  };

  fprintf(presentation->slideFile, textInit, presentation->objectNum, presentation->objectNum, to_angle(rotAngle), xmin*presentation->slideXRes, ymin*presentation->slideYRes, w*presentation->slideXRes,
          h*presentation->slideYRes, bold, italic, strikeout ? "sngStrike" : "noStrike", underline ? "sng" : "none", (size * 2 / 3) * 100,
          red, green, blue, alphaPct, typeface, typeface, typeface, typeface, text);

  presentation->objectNum++;
}

void pptxPixel(pptxPresentation *presentation, int x, int y, int width, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
  int alphaPct = (int)(((double)alpha / 255.) * 100 * 1000);
  int w = width*presentation->slideXRes;

  const char *pixel =
  {
    "\t\t\t<p:sp>\n"
    "\t\t\t\t<p:nvSpPr>\n"
    "\t\t\t\t\t<p:cNvPr id=\"%d\" name=\"Shape %d\"/>\n"
    "\t\t\t\t<p:cNvSpPr/>\n"
    "\t\t\t\t<p:nvPr/>\n"
    "\t\t\t</p:nvSpPr>\n"
    "\t\t\t<p:spPr>\n"
    "\t\t\t\t<a:xfrm>\n"
    "\t\t\t\t\t<a:off x=\"%d\" y=\"%d\"/>\n"
    "\t\t\t\t\t<a:ext cx=\"%d\" cy=\"%d\"/>\n"
    "\t\t\t\t</a:xfrm>\n"
    "\t\t\t\t<a:custGeom>\n"
    "\t\t\t\t\t<a:pathLst>\n"
    "\t\t\t\t\t\t<a:path extrusionOk=\"0\" w=\"%d\" h=\"%d\">\n"
    "\t\t\t\t\t\t\t<a:moveTo>\n"
    "\t\t\t\t\t\t\t\t<a:pt x=\"0\" y=\"0\"/>\n"
    "\t\t\t\t\t\t\t</a:moveTo>\n"
    "\t\t\t\t\t\t\t<a:lnTo>\n"
    "\t\t\t\t\t\t\t\t<a:pt x=\"%d\" y=\"0\"/>\n"
    "\t\t\t\t\t\t\t</a:lnTo>\n"
    "\t\t\t\t\t\t\t<a:lnTo>\n"
    "\t\t\t\t\t\t\t\t<a:pt x=\"%d\" y=\"%d\"/>\n"
    "\t\t\t\t\t\t\t</a:lnTo>\n"
    "\t\t\t\t\t\t\t<a:lnTo>\n"
    "\t\t\t\t\t\t\t\t<a:pt x=\"0\" y=\"%d\"/>\n"
    "\t\t\t\t\t\t\t</a:lnTo>\n"
    "\t\t\t\t\t\t\t<a:lnTo>\n"
    "\t\t\t\t\t\t\t\t<a:pt x=\"0\" y=\"0\"/>\n"
    "\t\t\t\t\t\t\t</a:lnTo>\n"
    "\t\t\t\t\t\t</a:path>\n"
    "\t\t\t\t\t</a:pathLst>\n"
    "\t\t\t\t</a:custGeom>\n"
    "\t\t\t\t<a:noFill/>\n"
    "\t\t\t\t\t<a:ln cap=\"flat\" cmpd=\"sng\" w=\"%d\">\n"
    "\t\t\t\t\t\t<a:solidFill>\n"
    "\t\t\t\t\t\t\t<a:srgbClr val=\"%02X%02X%02X\">\n"
    "\t\t\t\t\t\t\t\t<a:alpha val=\"%d\"/>\n"
    "\t\t\t\t\t\t\t</a:srgbClr>\n"
    "\t\t\t\t\t\t</a:solidFill>\n"
    "\t\t\t\t\t\t<a:prstDash val=\"solid\"/>\n"
    "\t\t\t\t\t\t<a:round/>\n"
    "\t\t\t\t\t\t<a:headEnd len=\"lg\" w=\"lg\" type=\"none\"/>\n"
    "\t\t\t\t\t\t<a:tailEnd len=\"lg\" w=\"lg\" type=\"none\"/>\n"
    "\t\t\t\t\t</a:ln>\n"
    "\t\t\t\t</p:spPr>\n"
    "\t\t\t</p:sp>\n"
  };

  fprintf(presentation->slideFile, pixel, presentation->objectNum, presentation->objectNum, x*presentation->slideXRes, y*presentation->slideYRes, presentation->pixelSize / 4,
          w, w, w, w, w, w, w, w, red, green, blue, alphaPct);

  presentation->objectNum++;
}

void pptxImageMap(pptxPresentation *presentation, int iw, int ih, const unsigned char *index, const long int *colors, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  const char *image =
  {
    "\t\t\t<p:pic>\n"
    "\t\t\t\t<p:nvPicPr>\n"
    "\t\t\t\t\t<p:cNvPr id=\"%d\" name=\"Shape %d\"/>\n"
    "\t\t\t\t<p:cNvPicPr preferRelativeResize=\"0\"/>\n"
    "\t\t\t\t<p:nvPr/>\n"
    "\t\t\t\t</p:nvPicPr>\n"
    "\t\t\t\t<p:blipFill>\n"
    "\t\t\t\t\t<a:blip r:embed=\"rId%d\">\n"
    "\t\t\t\t\t\t<a:alphaModFix/>\n"
    "\t\t\t\t\t</a:blip>\n"
    "\t\t\t\t\t<a:stretch>\n"
    "\t\t\t\t\t\t<a:fillRect/>\n"
    "\t\t\t\t\t</a:stretch>\n"
    "\t\t\t\t</p:blipFill>\n"
    "\t\t\t\t<p:spPr>\n"
    "\t\t\t\t\t<a:xfrm>\n"
    "\t\t\t\t\t\t<a:off x=\"%d\" y=\"%d\"/>\n"
    "\t\t\t\t\t\t<a:ext cx=\"%d\" cy=\"%d\"/>\n"
    "\t\t\t\t\t</a:xfrm>\n"
    "\t\t\t\t\t<a:prstGeom prst=\"rect\">\n"
    "\t\t\t\t\t\t<a:avLst/>\n"
    "\t\t\t\t\t</a:prstGeom>\n"
    "\t\t\t\t\t<a:noFill/>\n"
    "\t\t\t\t\t<a:ln>\n"
    "\t\t\t\t\t\t<a:noFill/>\n"
    "\t\t\t\t\t</a:ln>\n"
    "\t\t\t\t</p:spPr>\n"
    "\t\t\t</p:pic>\n"
  };
  char filename[10240];

  sprintf(filename, "%s/"PPTX_IMAGE_FILE, presentation->baseDir, presentation->mediaNum);

  createImageFileMap(filename, iw, ih, index, colors);

  fprintf(presentation->slideFile, image, presentation->objectNum, presentation->objectNum, presentation->imageId, x*presentation->slideXRes, y*presentation->slideYRes,
          w*presentation->slideXRes, h*presentation->slideYRes);

  printSlideRels(presentation);

  presentation->objectNum++;
  presentation->mediaNum++;
  presentation->imageId++;
}

void pptxImageRGBA(pptxPresentation *presentation, int iw, int ih, const unsigned char *red, const unsigned char* green, const unsigned char *blue, const unsigned char *alpha, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  const char *image =
  {
    "\t\t\t<p:pic>\n"
    "\t\t\t\t<p:nvPicPr>\n"
    "\t\t\t\t\t<p:cNvPr id=\"%d\" name=\"Shape %d\"/>\n"
    "\t\t\t\t<p:cNvPicPr preferRelativeResize=\"0\"/>\n"
    "\t\t\t\t<p:nvPr/>\n"
    "\t\t\t\t</p:nvPicPr>\n"
    "\t\t\t\t<p:blipFill>\n"
    "\t\t\t\t\t<a:blip r:embed=\"rId%d\">\n"
    "\t\t\t\t\t\t<a:alphaModFix/>\n"
    "\t\t\t\t\t</a:blip>\n"
    "\t\t\t\t\t<a:stretch>\n"
    "\t\t\t\t\t\t<a:fillRect/>\n"
    "\t\t\t\t\t</a:stretch>\n"
    "\t\t\t\t</p:blipFill>\n"
    "\t\t\t\t<p:spPr>\n"
    "\t\t\t\t\t<a:xfrm>\n"
    "\t\t\t\t\t\t<a:off x=\"%d\" y=\"%d\"/>\n"
    "\t\t\t\t\t\t<a:ext cx=\"%d\" cy=\"%d\"/>\n"
    "\t\t\t\t\t</a:xfrm>\n"
    "\t\t\t\t\t<a:prstGeom prst=\"rect\">\n"
    "\t\t\t\t\t\t<a:avLst/>\n"
    "\t\t\t\t\t</a:prstGeom>\n"
    "\t\t\t\t\t<a:noFill/>\n"
    "\t\t\t\t\t<a:ln>\n"
    "\t\t\t\t\t\t<a:noFill/>\n"
    "\t\t\t\t\t</a:ln>\n"
    "\t\t\t\t</p:spPr>\n"
    "\t\t\t</p:pic>\n"
  };
  char filename[10240];

  sprintf(filename, "%s/"PPTX_IMAGE_FILE, presentation->baseDir, presentation->mediaNum);

  createImageFileRGBA(filename, iw, ih, red, green, blue, alpha);

  fprintf(presentation->slideFile, image, presentation->objectNum, presentation->objectNum, presentation->imageId, x*presentation->slideXRes, y*presentation->slideYRes,
          w*presentation->slideXRes, h*presentation->slideYRes);

  printSlideRels(presentation);

  presentation->objectNum++;
  presentation->mediaNum++;
  presentation->imageId++;
}


/*************************************  CREATE/KILL  *********************************************************/


static void createSubDirectory(const char* dirname, const char* subdir)
{
  char dir[10240];
  sprintf(dir, "%s/%s", dirname, subdir);
  cdMakeDirectory(dir);
}

static void removeSubDirectory(const char* dirname, const char* subdir)
{
  char dir[10240];
  sprintf(dir, "%s/%s", dirname, subdir);
  cdRemoveDirectory(dir);
}

static void removeFile(const char* dirname, const char* file)
{
  char filename[10240];
  sprintf(filename, "%s/%s", dirname, file);
  remove(filename);
}

pptxPresentation *pptxCreatePresentation(int width, int height)
{
  pptxPresentation *presentation = (pptxPresentation*)malloc(sizeof(pptxPresentation));
  double slideRatio, canvasRatio;


  // TODO: what's this????? - begin
  presentation->slideHeight = 6858000;
  presentation->slideWidth = 12192000;
  presentation->pixelSize = 9525;

  slideRatio = (double)presentation->slideWidth / presentation->slideHeight;
  canvasRatio = (double)width / height;
  if (canvasRatio > slideRatio)
    presentation->slideHeight = (int)(presentation->slideWidth / canvasRatio);
  else
    presentation->slideWidth = (int)(presentation->slideHeight * canvasRatio);
  // TODO: what's this????? - end


  presentation->slideXRes = presentation->slideWidth / width;
  presentation->slideYRes = presentation->slideHeight / height;

  cdStrTmpFileName(presentation->baseDir);
#ifdef WIN32
  remove(presentation->baseDir);
#endif
  if (!cdMakeDirectory(presentation->baseDir))
  {
    free(presentation);
    return NULL;
  }

  /* must be created in this order */
  createSubDirectory(presentation->baseDir, PPTX_PPT_DIR);
  createSubDirectory(presentation->baseDir, PPTX_PPT_RELS_DIR);
  createSubDirectory(presentation->baseDir, PPTX_LAYOUTS_DIR);
  createSubDirectory(presentation->baseDir, PPTX_LAYOUTS_RELS_DIR);
  createSubDirectory(presentation->baseDir, PPTX_MASTERS_DIR);
  createSubDirectory(presentation->baseDir, PPTX_MASTERS_RELS_DIR);
  createSubDirectory(presentation->baseDir, PPTX_SLIDES_DIR);
  createSubDirectory(presentation->baseDir, PPTX_SLIDES_RELS_DIR);
  createSubDirectory(presentation->baseDir, PPTX_THEME_DIR);
  createSubDirectory(presentation->baseDir, PPTX_MEDIA_DIR);
  createSubDirectory(presentation->baseDir, PPTX_RELS_DIR);

  presentation->slideNum = 1;
  presentation->objectNum = 51;
  presentation->mediaNum = 0;
  presentation->imageId = 4;

  if (!pptxOpenSlide(presentation))
  {
    free(presentation);
    return NULL;
  }

  pptxWritePresProps(presentation);

  pptxWriteRels(presentation);

  pptxWriteLayoutRels(presentation);

  pptxWriteLayout(presentation);

  pptxWriteMasterRels(presentation);

  pptxWriteMaster(presentation);

  pptxWriteTheme(presentation);

  return presentation;
}

static void closePresentation(pptxPresentation *presentation)
{
  pptxWritePresentation(presentation);

  pptxWriteContentTypes(presentation);

  pptxWritePptRels(presentation);

  pptxCloseSlide(presentation);
}

static void removeTempFiles(const char* dirname, const char **files, int nFiles)
{
  int i;

  for (i = 0; i < nFiles; i++)
    removeFile(dirname, files[i]);

  /* must be removed in this order (reverse from creation) */
  removeSubDirectory(dirname, PPTX_RELS_DIR);
  removeSubDirectory(dirname, PPTX_MEDIA_DIR);
  removeSubDirectory(dirname, PPTX_THEME_DIR);
  removeSubDirectory(dirname, PPTX_SLIDES_RELS_DIR);
  removeSubDirectory(dirname, PPTX_SLIDES_DIR);
  removeSubDirectory(dirname, PPTX_MASTERS_RELS_DIR);
  removeSubDirectory(dirname, PPTX_MASTERS_DIR);
  removeSubDirectory(dirname, PPTX_LAYOUTS_RELS_DIR);
  removeSubDirectory(dirname, PPTX_LAYOUTS_DIR);
  removeSubDirectory(dirname, PPTX_PPT_RELS_DIR);
  removeSubDirectory(dirname, PPTX_PPT_DIR);

  cdRemoveDirectory(dirname);
}

static int writeZipFile(pptxPresentation *presentation, const char* dirname, const char* filename)
{
  char **files;
  int i, j, k, ret,
    count = presentation->mediaNum + 2 * presentation->slideNum + 10;

  files = malloc(count * sizeof(char*));
  for (i = 0; i < count; ++i)
    files[i] = malloc(80);

  i = 0;
  strcpy(files[i], PPTX_CONTENT_TYPES_FILE); i++;
  strcpy(files[i], PPTX_PRESENTATION_FILE); i++;
  strcpy(files[i], PPTX_PRES_PROPS_FILE); i++;
  for (j = 0; j < presentation->mediaNum; j++)
    sprintf(files[i], PPTX_IMAGE_FILE, j); i++;
  strcpy(files[i], PPTX_SLIDE_LAYOUT_FILE); i++;
  strcpy(files[i], PPTX_SLIDE_LAYOUT_RELS_FILE); i++;
  strcpy(files[i], PPTX_SLIDE_MASTER_FILE); i++;
  strcpy(files[i], PPTX_SLIDE_MASTER_RELS); i++;
  for (k = 1; k < presentation->slideNum; k++)
  {
    sprintf(files[i], PPTX_SLIDE_FILE, k); i++;
    sprintf(files[i], PPTX_SLIDE_RELS_FILE, k); i++;
  }
  strcpy(files[i], PPTX_THEME_FILE); i++;
  strcpy(files[i], PPTX_PRESENTATION_RELS); i++;
  strcpy(files[i], PPTX_RELS_FILE); i++;

  ret = minizip(filename, dirname, files, count);

  removeTempFiles(dirname, files, count);

  for (i = 0; i < count; ++i)
    free(files[i]);
  free(files);

  return ret;
}

int pptxKillPresentation(pptxPresentation *presentation, const char* filename)
{
  int ret;

  closePresentation(presentation);

  ret = writeZipFile(presentation, presentation->baseDir, filename);

  free(presentation);

  return ret;
}
