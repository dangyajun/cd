#include <stdlib.h>
#include <stdio.h>

#include "cd_plus.h"


int main(int argc, char** argv)
{
  if (argc < 3)
  {
    printf("Missing parameters: emf_filename pdf_filename\n");
    return 1;
  }
  
  const char* emf_filename = argv[1];
  const char* pdf_filename = argv[2];
  
  //use A4 paper, 300 DPI, portrait
  cd::CanvasMetafilePDF pdf_canvas(pdf_filename, CD_A4, 300);
  if (pdf_canvas.Failed())
  {
    printf("CreateCanvas(CD_PDF) - Failed!\n");
    return 1;
  }

  int pdf_w, pdf_h;
  double w_mm, h_mm;
  pdf_canvas.GetSize(pdf_w, pdf_h, w_mm, h_mm);

  pdf_canvas.Play(CD_EMF, 0, pdf_w - 1, 0, pdf_h - 1, (void*)emf_filename);

  return 0;
}
