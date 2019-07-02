
#ifndef MINDI_H
#define MINDI_H

#include <stdint.h>

struct midi_chunk_head {
   char type[5];
   int32_t len;
} __attribute__( (packed) );

struct midi_chunk {
   struct midi_chunk_head head;
   uint8_t data[];
} __attribute__( (packed) );

int32_t read_int32_msb_first( int32_t* num );
struct midi_chunk* next_chunk( struct midi_chunk* chunk );

#endif /* MINDI_H */

