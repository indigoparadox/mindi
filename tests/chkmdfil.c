
#include "chksetup.h"

#include "../src/mindifil.h"
#include "../src/mindievt.h"

#include <stdio.h>

START_TEST( test_midi_file_header ) {
   ck_assert_int_eq( 14, mindi_header_sz( midi_bytes ) );
   ck_assert_int_eq( 1, mindi_header_format( midi_bytes ) );
   ck_assert_int_eq( 9, mindi_header_tracks( midi_bytes ) );
   ck_assert_int_eq( 0, strncmp( "MThd", (char*)midi_bytes, 4 ) );
}
END_TEST

START_TEST( test_midi_file_chunks ) {
   uint16_t tracks = 0;
   int16_t i = 0,
      offset = 0;

   tracks = mindi_header_tracks( midi_bytes );

   do {
      offset = mindi_next_chunk( midi_bytes, midi_bytes_sz, offset );
      i++;
   } while( 0 < offset );

   ck_assert_int_eq( MINDI_ERROR_OFFSET_OOB, offset );
   ck_assert_int_eq( i - 1, tracks );
}
END_TEST

START_TEST( test_midi_file_events ) {
   int32_t track_offset = 0,
      evt_offset = 0,
      track_sz = 0;

   /* Setup the track and initial event offsets. */
   track_offset = mindi_next_chunk( midi_bytes, midi_bytes_sz, 0 );
   track_sz = mindi_chunk_sz( midi_bytes, track_offset );
   evt_offset = track_offset;

   /* Cycle through track events. */
   while( 0 < evt_offset && (evt_offset - track_offset) < track_sz ) {
      evt_offset = mindi_next_event( midi_bytes, midi_bytes_sz, evt_offset );

      printf( "eo: %d to: %d sz: %d\n", evt_offset, track_offset, track_sz );

   }

}
END_TEST

Suite* midi_file_suite( void ) {
   Suite* s;
   TCase* tc_core;

   s = suite_create( "midi_file" );

   /* Core test case */
   tc_core = tcase_create( "Core" );

   tcase_add_checked_fixture( tc_core, setup_midi_file, teardown_midi_file );
   tcase_add_test( tc_core, test_midi_file_header );
   tcase_add_test( tc_core, test_midi_file_chunks );
   tcase_add_test( tc_core, test_midi_file_events );

   suite_add_tcase( s, tc_core );

   return s;
}

