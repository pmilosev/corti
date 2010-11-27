#ifndef __AIFF_H__
#define __AIFF_H__

#include "common.h"

#define SIZE_OF_LONG		 4L
#define SIZE_OF_LONGDOUBLE	10L
#define SIZE_OF_SHORT		 2L
#define SIZE_OF_CHAR		 1L

struct aiff_file
{
	/* the first two chunks of the file */
	struct common_chunk
	{
		char 		id[5];
		long int	chunk_size;

		short int 	num_channels;
		unsigned long 	num_sample_frames;
		short int 	sample_size;
		long double	sample_rate;
	}cc;

	struct sound_data_chunk
	{
		char 		id[5];
		long int	chunk_size;

		unsigned long	offset;
		unsigned long	block_size;
		void		*waveform_data;
	}sdc;

	/* the rest of the file is of no interest */
	/* this is not part of the file ... it will be calculated */
	unsigned char sample_byte_size;
	unsigned char sample_justify_size;
	unsigned char frame_byte_size;
	unsigned long waveform_length;
};

void reverse_byte_order(void *buff, size_t buff_size);
struct aiff_file *load_aiff(const char *file_name);
void aiff_destroy(struct aiff_file *mem_file);

#endif
