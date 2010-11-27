#include "aiff.h"

void reverse_byte_order(void *buff, size_t buff_size)
{
	char temp_byte = 0;
	size_t i = 0;

	for(i=0; i < buff_size/2; i++)
	{
		temp_byte = *((char *) buff + i);
		*((char *) buff + i) = *((char *) buff+buff_size-1 - i);
		*((char *) buff+buff_size-1 - i) = temp_byte;
	}
}

struct aiff_file *load_aiff(const char *file_name)
{
	FILE *in_file=NULL;
	struct aiff_file *mem_file = NULL;
	char temp_id[5];
	unsigned long count = 0;

	/* try to open the input file */
	F_OPEN(in_file, file_name, "rb");

	/* check if the file provided is of AIFF format */
	temp_id[4] = '\0';
	F_READ(temp_id, SIZE_OF_CHAR, 4L, in_file);
	if(strcmp(temp_id, "FORM"))
	{
		pg_error_set(true, pg_file_format, "file '%s'; FORM id not found", file_name);
		pg_error_handler();
	}

	F_READ(temp_id, SIZE_OF_CHAR, 4L, in_file);
	F_READ(temp_id, SIZE_OF_CHAR, 4L, in_file);
	if(strcmp(temp_id, "AIFF"))
	{
		pg_error_set(true, pg_file_format, "file '%s'", file_name);
		pg_error_handler();
	}
	
	ALLOC(mem_file, 1L, sizeof(struct aiff_file));

	/* read the common chunk ID */
	F_READ(&mem_file->cc.id[0], SIZE_OF_CHAR, 4L, in_file);
	mem_file->cc.id[5] = '\0';

	if(strcmp(mem_file->cc.id, "COMM"))
	{
		free(mem_file);

		pg_error_set(true, pg_file_format, "file '%s'; COMM id not found", file_name);
		pg_error_handler();
	}

	/* read the common_chunk size */
	F_READ(&mem_file->cc.chunk_size, SIZE_OF_LONG, 1L, in_file);
	reverse_byte_order((void *) &mem_file->cc.chunk_size, SIZE_OF_LONG);

	/* read the common_chunk number of channels */
	F_READ(&mem_file->cc.num_channels, SIZE_OF_SHORT, 1L, in_file);
	reverse_byte_order((void *) &mem_file->cc.num_channels, SIZE_OF_SHORT);

	/* read the common_chunk number of frames */
	F_READ(&mem_file->cc.num_sample_frames, SIZE_OF_LONG, 1L, in_file);
	reverse_byte_order((void *) &mem_file->cc.num_sample_frames, SIZE_OF_LONG);

	/* read the common_chunk sample size */
	F_READ(&mem_file->cc.sample_size, SIZE_OF_SHORT, 1L, in_file);
	reverse_byte_order((void *) &mem_file->cc.sample_size, SIZE_OF_SHORT);

	mem_file->sample_byte_size = ceil(mem_file->cc.sample_size / 8.0);
	mem_file->sample_justify_size = mem_file->sample_byte_size * 8 - mem_file->cc.sample_size; 

	/* we will not support sizes that are not multiple of 8 for now */
	if(mem_file->sample_justify_size != 0)
	{
		free(mem_file);

		pg_error_set(true, pg_file_format, "file '%s'; justified file", file_name);
		pg_error_handler();
	}

	mem_file->frame_byte_size = mem_file->sample_byte_size * mem_file->cc.num_channels;

	/* read the common_chunk sample rate */
	F_READ(&mem_file->cc.sample_rate, SIZE_OF_LONGDOUBLE, 1L, in_file);
	reverse_byte_order((void *) &mem_file->cc.sample_rate, SIZE_OF_LONGDOUBLE);

	/* read the sound data chunk ID */
	F_READ(&mem_file->sdc.id[0], SIZE_OF_CHAR, 4L, in_file);
	mem_file->sdc.id[5] = '\0';

	if(strcmp(mem_file->sdc.id, "SSND"))
	{
		free(mem_file);

		pg_error_set(true, pg_file_format, "file '%s'; SSND id not found", file_name);
		pg_error_handler();
	}

	/* read the sound data chunk size */
	F_READ(&mem_file->sdc.chunk_size, SIZE_OF_LONG, 1L, in_file);
	reverse_byte_order((void *) &mem_file->sdc.chunk_size, SIZE_OF_LONG);

	mem_file->waveform_length = mem_file->sdc.chunk_size - 8;

	/* see if there is any mismatch in the calculated size */
	if(mem_file->waveform_length - mem_file->frame_byte_size*mem_file->cc.num_sample_frames != 0)
	{
		free(mem_file);
	
		pg_error_set(true, pg_file_format, "file '%s'; size / calculated size mismatch", file_name);
		pg_error_handler();
	}

	mem_file->waveform_length = (mem_file->waveform_length & (long) 1) ? mem_file->waveform_length + 1 : mem_file->waveform_length;

	/* read the sound data chunk offset */
	F_READ(&mem_file->sdc.offset, SIZE_OF_LONG, 1L, in_file);
	reverse_byte_order((void *) &mem_file->sdc.offset, SIZE_OF_LONG);

	/* we will not support offset aligned data for now */
	if(mem_file->sdc.offset != 0)
	{
		free(mem_file);
	
		pg_error_set(true, pg_file_format, "file '%s'; offset aligned data", file_name);
		pg_error_handler();
	}

	/* read the sound data chunk block size */
	F_READ(&mem_file->sdc.block_size, SIZE_OF_LONG, 1L, in_file);
	reverse_byte_order((void *) &mem_file->sdc.block_size, SIZE_OF_LONG);

	/* we will not support block alligned data for now */
	if(mem_file->sdc.block_size != 0)
	{
		free(mem_file);
	
		pg_error_set(true, pg_file_format, "file '%s'; block aligned data", file_name);
		pg_error_handler();
	}

	/* read the waveform */
	mem_file->sdc.waveform_data = calloc(1L, mem_file->waveform_length);
	for(count=0; count < mem_file->cc.num_sample_frames; count++)
	{
		/* this block should be changed to allow
		 * 	sample sizes that are not multiple of 8
		 * 	waveforms that are offset / block aligned
		*/

		F_READ(((char *) mem_file->sdc.waveform_data) + (count * mem_file->frame_byte_size), (size_t) mem_file->frame_byte_size, 1L, in_file);
		reverse_byte_order(((char *) mem_file->sdc.waveform_data) + (count * mem_file->frame_byte_size), (size_t) mem_file->frame_byte_size);
	}

	F_CLOSE(in_file);
	return mem_file;
}

void aiff_destroy(struct aiff_file *mem_file)
{
	if(!mem_file)
		return;

	if(mem_file->sdc.waveform_data)
		free(mem_file->sdc.waveform_data);

	free(mem_file);
}
