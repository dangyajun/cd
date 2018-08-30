#include "cd_d2d.h"
#include "cd.h"

/* According to MSDN, GUID_WICPixelFormat32bppPBGRA is the recommended pixel
* format for cooperation with Direct2D. Note we define it here manually to
* avoid need to link with UUID.LIB. */
const GUID wic_cd_pixel_format =
{ 0x6fddc324, 0x4e03, 0x4bfe, { 0xb1, 0x85, 0x3d, 0x77, 0x76, 0x8d, 0xc9, 0x10 } };


static void bufferRGB2Bitmap(BYTE* Scan0, INT dstStride, UINT channels, UINT width, UINT height, const unsigned char *red, const unsigned char *green, const unsigned char *blue)
{
  UINT i, j;

  for (j = 0; j < height; j++)
  {
    UINT line_offset = (height - 1 - j) * width;
    const BYTE* r_line = red + line_offset;
    const BYTE* g_line = green + line_offset;
    const BYTE* b_line = blue + line_offset;
    BYTE* line_data = Scan0 + j * dstStride;

    for (i = 0; i < width; i++)
    {
      int offset = i * 3;
      int offset_data = i * channels;
      unsigned char r = r_line[offset];
      unsigned char g = g_line[offset];
      unsigned char b = b_line[offset];

      line_data[offset_data + 0] = b;  /* Blue */
      line_data[offset_data + 1] = g;  /* Green */
      line_data[offset_data + 2] = r;  /* Red */

      if (channels == 4)
        line_data[offset_data + 3] = 255;
    }
  }
}

static void bufferMap2Bitmap(BYTE* Scan0, INT dstStride, UINT channels, UINT width, UINT height, const BYTE* map, const long* palette)
{
  UINT i, j;

  for (j = 0; j < height; j++)
  {
    UINT line_offset = (height - 1 - j) * width;
    const BYTE* map_line = map + line_offset;
    BYTE* line_data = Scan0 + j * dstStride;

    for (i = 0; i < width; i++)
    {
      int map_index = map_line[i];
      long color = palette[map_index];

      int offset_data = i * channels;
      line_data[offset_data + 0] = cdBlue(color);
      line_data[offset_data + 1] = cdGreen(color);
      line_data[offset_data + 2] = cdRed(color);

      if (channels == 4)
        line_data[offset_data + 3] = 255;
    }
  }
}

static void bufferRGBA2Bitmap(BYTE* Scan0, INT dstStride, UINT width, UINT height, const unsigned char *red, const unsigned char *green, const unsigned char *blue, const unsigned char *alpha)
{
  UINT i, j;

  for (j = 0; j < height; j++)
  {
    UINT line_offset = (height - 1 - j) * width;
    const BYTE* r_line = red + line_offset;
    const BYTE* g_line = green + line_offset;
    const BYTE* b_line = blue + line_offset;
    const BYTE* a_line = alpha + line_offset;
    BYTE* line_data = Scan0 + j * dstStride;

    for (i = 0; i < width; i++)
    {
      int offset = i * 4;
      unsigned char r = r_line[offset + 0];
      unsigned char g = g_line[offset + 1];
      unsigned char b = b_line[offset + 2];
      unsigned char a = a_line[offset + 3];

      /* pre-multiplied alpha */
      line_data[offset + 0] = (b * a) / 255;  /* Blue */
      line_data[offset + 1] = (g * a) / 255;  /* Green */
      line_data[offset + 2] = (r * a) / 255;  /* Red */

      line_data[offset + 3] = a;  /* Alpha */
    }
  }
}

