#include <stdio.h>
#include <assert.h>
#include "decoder.h"

int main(int argc, char **argv)
{
	decoder_t *this = get_decoder();
	assert(this->init_decoder(this, argc, argv) == true);

	fprintf(stdout, "Decoder is configured with InFile: %s %p, Outfile: %s %p\n",
		this->get_in_file_name(this), this->get_in_file(this),
		this->get_out_file_name(this), this->get_out_file(this));

	if (this->decode(this))
	{
		this->print_file(this);
	}
	this->destroy(this);
	printf("Hello, World!\n");
	return 0;
}
