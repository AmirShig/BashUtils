#include "s21_cat.h"

int main(int argc, char *argv[]) {
  Flags f = {0};
  int error = 0;
  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};
  error = parser(argc, argv, &f, long_options);
  if (!error) {
    realise_cat(&f, argc, argv);
  }
  return 0;
}

int parser(int argc, char *argv[], Flags *f,
           const struct option long_options[]) {
#ifdef __APPLE__
  const char *short_options = "+benstvTE";
#else
  const char *short_options = "benstvTE";
#endif
  int res, option_index = -1, error = 0;
  while ((res = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {
    if (res == 'b') {
      f->b_flag = 1;
    } else if (res == 'e') {
      f->e_flag = 1;
    } else if (res == 'n') {
      f->n_flag = 1;
    } else if (res == 's') {
      f->s_flag = 1;
    } else if (res == 't') {
      f->t_flag = 1;
    } else if (res == 'v') {
      f->v_flag = 1;
    } else if (res == 'T') {
      f->T_falg = 1;
    } else if (res == 'E') {
      f->E_flag = 1;
    } else {
      printf("usage: s21_cat [-benstv] [file...]\n");
      error = 1;
    }
    option_index = -1;
  }
  return error;
}

void realise_cat(Flags *f, int argc, char *argv[]) {
  FILE *fp;
  int empty_line_count = 0, cur_file = optind;
  while (cur_file < argc) {
    if (cur_file != argc) {
      fp = fopen(argv[cur_file], "r");
    }
    if (!fp) {
      fprintf(stderr, "%s: %s: No such file or directory\n", argv[0],
              argv[argc - 1]);
    } else {
      read_file(fp, &empty_line_count, f);
    }
    cur_file++;
  }
}

void read_file(FILE *fp, int *empty_line_count, Flags *f) {
  int line_num = 1, prev_sym = '\n', error = 0;
  while (!feof(fp)) {
    int cur_sym = fgetc(fp);
    if (cur_sym == EOF) {
      error = 1;
    }
    if (f->s_flag && prev_sym == '\n' && cur_sym == '\n' && !error) {
      *empty_line_count += 1;
      if (*empty_line_count > 1) {
        continue;
      }
    } else if (!error) {
      *empty_line_count = 0;
    }
    if (f->b_flag && prev_sym == '\n' && !error) {
      if (prev_sym == '\n' && cur_sym != '\n') {
        printf("%6d\t", line_num++);
      }
    } else if (f->n_flag && prev_sym == '\n' && !error) {
      printf("%6d\t", line_num++);
    }
    if ((f->t_flag || f->T_falg) && !error) {
      if (cur_sym == '\t') {
        printf("^");
        cur_sym = 'I';
      }
    }
    if ((f->e_flag || f->E_flag) && cur_sym == '\n' && !error) {
      printf("$");
    }
    if ((f->v_flag || f->t_flag || f->e_flag) && !error) {
      if ((cur_sym < 33 || cur_sym == 127) && cur_sym != '\n' &&
          cur_sym != ' ' && cur_sym != '\t') {
        printf("^");
        if (cur_sym < 33) {
          cur_sym += 64;
        }
        if (cur_sym == 127) {
          cur_sym -= 64;
        }
      }
    }
    prev_sym = cur_sym;
    if (!error) {
      printf("%c", cur_sym);
    }
  }
  fclose(fp);
}
