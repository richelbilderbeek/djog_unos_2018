#include "object_blob_list.h"

#ifdef WHY_WOULD_ONE_WANT_THIS_EVER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

object_blob_list::object_blob_list() {
  num_blobs = 0;
  blob_name = NULL;
  blobs = NULL;
}
object_blob_list::~object_blob_list() {
  for (int b = 0; b < num_blobs; b++) {
    free(blob_name[b]);
    delete blobs[b];
  }
  free(blob_name);
  free(blobs);
}
int object_blob_list::get_num_blobs() { return num_blobs; }
/*
addBlob will create a new object_blob identified by name and return it
*/
object_blob *object_blob_list::add_blob(char *name) {
  if (get_blob_idx(name) >= 0)
    return NULL;
  printf("Adding Blob: %s\n", name);
  num_blobs++;
  blob_name = (char **)realloc(blob_name, sizeof(char *) * num_blobs);
  blob_name[num_blobs - 1] = strdup(name);
  blobs = (object_blob **)realloc(blobs, sizeof(object_blob *) * num_blobs);
  blobs[num_blobs - 1] = new object_blob;
  return blobs[num_blobs - 1];
}
/*
getBlob(int idx) returns the object_blob located at idx
*/
object_blob *object_blob_list::get_blob(int idx) {
  if (idx >= 0 && idx < num_blobs) {
    return blobs[idx];
  }
  return NULL;
}
/*
getBlob(char *name) returns the named object_blob
*/
object_blob *object_blob_list::get_blob(char *name) {
  for (int b = 0; b < num_blobs; b++) {
    if (strcmp(blob_name[b], name) == 0) {
      return blobs[b];
    }
  }
  printf("Object Blob List Error: Couldn't find data handle '%s'.\n", name);
  return NULL;
}
/*
getBlobIdx get the internal index of the named object_blob
*/
int object_blob_list::get_blob_idx(char *name) {
  for (int b = 0; b < num_blobs; b++) {
    if (strcmp(blob_name[b], name) == 0) {
      return b;
    }
  }
  printf("Object Blob List Error: Couldn't find data handle '%s'.\n", name);
  return -1;
}
/*
save writes all of the contained object_blobs to the file identified by filename
*/
bool object_blob_list::save(char *filename) {
  FILE *outfile = fopen(filename, "wb");
  if (outfile == NULL)
    return false;
  printf("Writing out %i blobs.\n", numBlobs);
  fwrite(&numBlobs, sizeof(int), 1, outfile);
  int blob_name_length = 0;
  int blob_size = 0;
  char *blob_data;
  for (int b = 0; b < num_blobs; b++) {
    blob_name_length = strlen(blob_name[b]) + 1;
    fwrite(&blob_name_length, sizeof(int), 1, outfile);
    fwrite(blob_name[b], 1, blob_name_length, outfile);
    blob_data = blobs[b]->save_blob(&blob_size);
    fwrite(&blob_size, sizeof(int), 1, outfile);
    fwrite(blob_data, 1, blob_size, outfile);
    free(blob_data);
  }
  fclose(outfile);
  return true;
}
/*
load reads the object_blob_list file identified by filename
*/
bool object_blob_list::load(char *filename) {
  FILE *infile = fopen(filename, "rb");
  if (infile == NULL)
    return false;
  int num_blobs_temp = 0;
  fread(&num_blobs_temp, sizeof(int), 1, infile);
  printf("Reading in %i blobs.\n", num_blobs_temp);
  int blob_name_length = 0;
  int blob_size = 0;
  char *blob_name_temp = (char *)malloc(1);
  blob_name_temp[0] = 0;
  object_blob *new_blob;
  for (int b = 0; b < num_blobs_temp; b++) {
    fread(&blob_name_length, sizeof(int), 1, infile);
    printf("Reading in blob name of length %i.\n", blob_name_length);
    blob_name_temp = (char *)realloc(blob_name_temp, blob_name_length);
    fread(blob_name_temp, 1, blob_name_length, infile);
    printf("Blob name temp = %s\n", blob_name_temp);
    new_blob = add_blob(blob_name_temp);
    if (new_blob == NULL) {
      fclose(infile);
      free(blob_name_temp);
      return false;
    }
    fread(&blob_size, sizeof(int), 1, infile);
    char *blob_data = (char *)malloc(blob_size);
    fread(blob_data, 1, blob_size, infile);
    new_blob->load_blob(blob_data);
    free(blob_data);
  }
  free(blob_name_temp);
  return true;
}

#endif // WHY_WOULD_ONE_WANT_THIS_EVER
