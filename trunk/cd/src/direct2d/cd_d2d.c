/** \file
 * \brief Windows GDI+ Base Driver
 *
 * See Copyright Notice in cd.h
 */

#include "cd_d2d.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <tchar.h>
#include <malloc.h>


HMODULE d2d_cd_dll = NULL;
HMODULE wic_cd_dll = NULL;
HMODULE dwrite_cd_dll = NULL;

dummy_ID2D1Factory* d2d_cd_factory = NULL;
IWICImagingFactory* wic_cd_factory = NULL;
dummy_IDWriteFactory* dwrite_cd_factory = NULL;


static int d2d_cd_enabled(void)
{
  return (d2d_cd_factory != NULL);
}

static HMODULE d2d_cd_load_system_dll(const TCHAR* dll_name)
{
  HMODULE dll_kernel32;
  HMODULE dll;

  /* Check whether flag LOAD_LIBRARY_SEARCH_SYSTEM32 is supported on this
  * system. It has been added in Win Vista/7 with the security update
  * KB2533623. The update also added new symbAddDllDirectory() so we
  * use that as a canary. */
  dll_kernel32 = GetModuleHandle(_T("KERNEL32.DLL"));
  if (dll_kernel32 != NULL  &&
      GetProcAddress(dll_kernel32, "AddDllDirectory") != NULL)
    dll = LoadLibraryEx(dll_name, NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
  else {
    TCHAR path[MAX_PATH];
    UINT dllname_len;
    UINT sysdir_len;

    dllname_len = (UINT)_tcslen(dll_name);
    sysdir_len = GetSystemDirectory(path, MAX_PATH);
    if (sysdir_len + 1 + dllname_len >= MAX_PATH) {
      return NULL;
    }

    path[sysdir_len] = _T('\\');
    memcpy(path + sysdir_len + 1, dll_name, (dllname_len + 1) * sizeof(TCHAR));
    dll = LoadLibraryEx(path, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
  }

  return dll;
}

static int d2d_cd_init(void)
{
  static const dummy_D2D1_FACTORY_OPTIONS factory_options = { dummy_D2D1_DEBUG_LEVEL_NONE };
  HRESULT(WINAPI* fn_D2D1CreateFactory)(dummy_D2D1_FACTORY_TYPE, REFIID, const dummy_D2D1_FACTORY_OPTIONS*, void**);
  HRESULT hr;

  /* Load D2D1.DLL. */
  d2d_cd_dll = d2d_cd_load_system_dll(_T("D2D1.DLL"));
  if (d2d_cd_dll == NULL) {
    goto err_LoadLibrary;
  }

  fn_D2D1CreateFactory = (HRESULT(WINAPI*)(dummy_D2D1_FACTORY_TYPE, REFIID, const dummy_D2D1_FACTORY_OPTIONS*, void**))
    GetProcAddress(d2d_cd_dll, "D2D1CreateFactory");
  if (fn_D2D1CreateFactory == NULL) {
    goto err_GetProcAddress;
  }

  /* Create D2D factory object. Note we use D2D1_FACTORY_TYPE_SINGLE_THREADED
  * for performance reasons and manually synchronize calls to the factory.
  * This still allows usage in multi-threading environment but all the
  * created resources can only be used from the respective threads where
  * they were created. */
  hr = fn_D2D1CreateFactory(dummy_D2D1_FACTORY_TYPE_SINGLE_THREADED,
                            &dummy_IID_ID2D1Factory, &factory_options, (void**)&d2d_cd_factory);
  if (FAILED(hr)) {
    goto err_CreateFactory;
  }

  /* Success */
  return 0;

  /* Error path unwinding */
err_CreateFactory:
err_GetProcAddress :
  FreeLibrary(d2d_cd_dll);
                   d2d_cd_dll = NULL;
                 err_LoadLibrary:
                   return -1;
}

static void d2d_cd_fini(void)
{
  dummy_ID2D1Factory_Release(d2d_cd_factory);
  FreeLibrary(d2d_cd_dll);
  d2d_cd_dll = NULL;
}

static int wic_cd_init(void)
{
#ifdef WINCODEC_SDK_VERSION1
  /* This is only available in newer SDK headers. */
  static const UINT wic_version = WINCODEC_SDK_VERSION1;
#else
  static const UINT wic_version = WINCODEC_SDK_VERSION;
#endif

  HRESULT(WINAPI* fn_WICCreateImagingFactory_Proxy)(UINT, IWICImagingFactory**);
  HRESULT hr;

  wic_cd_dll = d2d_cd_load_system_dll(_T("WINDOWSCODECS.DLL"));
  if (wic_cd_dll == NULL) {
    goto err_LoadLibrary;
  }

  fn_WICCreateImagingFactory_Proxy =
    (HRESULT(WINAPI*)(UINT, IWICImagingFactory**))
    GetProcAddress(wic_cd_dll, "WICCreateImagingFactory_Proxy");
  if (fn_WICCreateImagingFactory_Proxy == NULL) {
    goto err_GetProcAddress;
  }

  hr = fn_WICCreateImagingFactory_Proxy(wic_version, &wic_cd_factory);
  if (FAILED(hr)) {
    goto err_WICCreateImagingFactory_Proxy;
  }

  /* Success. */
  return 0;

  /* Error path unwinding. */
err_WICCreateImagingFactory_Proxy:
err_GetProcAddress :
  FreeLibrary(wic_cd_dll);
                   wic_cd_dll = NULL;
                 err_LoadLibrary:
                   return -1;
}

static void wic_cd_fini(void)
{
  IWICImagingFactory_Release(wic_cd_factory);
  wic_cd_factory = NULL;

  FreeLibrary(wic_cd_dll);
  wic_cd_dll = NULL;
}

static int dwrite_init(void)
{
  HMODULE dll_kernel32;
  HRESULT(WINAPI* fn_DWriteCreateFactory)(int, REFIID, void**);
  HRESULT hr;

  dwrite_cd_dll = d2d_cd_load_system_dll(_T("DWRITE.DLL"));
  if (dwrite_cd_dll == NULL) {
    goto err_LoadLibrary;
  }

  fn_DWriteCreateFactory = (HRESULT(WINAPI*)(int, REFIID, void**))
    GetProcAddress(dwrite_cd_dll, "DWriteCreateFactory");
  if (fn_DWriteCreateFactory == NULL) {
    goto err_GetProcAddress;
  }

  hr = fn_DWriteCreateFactory(dummy_DWRITE_FACTORY_TYPE_SHARED,
                              &dummy_IID_IDWriteFactory, (void**)&dwrite_cd_factory);
  if (FAILED(hr)) {
    goto err_DWriteCreateFactory;
  }

  /* We need locale name for creation of dummy_IDWriteTextFormat. This
  * functions is available since Vista (which covers all systems with
  * Direct2D and DirectWrite). */
  dll_kernel32 = GetModuleHandle(_T("KERNEL32.DLL"));
  if (dll_kernel32 != NULL) {
      GetProcAddress(dll_kernel32, "GetUserDefaultLocaleName");
  }

  /* Success. */
  return 0;

  /* Error path unwinding. */
err_DWriteCreateFactory:
err_GetProcAddress :
  FreeLibrary(dwrite_cd_dll);
                 err_LoadLibrary:
                   return -1;
}

static void dwrite_cd_fini(void)
{
  dummy_IDWriteFactory_Release(dwrite_cd_factory);
  FreeLibrary(dwrite_cd_dll);
  dwrite_cd_factory = NULL;
}

void d2dStartup(void)
{
  if (!d2d_cd_enabled())
  {
    d2d_cd_init();
    wic_cd_init();
    dwrite_init();
  }
}

void d2dShutdown(void)
{
  if (d2d_cd_enabled())
  {
    d2d_cd_fini();
    wic_cd_fini();
    dwrite_cd_fini();
  }
}

d2dCanvas* d2dCanvasCreate(dummy_ID2D1RenderTarget* target, WORD type, BOOL rtl)
{
  d2dCanvas* c;

  c = (d2dCanvas*)malloc(sizeof(d2dCanvas));
  if (c == NULL) {
    return NULL;
  }

  memset(c, 0, sizeof(d2dCanvas));

  c->type = type;
  c->flags = (rtl ? D2D_CANVASFLAG_RTL : 0);
  c->target = target;

  /* We use raw pixels as units. D2D by default works with DIPs ("device
  * independent pixels"), which map 1:1 to physical pixels when DPI is 96.
  * So we enforce the render target to think we have this DPI. */
  dummy_ID2D1RenderTarget_SetDpi(c->target, 96.0f, 96.0f);

  d2dResetTransform(target);

  return c;
}

void d2dResetTransform(dummy_ID2D1RenderTarget* target)
{
  dummy_D2D1_MATRIX_3X2_F m;

  m._11 = 1.0f;   m._12 = 0.0f;
  m._21 = 0.0f;   m._22 = 1.0f;
  m._31 = D2D_BASEDELTA_X;
  m._32 = D2D_BASEDELTA_Y;

  dummy_ID2D1RenderTarget_SetTransform(target, &m);
}


static void matrix_mult(dummy_D2D1_MATRIX_3X2_F* res, const dummy_D2D1_MATRIX_3X2_F* a, const dummy_D2D1_MATRIX_3X2_F* b)
{
  res->_11 = a->_11 * b->_11 + a->_12 * b->_21;
  res->_12 = a->_11 * b->_12 + a->_12 * b->_22;
  res->_21 = a->_21 * b->_11 + a->_22 * b->_21;
  res->_22 = a->_21 * b->_12 + a->_22 * b->_22;
  res->_31 = a->_31 * b->_11 + a->_32 * b->_21 + b->_31;
  res->_32 = a->_31 * b->_12 + a->_32 * b->_22 + b->_32;
}

static void apply_transform(dummy_ID2D1RenderTarget* target, const dummy_D2D1_MATRIX_3X2_F* matrix)
{
  dummy_D2D1_MATRIX_3X2_F res;
  dummy_D2D1_MATRIX_3X2_F old_matrix;

  dummy_ID2D1RenderTarget_GetTransform(target, &old_matrix);
  matrix_mult(&res, matrix, &old_matrix);
  dummy_ID2D1RenderTarget_SetTransform(target, &res);
}

void d2dRotateWorld(dummy_ID2D1RenderTarget *target, float cx, float cy, float fAngle)
{
  dummy_D2D1_MATRIX_3X2_F m;
  float a_rads = fAngle * (PI / 180.0f);
  float a_sin = sinf(a_rads);
  float a_cos = cosf(a_rads);

  m._11 = a_cos;  m._12 = a_sin;
  m._21 = -a_sin; m._22 = a_cos;
  m._31 = cx - cx*a_cos + cy*a_sin;
  m._32 = cy - cx*a_sin - cy*a_cos;
  apply_transform(target, &m);
}

void d2dResetClip(d2dCanvas* c)
{
  if (c->clip_layer != NULL) 
  {
    dummy_ID2D1RenderTarget_PopLayer(c->target);
    dummy_ID2D1Layer_Release(c->clip_layer);
    c->clip_layer = NULL;
  }

  if (c->flags & D2D_CANVASFLAG_RECTCLIP) 
  {
    dummy_ID2D1RenderTarget_PopAxisAlignedClip(c->target);
    c->flags &= ~D2D_CANVASFLAG_RECTCLIP;
  }
}

void d2dSetClip(d2dCanvas *canvas, const dummy_D2D1_RECT_F* pRect)
{
  if (canvas->flags & D2D_CANVASFLAG_RECTCLIP)
  {
    dummy_ID2D1RenderTarget_PopAxisAlignedClip(canvas->target);
    canvas->flags &= ~D2D_CANVASFLAG_RECTCLIP;
  }

  if (pRect != NULL)
  {
    dummy_ID2D1RenderTarget_PushAxisAlignedClip(canvas->target, pRect, dummy_D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    canvas->flags |= D2D_CANVASFLAG_RECTCLIP;
  }
}
