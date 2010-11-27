#ifndef __CORTI_H__
#define __CORTI_H__

#include "aiff.h"
#include "fft.h"
#include "window.h"
#include "ann.h"
#include "fann.h"
#include "common.h"

#define MAX_MULTY_OPT	5

void main_new();
void main_list();
void main_update();
void main_train();
void main_detect();

#endif
