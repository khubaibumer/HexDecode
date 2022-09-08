//
// Created by khubaibumer on 9/7/22.
//

#include <stdlib.h>
#include <string.h>
#include "packet.h"
#include "internal.h"

#define g_literal_character 0xff
#define g_speical_character_lo 0xfe
#define g_speical_character_hi 0xef
#define g_padding 0xfd
#define g_space 0xfc

typedef struct broken_byte
{
	uint8_t hi: 4;
	uint8_t lo: 4;
} broken_byte_t;

broken_byte_t break_byte(uint8_t in_byte)
{
	broken_byte_t byte = {
		.hi = (in_byte & 0xf0) >> 4,
		.lo = in_byte & 0x0f
	};
	return byte;
}

bool is_special_character(uint8_t nibble)
{
	switch (nibble)
	{
	case 0xa:
	case 0xb:
	case 0xc:
	case 0xd:
	case 0xe:
		return true;
	default:
		return false;
	}
}

uint8_t translate_nibble(uint8_t nibble)
{
	switch (nibble)
	{
	case 0 ... 9:
		return nibble;
	case 0xa:
		return ' ';
	case 0xb:
		return ':';
	case 0xc:
		return '/';
	case 0xd:
		return '\r';
	case 0xe:
		return '\n';
	default:
		return 0xf;
	}
}

uint8_t decompress_nibble(uint8_t in)
{
	broken_byte_t byte = break_byte(in);
	if (byte.hi == 0xa)
	{
		return g_space;
	}
	else if (byte.hi == 0xf)
	{
		return g_padding;
	}
	else if (byte.lo == 0xf)
	{
		return g_literal_character;
	}
	else if (is_special_character(byte.hi))
	{
		return g_speical_character_hi;
	}
	else if (is_special_character(byte.lo))
	{
		return g_speical_character_lo;
	}
	return (byte.lo << 4) | byte.hi;
}

char* decompress_spaces(uint8_t level2)
{
	uint8_t lo = 0x0F & level2;
	uint8_t hi = (level2 >> 4);
	if (hi == 0xa)
	{
		char* str = calloc(3, sizeof(char));
		sprintf(&str[0], "%01x", lo);
		str[1] = ' ';
		return str;
	}
	else if (level2 > 0x80)
	{
		uint8_t spaces = level2 - 0x80;
		char* str = calloc(spaces + 1, sizeof(char));
		for (int i = 0; i < spaces; ++i)
		{
			str[i] = ' ';
		}
		return str;
	}
	return 0x00;
}

void test_decoder()
{
	uint8_t testArr[] = { 0x10, 0x32, 0x54, 0xa6, 0x4f, 0xd3, 0xfe };
	bool skip_lo = false;
	uint8_t in_byte;
	for (int i = 0; i < sizeof testArr; ++i)
	{
		if (skip_lo)
		{
			in_byte = (testArr[i] & 0xf0) >> 4;
			skip_lo = false;
		}
		else
		{
			in_byte = testArr[i];
		}
		uint8_t byte = decompress_nibble(in_byte);
		switch (byte)
		{
		case g_literal_character:
		{
			uint8_t _1 = (testArr[i] & 0xf0);
			uint8_t _2 = (testArr[i + 1] & 0x0f);
			uint8_t ch = (_1 | _2);
			printf("%c", ch);
			skip_lo = true;
			break;
		}
		case g_speical_character_hi:
		{
			uint8_t _1 = translate_nibble(break_byte(in_byte).hi);
			printf("%c", _1);
			break;
		}
		case g_speical_character_lo:
		{
			uint8_t _1 = translate_nibble(break_byte(in_byte).lo);
			printf("%c", _1);
			break;
		}
		case g_space:
		{
			char* str = decompress_spaces(in_byte);
			printf("%s", str ? str : "");
			str ? free(str) : 0;
			break;
		}
		case g_padding:
		{
			printf("%c", translate_nibble(break_byte(in_byte).lo));
			break;
		}
		default:
		{
			if (in_byte < 128)
			{
				printf("%02x", byte);
			}
			else
			{
				printf("ERROR: %d", byte);
			}
			break;
		}
		}
	}
	printf("\n");
	fflush(stdout);
}

bool process_records(void* _this, record_t* records, size_t num)
{
	test_decoder(); // Test decoding logic with given sample

	bool skip_lo = false;
	uint8_t in_byte;
	for (int i = 0; i < num; ++i)
	{
		if (!records[i].valid) continue;
		printf("Record# %d : - ", i);
		record_t* record = &records[i];
		if (record->valid)
		{
			for (int j = 0; j < record->len2; ++j)
			{
				if (skip_lo)
				{
					in_byte = (record->data[j] & 0xf0) >> 4;
					skip_lo = false;
				}
				else
				{
					in_byte = record->data[j];
				}
				uint8_t byte = decompress_nibble(in_byte);
				switch (byte)
				{
				case g_literal_character:
				{
					uint8_t _1 = (record->data[j] & 0xf0);
					uint8_t _2 = (record->data[j + 1] & 0x0f);
					uint8_t ch = (_1 | _2);
					fprintf(GET_DECODER(_this)->get_out_file(_this), "%c", ch);
					skip_lo = true;
					break;
				}
				case g_speical_character_hi:
				{
					uint8_t _1 = translate_nibble(break_byte(in_byte).hi);
					fprintf(GET_DECODER(_this)->get_out_file(_this), "%c", _1);
					break;
				}
				case g_speical_character_lo:
				{
					uint8_t _1 = translate_nibble(break_byte(in_byte).lo);
					fprintf(GET_DECODER(_this)->get_out_file(_this), "%c", _1);
					break;
				}
				case g_space:
				{
					char* str = decompress_spaces(in_byte);
					fprintf(GET_DECODER(_this)->get_out_file(_this), "%s", str ? str : "");
					str ? free(str) : 0;
					break;
				}
				case g_padding:
				{
					fprintf(GET_DECODER(_this)->get_out_file(_this), "%c", translate_nibble(break_byte(in_byte).lo));
					break;
				}
				default:
				{
					if (in_byte < 128)
					{
						fprintf(GET_DECODER(_this)->get_out_file(_this), "%02x", byte);
					}
					else
					{
						// shouldn't come here
					}
					break;
				}
				}
			}
			fprintf(GET_DECODER(_this)->get_out_file(_this), "\n");
			fflush(stdout);
		}
	}
	return true;
}

records_t* decompress_data(void* _this, flashdata_t* data)
{
	record_t* records = calloc(data->number_records, sizeof(record_t));
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
			fprintf(stderr, "LengthBytes => (length - 3) != (len2)");
			continue;
		}
		record->data = calloc(rec_len, sizeof(uint8_t));
		memcpy(record->data, &data->data[offset], rec_len);
		record->valid = true;
		offset += rec_len;
	}
	process_records(_this, records, data->number_records);
	records_t* all_recs = calloc(1, sizeof(records_t));
	all_recs->num_records = data->number_records;
	all_recs->record = records;
	return all_recs;
}
