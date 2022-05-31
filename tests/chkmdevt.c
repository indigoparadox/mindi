
#include "chksetup.h"

#include <stdio.h>

#include "../src/mindievt.h"

#define TEST_EVT_SZ 8

#define TEST_EVT_CT 12

uint8_t test_events[TEST_EVT_CT][TEST_EVT_SZ] = {
   { 0x00, 0xff, 0x58, 0x04, 0x04, 0x02, 0x18, 0x08 /* x */ },
   { 0x00, 0xff, 0x59, 0x02, 0x00, 0x00, /* x */ 0x00, 0x00 },
   { 0x00, 0xff, 0x51, 0x03, 0x05, 0xb8, 0xd8, /* x */ 0x00 },
   /* Mode on ch3 to 0. */
   { 0x00, 0xb3, 0x79, 0x00, /* x */ 0x00, 0x00, 0x00, 0x00 },
   /* Change ch3 to program 48. */
   { 0x00, 0xc3, 0x30, /* x */ 0x00, 0x00, 0x00, 0x00, 0x00 },
   /* Main volume on ch3 to 90, */
   { 0x00, 0xb3, 0x07, 0x5a, /* x */ 0x00, 0x00, 0x00, 0x00 },
   /* Prev status, pan to 54, */
   { 0x00, 0x0a, 0x36, /* x */ 0x00, 0x00, 0x00, 0x00, 0x00 },
   /* Prev status, reverb to 0, */
   { 0x00, 0x5b, 0x00, /* x */ 0x00, 0x00, 0x00, 0x00, 0x00 },
   /* Prev status, chorus to 0. */
   { 0x00, 0x5d, 0x00, /* x */ 0x00, 0x00, 0x00, 0x00, 0x00 },
   { 0x00, 0xff, 0x21, 0x01, 0x00, /* x */ 0x00, 0x00, 0x00 },
   { 0x50, 0x90, 0x26, 0x3C, /* x */ 0x00, 0x00, 0x00, 0x00 },
   { 0x83, 0x60, 0x80, 0x26, 0x3C, /* x */ 0x00, 0x00, 0x00 }
};

uint32_t test_event_times[TEST_EVT_CT] =
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 80, 480 };

uint32_t test_event_time_szs[TEST_EVT_CT] =
   { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 };

uint32_t test_event_szs[TEST_EVT_CT] =
   { 8, 6, 7, 4, 3, 4, 3, 3, 3, 5, 4, 5 };

uint8_t test_event_types[TEST_EVT_CT] =
   { 0xff, 0xff, 0xff, 0xb3, 0xc3, 0xb3, 0xb3, 0xb3, 0xb3, 0xff, 0x90, 0x80 };

uint8_t test_event_param_0s[TEST_EVT_CT] =
   { 0x58, 0x59, 0x51, 0x79, 0x30, 0x07, 0x0a, 0x5b, 0x5d, 0x21, 0x26, 0x26 };

START_TEST( test_midi_event_time ) {
   uint32_t evt_time = 0;
   
   evt_time = mindi_event_multi_byte( test_events[_i], TEST_EVT_SZ, 0 );

   ck_assert_int_eq( evt_time, test_event_times[_i] );
}
END_TEST

START_TEST( test_midi_event_time_sz ) {
   int32_t evt_sz = 0;
   
   evt_sz = mindi_event_multi_byte_sz( test_events[_i], TEST_EVT_SZ, 0 );

   ck_assert_int_eq( evt_sz, test_event_time_szs[_i] );
}
END_TEST

START_TEST( test_midi_event_sz ) {
   int32_t evt_sz = 0;
   uint8_t evt_prev = 0;
   
   if( 0 == _i ) {
      evt_prev = 0;
   } else {
      /* Fake previous event state. */
      evt_prev = test_event_types[_i - 1];
   }
   
   evt_sz = mindi_event_sz(
      test_events[_i], TEST_EVT_SZ, 0, evt_prev );

   ck_assert_int_eq( evt_sz, test_event_szs[_i] );
}
END_TEST

START_TEST( test_midi_event_type ) {
   uint8_t evt_prev = 0,
      evt_type = 0,
      params[10] = { 0 };

   if( 0 == _i ) {
      evt_prev = 0;
   } else {
      /* Fake previous event state. */
      evt_prev = test_event_types[_i - 1];
   }
   
   evt_type = mindi_event_type(
      test_events[_i], TEST_EVT_SZ, 0, evt_prev, params, 10 );

   ck_assert_int_eq( evt_type, test_event_types[_i] );
   ck_assert_int_eq( params[0], test_event_param_0s[_i] );
}
END_TEST

Suite* midi_event_suite( void ) {
   Suite* s;
   TCase* tc_core;

   s = suite_create( "midi_event" );

   /* Core test case */
   tc_core = tcase_create( "Core" );

   tcase_add_loop_test( tc_core, test_midi_event_time, 0, TEST_EVT_CT );
   tcase_add_loop_test( tc_core, test_midi_event_time_sz, 0, TEST_EVT_CT );
   tcase_add_loop_test( tc_core, test_midi_event_sz, 0, TEST_EVT_CT );
   tcase_add_loop_test( tc_core, test_midi_event_type, 0, TEST_EVT_CT );

   suite_add_tcase( s, tc_core );

   return s;
}

