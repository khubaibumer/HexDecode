//
// Created by khubaibumer on 9/6/22.
//
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include "internal.h"

bool parse_program_options(void* _this, int argc, char** argv)
{
	char ch;
	GET_OPTS(_this) = calloc(1, sizeof(struct program_options));
	struct program_options *ops = GET_OPTS(_this);
	if (ops)
	{
		const struct option long_opts[] = {
			{ "in", required_argument, NULL, 'i' },
			{ "out", required_argument, NULL, 'o' },
			{ NULL, 0, NULL, 0 }
		};
		while ((ch = getopt_long(argc, argv, "i:o:", long_opts, NULL)) != -1)
		{
			switch (ch)
			{
			case 'i':
				ops->in_file = (optarg) ? strdup(optarg) : NULL;
				break;
			case 'o':
				ops->out_file = (optarg) ? strdup(optarg) : NULL;
				break;
			default:
				break;
			}
		}
	}
	return ops;
}
