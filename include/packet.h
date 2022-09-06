//
// Created by khubaibumer on 9/6/22.
//

#ifndef HEXTODATA_INCLUDE_PACKET_H
#define HEXTODATA_INCLUDE_PACKET_H

#include <stdio.h>
#include "types.h"

#define PARSE_VALUE(type, offset, ptr) ({ \
    type tmp = *((type*) ((ptr) + (offset)));     \
    (offset) += sizeof(type);                  \
    (tmp);\
})

packet_t* read_packet(FILE* file);

#endif //HEXTODATA_INCLUDE_PACKET_H
