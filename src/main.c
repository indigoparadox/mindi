
#include "mindifil.h"

#include <stdio.h>
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

int main( int argc, char** argv ) {
   int midi_handle = 0,
      track_seek = -1,
      track_iter = 0;
   struct stat st;
   uint8_t* midi_map = NULL,
      midi_params[10] = { 0 },
      evt_type = 0;
   int32_t track_offset = 0,
      evt_offset = 0,
      track_sz = 0,
      midi_bytes_sz = 0;

   if( 2 > argc ) {
      fprintf( stderr, "usage: %s <midi file> [track number]\n", argv[0] );
      goto cleanup;
   }

   if( 3 == argc ) {
      track_seek = atoi( argv[2] );
   }

   stat( argv[1], &st );
   midi_bytes_sz = st.st_size;

   midi_handle = open( argv[1], O_RDONLY, 0 );
   assert( 0 < midi_handle );

   midi_map = mmap( NULL, midi_bytes_sz, PROT_READ, MAP_PRIVATE | MAP_POPULATE,
      midi_handle, 0 );
   assert( MAP_FAILED != midi_map );

   do {
      /* Setup the track and initial event offsets. */
      track_offset = mindi_next_chunk( midi_map, midi_bytes_sz, track_offset );
      track_sz = mindi_chunk_sz( midi_map, track_offset );
      evt_offset = track_offset;

      if( 0 <= track_seek && track_seek != track_iter ) {
         /* Skip to sought track if there is one. */
         track_iter++;
         continue;
      }

      /* Cycle through track events. */
      while( 0 < evt_offset && (evt_offset - track_offset) < track_sz ) {
         evt_offset = mindi_next_event(
            midi_map, midi_bytes_sz, evt_offset, evt_type );
         if( 0 > evt_offset ) {
            if( MINDI_ERROR_TRACK_END == evt_offset ) {
               printf( "track %d finished\n", track_iter );
            }
            break;
         }
         evt_type = mindi_event_type(
            midi_map, midi_bytes_sz, evt_offset, evt_type, midi_params, 10 );

         printf( "eo: %d type: 0x%02x\n", evt_offset, evt_type );
      }

      track_iter++;
   } while( 0 <= track_offset );

cleanup:

   if( 0 < midi_handle ) {
      close( midi_handle );
   }

   if( NULL != midi_map ) {
      munmap( midi_map, midi_bytes_sz );
   }

   return 0;
}

