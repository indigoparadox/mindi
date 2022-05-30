
#ifndef MINDIEVT_H
#define MINDIEVT_H

#include <stdint.h>

#define MINDI_MASK_TIME_BYTE 0x7f

#define MINDI_ERROR_OFFSET_OOB -1

#define MINDI_ERROR_INVALID_TRACK -2

/*! \brief Swap the endianness of a 32-bit unsigned integer. */
#define mindi_endian_swap_32( num ) (((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000))

/*! \brief Swap the endianness of a 16-bit unsigned integer. */
#define mindi_endian_swap_16( num ) ((num>>8) | (num<<8))

uint32_t mindi_event_time(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, int32_t* p_offset );

#endif /* !MINDIEVT_H */

