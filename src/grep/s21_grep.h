#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define BUF_SIZE_4 4096

typedef struct Flags {
  int e_flag;
  int i_flag;
  int v_flag;
  int c_flag;
  int l_flag;
  int n_flag;
  int h_falg;
  int s_flag;
  int f_flag;
  char filename[32][256];
  // char fe_pattern[8192];
  // char* fe_pattern;
  // char fe_pattern[512][BUF_SIZE];
  int fe_count_pattern;
  int o_flag;
} Flags;

int parser(int argc, char *argv[], Flags *f, char *fe_pattern);
void realise_grep(Flags *f, int argc, char *argv[], char fe_pattern[]);
int flag_f_prepare(Flags *f, char *fe_pattern);
void reading_file(Flags *f, regex_t template, int argc, char *argv[],
                  int cur_file, int *match_line_count, int *file_match,
                  int *no_match_line_count, size_t nmatch, regmatch_t pmatch[],
                  int val, char *str, int cur_line_number);
// regex_t e_opt(Flags *f, int *val, regex_t template, int cflag, char *str,
//               size_t nmatch, regmatch_t pmatch[]);
void n_opt(int argc, char *argv[], int cur_line_number, int cur_file, Flags *f);
void o_opt(char *str, Flags *f, regex_t template, int argc, char *argv[],
           int cur_file, size_t nmatch, regmatch_t pmatch[]);
void v_opt(int argc, char *argv[], char *str, int cur_line_number, int cur_file,
           Flags *f);
void lc_opt(int argc, char *argv[], Flags *f, int cur_file,
            int no_match_line_count, int file_match, int match_line_count);
void free_arr(Flags *f);

#endif
