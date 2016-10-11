#ifndef UTILS_H
#define UTILS_H

#define CMDBUFLEN 100
#define BUFLEN 100

int run_cmd(char *cmd, ...);
void print_hex(char *str, int len);
void print_err(char *str, ...);

#endif
