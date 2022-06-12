
#include "mindifil.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <mbeep.h>

#ifdef PLATFORM_XLIB
#include <unistd.h>
#endif /* PLATFORM_XLIB */

#ifdef USE_MMAP
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif /* USE_MMAP */

#define MIDI_PARAMS_SZ 10

#define NOTE_MS_DEFAULT 200

/* const float gc_midi_tones[128] = { 8.18, 8.66, 9.18, 9.72, 10.30, 10.91, 11.56, 12.25, 12.98, 13.75, 14.57, 15.43, 16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87, 32.70, 34.65, 36.71, 38.89, 41.20, 43.65, 46.25, 49.00, 51.91, 55.00, 58.27, 61.74, 65.41, 69.30, 73.42, 77.78, 82.41, 87.31, 92.50, 98.00, 103.83, 110.00, 116.54, 123.47, 130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.00, 196.00, 207.65, 220.00, 233.08, 246.94, 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 440.00, 466.16, 493.88, 523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99, 783.99, 830.61, 880.00, 932.33, 987.77, 1046.50, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 1760.00, 1864.66, 1975.53, 2093.00, 2217.46, 2349.32, 2489.02, 2637.02, 2793.83, 2959.96, 3135.96, 3322.44, 3520.00, 3729.31, 3951.07, 4186.01, 4434.92, 4698.64, 4978.03, 5274.04, 5587.65, 5919.91, 6271.93, 6644.88, 7040.00, 7458.62, 7902.13, 8372.02, 8869.84, 9397.27, 9956.06, 10548.08, 11175.30, 11839.82, 12543.85 }; */

const int gc_midi_tones[128] = { 8, 8, 9, 9, 10, 10, 11, 12, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25, 27, 29, 30, 32, 34, 36, 38, 41, 43, 46, 49, 51, 55, 58, 61, 65, 69, 73, 77, 82, 87, 92, 98, 103, 110, 116, 123, 130, 138, 146, 155, 164, 174, 185, 196, 207, 220, 233, 246, 261, 277, 293, 311, 329, 349, 369, 392, 415, 440, 466, 493, 523, 554, 587, 622, 659, 698, 739, 783, 830, 880, 932, 987, 1046, 1108, 1174, 1244, 1318, 1396, 1479, 1567, 1661, 1760, 1864, 1975, 2093, 2217, 2349, 2489, 2637, 2793, 2959, 3135, 3322, 3520, 3729, 3951, 4186, 4434, 4698, 4978, 5274, 5587, 5919, 6271, 6644, 7040, 7458, 7902, 8372, 8869, 9397, 9956, 10548, 11175, 11839, 12543 };

