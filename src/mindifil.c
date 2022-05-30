
#include "mindifil.h"

int32_t mindi_next_chunk(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset
) {

   if( 0 == offset ) {
      /* Return the offset of the first chunk. */
      offset = mindi_header_sz( midi_bytes );
   } else {
      offset += mindi_chunk_sz( midi_bytes, offset );
   }

   /* Ensure track is within bounds. */
   if( offset >= midi_bytes_sz ) {
      offset = MINDI_ERROR_OFFSET_OOB;
      goto cleanup;
   }

   /* Ensure track is valid. */
   if(
      'M' != midi_bytes[offset] ||
      'T' != midi_bytes[offset + 1] ||
      'r' != midi_bytes[offset + 2] ||
      'k' != midi_bytes[offset + 3]
   ) {
      offset = MINDI_ERROR_INVALID_TRACK;
      goto cleanup;
   }

cleanup:

   return offset;
}

int32_t mindi_next_event(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset
) {

   if(
      'M' == midi_bytes[offset] &&
      'T' == midi_bytes[offset + 1] &&
      'r' == midi_bytes[offset + 2] &&
      'k' == midi_bytes[offset + 3]
   ) {
      /* Given offset is track start, so first event comes after the header. */
      offset += MINDI_CHUNK_HEADER_SZ;
   } else {
      /* TODO */
      offset = MINDI_ERROR_INVALID_TRACK;
   }

   /* Ensure event is within bounds. */
   if( offset >= midi_bytes_sz ) {
      offset = MINDI_ERROR_OFFSET_OOB;
      goto cleanup;
   }

cleanup:

   return offset;
}

int32_t mindi_get_event(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t evt_offset,
   uint32_t* p_time, uint32_t* p_evt_data_offset
) {
   
   *p_time = 0;

   /* Read the variable-length timestamp, one byte at a time. */
   do {
      *p_time |= (midi_bytes[evt_offset] & 0x7f);
      evt_offset++;

      if( evt_offset > midi_bytes_sz ) {
         evt_offset = MINDI_ERROR_OFFSET_OOB;
         goto cleanup;
      }
   } while( 0x80 == midi_bytes[evt_offset] );

cleanup:

   return evt_offset;
}

