/* Arquivo gerado automaticamente por ledc 2.5         */

#include <stdlib.h>
#include <stdarg.h>
#include <iup.h>

static Ihandle* named[     13 ];

static Ihandle* decl( char* name, Ihandle* elem, char* first, ...)
{
  char *attr, *val;
  va_list arg;
  va_start (arg, first);
  attr = first;
  while (attr)
  {
    val = va_arg(arg,char*);
    IupSetAttribute( elem, attr, val );
    attr = va_arg(arg,char*);
  }
  va_end (arg);
  if(name) IupSetHandle( name, elem );
  return elem;
}


void simple_loadled (void)
{
  named[0] = decl( "mnOpen", IupMenu(
    IupItem( "CGM - Binary", "SimplePlayCGMBin" ),
    IupItem( "CGM - Text", "SimplePlayCGMText" ),
    IupItem( "METAFILE", "SimplePlayMetafile" ),
    IupItem( "WMF", "SimplePlayWMF" ),
    IupItem( "EMF", "SimplePlayEMF" ),
  NULL), NULL );
  named[1] = decl( "mnSaveAs", IupMenu(
    IupItem( "DEBUG", "SimpleDrawDebug" ),
    IupItem( "CGM - Binary", "SimpleDrawCGMBin" ),
    IupItem( "CGM - Text", "SimpleDrawCGMText" ),
    IupItem( "DGN", "SimpleDrawDGN" ),
    IupItem( "DXF", "SimpleDrawDXF" ),
    IupItem( "EMF", "SimpleDrawEMF" ),
    IupItem( "METAFILE", "SimpleDrawMetafile" ),
    IupItem( "PDF", "SimpleDrawPDF" ),
    IupItem( "PS", "SimpleDrawPS" ),
    IupItem( "EPS", "SimpleDrawEPS" ),
    IupItem( "WMF", "SimpleDrawWMF" ),
  NULL), NULL );
  named[2] = decl( "mnFile", IupMenu(
    IupSubmenu( "Open",
      named[0] /* mnOpen */
    ),
    IupSubmenu( "Save As",
      named[1] /* mnSaveAs */
    ),
    IupSeparator(),
    IupItem( "Print", "SimpleDrawPrint" ),
    IupItem( "Print Dialog...", "SimpleDrawPrintDialog" ),
    IupSeparator(),
    IupItem( "Exit", "cmdExit" ),
  NULL), NULL );
  named[3] = decl( "mnEdit", IupMenu(
    IupItem( "Copy as Metafile", "SimpleDrawClipboardMetafile" ),
    IupItem( "Copy as EMF", "SimpleDrawClipboardEMF" ),
    IupItem( "Copy as Bitmap", "SimpleDrawClipboardBitmap" ),
    IupItem( "Paste", "SimplePlayClipboard" ),
  NULL), NULL );
  named[4] = decl( "mnClipping", IupMenu(
    IupItem( "Off", "SimpleClippingOff" ),
    IupItem( "Area", "SimpleClippingArea" ),
    IupItem( "Polygon", "SimpleClippingPolygon" ),
    IupItem( "Region", "SimpleClippingRegion" ),
  NULL), NULL );
  named[5] = decl( "mnWriteMode", IupMenu(
    IupItem( "Replace", "SimpleReplace" ),
    IupItem( "Xor", "SimpleXor" ),
    IupItem( "Not Xor", "SimpleNotXor" ),
  NULL), NULL );
  named[6] = decl( "mnOptions", IupMenu(
    IupSubmenu( "Clipping",
      named[4] /* mnClipping */
    ),
    IupSubmenu( "Write Mode",
      named[5] /* mnWriteMode */
    ),
    IupItem( "Simulate", "SimpleDrawSimulate" ),
    IupItem( "Transform", "SimpleTransform" ),
    IupItem( "Context Plus", "SimpleContextPlus" ),
  NULL), NULL );
  named[7] = decl( "mnSurface", IupMenu(
    IupItem( "Window", "SimpleDrawWindow" ),
    IupItem( "Server Image", "SimpleDrawImage" ),
    IupItem( "Image RGB", "SimpleDrawImageRGB" ),
  NULL), NULL );
  named[8] = decl( "mnPrimitives", IupMenu(
    IupItem( "All", "SimpleDrawAll" ),
    IupItem( "Text Align", "SimpleDrawTextAlign" ),
    IupItem( "Text Fonts", "SimpleDrawTextFonts" ),
    IupItem( "Test", "SimpleDrawTest" ),
  NULL), NULL );
  named[9] = decl( "mnSimpleMenu", IupMenu(
    IupSubmenu( "File",
      named[2] /* mnFile */
    ),
    IupSubmenu( "Edit",
      named[3] /* mnEdit */
    ),
    IupSubmenu( "Options",
      named[6] /* mnOptions */
    ),
    IupSubmenu( "Surface",
      named[7] /* mnSurface */
    ),
    IupSubmenu( "Primitives",
      named[8] /* mnPrimitives */
    ),
  NULL), NULL );
  named[10] = decl( "SimpleCanvas", IupCanvas( "SimpleRepaint" ), 
    "BORDER", "0", NULL );
  named[11] = decl( "SimpleDialog", IupDialog(
    named[10] /* SimpleCanvas */
  ), 
    "TITLE", "Simple Draw", 
    "MENU", "mnSimpleMenu", NULL );
}
