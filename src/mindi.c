
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

struct midi_chunk {
   char type[5];
   int32_t len;
   uint8_t data[];
} __attribute__( (packed) );

/** \brief  Given an int32 stored as MSB-first, give a platform-appropriate
 *          int32.
 */
int32_t read_int32_msb_first( int32_t* num ) {
   int32_t out = 0;
   uint8_t* bytes = (uint8_t*)num;
   int i;
 
   for( i = 0 ; 3 > i ; i++ ) {
      out <<= 8;
      out |= bytes[i];
   }

   return out;
}

/** \brief  Given a chunk, get the chunk following it in the MIDI stream.
 */
struct midi_chunk* next_chunk( struct midi_chunk* chunk ) {
   int32_t chunk_len = 0;
   uint8_t* byte_ptr = (uint8_t*)chunk;

   chunk_len = read_int32_msb_first( &(chunk->len) );

   /* Subtract 1 for the dummy data ptr. */
   return (struct midi_chunk*)
      (byte_ptr + sizeof( struct midi_chunk ) + chunk_len - 1 );
}

int main( int argc, char** argv ) {
   int midi_handle = 0;
   char* filename = "creator.mid";
   int32_t chunk_len = 0;
   struct midi_chunk* chunk = NULL;
   size_t filesize = 0;
   struct stat st;
   uint8_t* midi_map = NULL;
   size_t bytes_iter = 0;
  
   stat( filename, &st );
   filesize = st.st_size;

   midi_handle = open( filename, O_RDONLY, 0 );
   assert( 0 < midi_handle );

   midi_map = mmap( NULL, filesize, PROT_READ, MAP_PRIVATE | MAP_POPULATE,
      midi_handle, 0 );
   assert( MAP_FAILED != midi_map );

   /* Start iterating chunks. */
   chunk = (struct midi_chunk*)midi_map;
   chunk_len = read_int32_msb_first( &(chunk->len) );
   do {
      printf( "Chunk: Type \"%s\", %d bytes long.\n",
         chunk->type, chunk->len );

      /* Iterate the next chunk. */
      bytes_iter += chunk_len;
      bytes_iter += sizeof( struct midi_chunk ) - 1;
      chunk = next_chunk( chunk );
      chunk_len = read_int32_msb_first( &(chunk->len) );
   } while( 0 < chunk_len && bytes_iter < filesize );

   if( 0 < midi_handle ) {
      close( midi_handle );
   }

   if( NULL != midi_map ) {
      munmap( midi_map, filesize );
   }

   return 0;
}

