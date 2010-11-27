#include "common.h"

inline int print_gverbose(gverbose_t level, int handle_index, const char *identifier, const double x, const double y)
{
#define MAX_PLOT_HANDLERS	5

	static int  handle_array[MAX_PLOT_HANDLERS];
	static char handle_identifier[MAX_PLOT_HANDLERS][MAX_PATH_LEN + 1];
	static char template[MAX_PATH_LEN + 1] = {'\0'};
	char coord[1024];
	size_t n = 0;

	static bool first_time = true;

	if(first_time)
	{
		first_time = false;
		for(n=0; n < MAX_PLOT_HANDLERS; n++)
		{
			handle_array[n] = -1;
			handle_identifier[n][0] = '\0';
		}

		n = 0;
	}

	if(options->gverbose < level)
		return handle_index;

	/* if handle not initialized or identifier has changed */
	if((handle_index == -1) || (handle_array[handle_index] == -1) || (strcmp(handle_identifier[handle_index], identifier)) != 0)
	{
		/* if handle provided */
		if(handle_index != -1)
		{
			/* close the handle */
			if((handle_array[handle_index] != -1) && close(handle_array[handle_index]))
			{
				pg_error_set(false, pg_fclose, "%s", handle_identifier[handle_index]);
				pg_error_handler();
			}

			handle_identifier[handle_index][0] = '\0';
			handle_array[handle_index] = -1;
		}
		else if(!identifier[0])
		{
			/* if handle is -1 and identifier is "" close all handles */
			for(n=0; n < MAX_PLOT_HANDLERS; n++)
			{
				if((handle_array[handle_index] != -1) && close(handle_array[handle_index]))
				{
					pg_error_set(false, pg_fclose, "%s", handle_identifier[handle_index]);
					pg_error_handler();
				}

				handle_identifier[handle_index][0] = '\0';
				handle_array[handle_index] = -1;
			}
		}
		
		/* if identifier is provided */
		if(identifier[0])
		{
			sprintf(template, "%s/%s_XXXXXX", options->gpath, identifier);
	
			/* find next free handle position */
			for(n=0; (n < MAX_PLOT_HANDLERS) && (handle_array[n] != -1); n++)
				;

			handle_index = (n < MAX_PLOT_HANDLERS) ? (int) n : -1;
			if((handle_index == -1) || ((handle_array[handle_index] = mkstemp(template)) == -1))
			{
				pg_error_set(false, pg_fopen, "%s", template);
				pg_error_handler();
				handle_index = -1;
			}
			else
				strcpy(handle_identifier[handle_index], identifier);
		}
	}

	if((handle_index != -1) && (handle_array[handle_index] != -1))
	{
		sprintf(coord, "%f\t%f\n", x, y);
	
		if((n = write(handle_array[handle_index], coord, strlen(coord) * sizeof(char))) != (strlen(coord) * sizeof(char)))
		{
			pg_error_set(false, pg_fwrite, "%d / %d bytes written", n, strlen(coord) * sizeof(char));
			pg_error_handler();
		}
	}

	return handle_index;
}

inline void print_verbose(verbose_t level, const char *fmt, ...)
{
	if(options->verbose < level)
		return;

	va_list ap;
	va_start(ap, fmt);

	vfprintf(stdout, fmt, ap);

	va_end(ap);
}

inline void print_error(verbose_t level, const char *fmt, ...)
{
	if(options->verbose < level)
		return;

	va_list ap;
	va_start(ap, fmt);

	vfprintf(stderr, fmt, ap);

	va_end(ap);
}

inline void pg_error_set(bool critical, pg_error_code_t code, char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	pg_global_error->critical = critical;
	pg_global_error->code = code;
	vsnprintf(pg_global_error->description, (size_t) MAX_PG_ERROR_DESC + 1, fmt, ap);

	va_end(ap);
}

inline void pg_error_def_handler(void)
{
	switch(pg_global_error->code)
	{
		case pg_ok:
			return;
			break;
		case pg_alloc:
			print_error((pg_global_error->critical ? v_normal : v_verbose), "%s: %s [%s]\n",\
				(pg_global_error->critical ? "error" : "warning"),\
				"could not allocate memory",\
				pg_global_error->description);
			break;
		case pg_file_format:
			print_error((pg_global_error->critical ? v_normal : v_verbose), "%s: %s [%s]\n",\
				(pg_global_error->critical ? "error" : "warning"),\
				"file format problem",\
				pg_global_error->description);
			break;
		default:
		case pg_unknown:
			print_error((pg_global_error->critical ? v_normal : v_verbose), "%s: %s [%s]\n",\
				(pg_global_error->critical ? "error" : "warning"),\
				"unknown problem",\
				pg_global_error->description);
			break;
	}

	if(pg_global_error->critical)
		exit(0);
	else
		pg_error_set(false, pg_ok, "");
}

