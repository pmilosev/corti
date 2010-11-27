#include "corti.h"
#include "argtable2.h"
#include "sys/types.h"
#include "regex.h"

/* TODO more options should be added on the command line */
/* NEW command options */
struct arg_rex *command_new;
struct arg_lit *verbose_new;
struct arg_lit *quiet_new;
struct arg_file *config_new;
struct arg_file *knowledge_new;
struct arg_end *end_new;

/* LIST command options */
struct arg_rex *command_list;
struct arg_lit *verbose_list;
struct arg_file *knowledge_list;
struct arg_end *end_list;

/* TODO UPDATE command options */

/* TRAIN command options */
struct arg_rex *command_train;
struct arg_lit *verbose_train;
struct arg_lit *graphicaly_verbose_train;
struct arg_lit *quiet_train;
struct arg_file *config_train;
struct arg_file *knowledge_train;
struct arg_file *book_train;
struct arg_end *end_train;

/* TODO DETECT command options */
struct arg_rex *command_detect;
struct arg_lit *verbose_detect;
struct arg_lit *graphicaly_verbose_detect;
struct arg_lit *quiet_detect;
struct arg_file *config_detect;
struct arg_file *knowledge_detect;
struct arg_file *book_detect;
struct arg_end *end_detect;

/* version option */
struct arg_lit *version_version;
struct arg_end *end_version;

/* help option */
struct arg_lit *help_help;
struct arg_end *end_help;

