//
// Created by khubaibumer on 9/7/22.
//

#include <stdlib.h>
#include <string.h>
#include "internal.h"
#include "packet.h"

#define FILENAME_ID 0x0002

record_t* decompress_data(flashdata_t* data);

flashdata_t* prepare_payload(packet_t* pkt)
{
	size_t offset = 0;
	uint16_t filename_id = PARSE_VALUE(uint16_t, offset, pkt->data);
	if (filename_id != FILENAME_ID)
	{
		return NULL;
	}
	uint32_t seq_in_file = PARSE_VALUE(uint32_t, offset, pkt->data);
	uint16_t num_records = PARSE_VALUE(uint16_t, offset, pkt->data);
	uint16_t data_len = PARSE_VALUE(uint16_t, offset, pkt->data);
	if (data_len > MAX_DATA_SIZE)
	{
		return NULL;
	}
	flashdata_t* data = calloc(1, sizeof(flashdata_t));
	if (data == NULL)
	{
		return NULL;
	}

	data->filename_id = filename_id;
	data->sequence_in_file = seq_in_file;
	data->number_records = num_records;
	data->data_length = data_len;

	data->flags = PARSE_VALUE(uint16_t, offset, pkt->data);
	data->compression_type = PARSE_VALUE(unsigned char, offset, pkt->data);
	data->spare = PARSE_VALUE(unsigned char, offset, pkt->data);
	memcpy(data->data, &pkt->data[offset], data_len);
	return data;
}

void dump_packet(packet_t* pkt, const char* file)
{
	FILE* ptr = fopen(file, "a+");
	for (int i = 1; i < pkt->count + 1; ++i)
	{
		fprintf(ptr, "%02X ", pkt->data[i - 1]);
		if (i % 16 == 0)
			fprintf(ptr, " \n");
	}
	fprintf(ptr, "\n\n");
	fclose(ptr);
}

bool decode(void* _this)
{
	packet_t* pkt = NULL;
	flashdata_t *payload = NULL;
	record_t *records = NULL;
	size_t packet_count = 0;
	while ((pkt = read_packet(GET_DATA(_this)->in, NULL)) != NULL)
	{
		dump_packet(pkt, "PacketDump");
		payload = prepare_payload(pkt);
		if (!payload)
		{
			fprintf(stderr, "Failure preparing Payload. Packet#%ld\n", pkt->packet_number);
			goto release_mem;
		}
		records = decompress_data(payload);
		packet_count += payload->number_records;
		GET_DECODER(_this)->print_file(records);

	release_mem:
		free_packet(&pkt);
		free_records(&records);
		free_flashdata(&payload);
	}
	printf("total objects: %ld\n", packet_count);
	return true;
}
