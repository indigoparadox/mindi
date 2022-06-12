
#ifndef CHKSETUP_H
#define CHKSETUP_H

#include <mtypes.h>

#include <check.h>

#define TEST_MIDI_FILENAME "test.mid"

void setup_midi_file();
void teardown_midi_file();

#ifdef CHKSETUP_C
uint8_t* midi_bytes = NULL;
uint32_t midi_bytes_sz = 0;
#else
extern uint8_t* midi_bytes;
extern uint32_t midi_bytes_sz;
#endif /* CHKSETUP_C */

#endif /* !CHKSETUP_H */