int main(int argc, char *argv[])
{
	int ret = 0;
	char *progname = "corti";

	void *argtable_new[6];
	void *argtable_list[4];
	void *argtable_train[8];
	void *argtable_detect[8];
	void *argtable_version[2];
	void *argtable_help[2];

	pg_error_t local_err;
	pg_global_error = &local_err;
	pg_error_handler = &pg_error_def_handler;

	/* NEW command init */
	command_new              = arg_rexn(NULL, NULL, "^NEW$", "NEW", 1, 1, REG_EXTENDED|REG_ICASE, "create new knowledge file");
	verbose_new              = arg_litn("v", "verbose", 0, 2, "explain what is been done");
	quiet_new                = arg_litn("q", "quiet", 0, 1, "supress ALL messages");
	config_new               = arg_filen(NULL, "config", "<CONFIGURATION FILE>", 1, 1, "configuration file");
	knowledge_new		 = arg_filen(NULL, NULL, "<KNOWLEDGE FILE>", 1, 1, "knowledge file name");

	end_new = arg_end(3);

	argtable_new[0] = command_new;
	argtable_new[1] = verbose_new;
	argtable_new[2] = quiet_new;
	argtable_new[3] = config_new;
	argtable_new[4] = knowledge_new;
	argtable_new[5] = end_new;

	/* LIST command init */
	command_list   = arg_rexn(NULL, NULL, "^LIST$", "LIST", 1, 1, REG_EXTENDED|REG_ICASE, "list the knowledge file");
	verbose_list   = arg_litn("v", "verbose", 0, 2, "show more details");
	knowledge_list = arg_filen(NULL, NULL, "<KNOWLEDGE FILE>", 1, 1, "knowledge file name");
	end_list = arg_end(5);
	
	argtable_list[0] = command_list;
	argtable_list[1] = verbose_list;
	argtable_list[2] = knowledge_list;
	argtable_list[3] = end_list;

	/* UPDATE command init */
	/* TRAIN option init */
	command_train              = arg_rexn(NULL, NULL, "^TRAIN$", "TRAIN", 1, 1, REG_EXTENDED|REG_ICASE, "train the network");
	verbose_train              = arg_litn("v", "verbose", 0, 2, "explain what is been done");
	graphicaly_verbose_train   = arg_litn("g", "graphicaly-verbose", 0, 2, "show graphics");
	quiet_train                = arg_litn("q", "quiet", 0, 1, "supress ALL messages");
	config_train               = arg_filen(NULL, "config", "<CONFIGURATION FILE>", 1, 1, "configuration file");
	knowledge_train		   = arg_filen(NULL, NULL, "<KNOWLEDGE FILE>", 1, 1, "knowledge file name");
	book_train		   = arg_filen(NULL, NULL, "<BOOK FILE>", 1, 1, "book file name");

	end_train = arg_end(3);

	argtable_train[0] = command_train;
	argtable_train[1] = verbose_train;
	argtable_train[2] = graphicaly_verbose_train;
	argtable_train[3] = quiet_train;
	argtable_train[4] = config_train;
	argtable_train[5] = knowledge_train;
	argtable_train[6] = book_train;
	argtable_train[7] = end_train;

	/* DETECT option init */
	command_detect              = arg_rexn(NULL, NULL, "^DETECT$", "DETECT", 1, 1, REG_EXTENDED|REG_ICASE, "clasify sound file");
	verbose_detect              = arg_litn("v", "verbose", 0, 2, "explain what is been done");
	graphicaly_verbose_detect   = arg_litn("g", "graphicaly-verbose", 0, 2, "show graphics");
	quiet_detect                = arg_litn("q", "quiet", 0, 1, "supress ALL messages");
	config_detect               = arg_filen(NULL, "config", "<CONFIGURATION FILE>", 1, 1, "configuration file");
	knowledge_detect	    = arg_filen(NULL, NULL, "<KNOWLEDGE FILE>", 1, 1, "knowledge file name");
	book_detect		    = arg_filen(NULL, NULL, "<SOUND FILE>", 1, 1, "sound file name");

	end_detect = arg_end(3);

	argtable_detect[0] = command_detect;
	argtable_detect[1] = verbose_detect;
	argtable_detect[2] = graphicaly_verbose_detect;
	argtable_detect[3] = quiet_detect;
	argtable_detect[4] = config_detect;
	argtable_detect[5] = knowledge_detect;
	argtable_detect[6] = book_detect;
	argtable_detect[7] = end_detect;

	/* version options */
	version_version = arg_litn(NULL, "version", 1, 1, "show version");
	end_version = arg_end(5);

	argtable_version[0] = version_version;
	argtable_version[1] = end_version;

	/* help options */
	help_help = arg_litn(NULL, "help", 1, 1, "display this help");
	end_help = arg_end(5);

	argtable_help[0] = help_help;
	argtable_help[1] = end_help;

	/* parse arguments */
	if ((arg_nullcheck(argtable_new) != 0) ||\
		(arg_nullcheck(argtable_list)) ||\
		(arg_nullcheck(argtable_version)) ||\
		(arg_nullcheck(argtable_help)))
	{
		fprintf(stderr, "error: can not allocate argumment tables\n");
		return 1;
	}

	if(!arg_parse(argc, argv, argtable_new))
	{
		/* call the main function for the NEW command */
		main_new();
	}
	else if(command_new->count > 0)
	{
		/* show syntax of the NEW command */
		arg_print_errors(stderr, end_new, progname);
		fprintf(stderr, "usage:\n");
		fprintf(stderr, "\t%s", progname);
		arg_print_syntaxv(stderr, argtable_new, "\n");

		ret = 1;
	}
	else if(!arg_parse(argc, argv, argtable_list))
	{
		/* call the main function for the LIST command */
		main_list();
	}
	else if(command_list->count > 0)
	{
		/* show syntax of the LIST command */
		arg_print_errors(stderr, end_list, progname);
		fprintf(stderr, "usage:\n");
		fprintf(stderr, "\t%s", progname);
		arg_print_syntaxv(stderr, argtable_list, "\n");

		ret = 1;
	}
//	else if(!arg_parse(argc, argv, argtable_update))
//	{
//		/* call the main function for the UPDATE command */
//	}
//	else if(command_update->count > 0)
//	{
//		/* show syntax of the UPDATE command */
//		arg_print_errors(stderr, end_update, progname);
//		fprintf(stderr, "usage:\n");
//		fprintf(stderr, "\t%s", progname);
//		arg_print_syntaxv(stderr, argtable_update, "\n");
//
//		ret = 1;
//	}
	else if(!arg_parse(argc, argv, argtable_train))
	{
		/* call the main function for the TRAIN command */
		main_train();
	}
	else if(command_train->count > 0)
	{
		/* show syntax of the TRAIN command */
		arg_print_errors(stderr, end_train, progname);
		fprintf(stderr, "usage:\n");
		fprintf(stderr, "\t%s", progname);
		arg_print_syntaxv(stderr, argtable_train, "\n");

		ret = 1;
	}
	else if(!arg_parse(argc, argv, argtable_detect))
	{
		/* call the main function for the DETECT command */
		main_detect();
	}
	else if(command_detect->count > 0)
	{
		/* show syntax of the DETECT command */
		arg_print_errors(stderr, end_detect, progname);
		fprintf(stderr, "usage:\n");
		fprintf(stderr, "\t%s", progname);
		arg_print_syntaxv(stderr, argtable_detect, "\n");

		ret = 1;
	}
	else if(!arg_parse(argc, argv, argtable_version))
	{
		/* display version */
		fprintf(stdout, "version 0.0.0\n");
		ret = 0;
	}
	else if(!arg_parse(argc, argv, argtable_help))
	{
		/* display help */
		fprintf(stdout, "SYNTAX:\n");
		printf("%s { <COMMAND> <OPTIONS> | [-h | --help] [--version] }\n",progname);
		fprintf(stdout, "\nCOMMANDs and OPTIONS:\n");

		fprintf(stdout, "%s", progname);
		arg_print_syntax(stdout, argtable_new, "\n\n");
		arg_print_glossary_gnu(stdout, argtable_new);

		fprintf(stdout, "\n\n%s", progname);
		arg_print_syntax(stdout, argtable_list, "\n\n");
		arg_print_glossary_gnu(stdout, argtable_list);

//		fprintf(stdout, "\n\n%s", progname);
//		arg_print_syntax(stdout, argtable_update, "\n\n");
//		arg_print_glossary_gnu(stdout, argtable_update);
//
		fprintf(stdout, "\n\n%s", progname);
		arg_print_syntax(stdout, argtable_train, "\n\n");
		arg_print_glossary_gnu(stdout, argtable_train);

		fprintf(stdout, "\n\n%s", progname);
		arg_print_syntax(stdout, argtable_detect, "\n\n");
		arg_print_glossary_gnu(stdout, argtable_detect);

		fprintf(stdout, "\n\n%s", progname);
		arg_print_syntax(stdout, argtable_version, "\n\n");
		arg_print_glossary_gnu(stdout, argtable_version);

		fprintf(stdout, "\n\n%s", progname);
		arg_print_syntax(stdout, argtable_help, "\n\n");
		arg_print_glossary_gnu(stdout, argtable_help);

		ret = 0;
	}
	else
	{
		/* no command was specified */
		fprintf(stderr, "SYNTAX:\n");
		printf("%s { <COMMAND> <OPTIONS> | [-h | --help] [--version] }\n",progname);
		fprintf(stderr, "\nUSAGE:\n");
		fprintf(stderr, "%s", progname);
		arg_print_syntax(stderr, argtable_new, "\n");
		fprintf(stderr, "\n%s", progname);
		arg_print_syntax(stderr, argtable_list, "\n");
		fprintf(stderr, "\n%s", progname);
//		arg_print_syntax(stderr, argtable_update, "\n");
//		fprintf(stderr, "\n%s", progname);
		arg_print_syntax(stderr, argtable_train, "\n");
		fprintf(stderr, "\n%s", progname);
		arg_print_syntax(stderr, argtable_detect, "\n");
		fprintf(stderr, "\n%s", progname);
		arg_print_syntax(stderr, argtable_version, "\n");
		fprintf(stderr, "\n%s", progname);
		arg_print_syntax(stderr, argtable_help, "\n");
	}

	arg_freetable(argtable_new, sizeof(argtable_new) / sizeof(argtable_new[0]));
	arg_freetable(argtable_list, sizeof(argtable_list) / sizeof(argtable_list[0]));
//	arg_freetable(argtable_update, sizeof(argtable_update) / sizeof(argtable_update[0]));
	arg_freetable(argtable_train, sizeof(argtable_train) / sizeof(argtable_train[0]));
	arg_freetable(argtable_detect, sizeof(argtable_detect) / sizeof(argtable_detect[0]));
	arg_freetable(argtable_version, sizeof(argtable_version) / sizeof(argtable_version[0]));
	arg_freetable(argtable_help, sizeof(argtable_help) / sizeof(argtable_help[0]));

	return ret;
}

