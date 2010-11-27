#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "common.h"

#define ITOF(ind, len, srate)	(((srate / 2.00) / len) * ind)
#define FTOM(f)	(1127.01048 * log(1 + f/700.00))
#define MTOF(m) (700 * (pow(M_E, m/1127.01048) - 1))

typedef double(*pt2windowf)(size_t, size_t);

void filterbank(const double *s, double **w, size_t in_len, size_t num_windows, pt2windowf window_function, const char *fmt, ...);
void windowize(double *s, double *w, size_t N, pt2windowf window_function);

inline double wRect(size_t n, size_t N);
inline double wGaus(size_t n, size_t N);
inline double wHamming(size_t n, size_t N);
inline double wHann(size_t n, size_t N);
inline double wBartlett(size_t n, size_t N);
inline double wTriang(size_t n, size_t N);
inline double wBarHann(size_t n, size_t N);
inline double wBlackman(size_t n, size_t N);

#endif