options_t *options_create(void)
{
	options_t *new_opt = NULL;

	ALLOC(new_opt, 1L, sizeof(options_t));

	new_opt->verbose = v_normal;
	new_opt->gverbose = g_quiet;
	strcpy(new_opt->gpath, "./grafici/");	
	strcpy(new_opt->knowledge_file, "");	
	new_opt->connection_rate = 1.0;
	new_opt->learning_rate = 0.7;
	new_opt->max_error = 0.001;
	new_opt->max_epochs = 10000;
	new_opt->epochs_between_reports = 500;

	new_opt->num_layers = 3;

	ALLOC(new_opt->num_neurons, new_opt->num_layers, sizeof(size_t));

	new_opt->num_neurons[0] = 1;
	new_opt->num_neurons[1] = 1;
	new_opt->num_neurons[2] = 1;

	return new_opt;
}

options_t *options_load(const char *config_filename)
{
	/*FIXME this function should be fixed with a dictionary loader */
	FILE *fin = NULL;
	char key[128];
	char value[128];
	char buff[BUFSIZ];

	/* create the options with the deffault values */
	options_t *ret = options_create();

	/* set the vlues specified in the configuration file */
	F_OPEN(fin, config_filename, "r");

	key[0] = '\0';
	value[0] = '\0';
	while(fgets(buff, BUFSIZ, fin))
	{
		if((sscanf (buff, "%[^=] = \"%[^\"]\"", key, value) != 2) &&\
	       		(sscanf (buff, "%[^=] = '%[^\']'", key, value) != 2) &&\
	       		(sscanf (buff, "%[^=] = %[^#]", key, value) != 2))
			continue;

		TRIM(key);
		TRIM(value);

		if(!strcmp(key, "num_windows"))
		{
			ret->num_windows = strtoul(value, NULL, 10);
		}
		else if(!strcmp(key, "window_overlap"))
		{
			ret->window_overlap = strtod(value, NULL);
			ret->window_overlap = ret->window_overlap < 0 ? 0 :\
						  ret->window_overlap > 1 ? 1 :\
						  ret->window_overlap;

		}
		else if(!strcmp(key, "window_type"))
		{
			ret->window_type = !strcmp(value, "rect") ? w_rect:\
						!strcmp(value, "gaus") ? w_gaus:\
						!strcmp(value, "hamming") ? w_hamming:\
						!strcmp(value, "hann") ? w_hann:\
						!strcmp(value, "bartlett") ? w_bartlett:\
						!strcmp(value, "triang") ? w_triang:\
						!strcmp(value, "bar_hann") ? w_bar_hann:\
						!strcmp(value, "blackman") ? w_blackman:\
						ret->window_type;
			
		}
		else if(!strcmp(key, "num_coefitients"))
		{
			ret->num_coefitients = strtoul(value, NULL, 10);
		}
		else if(!strcmp(key, "first_coefitient"))
		{
			ret->first_coefitient = strtoul(value, NULL, 10);
		}
		else if(!strcmp(key, "scale_type"))
		{
			ret->scale_type = !strcmp(value, "normal") ? s_normal:\
					      !strcmp(value, "mel") ? s_mel:\
					      ret->scale_type;

		}
		else if(!strcmp(key, "min_frequency"))
		{
			ret->min_frequency = strtod(value, NULL);
		}
		else if(!strcmp(key, "max_frequency"))
		{
			ret->max_frequency = strtod(value, NULL);
		}
		else if(!strcmp(key, "num_layers"))
		{
			ret->num_layers = strtoul(value, NULL, 10);
			if(ret->num_layers < 2)
			{
				pg_error_set(true, pg_num_layers, "%d layers requested", ret->num_layers);
				pg_error_handler();
			}

			REALLOC(ret->num_neurons, sizeof(size_t) * ret->num_layers);
			/* FIXME initialization needed */

		}
		else if(!strcmp(key, "num_neurons_in"))
		{
			ret->num_neurons[0] = strtoul(value, NULL, 10);
		}
		else if(!strncmp(key, "num_neurons_hidden_", 19L))
		{
			size_t i = 0;
			for(i=1; i < ret->num_layers - 1; i++)
				ret->num_neurons[i] = (i == strtoul(key + 19, NULL, 10)) ? strtoul(value, NULL, 10):\
							  ret->num_neurons[i];
		}
		else if(!strcmp(key, "num_neurons_out"))
		{
			ret->num_neurons[ret->num_layers - 1] = strtoul(value, NULL, 10);
		}
		else if(!strcmp(key, "connection_rate"))
		{
			ret->connection_rate = strtod(value, NULL);
		}
		else if(!strcmp(key, "learning_rate"))
		{
			ret->learning_rate = strtod(value, NULL);
		}
		else if(!strcmp(key, "max_error"))
		{
			ret->max_error = strtod(value, NULL);
		}
		else if(!strcmp(key, "max_epochs"))
		{
			ret->max_epochs = strtoul(value, NULL, 10);
		}
		else if(!strcmp(key, "epochs_between_reports"))
		{
			ret->epochs_between_reports = strtoul(value, NULL, 10);
		}

		key[0] = '\0';
		value[0] = '\0';
	}

	return ret;
}

void options_destroy(options_t *old_opt)
{
	if(old_opt)
	{
		if(old_opt->num_neurons)
			free(old_opt->num_neurons);
		free(old_opt);
	}
}
