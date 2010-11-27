#ifndef __ANN_H__
#define __ANN_H__

#include <common.h>
#include <fann.h>
#include <sys/times.h>
#include <time.h>
/* definitions */

/* costume types */
typedef struct fann ann_t;
typedef struct fann_train_data book_t;

typedef enum{ann_all,\
		ann_algorithm,\
		ann_learning_rate,\
		ann_error_function} ann_param_t;

/* global variables */

/* functions */
ann_t *ann_create();
ann_t *ann_load();
void ann_save(ann_t *net);
void ann_destroy(ann_t *net);

book_t *book_create(size_t num_lessons, size_t num_input, size_t num_output);
void book_set_lesson(book_t *book, size_t lesson, double *in, double *out);
void book_destroy(book_t *book);

void ann_train(ann_t *net, book_t *book, int (*callback)(unsigned int epochs, float error));
double *ann_run(ann_t *net, double *in);
static int ann_report_default(unsigned int epochs, float error);

void ann_list(ann_t *net);

#endif
