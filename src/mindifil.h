
#ifndef MINDIFIL_H
#define MINDIFIL_H

#include <stdint.h>

#include "mindievt.h"

/**
 * \file mindifil.h
 * \brief Functions, constants, and macros for dealing with MIDI files.
 */

/**
 * \addtogroup midi_file_header MIDI File Header
 * \brief Information on navigating the header of a Standard MIDI File.
 * \{
 */

/*! \brief Offset in bytes of the MIDI header size in the MIDI header. */
#define MINDI_HEADER_SZ_OFFSET 4

/*! \brief Offset in bytes of the MIDI format in the MIDI header. */
#define MINDI_HEADER_FMT_OFFSET 8

/*! \brief Offset in bytes of the number of tracks in the MIDI header. */
#define MINDI_HEADER_TRACKS_OFFSET 10

/*! \brief Offset in bytes of the time divisions in the MIDI header. */
#define MINDI_HEADER_TDIV_OFFSET 12

/*! \} */

/**
 * \addtogroup midi_chunk_header MIDI Chunk Header
 * \brief Information on navigating chunks/tracks in a Standard MIDI File.
 * \{
 */

/*! \brief Offset in bytes of the size of the chunk in the chunk header. */
#define MINDI_CHUNK_SZ_OFFSET 4

/*! \brief Size of a chunk header in bytes. */
#define MINDI_CHUNK_HEADER_SZ 8

/*! \} */

/**
 * \addtogroup midi_file_header
 * \{
 */

/**
 * \brief Return the FULL size of the MIDI header in bytes.
 * \param bytes Pointer to a byte buffer containing the MIDI header.
 */
#define mindi_header_sz( bytes ) (MINDI_HEADER_SZ_OFFSET + sizeof( uint32_t ) + (mindi_endian_swap_32( *((uint32_t*)(&bytes[MINDI_HEADER_SZ_OFFSET])))))

/**
 * \brief Return The MIDI format (0, 1, or 2).
 * \param bytes Pointer to a byte buffer containing the MIDI file contents.
 */
#define mindi_header_format( bytes ) ((mindi_endian_swap_16( *((uint16_t*)(&bytes[MINDI_HEADER_FMT_OFFSET])))) & 0xffff)

/**
 * \brief Return the number of tracks in the MIDI file.
 * \param bytes Pointer to a byte buffer containing the MIDI file contents.
 */
#define mindi_header_tracks( bytes ) ((mindi_endian_swap_16( *((uint16_t*)(&bytes[MINDI_HEADER_TRACKS_OFFSET])))) & 0xffff)

#define mindi_header_tdiv( bytes ) (MINDI_HEADER_TDIV_OFFSET + sizeof( uint16_t ) + (mindi_endian_swap_16( *((uint32_t*)(&bytes[MINDI_HEADER_TDIV_OFFSET])))))

/*! \} */

/**
 * \addtogroup midi_chunk_header
 * \{
 */

/**
 * \brief Return the FULL size of the MIDI chunk in bytes.
 * \param bytes Pointer to a byte buffer containing the MIDI file contents.
 * \param offset Offset of the chunk header in bytes.
 */
#define mindi_chunk_sz( bytes, offset ) (MINDI_CHUNK_SZ_OFFSET + sizeof( uint32_t ) + (mindi_endian_swap_32( *((uint32_t*)(&bytes[offset + MINDI_CHUNK_SZ_OFFSET])))))

/**
 * \brief Return the offset of the next chunk, given the offset of an existing
 *        chunk, or the offset of the first chunk of the previous offset is 0.
 * \param midi_bytes MIDI file byte stream.
 * \param midi_bytes_sz Size of the MIDI file byte stream buffer.
 * \param offset Offset of the chunk header in bytes or 0.
 */
int32_t mindi_next_chunk(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset );

/**
 * \brief Return the offset of the next event, given the offset of an existing
 *        event, or the offset of the first event if the given offset is of a
 *        MIDI chunk header.
 * \param midi_bytes MIDI file byte stream.
 * \param midi_bytes_sz Size of the MIDI file byte stream buffer.
 * \param offset Offset of the event in bytes or offset of a chunk header.
 */
int32_t mindi_next_event(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t offset, uint8_t prev );

/*! \} */

#endif /* !MINDIFIL_H */

