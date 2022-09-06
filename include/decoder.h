//
// Created by khubaibumer on 9/6/22.
//

#ifndef HEXTODATA_INCLUDE_DECODER_H
#define HEXTODATA_INCLUDE_DECODER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct decoder
{
	bool (* init_decoder)(void *_this, int argc, char** argv);
	FILE* (* get_in_file)(void* _this);
	const char* (* get_in_file_name)(void* _this);
	FILE* (* get_out_file)(void* _this);
	const char* (* get_out_file_name)(void* _this);
	bool (* decode)(void* _this);
	bool (* print_file)(void* _this);
	void (* destroy)(void* _this);

	bool ready;
	void* data;
} decoder_t;

decoder_t* get_decoder(void);

decoder_t* get_thread_local_decoder(void);

#endif //HEXTODATA_INCLUDE_DECODER_H
