void cgm_setlinestyle ( int );
void cgm_setlinewidth ( double );
void cgm_setmarktype ( int );
void cgm_setmarksize ( double );
long int cgm_getcolor ( tcolor );
long int *cgm_setpattern ( pat_table );
int cgm_setfont ( int, int, double );
int cgm_setintstyle ( int );
int cgm_vdcx2canvas ( double );
int cgm_vdcy2canvas ( double );
int cgm_delta_vdc2canvas ( double );
double cgm_canvas2vdcx ( int );
double cgm_canvas2vdcy ( int );
void cgm_getpolybbox ( tpoint *, int, double *n, double *, double *, double * );
void cgm_getincpolybbox ( tpoint *, int, double *, double *, double *, double * );