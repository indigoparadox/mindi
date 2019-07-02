
#include "mindi.h"

#include <assert.h>

/** \brief  Given an int32 stored as MSB-first, give a platform-appropriate
 *          int32.
 */
int32_t read_int32_msb_first( int32_t* num ) {
   int32_t out = 0;
   uint8_t* bytes = (uint8_t*)num;
   int i;
 
   for( i = 0 ; 3 > i ; i++ ) {
      out <<= 8;
      out |= bytes[i];
   }

   return out;
}

/** \brief  Given a chunk, get the chunk following it in the MIDI stream.
 */
struct midi_chunk* next_chunk( struct midi_chunk* chunk ) {
   int32_t chunk_len = 0;
   uint8_t* byte_ptr = (uint8_t*)chunk;

   chunk_len = read_int32_msb_first( &(chunk->head.len) );

   return (struct midi_chunk*)
      (byte_ptr + sizeof( struct midi_chunk_head ) + chunk_len - 1 );
}

