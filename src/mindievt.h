
#ifndef MINDIEVT_H
#define MINDIEVT_H

#include <mtypes.h>

/**
 * \file mindievt.h
 * \brief Functions, constants, and macros for dealing with MIDI events, either
 *        in files or packets.
 */

/*! \brief Bitmask to determine if a number is multi-byte. */
#define MINDI_MASK_MULTI_BYTE 0x7f

/*! \brief Binary AND this mask with a status byte to get its channel. */
#define MINDI_MASK_STATUS_CHANNEL 0x0f

/**
 * \addtogroup mindi_errors Errors
 * \brief Errors that can occur during the parsing of MIDI data.
 * \{
 */

/*! \brief Return error: offset out of bounds. */
#define MINDI_ERROR_OFFSET_OOB -1

/*! \brief Return error: invalid track chunk found. */
#define MINDI_ERROR_INVALID_TRACK -2

/*! \brief Return error: invalid event data found. */
#define MINDI_ERROR_INVALID_EVENT -3

/*! \brief Return error: track has ended. */
#define MINDI_ERROR_TRACK_END -4

/*! \} */

/**
 * \addtogroup mindi_event_statuses Event Status Bytes
 * \brief Binary AND these with a status byte to get the type of event.
 * \{
 */

#define MINDI_STATUS_NOTE_OFF 0x80

#define MINDI_STATUS_NOTE_ON 0x90

#define MINDI_STATUS_AFTERTOUCH 0xa0

#define MINDI_STATUS_CONTROLLER 0xb0

#define MINDI_STATUS_PROGRAM 0xc0

#define MINDI_STATUS_PRESSURE 0xd0

#define MINDI_STATUS_PITCH 0xe0

#define MINDI_STATUS_SYSEX 0xf0

#define MINDI_STATUS_META 0xff

/*! \} */

/*! \brief Swap the endianness of a 32-bit unsigned integer. */
#define mindi_endian_swap_32( num ) (((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000))

/*! \brief Swap the endianness of a 16-bit unsigned integer. */
#define mindi_endian_swap_16( num ) ((num>>8) | (num<<8))

/**
 * \addtogroup midi_events MIDI Events
 * \brief Information on navigating MIDI events, either one at a time in a
 *        buffer or in the context of a Standard MIDI File.
 * \{
 */

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

/**
 * \brief Parse a MIDI event at the given offset in a byte stream.
 * \param midi_bytes MIDI byte stream (event packet or MIDI file).
 * \param midi_bytes_sz Size of the MIDI byte stream buffer.
 * \param offset Offset of the event in the byte stream (0 for single packet).
 * \param prev MIDI message type of the event before this one.
 * \param params_out Pointer to a buffer to place the parameters of the MIDI
 *        message in.
 * \param params_out Size of the params_out buffer in bytes.
 * \return The MIDI message status byte.
 */
uint8_t mindi_event_type(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset,
   uint8_t prev, uint8_t* params_out, uint8_t params_out_sz );

/*! \} */

#endif /* !MINDIEVT_H */

