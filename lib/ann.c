#include <ann.h>

ann_t *ann_create()
{
	unsigned int *nn = NULL;
	size_t i = 0;

	ALLOC(nn, options->num_layers, sizeof(unsigned int));

	for(i=0; i<options->num_layers; i++)
		nn[i] = (unsigned int) options->num_neurons[i];
	ann_t *net = (ann_t *) fann_create_array((float) options->connection_rate,\
					(float) options->learning_rate,\
					(unsigned int) options->num_layers,\
					nn);


	if(!net)
	{
		/* FIXME */
		exit(0);
	}

	return net;
}

ann_t *ann_load()
{
	ann_t *net = (ann_t *) fann_create_from_file((const char *) options->knowledge_file);

	if(!net)
	{
		/* FIXME */
		exit(0);
	}

	return net;
}

void ann_set(ann_t *net, ann_param_t param)
{
	if((param == ann_all) || (param == ann_learning_rate))
		fann_set_learning_rate((struct fann *) net, options->learning_rate);
}

void ann_save(ann_t *net)
{
	fann_save((struct fann *) net, (const char *) options->knowledge_file);
}

void ann_destroy(ann_t *net)
{
	fann_destroy((struct fann *) net);
}

book_t *book_create(size_t num_lessons, size_t num_input, size_t num_output)
{
	fann_type *in, *out;
	size_t i = 0;
	struct fann_train_data *book = NULL;

	ALLOC(book, 1L, sizeof(struct fann_train_data));

	fann_init_error_data((struct fann_error *) book);

	book->num_data = num_lessons;
	book->num_input = num_input;
	book->num_output = num_output;

	ALLOC(book->input, num_lessons, sizeof(fann_type *));
	ALLOC(book->output, num_lessons, sizeof(fann_type *));

	ALLOC(in, num_input * num_lessons, sizeof(fann_type));
	ALLOC(out, num_output * num_lessons, sizeof(fann_type));

	for(i = 0; i != num_lessons; i++)
	{
		book->input[i] = in;
		in += num_input;
		book->output[i] = out;
		out += num_output;
	}

	return (book_t *) book;
}

void book_set_lesson(book_t *book, size_t lesson, double *in, double *out)
{
	size_t i = 0;

	if(book->num_data < lesson)
	{
		pg_error_set(true, pg_book_overflow, "lesson %d / %d", lesson, book->num_data);
		pg_error_handler();
	}

	lesson--;
	for(i = 0; i < book->num_input; i++)
		book->input[lesson][i] = (fann_type) in[i];

	for(i = 0; i < book->num_output; i++)
		book->output[lesson][i] = (fann_type) out[i];
}

void book_destroy(book_t *book)
{
	fann_destroy_train((struct fann_train_data *) book);
}

void ann_train(ann_t *net, book_t *book, int (*callback)(unsigned int epochs, float error))
{

	if(!callback)
		callback = &ann_report_default;

	fann_train_on_data_callback((struct fann *) net,\
			(struct fann_train_data *) book,\
			(unsigned int) options->max_epochs,\
			(unsigned int) options->epochs_between_reports,\
			(float) options->max_error,\
			callback);
}

double *ann_run(ann_t *net, double *in)
{
	double *out = NULL;
	fann_type *fann_out = NULL;
	fann_type *fann_in = NULL;

	size_t i = 0;

	ALLOC(fann_in, options->num_neurons[0], sizeof(fann_type));
	ALLOC(out, options->num_neurons[options->num_layers - 1], sizeof(double));

	for(i=0; i < options->num_neurons[0]; i++)
		fann_in[i] = (fann_type) in[i];

	fann_out = fann_run((struct fann *) net, fann_in);

	for(i=0; i < options->num_neurons[options->num_layers - 1]; i++)
		out[i] = (double) fann_out[i];

	free(fann_out);

	return out;
}

static int ann_report_default(unsigned int epochs, float error)
{
	static clock_t start = (clock_t) -1;
	struct tms ts;
	static int c_handle = -1;
	static int c_handle2 = -1;

	times(&ts);
	if(start == (clock_t) -1)
		start = ts.tms_utime + ts.tms_stime;

	print_verbose(v_verbose, "epoch %d / %d\terror %f / %f\n", epochs, options->max_epochs, error, options->max_error);

	c_handle = print_gverbose(g_verbose, c_handle, "error_function", epochs, error);
	c_handle2 = print_gverbose(g_verbose, c_handle2, "error_in_time", ((double)(ts.tms_utime + ts.tms_stime - start) / CLOCKS_PER_SEC), error);

	if((error <= options->max_error) || (epochs >= options->max_epochs))
	{
		c_handle = print_gverbose(g_verbose, c_handle, "", 0, 0);
		c_handle2 = print_gverbose(g_verbose, c_handle2, "", 0, 0);
	}
	
	return 0;
}

void ann_list(ann_t *net)
{
	fann_print_parameters((struct fann *) net);
}

