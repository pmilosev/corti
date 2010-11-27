#ifndef __FFT_H__
#define __FFT_H__

#include "fftw3.h"

typedef enum{initial, f_c2c, f_r2c, h_r2c} conv_type;

int fft (double *rin, double *iin, int i_len, double *rout, double *iout);
int dct(double *rin, int i_len, double *rout);
inline double itof (double min_f, double max_f, size_t N, size_t n);

#endif