void main_new()
{
	ann_t *net = NULL;
	/* initialize the options */
	if(config_new->count)
		options = options_load(config_new->filename[0]);
	else
		options = options_create();

	/* TODO more command line options should be included  */
	options->verbose = (verbose_new->count + 1) * (1 - quiet_new->count);
	strcpy(options->knowledge_file, knowledge_new->filename[0]);

	/* create and save the network */
	net = ann_create();
	ann_save(net);
	ann_destroy(net);
	options_destroy(options);
}

void main_list()
{
	ann_t *net = NULL;
	/* initialize the options */
	options = options_create();

	/* set provided options */
	strcpy(options->knowledge_file, knowledge_list->filename[0]);
	net = ann_load();

	/* list the parameters */
	ann_list(net);
	ann_destroy(net);
}

//int main_update()
//{
//	return 0;
//}
//
void main_train()
{
	FILE *fin = NULL;

	struct aiff_file *sound_file = NULL;
	ann_t *net = NULL;
	book_t *book = NULL;

	double *coefitients = NULL;

	double sampling_rate = 0;

	size_t *windowed_points = NULL;
	double *windowed = NULL;

	size_t *filtered_points = NULL;
	double *filtered = NULL;

	size_t waveform_size = 0;
	double *waveform = NULL;

	size_t sample = 0;
	size_t window = 0;

	size_t num_lessons = 0;
	char section[1024];
	char sound_filename[BUFSIZ];
	char text_answer[1024];
	double answer[128];

	char *p1 = NULL;
	char line_buff[BUFSIZ];
	char title[1024];
	int c_handle = -1;
	char title2[1024];
	int c_handle2 = -1;
	double max = 0;
	double freq = 0;

	/* initialize the options */
	if(config_train->count)
		options = options_load(config_train->filename[0]);
	else
		options = options_create();

	/* TODO more command line options should be included  */
	options->verbose = (verbose_train->count + 1) * (1 - quiet_train->count);
	options->gverbose = (graphicaly_verbose_train->count + 1) * (1 - quiet_train->count);

	strcpy(options->knowledge_file, knowledge_train->filename[0]);
	strcpy(options->book_file, book_train->filename[0]);

	/* do the training */
	F_OPEN(fin, options->book_file, "r");
	section[0] = '\0';
	while(fgets(line_buff, BUFSIZ, fin))
	{
		if ((sscanf (line_buff, "num_lessons = %[^#]", section) == 1))
			break;
	}
	num_lessons = strtoul(section, NULL, 10);
	if(!num_lessons)
	{
		pg_error_set(true, pg_file_format, "file '%s'", options->book_file);
		pg_error_handler();
	}

	printf("num: %d\n", num_lessons);

	net = ann_load();
	book = book_create(num_lessons, options->num_neurons[0], options->num_neurons[options->num_layers - 1]);
	while(num_lessons--)
	{
		/*TODO a dictionary framework should be added for the CFG files */
		/* read from the book */
		section[0] = '\0';
		c_handle = -1;
		while(fgets(line_buff, BUFSIZ, fin))
		{
			if ((sscanf (line_buff, "[%[^]]]", section) == 1))
				break;
		}
		if(!section[0])
		{
			pg_error_set(true, pg_file_format, "file '%s'", options->book_file);
			pg_error_handler();
		}

		print_verbose(v_verbose, "%s\n", section);

		sound_filename[0] = '\0';
		while(fgets(line_buff, BUFSIZ, fin))
		{
			if ((sscanf (line_buff, "file = %[^#\n]", sound_filename) == 1))
				break;
		}
		if(!sound_filename[0])
		{
			pg_error_set(true, pg_file_format, "file '%s'", options->book_file);
			pg_error_handler();
		}

		text_answer[0] = '\0';
		while(fgets(line_buff, BUFSIZ, fin))
		{
			if ((sscanf (line_buff, "answer = %[^#\n]", text_answer) == 1))
				break;
		}
		if(!text_answer[0])
		{
			pg_error_set(true, pg_file_format, "file '%s'", options->book_file);
			pg_error_handler();
		}

		/* load the sound */
		sound_file = load_aiff(sound_filename);

		/* transform the answer into numbers */
		p1 = text_answer;
		for(sample=0; sample < options->num_neurons[options->num_layers - 1]; sample++)
		{
			while(p1 && !isdigit((unsigned char) *p1))
				p1++;

			answer[sample] = strtod(p1, &p1);
		}

		/* FIXME for now only mono 16 bit aiff files will be acceptible */
		sampling_rate = (double) sound_file->cc.sample_rate;
		waveform_size = sound_file->cc.num_sample_frames;
		ALLOC(waveform, waveform_size, sizeof(double));

		sprintf(title, "waveform_%s", section);
		c_handle = -1;
		for(sample=0; sample < waveform_size; sample++)
		{
			waveform[sample] = (double) ((short *) sound_file->sdc.waveform_data)[sample];
			c_handle = print_gverbose(g_verbose2, c_handle, title, sample, waveform[sample]);
		}
		c_handle = print_gverbose(g_verbose2, c_handle, "", 0, 0);

		aiff_destroy(sound_file);

		/* calculate the windows */
		ALLOC(windowed_points, (options->num_windows * 3), sizeof(size_t));
		filterbank(waveform, &windowed, waveform_size, options->num_windows,\
				options->window_type == w_triang ? &wTriang :\
				options->window_type == w_rect ? &wRect :\
				options->window_type == w_hamming ? &wHamming :\
				options->window_type == w_hann ? &wHann :\
				options->window_type == w_gaus ? &wGaus :\
				options->window_type == w_bartlett ? &wBartlett :\
				options->window_type == w_bar_hann ? &wBarHann :\
							&wBlackman,\
				"i", (size_t *) windowed_points);

		ALLOC(filtered_points, (options->num_coefitients * 3), sizeof(size_t));
		ALLOC(coefitients, (options->num_coefitients) * 3, sizeof(double));

		sprintf(title, "spectrum_%s", section);
		c_handle = -1;
		sprintf(title2, "decibel_spectrum_%s", section);
		c_handle2 = -1;
		for(window=0; window < options->num_windows; window++)
		{
			/* turn the window into frequency domain */
			fft((windowed + windowed_points[window * 3]), NULL,\
					(int) (windowed_points[window * 3 + 2] - windowed_points[window * 3] + 1),\
					(windowed + windowed_points[window * 3]), NULL);

			/* compute the pitch */
			for(sample=windowed_points[window * 3]; sample != windowed_points[window * 3 + 2]; sample++)
			{
				freq = ITOF(sample - windowed_points[window * 3],\
						windowed_points[window * 3 + 2]-windowed_points[window * 3], sampling_rate);

				if((freq >= options->min_frequency) && (freq <= options->max_frequency))
					c_handle = print_gverbose(g_verbose2, c_handle, title, freq, windowed[sample]);

				windowed[sample] = log(fabs(windowed[sample]));

				if((freq >= options->min_frequency) && (freq <= options->max_frequency))
					c_handle2 = print_gverbose(g_verbose2, c_handle2, title2, freq, windowed[sample]);
			}

			/* apply filterbank to the pitch */
			/* FIXME due to problem with REALLOC always pass NULL pointer for the output array */
			if(filtered)
				free(filtered);
			filtered=NULL;

			filterbank(windowed + windowed_points[window * 3],\
					&filtered,\
					windowed_points[window * 3 + 2] - windowed_points[window * 3] + 1,\
					options->num_coefitients,\
					&wTriang,\
					"trfFib",\
						options->scale_type,\
						sampling_rate, 
						(double) options->min_frequency,\
						(double) options->max_frequency,\
						(size_t *) filtered_points,\
						0);

			/* turn the filtered pitch into cepstral domain */
			dct(filtered, (int) (filtered_points[(options->num_coefitients-1) * 3 + 2] - filtered_points[0] + 1), filtered);

			/* take the most significant coefitients */
			for(sample=0; sample < options->num_coefitients; sample++)
			{
				coefitients[window * options->num_coefitients + sample] = filtered[sample + options->first_coefitient - 1];
				max = max < fabs(filtered[sample + options->first_coefitient - 1]) ?\
				      fabs(filtered[sample + options->first_coefitient - 1]) : max;
			}
		}
		c_handle = print_gverbose(g_verbose2, c_handle, "", 0, 0);
		c_handle2 = print_gverbose(g_verbose2, c_handle2, "", 0, 0);

		sprintf(title, "coefitients_%s", section);
		c_handle = -1;
		for(sample=0; sample < options->num_coefitients * options->num_windows; sample++)
		{
			coefitients[sample] /= max ? max : 1;
			c_handle = print_gverbose(g_verbose, c_handle, title, sample, coefitients[sample]);
		}
		c_handle = print_gverbose(g_normal, c_handle, "", 0, 0);
		
		book_set_lesson(book, num_lessons + 1, coefitients, answer);

		/* FIXME free / alloc in each iteration should be avoided at least with REALLOC */
		free(windowed_points);
		free(windowed);
		windowed_points = NULL;
		windowed = NULL;

		free(filtered_points);
		free(filtered);
		filtered_points = NULL;
		filtered = NULL;

		free(waveform);
		waveform_size = 0;
		waveform = NULL;
	}

	c_handle = print_gverbose(g_normal, -1, "", 0, 0);

	ann_train(net, book, NULL);

	/* TODO if dry run we should skip saving */
	ann_save(net);

	ann_destroy(net);
	book_destroy(book);
	options_destroy(options);
}

