#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "object_blob.h"

object_blob::object_blob()
{
    num_data = 0;
    data_name = NULL;
    data_type = NULL;
    data_size = NULL;
    data_length = NULL;
    data = NULL;
}
object_blob::~object_blob()
{
    for(int d=0;d<num_data;d++)
    {
        free(data_name[d]);
        free(data[d]);
    }
    free(data_name);
    free(data);
    free(data_type);
    free(data_size);
    free(data_length);
}
/*
addData takes a named handle, a pointer to data, a known type (as defined in
object_blob.h) and the length or amount of data located at that pointer and
copies it into the blob's storage in a consistent format.
*/
void object_blob::add_data(char *name, void *data_in, int type, int length)
{
    num_data++;
    data_name = (char **)realloc(data_name, sizeof(char *)*num_data);
    data_name[num_data-1] = strdup(name);
    data_type = (int *)realloc(data_type, sizeof(int)*num_data);
    data_type[num_data-1] = type;
    data_size = (int *)realloc(data_size, sizeof(int)*num_data);
    data_size[num_data-1] = get_type_size(type);
    data_length = (int *)realloc(data_length, sizeof(int)*num_data);
    data_length[num_data-1] = length;
    data = (char **)realloc(data, sizeof(char *)*num_data);
    data[num_data-1] = (char *)malloc(sizeof(char)*data_size[num_data-1]*length);
    massage_data(type, data_in, length, data_size[num_data-1]);
}
/*
get_data_size returns the size of the data_type stored at the named handle
*/
int object_blob::get_data_size(char *name)
{
    int idx = get_data_idx(name);
    if(idx >= 0 && idx < num_data)
    {
        return data_size[idx];
    }
    return -1;
}
/*
get_data_type returns the type of data stored at the named handle
*/
int object_blob::get_data_type(char *name)
{
    int idx = get_data_idx(name);
    if(idx >= 0 && idx < num_data)
    {
        return data_type[idx];
    }
    return -1;
}
/*
get_data_length returns the number of data stored at the named handle
*/
int object_blob::get_data_length(char *name)
{
    int idx = get_data_idx(name);
    if(idx >= 0 && idx < num_data)
    {
        return data_length[idx];
    }
    return -1;
}
/*
get_data_idx returns the internal index of the stored data at the named
handle
*/
int object_blob::get_data_idx(char *name)
{
    for(int d=0;d<num_data;d++)
    {
        if(strcmp(data_name[d], name) == 0)
        {
            return d;
        }
    }
    printf("Object Blob Error: Couldn't find data handle '%s'.\n",name);
    return -1;
}
/*
get_data(char *name) returns a pointer to the raw data identified at
the named handle
*/
void *object_blob::get_data(char *name)
{
    int idx = get_data_idx(name);
    if(idx >= 0 && idx < num_data)
    {
        return data[idx];
    }
    return NULL;
}
/*
get_data(char *name, void *data_out) loads the stored data identified by
the named handle into the area pointed to by data_out.

Ideally this should massage the data into the expected platform format
before copying the stored data into data_out.
*/
void object_blob::get_data(char *name, void *data_out)
{
    int idx = get_data_idx(name);
    if(idx >= 0 && idx < num_data)
    {
        memcpy(data_out, data[idx], data_size[idx]*data_length[idx]);
    }
}
/*
get_type_size returns the internal storage size of the indicated data type
*/
int object_blob::get_type_size(int type)
{
    //These are hard-coded in the event I decide to use platforms with different
    //fundamental data type sizes, the save files will be consistent with this
    //to allow interchange of save data
    switch (type) {
        case D_BOOL:
            return 1;
            break;
        case D_CHAR:
            return 1;
            break;
        case D_INT:
            return 4;
            break;
        case D_FLOAT:
            return 4;
            break;
        case D_DOUBLE:
            return 8;
            break;
    }
    return 0;
}
/*
massage_data is a wrapper that allows object_blob to be truly cross
platform, currently it is just a simple pass-through but the idea
is this can deal with data size mismatches and endian differences
across platform types.
*/
void object_blob::massage_data(int type, void *data_in, int length, int size)
{
    //passthrough for LE (ie x86, iOS)
    //stored data is Little Endian
    memcpy(data[num_data-1], data_in, length*size);
}
/*
save_blob creates a character array that contains the data currently
stored in the object_blob, the integer pointed to by size will be
filled with the size of the array
*/
char *object_blob::save_blob(int *size)
{
    int blob_size = 0;
    int blob_offset = 0;
    int int_temp;
    char *blobTemp = (char *)malloc(sizeof(int));
    //save number of data blocks in the blob
    memcpy(blobTemp, &num_data, sizeof(int));
    blob_offset += sizeof(int);
    blob_size += sizeof(int);
    //save blocks
    //int - name length
    //char * - name
    //int - type
    //int - size
    //int - length
    //char (size*length) - data
    for(int d=0;d<num_data;d++)
    {
        blob_size += sizeof(int)*4+strlen(data_name[d])+1+data_length[d]*data_size[d];
        blobTemp = (char *)realloc(blobTemp, blob_size);
        int_temp = strlen(data_name[d])+1;
        memcpy(&blobTemp[blob_offset], &int_temp, sizeof(int));
        blob_offset += sizeof(int);
        memcpy(&blobTemp[blob_offset], data_name[d], int_temp);
        blob_offset += int_temp;
        memcpy(&blobTemp[blob_offset], &data_type[d], sizeof(int));
        blob_offset += sizeof(int);
        memcpy(&blobTemp[blob_offset], &data_size[d], sizeof(int));
        blob_offset += sizeof(int);
        memcpy(&blobTemp[blob_offset], &data_length[d], sizeof(int));
        blob_offset += sizeof(int);
        memcpy(&blobTemp[blob_offset], data[d], data_length[d]*data_size[d]);
        blob_offset += data_length[d]*data_size[d];
    }
    *size = blob_size;
    return blobTemp;
}
/*
loadBlob will take a character array that was previously created by
save_blob and fill the object_blob with its contents.
*/
void object_blob::loadBlob(char *blob)
{
    int numBlocks = 0;
    int blob_offset = 0;
    memcpy(&numBlocks, &blob[blob_offset], sizeof(int));
    blob_offset += sizeof(int);
    int nameLength;
    char *nameTemp;
    int typeTemp;
    int sizeTemp;
    int lengthTemp;
    char *dataTemp;
    for(int d=0;d<numBlocks;d++)
    {
        memcpy(&nameLength, &blob[blob_offset], sizeof(int));
        blob_offset+=sizeof(int);
        nameTemp = &blob[blob_offset];
        blob_offset += nameLength;
        memcpy(&typeTemp, &blob[blob_offset], sizeof(int));
        blob_offset += sizeof(int);
        memcpy(&sizeTemp, &blob[blob_offset], sizeof(int));
        blob_offset += sizeof(int);
        memcpy(&lengthTemp, &blob[blob_offset], sizeof(int));
        blob_offset += sizeof(int);
        dataTemp = &blob[blob_offset];
        addData(nameTemp, dataTemp, typeTemp, lengthTemp);
        blob_offset += lengthTemp*sizeTemp;
    }
}
