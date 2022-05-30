
#include "mindievt.h"

#include <stddef.h>
#include <stdio.h>

uint32_t mindi_event_time(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset
) {
   uint32_t time = 0;

   /* Read the variable-length timestamp, one byte at a time. */
   do {
      time <<= 7; /* Not the full byte! */
      time |= (midi_bytes[offset] & MINDI_MASK_TIME_BYTE);

      offset++;

      if( offset > midi_bytes_sz ) {
         offset = MINDI_ERROR_OFFSET_OOB;
         goto cleanup;
      }
   } while( 0x80 == (midi_bytes[offset - 1] & 0x80) );

cleanup:
   return time;
}

int32_t mindi_event_time_sz(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset
) {
   int32_t sz = 0;

   /* Skip time bytes until one doesn't start with 0x80. */
   do {
      sz++;
      if( offset + sz > midi_bytes_sz ) {
         sz = MINDI_ERROR_OFFSET_OOB;
         goto cleanup;
      }
   } while( 0x80 == (midi_bytes[(offset + sz) - 1] & 0x80) );

cleanup:
   return sz;
}

int32_t mindi_event_sz(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset
) {
   int32_t sz = 0;

   sz = mindi_event_time_sz( midi_bytes, midi_bytes_sz, offset );

   if( 0 > sz ) {
      /* Pass along error. */
      goto cleanup;
   }

   offset += sz;

   if( 0x80 > midi_bytes[offset] ) {
      /* Invalid MIDI message. */
      sz = MINDI_ERROR_INVALID_EVENT;
      goto cleanup;
   }

   if( 0xD0 == midi_bytes[offset] ) {
      sz += 2; /* Status byte + 1 parm. */
   } else {
      sz += 3; /* Status byte + 2 parms. */
   }

cleanup:
   return sz; 
}

uint8_t mindi_event_type(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset,
   uint8_t* p_param_0, uint8_t* p_param_1
) {
   uint8_t evt_type = 0;
   int32_t time_sz = 0;

   time_sz = mindi_event_time_sz( midi_bytes, midi_bytes_sz, offset );

   if( 0 > time_sz ) {
      /* Give up due to error. */
      goto cleanup;
   }

   if( 0xD0 == midi_bytes[offset] ) {
      if( offset + time_sz + 1 >= midi_bytes_sz ) {
         /* Give up due to out of bounds. */
         goto cleanup;
      }
      evt_type = midi_bytes[offset + time_sz];
      *p_param_0 = midi_bytes[offset + time_sz + 1];

   } else {
      if( offset + time_sz + 2 >= midi_bytes_sz ) {
         /* Give up due to out of bounds. */
         goto cleanup;
      }
      evt_type = midi_bytes[offset + time_sz];
      *p_param_0 = midi_bytes[offset + time_sz + 1];
      *p_param_1 = midi_bytes[offset + time_sz + 2];

   }

cleanup:
   return evt_type;
}

