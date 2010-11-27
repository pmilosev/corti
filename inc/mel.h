#ifndef __MEL_H__
#define __MEL_H__

#include "common.h"

#define FTOM(F)	(1127.01048*log(1+(F/700)))
#define MTOF(M)	(700*(exp(M/1127.01048)-1))

inline double mel_window(double f, double min_f, double mid_f, double max_f);
void mel_filter(double s[], double f[], size_t s_len, size_t n_win, double *mel_coef);

#endif
