/** \file
 * \brief GDI+ Control
 *
 * See Copyright Notice in cd.h
 */
 
#include "cd.h"
#include "cd_private.h"
#include "cd_d2d.h"
#include <stdlib.h>
#include <memory.h>

cdContext* cdContextNativeWindowPlus(void);
//cdContext* cdContextImagePlus(void);
//cdContext* cdContextPrinterPlus(void);
//cdContext* cdContextEMFPlus(void);
//cdContext* cdContextClipboardPlus(void);

void cdInitContextPlus(void)
{
  cdContext* ctx_list[CD_CTXPLUS_COUNT];
  memset(ctx_list, 0, sizeof(ctx_list));

  ctx_list[CD_CTXPLUS_NATIVEWINDOW] = cdContextNativeWindowPlus();
  //ctx_list[CD_CTXPLUS_IMAGE] = cdContextImagePlus();
  ctx_list[CD_CTXPLUS_DBUFFER] = cdContextNativeWindowPlus();  /* always double buffered */
  //ctx_list[CD_CTXPLUS_PRINTER] = cdContextPrinterPlus();
  //ctx_list[CD_CTXPLUS_EMF] = cdContextEMFPlus();
  //ctx_list[CD_CTXPLUS_CLIPBOARD] = cdContextClipboardPlus();

  cdInitContextPlusList(ctx_list);

  d2dStartup();
}

void cdFinishContextPlus(void)
{
  d2dShutdown();
}
