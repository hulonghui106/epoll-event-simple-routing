#include <stdlib.h>



struct request{
    char *srcName;
    char *destName;
    char *data;
};

struct fd_element
{
    /** the file descriptor*/
    char *name;
    int fd;
    
    UT_hash_handle hh;
};


int request_data_parse(char *buf);
int request_save_srcfd_to_hash(int src_fd);
int request_get_destfd_from_hash(void);
int request_remove_fd(char *name);

