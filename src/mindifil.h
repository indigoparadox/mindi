
#ifndef MINDI_H
#define MINDI_H

#include <stdint.h>

#define MINDI_HEADER_SZ_OFFSET 4

#define MINDI_HEADER_FMT_OFFSET 8

#define MINDI_HEADER_TRACKS_OFFSET 10

#define MINDI_HEADER_TDIV_OFFSET 12

#define MINDI_CHUNK_HEADER_SZ_OFFSET 4

#define MINDI_CHUNK_HEADER_SZ 8

/*! \brief Swap the endianness of a 32-bit unsigned integer. */
#define mindi_endian_swap_32( num ) (((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000))

/*! \brief Swap the endianness of a 16-bit unsigned integer. */
#define mindi_endian_swap_16( num ) ((num>>8) | (num<<8))

#define mindi_header_sz( bytes ) (mindi_endian_swap_32( *((uint32_t*)(&bytes[MINDI_HEADER_SZ_OFFSET]))))

/* #define mindi_header_format( bytes ) (mindi_endian_swap_16( *((uint16_t*)(&(((uint8_t*)bytes)[MINDI_HEADER_FMT_OFFSET]))))) */

#define mindi_header_format( bytes ) ((mindi_endian_swap_16( *((uint16_t*)(&bytes[MINDI_HEADER_FMT_OFFSET])))) & 0xffff)

#define mindi_header_tracks( bytes ) ((mindi_endian_swap_16( *((uint16_t*)(&bytes[MINDI_HEADER_TRACKS_OFFSET])))) & 0xffff)

uint32_t mindi_next_chunk(
   uint8_t* midi_bytes, uint32_t midi_bytes_sz, uint32_t prev_offset );

#endif /* MINDI_H */

