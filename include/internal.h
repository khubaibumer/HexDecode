//
// Created by khubaibumer on 9/6/22.
//
#ifndef HEXTODATA_INCLUDE_INTERNAL_H
#define HEXTODATA_INCLUDE_INTERNAL_H

#include <stdio.h>
//#include "types.h"
#include "decoder.h"

struct program_options
{
	char* in_file;
	char* out_file;
};

typedef struct internal_data
{
	struct program_options* ops;
	size_t in_file_offset;
	size_t out_file_offset;
	FILE* in;
	FILE* out;
} internal_data_t;

#define GET_DECODER(x) ((decoder_t*)(x))

#define GET_DATA(x) ((internal_data_t*)((decoder_t*)(x))->data)

#define GET_OPTS(x) ((internal_data_t*)((decoder_t*)(x))->data)->ops

#endif /*HEXTODATA_INCLUDE_INTERNAL_H*/