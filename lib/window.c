#include "window.h"


/* filterbank function
 *
 * 	s 		- input array of samples
 * 	w		- pointer to the output buffer (may be reallocated)
 * 	in_len 		- length of the input array
 * 	num_windows 	- number of windows that should be applied
 * 	window_function - one of the several window functions defined bellow
 * 	fmt 		- if not NULL and not empty "" then it should contain a string of parameter identifiers
 * 		"t" 		- scale type (default is s_normal)
 * 		"r"		- sampling rate (default is 2 * in_len)
 * 		"f"		- min frequency, from where to start (default is 0)
 * 		"F"		- max frequency, where to stop (deffault is sampling rate / 2)
 * 		"o"		- window overlap (deffault is 0.5 (the next window starts at the middle of the previous))
 * 		"b"		- length of the output buffer (deffault is in_len if not NULL)
 * 		"i"		- pointer to the buffer where window points will be stored (lenght 3 * num_windows)
 *
 * 	even if we apply window function to something that is not frequency we will calculate the window margins after converting
 * 	indexes to frequency. This will make no change for general purpose filterbanks and will generalize the idea for frequency 
 * 	filterbanks in different scales
 *
 * 	when filtering by frequency ... the input array have to contain the whole range from 0 - sampling rate/2 ... if we whant
 * 	to operate only on subset of frequencies they should be provided as arguments
 *
 */
void filterbank(const double *s, double **w, size_t in_len, size_t num_windows, pt2windowf window_function, const char *fmt, ...)
{
	size_t window = 0;
	size_t sample = 0;
	size_t jump = 0;
	size_t x = 0;
	size_t buff_size = in_len;
	size_t *indexes = NULL;

	double fband = 0;
	double wlenf = 0;
	double wbegf = 0;
	double wmidf = 0;
	double wendf = 0;
	size_t wleni = 0;
	size_t wbegi = 0;
	size_t wmidi = 0;
	size_t wendi = 0;

	scale_t scale_type = s_normal;
	double	sample_rate = 2 * in_len;
	double	first_freq = 0;
	double	last_freq = in_len;
	double	overlap = 0.5;

	/* get extra arguments */
	if(fmt)
	{
		va_list ap;
		const char *p = fmt;

		va_start(ap, fmt);
		while(*p)
		{
			switch(*p)
			{
				case 't':
					/* if scale type place holder was found */
					scale_type = va_arg(ap, scale_t);
					break;
				case 'r':
					/* if sample rate place holder was found */
					sample_rate = va_arg(ap, double);
					last_freq = sample_rate / 2;
					break;
				case 'f':
					/* if first freq. place holder was found */
					first_freq = va_arg(ap, double);
					break;
				case 'F':
					/* if last freq. place holder was found */
					last_freq = va_arg(ap, double);
					break;
				case 'o':
					/* if overlap place holder was found */
					overlap = va_arg(ap, double);
					break;
				case 'b':
					/* if output buff size place holder was found */
					buff_size = va_arg(ap, size_t);
					break;
				case 'i':
					/* if indexes place holder was found */
					indexes = va_arg(ap, size_t *);
					break;
				default:
					/* if not one of the above ignore it (usefull for using separators) */
					break;
			}
			p++;
		}
		va_end(ap);
	}

	/* how much of the frquency spectar does each index covers */
	fband = ITOF(1, in_len, sample_rate);

	/* map frequencies to the scale we will use and calculate the window len in that scale */
	first_freq = scale_type == s_normal ? first_freq : FTOM(first_freq);
	last_freq = scale_type == s_normal ? last_freq : FTOM(last_freq);
	if(num_windows > 1)
		wlenf = (((last_freq - first_freq) / (num_windows+1)) * (2 + 2*(num_windows-1)*overlap + (num_windows-2)*(overlap-1)))/ num_windows;
	else
		wlenf = last_freq - first_freq;

	if((*w) == NULL)
	{
		/* FIXME the buff size is not calculated directly for MEL filters and REALLOC may be needed */
		buff_size = scale_type == s_normal ? wlenf * num_windows : MTOF(wlenf * num_windows) / fband;
		ALLOC((*w), buff_size, sizeof(double));
	}

	wbegf = first_freq;
	for (window=0; window < num_windows; window++)
	{
		/* each window is of equal size in the scale provided */
		wendf = wbegf + wlenf;
		wmidf = (wendf + wbegf) / 2.00;
		
		/* find the indexes containing window margin frequencies */
		wbegi =	(size_t) (scale_type == s_normal ? wbegf :\
				MTOF(wbegf)\
			) / fband;
		wmidi =	(size_t) (scale_type == s_normal ? wmidf :\
				MTOF(wmidf)\
			) / fband;
		wendi =	(size_t) (scale_type == s_normal ? wendf :\
				MTOF(wendf)\
			) / fband;

		/* beacouse of scale change the middle point may be not in the center after transformation to normal scale 
		 * first calculate the window up to its middle point */
		wleni = (wmidi - wbegi + 1) * 2;
		if((wleni + x) > buff_size)
		{
			buff_size = wleni + x;
			REALLOC((*w), buff_size * sizeof(double));
		}

		if(indexes)
			indexes[(3 * window) + 0] = x;

		for (sample = 0; sample < (wleni/2); sample++)
			(*w)[x++] = s[wbegi + sample] * window_function(sample, wleni);

		 /* then calculate the window (after size corrections) to its end point */
		wleni = (wendi - wmidi + 1) * 2;
		jump = (size_t) (wleni / 2) - sample;
		sample = (size_t) (wleni / 2);

		if(indexes)
			indexes[(3 * window) + 1] = x-1;

		if((wleni + x) > buff_size)
		{
			buff_size = wleni + x;
			REALLOC((*w), buff_size * sizeof(double));
		}
		for (sample = (wleni / 2); sample < wleni; sample++)
			(*w)[x++] = s[wbegi + sample - jump] * window_function(sample, wleni);

		if(indexes)
			indexes[(3 * window) + 2] = x-1;

		/* find the beggining of the next window */
		wbegf += wlenf * (1 - overlap);
	}

	/* FIXME free the unused memory */
	//REALLOC((*w), x * sizeof(double));
};

