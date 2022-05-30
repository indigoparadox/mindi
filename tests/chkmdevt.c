
#include "chksetup.h"

#include <stdio.h>

#include "../src/mindievt.h"

uint8_t test_event_0[4] = { 0x50, 0x90, 0x26, 0x3C };
uint8_t test_event_1[4] = { 0x83, 0x60, 0x26, 0x00 };

START_TEST( test_midi_event_time_1byte ) {
   uint32_t evt_time = 0;
   int32_t offset = 0;
   
   evt_time = mindi_event_time( test_event_0, 4, &offset );

   ck_assert_int_eq( evt_time, 80 );
   ck_assert_int_eq( offset, 1 );
}
END_TEST

START_TEST( test_midi_event_time_2byte ) {
   uint32_t evt_time = 0;
   int32_t offset = 0;
   
   evt_time = mindi_event_time( test_event_1, 4, &offset );

   ck_assert_int_eq( evt_time, 480 );
   ck_assert_int_eq( offset, 2 );
}
END_TEST

Suite* midi_event_suite( void ) {
   Suite* s;
   TCase* tc_core;

   s = suite_create( "midi_event" );

   /* Core test case */
   tc_core = tcase_create( "Core" );

   tcase_add_test( tc_core, test_midi_event_time_1byte );
   tcase_add_test( tc_core, test_midi_event_time_2byte );

   suite_add_tcase( s, tc_core );

   return s;
}

