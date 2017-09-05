#ifndef __HTTP_H
#define __HTTP_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void sendrequst(const char *host,const char *filename,const int size,char *msg);
int parseanswer(const char *httphead);

#endif  /* __HTTP_H */