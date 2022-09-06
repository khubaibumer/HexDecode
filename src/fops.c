//
// Created by khubaibumer on 9/6/22.
//

#include <string.h>
#include "internal.h"

#define INFILE "INDATA"
#define OUTFILE "OUTDATA"

bool open_files(void *_this)
{
	if (_this == NULL)
		return false;

	if (GET_OPTS(_this)->in_file == NULL)
	{
		GET_OPTS(_this)->in_file = strdup(INFILE);
	}
	if (GET_OPTS(_this)->out_file == NULL)
	{
		GET_OPTS(_this)->out_file = strdup(OUTFILE);
	}
	GET_DATA(_this)->in = fopen(GET_OPTS(_this)->in_file, "r");
	GET_DATA(_this)->out = fopen(GET_OPTS(_this)->out_file, "w");

	return (GET_DATA(_this)->in && GET_DATA(_this)->out);
}

FILE* get_in_file(void* _this)
{
	return GET_DATA(_this)->in;
}

const char* get_in_file_name(void* _this)
{
	return GET_OPTS(_this)->in_file;
}

FILE* get_out_file(void* _this)
{
	return GET_DATA(_this)->out;
}

const char* get_out_file_name(void* _this)
{
	return GET_OPTS(_this)->in_file;
}



