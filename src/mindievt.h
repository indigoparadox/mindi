
#ifndef MINDIEVT_H
#define MINDIEVT_H

#include <stdint.h>

/**
 * \file mindievt.h
 * \brief Functions, constants, and macros for dealing with MIDI events, either
 *        in files or packets.
 */

/*! \brief Bitmask to determine if a number is multi-byte. */
#define MINDI_MASK_MULTI_BYTE 0x7f

/**
 * \addtogroup mindi_errors Errors
 * \{
 */

/*! \brief Return error: offset out of bounds. */
#define MINDI_ERROR_OFFSET_OOB -1

/*! \brief Return error: invalid track chunk found. */
#define MINDI_ERROR_INVALID_TRACK -2

/*! \brief Return error: invalid event data found. */
#define MINDI_ERROR_INVALID_EVENT -3

/*! \} */

/*! \brief Swap the endianness of a 32-bit unsigned integer. */
#define mindi_endian_swap_32( num ) (((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000))

/*! \brief Swap the endianness of a 16-bit unsigned integer. */
#define mindi_endian_swap_16( num ) ((num>>8) | (num<<8))

/**
 * \brief Return the value of a variable-length multi-byte number
 *        (e.g. timestamp).
 * \param midi_bytes MIDI byte stream (event packet or MIDI file).
 * \param midi_bytes_sz Size of the MIDI byte stream buffer.
 * \param offset Offset of the event in the byte stream (0 for single packet).
 * \return Value of the number or error as appropriate.
 */
uint32_t mindi_event_multi_byte(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset );

/**
 * \brief Return the size of a variable-length multi-byte number in bytes.
 * \param midi_bytes MIDI byte stream (event packet or MIDI file).
 * \param midi_bytes_sz Size of the MIDI byte stream buffer.
 * \param offset Offset of the event in the byte stream (0 for single packet).
 * \return Size of the number in bytes or error as appropriate.
 */
int32_t mindi_event_multi_byte_sz(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset );

/**
 * \brief Return the size of the event at offset.
 * \param midi_bytes MIDI byte stream (event packet or MIDI file).
 * \param midi_bytes_sz Size of the MIDI byte stream buffer.
 * \param offset Offset of the event in the byte stream (0 for single packet).
 * \param prev MIDI message type of the event before this one.
 * \return Size of the MIDI event packet or error as appropriate.
 */
int32_t mindi_event_sz(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset, uint8_t prev );

uint8_t mindi_event_type(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset,
   uint8_t prev, uint8_t* p_param_0, uint8_t* p_param_1 );

#endif /* !MINDIEVT_H */

