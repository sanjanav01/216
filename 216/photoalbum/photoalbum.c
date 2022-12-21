#include "photoalbum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Photo *create_photo(int id, const char *description) {
    Photo *photo;
    photo = malloc(sizeof(Photo));
    if (photo == NULL) {
        return NULL;
    }
    if (description != NULL) {
        photo->description = malloc((strlen(description) + 1));
        if (photo->description == NULL) {
            return NULL;
        }
        strcpy(photo->description, description);
    } else {
        photo->description = NULL;
    }
    photo->id = id;
    return photo;
}


void print_photo(Photo *photo) {
    if (photo != NULL) {
        printf("Photo Id: %d, ", photo->id);
        if (photo->description != NULL) {
            printf("Description: %s\n", photo->description);
        } else{
            printf("Description: None\n");
        }
    }
}

void destroy_photo(Photo *photo) {
    if(photo != NULL){
        if (photo->description != NULL) {
            free(photo->description);
            photo->description = NULL;
        }
        free(photo);
        photo = NULL;
    }
}

void initialize_album(Album *album) {
    if (album != NULL) {
        album->size = 0;
    }
}

void print_album(const Album *album) {
    int i;
    if (album != NULL) {
        if (album->size == 0) {
            printf("Album has no photos.\n");
        } else {
            for (i = 0; i < album->size; i++) {
                print_photo(album->all_photos[i]);
            }
        }
    }
}

void destroy_album(Album *album) {
    int i;
    if (album != NULL) { 
        for (i = 0; i < album->size; i++) {
            destroy_photo(album->all_photos[i]);
        }
        album->size = 0;
    }
}

void add_photo_to_album(Album *album, int id, const char *description) {
    Photo *photo;
    if (album != NULL) {    
        if (album->size < MAX_ALBUM_SIZE) {
            photo = create_photo(id, description);
            if (photo != NULL) {
                album->all_photos[album->size] = photo;
                album->size += 1;
            }
        }
    }
}