/* main window function
 *
 * 	s - input array of samples
 * 	w - output array
 * 	N - length of the window (power of 2; length of s and w)
 * 	window_function - one of the several window functions defined bellow
 *
 */
void windowize(double *s, double *w, size_t N, pt2windowf window_function)
{
	size_t n=0;

	for (n=0; n < N; n++)
		w[n] = s[n] * window_function(n, N);
};

/* several windowing functions (www.wikipedia.org) 
 *
 * 	n - current position in the window (0 <= n < N)
 * 	N - length of the window
 */
inline double wRect(size_t n, size_t N) {N=n=1; return 1;};
inline double wGaus(size_t n, size_t N) {return exp(-0.5 * pow((n - (N-1)/2.0) / (0.5 * (N-1)/2.0), 2.0));};
inline double wHamming(size_t n, size_t N) {return 0.53836 - 0.46164 * cos(M_2_PI * (double) n/(N-1));};
inline double wHann(size_t n, size_t N) {return 0.5 * (1 - cos(M_2_PI * (double) n/(N-1)));};
inline double wBartlett(size_t n, size_t N) {return 2.0/(N-1) * ((N-1)/2.0 - fabs(n - (N-1)/2.0));};
inline double wTriang(size_t n, size_t N) {return 2.0/N * (N/2.0 - fabs(n - (N-1)/2.0));};
inline double wBarHann(size_t n, size_t N) {return 0.62 - 0.48*fabs((double) n/(N-1) - 0.5) - 0.38*cos(M_2_PI * (double) n/(N-1));};
inline double wBlackman(size_t n, size_t N) {return 0.42 - 0.5*cos(M_2_PI * (double) n/(N-1)) + 0.08*cos(2*M_2_PI * (double)n/(N-1));};
