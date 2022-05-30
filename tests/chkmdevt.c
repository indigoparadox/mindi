
#include "chksetup.h"

#include <stdio.h>

#include "../src/mindievt.h"

#define TEST_EVT_SZ 6

uint8_t test_events[2][TEST_EVT_SZ] = {
   { 0x50, 0x90, 0x26, 0x3C, 0x00, 0x00 },
   { 0x83, 0x60, 0x80, 0x26, 0x3C, 0x00 }
};

uint32_t test_event_times[2] = { 80, 480 };

uint32_t test_event_time_szs[2] = { 1, 2 };

uint32_t test_event_szs[2] = { 4, 5 };

uint8_t test_event_types[2] = { 0x90, 0x80 };

START_TEST( test_midi_event_time ) {
   uint32_t evt_time = 0;
   
   evt_time = mindi_event_time( test_events[_i], TEST_EVT_SZ, 0 );

   ck_assert_int_eq( evt_time, test_event_times[_i] );
}
END_TEST

START_TEST( test_midi_event_time_sz ) {
   int32_t evt_sz = 0;
   
   evt_sz = mindi_event_time_sz( test_events[_i], TEST_EVT_SZ, 0 );

   ck_assert_int_eq( evt_sz, test_event_time_szs[_i] );
}
END_TEST

START_TEST( test_midi_event_sz ) {
   int32_t evt_sz = 0;
   
   evt_sz = mindi_event_sz( test_events[_i], TEST_EVT_SZ, 0 );

   ck_assert_int_eq( evt_sz, test_event_szs[_i] );
}
END_TEST

START_TEST( test_midi_event_type ) {
   uint8_t evt_type = 0;
   uint8_t param_0;
   uint8_t param_1;
   
   evt_type = mindi_event_type(
      test_events[_i], TEST_EVT_SZ, 0, &param_0, &param_1 );

   ck_assert_int_eq( evt_type, test_event_types[_i] );
}
END_TEST

Suite* midi_event_suite( void ) {
   Suite* s;
   TCase* tc_core;

   s = suite_create( "midi_event" );

   /* Core test case */
   tc_core = tcase_create( "Core" );

   tcase_add_loop_test( tc_core, test_midi_event_time, 0, 2 );
   tcase_add_loop_test( tc_core, test_midi_event_time_sz, 0, 2 );
   tcase_add_loop_test( tc_core, test_midi_event_sz, 0, 2 );
   tcase_add_loop_test( tc_core, test_midi_event_type, 0, 2 );

   suite_add_tcase( s, tc_core );

   return s;
}

