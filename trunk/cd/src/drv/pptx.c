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

int minizip(const char *filename, const char *temDir, const char **files, const int nFiles);


static int createRGBImageFile(char *filename, int width, int height, const unsigned char *red, const unsigned char *green, const unsigned char *blue, const unsigned char *alpha)
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

static int pptxCreateImageFile(pptxPresentation*presentation, int width, int height, const unsigned char *index, const long int *colors)
{
  char filename[10240];

  sprintf(filename, "%s/image%d.png", presentation->mediaDir, presentation->mediaNum);

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
  for (int i = 0; i < nSlides; i++)
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


void pptxOpenSlide(pptxPresentation *presentation)
{
  char slideFileName[10240];
  char slideRelsFileName[10240];

  sprintf(slideFileName, "%s/slide%d.xml", presentation->slidesDir, presentation->slideNum);

  presentation->slideFile = fopen(slideFileName, "w");
  if (!presentation->slideFile)
    return;

  sprintf(slideRelsFileName, "%s/slide%d.xml.rels", presentation->slidesRelsDir, presentation->slideNum);

  presentation->slideRelsFile = fopen(slideRelsFileName, "w");
  if (!presentation->slideRelsFile)
    return;

  printOpenSlide(presentation->slideFile, presentation->objectNum);

  printOpenSlideRels(presentation->slideRelsFile);

  presentation->objectNum++;
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

void pptxWritePresProps(pptxPresentation *presentation)
{
  char filename[10240];
  FILE* presPropsFile;

  sprintf(filename, "%s/presProps.xml", presentation->pptDir);

  presPropsFile = fopen(filename, "w");
  if (!presPropsFile)
    return;

  printPresProps(presPropsFile);

  fclose(presPropsFile);
}

void pptxWriteRels(pptxPresentation *presentation)
{
  char filename[10240];
  FILE *relsFile;

  sprintf(filename, "%s/.rels", presentation->relsDir);

  relsFile = fopen(filename, "w");
  if (!relsFile)
    return;

  printRels(relsFile);

  fclose(relsFile);
}

void pptxWriteLayoutRels(pptxPresentation *presentation)
{
  char filename[10240];
  FILE *pptLayoutRels;

  sprintf(filename, "%s/slideLayout1.xml.rels", presentation->layoutsRelDir);

  pptLayoutRels = fopen(filename, "w");
  if (!pptLayoutRels)
    return;

  printLayoutRelsFile(pptLayoutRels);

  fclose(pptLayoutRels);
}

void pptxWriteLayout(pptxPresentation *presentation)
{
  char filename[10240];
  FILE *layoutRels;

  sprintf(filename, "%s/slideLayout1.xml", presentation->layoutsDir);

  layoutRels = fopen(filename, "w");
  if (!layoutRels)
    return;

  printLayoutFile(layoutRels);

  fclose(layoutRels);
}

void pptxWriteMasterRels(pptxPresentation *presentation)
{
  char filename[10240];
  FILE *masterRels;

  sprintf(filename, "%s/slideMaster1.xml.rels", presentation->masterRelsDir);

  masterRels = fopen(filename, "w");
  if (!masterRels)
    return;

  printMasterRelsFile(masterRels);

  fclose(masterRels);
}

void pptxWriteMaster(pptxPresentation *presentation)
{
  char filename[10240];
  FILE *master;

  sprintf(filename, "%s/slideMaster1.xml", presentation->mastersDir);

  master = fopen(filename, "w");
  if (!master)
    return;

  printMasterFile(master);

  fclose(master);
}

void pptxWriteTheme(pptxPresentation *presentation)
{
  char filename[10240];
  FILE *theme;

  sprintf(filename, "%s/theme1.xml", presentation->themeDir);

  theme = fopen(filename, "w");
  if (!theme)
    return;

  printThemeFile(theme);

  fclose(theme);
}

void pptxWritePresentation(pptxPresentation *presentation)
{
  char filename[10240];

  sprintf(filename, "%s/presentation.xml", presentation->pptDir);

  presentation->presentationFile = fopen(filename, "w");
  if (!presentation->presentationFile)
    return;

  printPresentation(presentation->presentationFile, presentation->slideNum, presentation->slideHeight, presentation->slideWidth);

  fclose(presentation->presentationFile);
}

void pptxWriteContentTypes(pptxPresentation *presentation)
{
  char filename[10240];
  FILE *ctFile;

  sprintf(filename, "%s/[Content_Types].xml", presentation->baseDir);

  ctFile = fopen(filename, "w");
  if (!ctFile)
    return;

  printContentTypes(ctFile, presentation->slideNum);

  fclose(ctFile);
}

void pptxWritePptRels(pptxPresentation *presentation)
{
  char filename[10240];
  FILE *pptRels;

  sprintf(filename, "%s/presentation.xml.rels", presentation->pptRelsDir);

  pptRels = fopen(filename, "w");
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

static void pptxCreatePatternImage(pptxPresentation *presentation, const unsigned char *red, const unsigned char* green, const unsigned char *blue, int width, int height)
{
  char filename[10240];
  sprintf(filename, "%s/image%d.png", presentation->mediaDir, presentation->mediaNum);
  createRGBImageFile(filename, width, height, red, green, blue, NULL);
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

  pptxCreatePatternImage(presentation, red, green, blue, width, height);

  fprintf(presentation->slideFile, img, presentation->imageId);

  printSlideRels(presentation);

  presentation->mediaNum++;
  presentation->imageId++;
}

static void pptxCreateStippleImage(pptxPresentation *presentation, unsigned char fRed, unsigned char fGreen, unsigned char fBlue, unsigned char bRed, unsigned char bGreen, unsigned char bBlue,
                            const unsigned char *stipple, int width, int height, int backOpacity)
{
  int lin, col;
  char filename[10240];
  unsigned char* red = (unsigned char*)malloc(width*height*sizeof(unsigned char));
  unsigned char* green = (unsigned char*)malloc(width*height*sizeof(unsigned char));
  unsigned char* blue = (unsigned char*)malloc(width*height*sizeof(unsigned char));
  unsigned char* alpha = (unsigned char*)malloc(width*height*sizeof(unsigned char));

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

  sprintf(filename, "%s/image%d.png", presentation->mediaDir, presentation->mediaNum);
  createRGBImageFile(filename, width, height, red, green, blue, alpha);
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

  pptxCreateStippleImage(presentation, fRed, fGreen, fBlue, bRed, bGreen, bBlue, stipple, width, height, backOpacity);

  fprintf(presentation->slideFile, img, presentation->imageId);

  printSlideRels(presentation);

  presentation->mediaNum++;
  presentation->imageId++;
}

void pptxEndLine(pptxPresentation *presentation, int width, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, const char* lineStyle, int nDashes, int *dashes)
{
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
    for (int i = 0; i < nDashes; i += 2)
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

  pptxCreateImageFile(presentation, iw, ih, index, colors);

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

  createRGBImageFile(presentation, iw, ih, red, green, blue, alpha);

  fprintf(presentation->slideFile, image, presentation->objectNum, presentation->objectNum, presentation->imageId, x*presentation->slideXRes, y*presentation->slideYRes,
          w*presentation->slideXRes, h*presentation->slideYRes);

  printSlideRels(presentation);

  presentation->objectNum++;
  presentation->mediaNum++;
  presentation->imageId++;
}


/*************************************  CREATE/KILL  *********************************************************/


static char* createRootDirectory(const char *rootName)
{
  cdMakeDirectory(rootName);

  return rootName;
}

static char* createSubDirectory(const char* rootName, const char* dirName)
{
  char* subdir = malloc(10240);

  subdir = strcpy(subdir, rootName);
  subdir = strcat(subdir, "/");
  subdir = strcat(subdir, dirName);

  cdMakeDirectory(subdir);

  return subdir;
}

pptxPresentation *pptxCreatePresentation(const char *dirname, int width, int height)
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

  presentation->slideXRes = presentation->slideWidth / width;
  presentation->slideYRes = presentation->slideHeight / height;
  // TODO: what's this????? - end


  presentation->baseDir = createRootDirectory(dirname);
  presentation->pptDir = createSubDirectory(dirname, PPTX_PPT_DIR);
  presentation->pptRelsDir = createSubDirectory(dirname, PPTX_PPT_RELS_DIR);
  presentation->layoutsDir = createSubDirectory(dirname, PPTX_LAYOUTS_DIR);
  presentation->layoutsRelDir = createSubDirectory(dirname, PPTX_LAYOUTS_RELS_DIR);
  presentation->mastersDir = createSubDirectory(dirname, PPTX_MASTERS_DIR);
  presentation->masterRelsDir = createSubDirectory(dirname, PPTX_MASTERS_RELS_DIR);
  presentation->slidesDir = createSubDirectory(dirname, PPTX_SLIDES_DIR);
  presentation->slidesRelsDir = createSubDirectory(dirname, PPTX_SLIDES_RELS_DIR);
  presentation->themeDir = createSubDirectory(dirname, PPTX_THEME_DIR);
  presentation->mediaDir = createSubDirectory(dirname, PPTX_MEDIA_DIR);
  presentation->relsDir = createSubDirectory(dirname, PPTX_RELS_DIR);

  presentation->slideNum = 1;
  presentation->objectNum = 51;
  presentation->mediaNum = 0;
  presentation->imageId = 4;

  pptxOpenSlide(presentation);

  pptxWritePresProps(presentation);

  pptxWriteRels(presentation);

  pptxWriteLayoutRels(presentation);

  pptxWriteLayout(presentation);

  pptxWriteMasterRels(presentation);

  pptxWriteMaster(presentation);

  pptxWriteTheme(presentation);

  return presentation;
}

static void pptxClosePresentation(pptxPresentation *presentation)
{
  pptxWritePresentation(presentation);

  pptxWriteContentTypes(presentation);

  pptxWritePptRels(presentation);

  pptxCloseSlide(presentation);
}

static int addFileToPPTX(char* filename, char *file)
{
  strcpy(file, filename);
  return 1;
}

static void removeTempFiles(const char* dirname, const char **files, int nFiles)
{
  char path[10240];
  char filename[10240];

  strcpy(path, dirname);
  strcat(path, "\\");

  for (int i = 0; i < nFiles; i++)
  {
    strcat(strcpy(filename, path), files[i]);
    remove(filename);
  }

  cdRemoveDirectory(strcat(path, PPTX_RELS_DIR));
  cdRemoveDirectory(strcat(path, PPTX_MEDIA_DIR));
  cdRemoveDirectory(strcat(path, PPTX_THEME_DIR));
  cdRemoveDirectory(strcat(path, PPTX_SLIDES_RELS_DIR));
  cdRemoveDirectory(strcat(path, PPTX_SLIDES_DIR));
  cdRemoveDirectory(strcat(path, PPTX_MASTERS_RELS_DIR));
  cdRemoveDirectory(strcat(path, PPTX_MASTERS_DIR));
  cdRemoveDirectory(strcat(path, PPTX_LAYOUTS_RELS_DIR));
  cdRemoveDirectory(strcat(path, PPTX_LAYOUTS_DIR));
  cdRemoveDirectory(strcat(path, PPTX_PPT_RELS_DIR));
  cdRemoveDirectory(strcat(path, PPTX_PPT_DIR));

  cdRemoveDirectory(dirname);
}

static int pptxWriteZipFile(pptxPresentation *presentation, const char* dirname, const char* filename)
{
  char **files;
  int i = 0;

  files = calloc(128, sizeof(char*));
  for (int i = 0; i < 128; ++i)
    files[i] = malloc(80);

  if (addFileToPPTX(PPTX_CONTENT_TYPES_FILE, files[i++]) == 0)
    return 0;
  if (addFileToPPTX(PPTX_PRESENTATION_FILE, files[i++]) == 0)
    return 0;
  if (addFileToPPTX(PPTX_PRES_PROPS_FILE, files[i++]) == 0)
    return 0;
  for (int j = 0; j < presentation->mediaNum; j++)
  {
    char s[80];
    sprintf(s, PPTX_IMAGE_FILE, j);
    if (addFileToPPTX(s, files[i++]) == 0)
      return 0;
  }
  if (addFileToPPTX(PPTX_SLIDE_LAYOUT_FILE, files[i++]) == 0)
    return 0;
  if (addFileToPPTX(PPTX_SLIDE_LAYOUT_RELS_FILE, files[i++]) == 0)
    return 0;
  if (addFileToPPTX(PPTX_SLIDE_MASTER_FILE, files[i++]) == 0)
    return 0;
  if (addFileToPPTX(PPTX_SLIDE_MASTER_RELS, files[i++]) == 0)
    return 0;
  for (int k = 1; k < presentation->slideNum; k++)
  {
    char s[80];
    sprintf(s, PPTX_SLIDE_FILE, k);
    if (addFileToPPTX(s, files[i++]) == 0)
      return 0;
    sprintf(s, PPTX_SLIDE_RELS_FILE, k);
    if (addFileToPPTX(s, files[i++]) == 0)
      return 0;
  }
  if (addFileToPPTX(PPTX_THEME_FILE, files[i++]) == 0)
    return 0;
  if (addFileToPPTX(PPTX_PRESENTATION_RELS, files[i++]) == 0)
    return 0;
  if (addFileToPPTX(PPTX_RELS_FILE, files[i++]) == 0)
    return 0;

  if (minizip(filename, dirname, files, i))
    return 0;

  removeTempFiles(dirname, files, i);

  free(files);

  return 1;
}

int pptxKillPresentation(pptxPresentation *presentation, const char* dirname, const char* filename)
{
  pptxClosePresentation(presentation);

  pptxWriteZipFile(presentation, dirname, filename);

  free(presentation);

  return 1;
}
