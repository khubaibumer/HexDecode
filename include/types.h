#include <stdbool.h>
#include <stdint.h>

#define MAX_DATA_SIZE (2048 - 14)
typedef struct flashdata
{
	uint16_t filename_id;                     // number for file
	uint32_t sequence_in_file;                // file sequence number
	uint16_t number_records;                  // recs in block
	uint16_t data_length;                     // data length
	uint16_t flags;                           // unused presently
	unsigned char compression_type;         // unused presently
	unsigned char spare;                    // unused presently
	unsigned char data[MAX_DATA_SIZE];            // the data itself
} flashdata_t;

static void free_flashdata(flashdata_t** data)
{
	if (*data)
	{
		free(*data);
	}
	*data = NULL;
}

typedef struct packet
{
	uint8_t* data;
	size_t count;
	size_t packet_number;
} packet_t;

static void free_packet(packet_t** pkt)
{
	if (*pkt)
	{
		if ((*pkt)->data)
		{
			free((*pkt)->data);
		}
		free(*pkt);
	}
	*pkt = NULL;
}

typedef struct record
{
	bool valid;
	uint8_t len;
	uint8_t flags;
	uint8_t len2;
	uint8_t* data;
} record_t;

typedef struct records {
	size_t num_records;
	record_t *record;
} records_t;

static void free_records(records_t** records)
{
	if (*records)
	{
		if ((*records)->num_records > 0)
		{
			for (int i = 0; i < (*records)->num_records; ++i)
			{
				if ((*records)->record[i].data)
				{
					free((*records)->record[i].data);
				}
			}
			free((*records)->record);
		}
		free(*records);
	}
	*records = NULL;
}