IWICBitmap* d2dCreateImageFromBufferRGB(UINT uWidth, UINT uHeight, const unsigned char *red, const unsigned char *green, const unsigned char *blue, const unsigned char *alpha)
{
  IWICBitmap* bitmap = NULL;
  HRESULT hr;
  WICRect rect;
  IWICBitmapLock *bitmap_lock = NULL;
  UINT cbBufferSize = 0;
  UINT dstStride = 0;
  BYTE *Scan0 = NULL;

  if (wic_cd_factory == NULL) {
    return NULL;
  }

  hr = IWICImagingFactory_CreateBitmap(wic_cd_factory, uWidth, uHeight, &wic_cd_pixel_format, WICBitmapCacheOnDemand, &bitmap);   /* GUID_WICPixelFormat32bppPBGRA - pre-multiplied alpha, BGRA order */
  if (FAILED(hr)) {
    return NULL;
  }

  rect.X = 0;
  rect.Y = 0;
  rect.Width = uWidth;
  rect.Height = uHeight;

  hr = IWICBitmap_Lock(bitmap, &rect, WICBitmapLockWrite, &bitmap_lock);
  if (FAILED(hr)) {
    IWICBitmap_Release(bitmap);
    return NULL;
  }

  IWICBitmapLock_GetStride(bitmap_lock, &dstStride);
  IWICBitmapLock_GetDataPointer(bitmap_lock, &cbBufferSize, &Scan0);

  if (alpha)
    bufferRGBA2Bitmap(Scan0, dstStride, uWidth, uHeight, red, green, blue, alpha);
  else
    bufferRGB2Bitmap(Scan0, dstStride, 4, uWidth, uHeight, red, green, blue);
 
  IWICBitmapLock_Release(bitmap_lock);
  return bitmap;
}

IWICBitmap* d2dCreateImageFromBufferMap(UINT uWidth, UINT uHeight, const unsigned char *map, const long* cPalette)
{
  IWICBitmap* bitmap = NULL;
  HRESULT hr;
  WICRect rect;
  IWICBitmapLock *bitmap_lock = NULL;
  UINT cbBufferSize = 0;
  UINT dstStride = 0;
  BYTE *Scan0 = NULL;

  if (wic_cd_factory == NULL) {
    return NULL;
  }

  hr = IWICImagingFactory_CreateBitmap(wic_cd_factory, uWidth, uHeight, &wic_cd_pixel_format, WICBitmapCacheOnDemand, &bitmap);   /* GUID_WICPixelFormat32bppPBGRA - pre-multiplied alpha, BGRA order */
  if (FAILED(hr)) {
    return NULL;
  }

  rect.X = 0;
  rect.Y = 0;
  rect.Width = uWidth;
  rect.Height = uHeight;

  hr = IWICBitmap_Lock(bitmap, &rect, WICBitmapLockWrite, &bitmap_lock);
  if (FAILED(hr)) {
    IWICBitmap_Release(bitmap);
    return NULL;
  }

  IWICBitmapLock_GetStride(bitmap_lock, &dstStride);
  IWICBitmapLock_GetDataPointer(bitmap_lock, &cbBufferSize, &Scan0);

  bufferMap2Bitmap(Scan0, dstStride, 4, uWidth, uHeight, map, cPalette);

  IWICBitmapLock_Release(bitmap_lock);
  return bitmap;
}

void d2dDestroyImage(IWICBitmap *image)
{
  IWICBitmapSource_Release(image);
}

void d2dBitBltImage(dummy_ID2D1RenderTarget *target, IWICBitmap *bitmap, const dummy_D2D1_RECT_F* pDestRect, const dummy_D2D1_RECT_F* pSourceRect)
{
  dummy_ID2D1Bitmap* b;
  HRESULT hr;
  dummy_D2D1_RECT_F pDest;

  /* Compensation for the translation in the base transformation matrix.
  * This is to fit the image precisely into the pixel grid the canvas
  * when there is no custom transformation applied.
  */
  pDest.left = pDestRect->left - D2D_BASEDELTA_X;
  pDest.top = pDestRect->top - D2D_BASEDELTA_X;
  pDest.right = pDestRect->right - D2D_BASEDELTA_X;
  pDest.bottom = pDestRect->bottom - D2D_BASEDELTA_X;

  hr = dummy_ID2D1RenderTarget_CreateBitmapFromWicBitmap(target, (IWICBitmapSource*)bitmap, NULL, &b);
  if (FAILED(hr))
    return;
  dummy_ID2D1RenderTarget_DrawBitmap(target, b, &pDest, 1.0f, dummy_D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, pSourceRect);
  dummy_ID2D1Bitmap_Release(b);
}