int main( int argc, char** argv ) {
#ifdef USE_MMAP
   int midi_handle = -1;
   struct stat st;
   uint8_t* midi_bytes = MAP_FAILED;
#else
   uint8_t* midi_bytes = NULL,
      invalid_arg = 0;
   FILE* midi_file = NULL;
   int32_t midi_bytes_read = 0;
#endif /* USE_MMAP */
   int track_seek = -1,
      track_iter = 0,
      freq_hz_prev = -1;
   uint8_t midi_params[MIDI_PARAMS_SZ] = { 0 },
#ifdef PLATFORM_DOS
      ch_out = 0,
#endif /* PLATFORM_DOS */
      evt_type = 0;
   int32_t track_offset = 0,
      evt_offset = 0,
      track_sz = 0,
      midi_bytes_sz = 0,
      i = 0;
   uint16_t ticks_per_qrtr = 48;
   uint32_t us_per_qrtr = 500000,
      us_per_tick = 0,
      evt_time = 0,
      event_ms = 0,
      note_ms = NOTE_MS_DEFAULT;
   char* filename = NULL;

   /* Parse command line arguments. */
   for( i = 1 ; argc > i ; i++ ) {
      if( 0 == strncmp( "-n", argv[i], 2 ) && i + 1 < argc ) {
         i++;
         note_ms = atoi( argv[i] );

      } else if( NULL == filename ) {
         filename = argv[i];

      } else if( 0 < atoi( argv[i] ) && 0 > track_seek ) {
         track_seek = atoi( argv[i] );

      } else {
         invalid_arg = 1;
      }
   }

   if( invalid_arg || NULL == filename || -1 == track_seek ) {
      fprintf( stderr, "usage: %s [-n NOTE_MS] <midi file> <track number>\n",
         argv[0] );
      fprintf( stderr, 
         "\nNOTE_MS: number of milliseconds to play each note.\n" );
      fprintf( stderr, "\ntrack selection mandatory; polyphony unsupported\n" );
      goto cleanup;
   }

#ifdef USE_MMAP

   stat( filename, &st );
   midi_bytes_sz = st.st_size;

   midi_handle = open( filename, O_RDONLY, 0 );
   assert( 0 < midi_handle );

   midi_bytes = mmap(
      NULL, midi_bytes_sz, PROT_READ, MAP_PRIVATE | MAP_POPULATE,
      midi_handle, 0 );
   assert( MAP_FAILED != midi_bytes );

#else

   midi_file = fopen( filename, "rb" );
   assert( NULL != midi_file );

   fseek( midi_file, 0, SEEK_END );
   midi_bytes_sz = ftell( midi_file );
   fseek( midi_file, 0, SEEK_SET );

   midi_bytes = calloc( midi_bytes_sz, 1 );
   assert( NULL != midi_bytes );

   midi_bytes_read = fread( midi_bytes, 1, midi_bytes_sz, midi_file );
   printf( "read %d of %d bytes\n", midi_bytes_read, midi_bytes_sz );
   assert( midi_bytes_read == midi_bytes_sz );

#endif /* USE_MMAP */

   ticks_per_qrtr = mindi_header_tdiv( midi_bytes );
   us_per_tick = us_per_qrtr / ticks_per_qrtr;

#ifdef PLATFORM_DOS
   printf( "press C or Q keys to quit playback\n" );
#endif /* PLATFORM_DOS */

   printf( "microseconds per quarter: %d\n", us_per_qrtr );
   printf( "ticks per quarter: %d\n", ticks_per_qrtr );
   printf( "microseconds per tick: %d\n", us_per_tick );

   do {
      /* Setup the track and initial event offsets. */
      track_offset = 
         mindi_next_chunk( midi_bytes, midi_bytes_sz, track_offset );
      track_sz = mindi_chunk_sz( midi_bytes, track_offset );
      evt_offset = track_offset;

      if( track_seek != track_iter ) {
         /* Skip to sought track if there is one. */
         track_iter++;
         continue;
      }

      /* Cycle through track events. */
      while( 0 < evt_offset && (evt_offset - track_offset) < track_sz ) {
         evt_offset = mindi_next_event(
            midi_bytes, midi_bytes_sz, evt_offset, evt_type );
         if( 0 > evt_offset ) {
            if( MINDI_ERROR_TRACK_END == evt_offset ) {
               printf( "track %d finished\n", track_iter );
            }
            break;
         }

#ifdef PLATFORM_DOS
         /* DOS doesn't do CTRL-C too well, so watch for key input. */
         if( kbhit() ) {
            ch_out = getch();
            if( 'c' == ch_out || 'q' == ch_out ) {
               printf( "quitting!\n" );
               break;
            }
         }

#endif /* PLATFORM_DOS */

         /* Get the time until next (this) event. */
         evt_time = mindi_event_multi_byte(
            midi_bytes, midi_bytes_sz, evt_offset );

         if( 0 < freq_hz_prev ) {
            /* TODO: Play the previous event until this one. */
            event_ms = (evt_time * us_per_tick) / 10;
            printf( "beep at %d hz for %d ms\n", freq_hz_prev, note_ms );
            beep( freq_hz_prev, note_ms, 0 );
            if( note_ms < event_ms ) {
               printf( "sleep for %d ms\n", event_ms - note_ms );
#ifdef PLATFORM_XLIB
               usleep( (event_ms - note_ms) * 1000 );
#elif defined( PLATFORM_DOS )
               delay( event_ms - note_ms );
#endif /* PLATFORM_XLIB || PLATFORM_DOS */
            }
         }

         /* Queue up this event. */
         evt_type = mindi_event_type(
            midi_bytes, midi_bytes_sz, evt_offset, evt_type, midi_params,
            MIDI_PARAMS_SZ );

         if( MINDI_STATUS_NOTE_ON == (MINDI_STATUS_NOTE_ON & evt_type) ) {
            /* Play a note. */
            freq_hz_prev = gc_midi_tones[midi_params[0]];
            printf( "eo: %d ticks: %d type: 0x%02x param0: 0x%02x\n",
               evt_offset, evt_time, evt_type, midi_params[0] );

         } else if(
            MINDI_STATUS_META == (MINDI_STATUS_META & evt_type) &&
            0x51 == (0x51 & midi_params[0]) /* Set Tempo */
         ) {
            freq_hz_prev = -1;
            /* Set Tempo */
            us_per_qrtr = mindi_event_multi_byte(
               &(midi_params[1]), MIDI_PARAMS_SZ - 1, 0 );
            us_per_tick = us_per_qrtr / ticks_per_qrtr;
            printf( "set tempo: %d\n", us_per_qrtr );
         } else {
            /* Non-note event. */
            freq_hz_prev = -1;
         }
      }

      track_iter++;
   } while( 0 <= track_offset );

cleanup:

#ifdef USE_MMAP

   if( 0 < midi_handle ) {
      close( midi_handle );
   }

   if( MAP_FAILED != midi_bytes ) {
      munmap( midi_bytes, midi_bytes_sz );
   }

#else

   if( NULL != midi_file ) {
      fclose( midi_file );
   }

   if( NULL != midi_bytes ) {
      free( midi_bytes );
   }

#endif /* USE_MMAP */

   return 0;
}

