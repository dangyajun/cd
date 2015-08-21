/** \file
 * \brief imImage driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_IM_H
#define __CD_IM_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextImImage(void);

#define CD_IMIMAGE cdContextImImage()


#ifdef __IM_IMAGE_H
void cdCanvasPutImImage(cdCanvas* canvas, const imImage* image, int x, int y, int w, int h);
#endif

#ifdef __cplusplus
}
#endif

#endif /* ifndef __CD_IM_ */
