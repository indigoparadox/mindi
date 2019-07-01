
#include <stdio.h>
#include <stdint.h>

void read_msb_first( FILE* stream, int32_t* out ) {
   uint8_t* byte_ptr = (uint8_t*)out;
   int i;
 
   for( i = 3 ; 0 <= i ; i-- ) {
      fread( &(byte_ptr[i]), 1, 1, stream );
   }
}

void read_chunk( ) {

}

int main( int argc, char** argv ) {
   FILE* midi = NULL;
   char* filename = "creator.mid";
   char chunk_type[5] = { 0 };
   int32_t chunk_len = 0;
  
   midi = fopen( filename, "rb" );
   fread( chunk_type, 1, 4, midi );
   read_msb_first( midi, &chunk_len );

   printf( "Chunk: Type \"%s\", %d bytes long.\n", chunk_type, chunk_len );



   fclose( midi );

   return 0;
}

