
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
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset, uint8_t prev
) {
   int32_t evt_sz = 0,
      time_sz = 0;

   time_sz = mindi_event_multi_byte_sz(
      &(midi_bytes[offset]), midi_bytes_sz - offset, 0 );
   if( offset + time_sz + 2 > midi_bytes_sz || offset + 4 > midi_bytes_sz ) {
      offset = MINDI_ERROR_OFFSET_OOB;
      goto cleanup;
   }

   if(
      'M' == midi_bytes[offset] &&
      'T' == midi_bytes[offset + 1] &&
      'r' == midi_bytes[offset + 2] &&
      'k' == midi_bytes[offset + 3]
   ) {
      /* Given offset is track start, so first event comes after the header. */
      offset += MINDI_CHUNK_HEADER_SZ;

   } else if(
      0xff == midi_bytes[offset + time_sz] &&
      0x2f == midi_bytes[offset + time_sz + 1]
   ) {
      /* Current event is the end of the track. */
      offset = MINDI_ERROR_TRACK_END;
      goto cleanup;

   } else {
      /* Assume  offset points to an event. */
      evt_sz = mindi_event_sz( midi_bytes, midi_bytes_sz, offset, prev );
      if( 0 > evt_sz ) {
         /* Pass along error. */
         offset = evt_sz;
         goto cleanup;
      }
      offset += evt_sz;
   }

   /* Ensure event is within bounds. */
   if( offset >= midi_bytes_sz ) {
      offset = MINDI_ERROR_OFFSET_OOB;
      goto cleanup;
   }

cleanup:

   return offset;
}

