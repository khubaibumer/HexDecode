//
// Created by khubaibumer on 9/7/22.
//

#include <stdlib.h>
#include <string.h>
#include "packet.h"

bool process_records(record_t *records, size_t num)
{
	for (int i = 0; i < num; ++i)
	{
		record_t *record = &records[i];
		if (record->valid)
		{

		}
	}
	return true;
}

records_t* decompress_data(flashdata_t* data)
{
	record_t *records = calloc(data->number_records, sizeof(record_t));
	size_t offset = 0;
	for (int i = 0; i < data->number_records; ++i)
	{
		uint8_t len = PARSE_VALUE(uint8_t, offset, data->data);
		record_t* record = &records[i];
		const size_t rec_len = len - 3; // 3-byte header
		record->len = len;
		record->flags = PARSE_VALUE(uint8_t, offset, data->data);
		record->len2 = PARSE_VALUE(uint8_t, offset, data->data);
		if (rec_len != record->len2)
		{
			fprintf(stderr, "LengthBytes - 3Bytes != len2Bytes");
			continue;
		}
		record->data = calloc(rec_len, sizeof(uint8_t));
		memcpy(record->data, &data->data[offset], rec_len);
		record->valid = true;
		offset += rec_len;
	}
	process_records(records, data->number_records);
	records_t *all_recs = calloc(1, sizeof(records_t));
	all_recs->num_records = data->number_records;
	all_recs->record = records;
	return all_recs;
}
