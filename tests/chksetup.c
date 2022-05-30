
#define CHKSETUP_C
#include "chksetup.h"

#include <stdio.h>
#include <stdlib.h>

void setup_midi_file() {
   FILE* midi_file = NULL;
   
   midi_file = fopen( TEST_MIDI_FILENAME, "rb" );
   ck_assert_ptr_ne( midi_file, NULL );

   /* Get MIDI file size. */
   fseek( midi_file, 0, SEEK_END );
   midi_bytes_sz = ftell( midi_file );
   fseek( midi_file, 0, SEEK_SET );

   midi_bytes = calloc( midi_bytes_sz + 1, 1 );
   fread( midi_bytes, midi_bytes_sz, 1, midi_file );

   fclose( midi_file );
}

void teardown_midi_file() {
   free( midi_bytes );
}

