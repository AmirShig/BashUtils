#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 4096

typedef struct Flags {
  int b_flag;
  int e_flag;
  int n_flag;
  int s_flag;
  int t_flag;
  int v_flag;
  int T_falg;
  int E_flag;
} Flags;

int parser(int argc, char *argv[], Flags *f,
           const struct option long_options[]);
void realise_cat(Flags *f, int argc, char *argv[]);
void read_file(FILE *fp, int *empty_line_count, Flags *f);

#endif
