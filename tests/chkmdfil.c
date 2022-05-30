
#include "chksetup.h"

#include "../src/mindifil.h"

#include <stdio.h>

START_TEST( test_midi_file_header ) {
   ck_assert_int_eq( 6, mindi_header_sz( midi_bytes ) );
   ck_assert_int_eq( 1, mindi_header_format( midi_bytes ) );
   ck_assert_int_eq( 9, mindi_header_tracks( midi_bytes ) );
   ck_assert_int_eq( 0, strncmp( "MThd", (char*)midi_bytes, 4 ) );
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

   suite_add_tcase( s, tc_core );

   return s;
}

