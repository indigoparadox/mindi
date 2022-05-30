
#include "mindievt.h"

#include <stddef.h>
#include <stdio.h>

uint32_t mindi_event_time(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, int32_t* p_offset
) {
   int32_t offset = -1; /* Counter initial inc in while() below. */
   uint32_t time = 0;

   /* Grab the event offset if given. */
   if( NULL != p_offset ) {
      offset = *p_offset - 1; /* Counter initial inc in while() below. */
   }

   /* Read the variable-length timestamp, one byte at a time. */
   do {
      offset++; /* Inc first, so comparison below works. */

      time <<= 7; /* Not the full byte! */
      time |= (midi_bytes[offset] & MINDI_MASK_TIME_BYTE);

      if( offset + 1 > midi_bytes_sz ) {
         offset = MINDI_ERROR_OFFSET_OOB;
         goto cleanup;
      }
   } while( 0x80 == (midi_bytes[offset] & 0x80) );

   offset++; /* Reflect our actual file position. */

   if( offset > midi_bytes_sz ) {
      offset = MINDI_ERROR_OFFSET_OOB;
      goto cleanup;
   }

cleanup:

   if( NULL != p_offset ) {
      *p_offset = offset;
   }

   return time;

}

