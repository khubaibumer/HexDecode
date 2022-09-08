//
// Created by khubaibumer on 9/6/22.
//

#include <string.h>
#include <stdlib.h>
#include "packet.h"

packet_t* read_packet(FILE* file)
{
	/// * In our input file after every 2 characters there is a space <br>
	/// * We will read one line at a time <br>
	/// * If the line has 'BLK' we will skip this and the next line <br>
	/// * If the line has 'COMP' we will mark the packet as complete <br>
	/// <br> Input file is assumed to have the following format: - </br>
	/**
	 * BLK [block_num] <br>
	 * 00000[block_num] <br>
	 * { actual payload } <br>
	 * empty line <br>
	 * COMPLETE <br>
	 * */
	char line[64] = { 0 };
	uint8_t packet[8096] = { 0 };
	size_t count = 0;
	bool skip_next = false;
	bool packet_complete = false;
	size_t packet_count = 0;
	while (fgets(&line[0], sizeof line, file) != NULL)
	{
		if (skip_next == true)
		{
			// If this is set, move to next line
			skip_next = false;
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

		for (int i = 0; i < 64; i += 3)
		{
			if (line[i] == '\r' || line[i] == '\n')
			{
				break;
			}
			uint8_t byte = strtoul(&line[i], NULL, 16);
			if (count >= sizeof packet)
			{
				fprintf(stderr, "count is %ld line is %s\n", count, line);
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
