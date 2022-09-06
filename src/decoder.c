//
// Created by khubaibumer on 9/6/22.
//

#include <stdlib.h>
#include <threads.h>
#include "decoder.h"
#include "internal.h"

/// Forward Declerations
bool init_decoder(void* _this, int argc, char** argv);
FILE* get_in_file(void* _this);
const char* get_in_file_name(void* _this);
FILE* get_out_file(void* _this);
const char* get_out_file_name(void* _this);
bool decode(void* _this);

bool print_file(void* _this)
{
};
void destroy(void* _this);

bool parse_program_options(void* _this, int argc, char** argv);
bool open_files(void* _this);

decoder_t* get_decoder(void)
{
	decoder_t* decoder = calloc(1, sizeof(decoder_t));
	decoder->init_decoder = &init_decoder;
	decoder->get_in_file = &get_in_file;
	decoder->get_in_file_name = &get_in_file_name;
	decoder->get_out_file = &get_out_file;
	decoder->get_out_file_name = &get_out_file_name;
	decoder->decode = &decode;
	decoder->print_file = &print_file;
	decoder->destroy = &destroy;
	decoder->ready = true;

	return decoder;
}

decoder_t* get_thread_local_decoder(void)
{
	static thread_local decoder_t decoder = {
		.init_decoder = &init_decoder,
		.get_in_file = &get_in_file,
		.get_in_file_name = &get_in_file_name,
		.get_out_file = &get_out_file,
		.get_out_file_name = &get_out_file_name,
		.decode = &decode,
		.print_file = &print_file,
		.destroy = &destroy,
		.ready = true
	};
	return &decoder;
}

bool init_decoder(void* _this, int argc, char** argv)
{
	if (GET_DECODER(_this)->data != NULL && GET_DECODER(_this)->ready)
	{
		fprintf(stderr, "Duplicate initialization!!\n");
		return false;
	}
	GET_DECODER(_this)->data = calloc(1, sizeof(internal_data_t));
	return parse_program_options(_this, argc, argv) && open_files(_this);
}

void destroy(void* _this)
{
	if (_this)
	{
		if (GET_DATA(_this))
		{
			if (GET_OPTS(_this))
			{
				if (GET_OPTS(_this)->in_file)
				{
					free(GET_OPTS(_this)->in_file);
				}
				if (GET_OPTS(_this)->out_file)
				{
					free(GET_OPTS(_this)->out_file);
				}
				free(GET_OPTS(_this));
			}
			if (GET_DATA(_this)->in)
			{
				fflush(GET_DATA(_this)->in);
				fclose(GET_DATA(_this)->in);
			}
			if (GET_DATA(_this)->out)
			{
				fflush(GET_DATA(_this)->out);
				fclose(GET_DATA(_this)->out);
			}
			free(GET_DATA(_this));
		}
		free(_this);
	}
}
