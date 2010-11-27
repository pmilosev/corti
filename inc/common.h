#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>

/* definitions */
#ifndef NULL
#define NULL	0
#endif

#define MAX_PG_ERROR_DESC	1024
#define MAX_PATH_LEN		BUFSIZ

#define RANDOM(x)		((double) (x * (rand()/RAND_MAX)))

#define ALLOC(b,n,s)	{\
				if(!(b = calloc(n, s)))\
				{\
					pg_error_set(true, pg_alloc, "%d blocks of size %d were requested", n, s);\
					pg_error_handler();\
				}\
			}

#define REALLOC(b,s)	{\
				void *temp = NULL;\
				if(!(temp = realloc (b, s)))\
				{\
					pg_error_set(true, pg_alloc, "new size requested is %d", s);\
					pg_error_handler();\
				}\
				b = temp;\
			}\

#define F_OPEN(f, n, t)	{\
				f = fopen(n, t);\
				if(!f)\
				{\
					pg_error_set(true, pg_fopen, "%s", n);\
					pg_error_handler();\
				}\
			}

#define F_CLOSE(f)	{\
				if(f && fclose(f))\
				{\
					pg_error_set(false, pg_fclose, "");\
					pg_error_handler();\
				}\
			}

#define F_WRITE(p, s, n, f)	{\
					size_t temp = fwrite(p, s, n, f);\
					if(temp < n)\
					{\
						pg_error_set(true, pg_fwrite, "%d / %d blocks of size %d written", temp, n, s);\
						pg_error_handler();\
					}\
				}

#define F_READ(p, s, n, f)	{\
					size_t temp = fread(p, s, n, f);\
					if(temp < n)\
					{\
						pg_error_set(true, pg_fread, "%d / %d blocks of size %d read", temp, n, s);\
						pg_error_handler();\
					}\
				}

#define TRIM(a)		{\
				char *p1 = NULL;\
				char *p2 = NULL;\
				p1 = a;\
				while(p1 && (isspace((unsigned char) *p1)))\
				{\
					p2 = p1;\
					while(p2 && *p2)\
					{\
						*p2 = *(p2 + 1);\
						 p2++;\
					}\
				}\
				p2 = p1 + strlen(p1) - 1;\
				while(p2 && isspace((unsigned char) *p2))\
					p2--;\
				p2++;\
				*(p2) = '\0';\
			}\
/* costum types */
typedef enum{false, true} bool;
typedef enum{c_new, c_list, c_update, c_train, c_detect} command_t;
typedef enum{v_quiet, v_normal, v_verbose, v_verbose2} verbose_t;
typedef enum{g_quiet, g_normal, g_verbose, g_verbose2} gverbose_t;
typedef enum{pg_ok,\
		pg_unknown,\
		pg_alloc,\
		pg_fopen,\
		pg_fclose,\
		pg_fread,\
		pg_fwrite,\
		pg_book_overflow,\
		pg_num_layers,\
		pg_file_format} pg_error_code_t;

typedef struct { bool critical; pg_error_code_t code; char description[MAX_PG_ERROR_DESC + 1];} pg_error_t;
typedef void(*pg_error_handler_t)(void);

/* FIXME these two types should go to window heder file but are needed here for the structure below */
typedef enum{w_rect, w_gaus, w_hamming, w_hann, w_bartlett, w_triang, w_bar_hann, w_blackman} window_t;
typedef enum{s_normal, s_mel} scale_t;

typedef struct
{
	verbose_t verbose;
	gverbose_t gverbose;

	/* FIXME these are the problematic types */
	window_t window_type;
	scale_t scale_type;

	char gpath[MAX_PATH_LEN + 1];
	char knowledge_file[MAX_PATH_LEN + 1];
	char book_file[MAX_PATH_LEN + 1];

	double connection_rate;
	double learning_rate;
	double max_error;
	double window_overlap;
	double min_frequency;
	double max_frequency;

	size_t max_epochs;
	size_t epochs_between_reports;
	size_t num_layers;
	size_t *num_neurons;
	size_t num_windows;
	size_t num_coefitients;
	size_t first_coefitient;
} options_t;

/* global variables */
pg_error_t *pg_global_error;
pg_error_handler_t pg_error_handler;
options_t *options;

/* functions */
inline int print_gverbose(gverbose_t level, int handle, const char *identifier, const double x, const double y);
inline void print_verbose(verbose_t level, const char *fmt, ...);
inline void print_error(verbose_t level, const char *fmt, ...);
inline void pg_error_set(bool critical, pg_error_code_t code, char *fmt, ...);
inline void pg_error_def_handler(void);
options_t *options_create(void);
options_t *options_load(const char *config_filename);
void options_destroy(options_t *net);

#endif
