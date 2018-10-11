#ifndef OBJECTBLOBLIST_H
#define OBJECTBLOBLIST_H

#include "object_blob.h"

class object_blob_list {
protected:
    int num_blobs;
    char **blob_name;
    object_blob **blobs;
public:
    objecy_blob_list();
    virtual ~object_blob_list();
    int get_num_blobs();
    object_blob *add_blob(char *name);
    object_blob *get_blob(int idx);
    object_blob *get_blob(char *name);
    int get_blob_idx(char *name);
    bool save(char *filename);
    bool load(char *filename);
};

#endif
