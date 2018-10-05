#ifndef OBJECTBLOB_H
#define OBJECTBLOB_H

/*
bool,char,int,float,double
*/

const static int D_BOOL = 0;
const static int D_CHAR = 1;
const static int D_INT = 2;
const static int D_FLOAT = 3;
const static int D_DOUBLE = 4;

class object_blob {
protected:
    int num_data;
    char **data_name;
    int *data_type;
    int *data_size;
    int *data_length;
    char **data;
public:
    object_blob();
    virtual ~object_blob();
    void add_data(char *name, void *data_in, int type, int length);
    int get_data_size(char *name);
    int get_data_type(char *name);
    int get_data_length(char *name);
    int get_data_idx(char *name);
    void *get_data(char *name);
    void get_data(char *name, void *data_out);
    int get_type_size(int type);
    void massage_data(int type, void *data_in, int length, int size);
    char *save_blob(int *size);
    void load_blob(char *blob);
};

#endif
