
#include "mindi.h"

#include <stdio.h>
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

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
   chunk_len = read_int32_msb_first( &(chunk->head.len) );
   do {
      printf( "Chunk: Type \"%s\", %d bytes long.\n",
         chunk->head.type, chunk->head.len );

      /* Iterate the next chunk. */
      bytes_iter += chunk_len;
      bytes_iter += sizeof( struct midi_chunk_head ) - 1;
      chunk = next_chunk( chunk );
      chunk_len = read_int32_msb_first( &(chunk->head.len) );
   } while( 0 < chunk_len && bytes_iter < filesize );

   if( 0 < midi_handle ) {
      close( midi_handle );
   }

   if( NULL != midi_map ) {
      munmap( midi_map, filesize );
   }

   return 0;
}

