
#include "mindievt.h"

#include <stddef.h>
#include <stdio.h>

uint32_t mindi_event_multi_byte(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset
) {
   uint32_t time = 0;

   /* Read the variable-length timestamp, one byte at a time. */
   do {
      time <<= 7; /* Not the full byte! */
      time |= (midi_bytes[offset] & MINDI_MASK_MULTI_BYTE);

      offset++;

      if( offset > midi_bytes_sz ) {
         offset = MINDI_ERROR_OFFSET_OOB;
         goto cleanup;
      }
   } while( 0x80 == (midi_bytes[offset - 1] & 0x80) );

cleanup:
   return time;
}

int32_t mindi_event_multi_byte_sz(
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
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset, uint8_t prev
) {
   int32_t sz = 0,
      meta_sz = 0;
   uint8_t evt_type = 0;

   sz = mindi_event_multi_byte_sz( midi_bytes, midi_bytes_sz, offset );

   if( 0 > sz ) {
      /* Pass along error. */
      goto cleanup;
   }

   offset += sz;

   /* If the status byte is invalid, maybe the previous isn't? */
   if( 0x80 <= midi_bytes[offset] ) {
      evt_type = midi_bytes[offset];
   } else {
      evt_type = prev;
   }

   if( 0x80 > evt_type ) {
      /* Invalid MIDI message. */
      sz = MINDI_ERROR_INVALID_EVENT;
      goto cleanup;
   }

   /* Event type byte present? */
   if( evt_type == midi_bytes[offset] ) {
      sz++;
   }

   /* Parameters size based on event type. */
   if( 0xff == evt_type ) {
      /* Meta type. */

      meta_sz = 
         mindi_event_multi_byte( midi_bytes, midi_bytes_sz, offset + 2 );
      sz += 2 + meta_sz; /* Meta type, param sz, param. */

   } else if( 0xd0 == (evt_type & 0xd0) || 0xc0 == (evt_type & 0xc0) ) {
      /* Single-param type. */
      sz += 1;

   } else {
      /* Most are double-param types. */
      sz += 2;
   }

cleanup:
   return sz; 
}

uint8_t mindi_event_type(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset,
   uint8_t prev, uint8_t* params_out, uint8_t params_out_sz
) {
   uint8_t evt_type = 0,
      params_sz = 0,
      i = 0,
      evt_byte_ct = 1;
   int32_t time_sz = 0;

   time_sz = mindi_event_multi_byte_sz( midi_bytes, midi_bytes_sz, offset );

   if( 0 > time_sz ) {
      /* Give up due to error. */
      goto cleanup;
   }

   if( 0x80 <= midi_bytes[offset + time_sz] ) {
      evt_type = midi_bytes[offset + time_sz];
   } else {
      /* Missing event type assumed. */
      evt_type = prev;
      evt_byte_ct = 0; /* Omitted. */
   }

   /* Params count based on event type. */
   if( 0xff == evt_type ) {
      /* Meta type. */
      params_sz = mindi_event_multi_byte(
         midi_bytes, midi_bytes_sz, offset + time_sz + evt_byte_ct + 1 );
   } else if( 0xD0 == evt_type ) {
      params_sz = 1;
   } else {
      params_sz = 2;
   }

   if( offset + time_sz + evt_byte_ct + params_sz >= midi_bytes_sz ) {
      /* Give up due to out of bounds. */
      goto cleanup;
   }

   if( params_sz > params_out_sz ) {
      /* Give up due to out of bounds. */
      goto cleanup;
   }

   /* Copy params to output buffer. */
   for( i = 0 ; params_sz > i ; i++ ) {
      params_out[i] = midi_bytes[offset + time_sz + evt_byte_ct + i];
   }

cleanup:
   return evt_type;
}

