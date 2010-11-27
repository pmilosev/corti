#include "fft.h"

int fft (double *rin, double *iin, int i_len, double *rout, double *iout)
{
	/* this function is only wrapper around the fftw function package 
	 *	rin  - real part of the input
	 *	iin  - imaginary part of the input
	 *	i_len- length of the input arrays
	 *	rout - real part of the output
	 *	iout - imaginary part of the output
	 *
	 * if iin is NULL then FULL real to complex transform is made
	 * if iout is NULL then iin is ignored and HALF real to complex is done stored as: 
	 * 	rout = r0, r1, r2, ..., rn/2, i(n+1)/2-1, ..., i2, i1
	 *
	 * call of this function with i_len = 0 to free all buffers. in this case rin and rout to be NULLs 
	 */

	static conv_type ct = initial;
	static int len_flag = 0;
	static fftw_plan plan;

	static double *r_in = NULL;
	static double *r_out = NULL;
	static fftw_complex *c_in = NULL;
	static fftw_complex *c_out = NULL;

	if(i_len == 0)
	{
		fftw_destroy_plan(plan);
		if(c_in)
			fftw_free(c_in);
		if(c_out)
			fftw_free(c_out);

		r_in = NULL;
		r_out= NULL;
		ct = initial;

		fftw_cleanup();

		return 0;
	}

	if(i_len < 1)
		return 1;

	if(!rin || !rout)
		return 1;

	if(iout == NULL)
	{
		/* gnerate new plan */
		if((len_flag != i_len) || (ct != h_r2c) || (r_in != rin) || (r_out != rout))
		{
			fftw_destroy_plan(plan);
			if(c_in)
				fftw_free(c_in);
			if(c_out)
				fftw_free(c_out);

			len_flag = i_len;
			r_in = rin;
			r_out = rout;

			ct = h_r2c;
			plan = fftw_plan_r2r_1d(len_flag, r_in, r_out, FFTW_R2HC, FFTW_ESTIMATE);
		}

		fftw_execute(plan);
		return 0;
	}

	if(iin == NULL)
	{
		if((len_flag != i_len) || (ct != f_r2c) || (r_in != rin))
		{
			fftw_destroy_plan(plan);
			if(c_in)
				fftw_free(c_in);
			if(c_out)
				fftw_free(c_out);
			r_out=NULL;

			len_flag = i_len;
			r_in = rin;
			c_out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * len_flag);

			ct = f_r2c;
			plan = fftw_plan_dft_r2c_1d(len_flag, r_in, c_out, FFTW_ESTIMATE);
		}

		fftw_execute(plan);
		for(len_flag = 0; len_flag < i_len; len_flag++)
		{
			rout[len_flag] = (double) c_out[len_flag][0];
			iout[len_flag] = (double) c_out[len_flag][1];
		}

		return 0;
	}

	if((len_flag != i_len) || (ct != f_c2c))
	{
		fftw_destroy_plan(plan);
		if(c_in)
			fftw_free(c_in);
		if(c_out)
			fftw_free(c_out);

		r_in = NULL;
		r_out= NULL;

		len_flag = i_len;
		c_in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * len_flag);
		c_out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * len_flag);

		ct = f_c2c;
		plan = fftw_plan_dft_1d(len_flag, c_in, c_out, FFTW_FORWARD, FFTW_ESTIMATE);
	}

	for(len_flag = 0; len_flag < i_len; len_flag++)
	{
		c_out[len_flag][0] = rin[len_flag];
		c_out[len_flag][1] = iin[len_flag];
	}

	fftw_execute(plan);

	for(len_flag = 0; len_flag < i_len; len_flag++)
	{
		rout[len_flag] = (double) c_out[len_flag][0];
		iout[len_flag] = (double) c_out[len_flag][1];
	}

	return 0;
}

int dct(double *rin, int i_len, double *rout)
{
	/* this function is only wrapper around the fftw function package - computes DCT-II 
	 *	rin  - real part of the input
	 *	i_len- length of the input arrays
	 *	rout - real part of the output
	 *
	 * call of this function with i_len = 0 to free all buffers. in this case rin and rout to be NULLs 
	 */

	static int len_flag = 0;
	static fftw_plan plan;

	static double *r_in = NULL;
	static double *r_out = NULL;

	if(i_len == 0)
	{
		fftw_destroy_plan(plan);

		r_in = NULL;
		r_out= NULL;

		fftw_cleanup();

		return 0;
	}

	if(i_len < 1)
		return 1;

	if(!rin || !rout)
		return 1;

	/* gnerate new plan */
	if((len_flag != i_len) || (r_in != rin) || (r_out != rout))
	{
		fftw_destroy_plan(plan);

		len_flag = i_len;
		r_in = rin;
		r_out = rout;

		plan = fftw_plan_r2r_1d(len_flag, r_in, r_out, FFTW_REDFT10, FFTW_ESTIMATE);
	}

	fftw_execute(plan);
	return 0;
}
