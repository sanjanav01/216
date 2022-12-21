#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "photoalbum.h"
#include "my_memory_checker_216.h"

int main() {
   Album album;
   
   start_memory_check();

   initialize_album(&album);
   printf("Size of all_photos Before Adding: %lu\n", sizeof(album.all_photos));
   add_photo_to_album(&album, 40, NULL); 
   add_photo_to_album(&album, 8, "TV"); 
   printf("Size of all_photos After Adding: %lu\n", sizeof(album.all_photos));
   print_album(&album); 
   destroy_album(&album);
   print_album(&album);
   printf("Size of all_photos After Destroying: %lu\n", sizeof(album.all_photos));

   add_photo_to_album(&album, 40, "Computer"); 
   add_photo_to_album(&album, 8, "TV"); 
   print_album(&album); 
   printf("%d\n", album.size);
   destroy_album(&album);

   stop_memory_check();

   return EXIT_SUCCESS;
}
