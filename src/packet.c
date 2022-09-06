//
// Created by khubaibumer on 9/6/22.
//

#include <string.h>
#include <stdlib.h>
#include "packet.h"

static int packet_count = 1;

packet_t* read_packet(FILE* file)
{
	// In our input file after every 2 characters there is a space
	// We will try and read 5-bytes per read to accomodate 2char+1space+2char
	// fgets will only read 5 bytes of data to make space for null byte
	char line[64] = { 0 };
	uint8_t packet[8096] = { 0 };
	size_t count = 0;
	bool skip_next = false;
	bool packet_complete = false;
	static int packet_ln_cnt = 0;
	while (fgets(&line[0], sizeof line, file) != NULL)
	{
		if (skip_next == true)
		{
			// If this is set, move to next line
			skip_next = false;
			packet_ln_cnt = 0;
			continue;
		}
		else if (strstr(line, "BLK")) //BLK
		{
			// skip this line and the next line
			skip_next = true;
			continue;
		}
		else if (strstr(line, "COMP")) // COMPLETE
		{
			packet_complete = true;
			break;
		}
		++packet_ln_cnt;

		for (int i = 0; i < 64; i += 3)
		{
			if (line[i] == '\r' || line[i] == '\n')
			{
				break;
			}
			uint8_t byte = strtoul(&line[i], NULL, 16);
			if (count >= sizeof packet)
			{
				fprintf(stderr, "count is %ld line is %s Num Packet: %d "
								"Packet Line Count: %d\n",
								count, line, packet_count, packet_ln_cnt);
				break;
			}
			packet[count++] = byte;
		}
	}
	++packet_count;
	if (packet_complete)
	{
		packet_t* pk = calloc(1, sizeof(packet_t));
		pk->count = count;
		pk->data = calloc(count, sizeof(uint8_t));
		pk->packet_number = packet_count;
		memcpy(pk->data, packet, count);
		return pk;
	}
	return NULL;
}
