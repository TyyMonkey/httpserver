#ifndef __MYHTTP_H__
#define __MYHTTP_H__

//int read_html(char *filename, char **content);
int myhttp_get_filename(char *msg, char **filename);
char *myhttp_gettype(const char *filename);

#endif