void main_detect()
{
	FILE *fin = NULL;

	struct aiff_file *sound_file = NULL;
	ann_t *net = NULL;

	double *coefitients = NULL;

	double sampling_rate = 0;

	size_t *windowed_points = NULL;
	double *windowed = NULL;

	size_t *filtered_points = NULL;
	double *filtered = NULL;

	size_t waveform_size = 0;
	double *waveform = NULL;

	size_t sample = 0;
	size_t window = 0;

	char pure_name[1024];
	char title[1024];
	int c_handle = -1;
	char title2[1024];
	int c_handle2 = -1;
	double max = 0;
	double freq = 0;
	char *p1=NULL;
	double *answer = NULL; 

	/* initialize the options */
	if(config_detect->count)
		options = options_load(config_detect->filename[0]);
	else
		options = options_create();

	/* TODO more command line options should be included  */
	options->verbose = (verbose_detect->count + 1) * (1 - quiet_detect->count);
	options->gverbose = (graphicaly_verbose_detect->count + 1) * (1 - quiet_detect->count);

	strcpy(options->knowledge_file, knowledge_detect->filename[0]);
	strcpy(options->book_file, book_detect->filename[0]);

	/* do the training */
	net = ann_load();
		
		p1 = &options->book_file[strlen(options->book_file)];
		while((p1 != options->book_file) && (*p1 != '/'))
			p1--;
		if(p1 == options->book_file)
			strcpy(pure_name, p1);
		else
			strcpy(pure_name, p1+1);

		print_verbose(v_verbose, "%s\n", pure_name);
		sound_file = load_aiff(options->book_file);

		/* FIXME for now only mono 16 bit aiff files will be acceptible */
		sampling_rate = (double) sound_file->cc.sample_rate;
		waveform_size = sound_file->cc.num_sample_frames;
		ALLOC(waveform, waveform_size, sizeof(double));

		sprintf(title, "waveform_%s", pure_name);
		c_handle = -1;
		for(sample=0; sample < waveform_size; sample++)
		{
			waveform[sample] = (double) ((short *) sound_file->sdc.waveform_data)[sample];
			c_handle = print_gverbose(g_verbose2, c_handle, title, sample, waveform[sample]);
		}
		c_handle = print_gverbose(g_verbose2, c_handle, "", 0, 0);

		aiff_destroy(sound_file);

		/* calculate the windows */
		ALLOC(windowed_points, (options->num_windows * 3), sizeof(size_t));
		filterbank(waveform, &windowed, waveform_size, options->num_windows,\
				options->window_type == w_triang ? &wTriang :\
				options->window_type == w_rect ? &wRect :\
				options->window_type == w_hamming ? &wHamming :\
				options->window_type == w_hann ? &wHann :\
				options->window_type == w_gaus ? &wGaus :\
				options->window_type == w_bartlett ? &wBartlett :\
				options->window_type == w_bar_hann ? &wBarHann :\
							&wBlackman,\
				"i", (size_t *) windowed_points);

		ALLOC(filtered_points, (options->num_coefitients * 3), sizeof(size_t));
		ALLOC(coefitients, (options->num_coefitients) * 3, sizeof(double));

		sprintf(title, "spectrum_%s", pure_name);
		c_handle = -1;
		sprintf(title2, "decibel_spectrum_%s", pure_name);
		c_handle2 = -1;
		for(window=0; window < options->num_windows; window++)
		{
			/* turn the window into frequency domain */
			fft((windowed + windowed_points[window * 3]), NULL,\
					(int) (windowed_points[window * 3 + 2] - windowed_points[window * 3] + 1),\
					(windowed + windowed_points[window * 3]), NULL);

			/* compute the pitch */
			for(sample=windowed_points[window * 3]; sample != windowed_points[window * 3 + 2]; sample++)
			{
				freq = ITOF(sample - windowed_points[window * 3],\
						windowed_points[window * 3 + 2]-windowed_points[window * 3], sampling_rate);

				if((freq >= options->min_frequency) && (freq <= options->max_frequency))
					c_handle = print_gverbose(g_verbose2, c_handle, title, sample, windowed[sample]);

				windowed[sample] = log(fabs(windowed[sample]));

				if((freq >= options->min_frequency) && (freq <= options->max_frequency))
					c_handle2 = print_gverbose(g_verbose2, c_handle2, title2, sample, windowed[sample]);
			}

			/* apply filterbank to the pitch */
			/* FIXME due to problem with REALLOC always pass NULL pointer for the output array */
			if(filtered)
				free(filtered);
			filtered=NULL;

			filterbank(windowed + windowed_points[window * 3],\
					&filtered,\
					windowed_points[window * 3 + 2] - windowed_points[window * 3] + 1,\
					options->num_coefitients,\
					&wTriang,\
					"trfFib",\
						options->scale_type,\
						sampling_rate, 
						(double) options->min_frequency,\
						(double) options->max_frequency,\
						(size_t *) filtered_points,\
						0);

			/* turn the filtered pitch into cepstral domain */
			dct(filtered, (int) (filtered_points[(options->num_coefitients-1) * 3 + 2] - filtered_points[0] + 1), filtered);

			/* take the most significant coefitients */
			for(sample=0; sample < options->num_coefitients; sample++)
			{
				coefitients[window * options->num_coefitients + sample] = filtered[sample + options->first_coefitient - 1];
				max = max < fabs(filtered[sample + options->first_coefitient - 1]) ?\
				      fabs(filtered[sample + options->first_coefitient - 1]) : max;
			}
		}
		c_handle = print_gverbose(g_verbose2, c_handle, "", 0, 0);
		c_handle2 = print_gverbose(g_verbose2, c_handle2, "", 0, 0);

		sprintf(title, "coefitients_%s", pure_name);
		c_handle = -1;
		for(sample=0; sample < options->num_coefitients * options->num_windows; sample++)
		{
			coefitients[sample] /= max ? max : 1;
			c_handle = print_gverbose(g_verbose, c_handle, title, sample, coefitients[sample]);
		}
		c_handle = print_gverbose(g_normal, c_handle, "", 0, 0);
		
		/* FIXME free / alloc in each iteration should be avoided at least with REALLOC */
		free(windowed_points);
		free(windowed);
		windowed_points = NULL;
		windowed = NULL;

		free(filtered_points);
		free(filtered);
		filtered_points = NULL;
		filtered = NULL;

		free(waveform);
		waveform_size = 0;
		waveform = NULL;

	c_handle = print_gverbose(g_normal, -1, "", 0, 0);

	answer = ann_run(net, coefitients);

	print_verbose(v_normal, "class probabilities:\n");
	for(sample=0; sample < options->num_neurons[options->num_layers-1]; sample++)
		print_verbose(v_normal, "%f ", answer[sample]);
	print_verbose(v_normal, "\n");

	free(answer);
	ann_destroy(net);
	options_destroy(options);
}
