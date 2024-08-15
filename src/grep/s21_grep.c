#include "s21_grep.h"

int main(int argc, char *argv[]) {
  Flags f = {0};
  // char *fe_pattern = malloc(sizeof(char) * BUF_SIZE);
  char fe_pattern[BUF_SIZE_4] = {0};
  int error = 0;
  if (argc < 3) {
    error = 1;
    fprintf(
        stderr,
        "usage: s21_grep [-eivclnhsfo] [-e pattern] [-f file] [pattern] [file "
        "...]\n");
  } else {
    error = parser(argc, argv, &f, fe_pattern);
  }
  if (!error) {
    realise_grep(&f, argc, argv, fe_pattern);
  }
  // if (fe_pattern != NULL) free(fe_pattern);
  return 0;
}

// int re_alloc(char ***array, int len_p, int len_a) {
//   **array = realloc(**array, sizeof(char) * (len_p + len_a + 10));
//   int error = (array != NULL ? 0 : 1);
//   return error;
// }

int parser(int argc, char *argv[], Flags *f, char *fe_pattern) {
  const char *short_options = "e:ivclnhsf:o";
  const struct option long_options[] = {{NULL, 0, NULL, 0}};
  int res, option_index = 0, error = 0, length_p = 0, length_opt = 0;
  while ((res = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {
    if (res == 'e') {
      length_p = strlen(fe_pattern);
      length_opt = strlen(optarg);
      if (BUF_SIZE_4 - length_p < length_opt) {
        error = 1;
        fprintf(stderr, "ERR: Exceeding the allowed size\n");
        // error = re_alloc(&fe_pattern, length_p, length_opt);
      }else {
        if (f->e_flag) strcat(fe_pattern, "|");
        strcat(fe_pattern, optarg);
        f->e_flag = 1;
        f->fe_count_pattern++;
      }
    } else if (res == 'i') {
      f->i_flag = 1;
    } else if (res == 'v') {
      f->v_flag = 1;
    } else if (res == 'c') {
      f->c_flag = 1;
    } else if (res == 'l') {
      f->l_flag = 1;
    } else if (res == 'n') {
      f->n_flag = 1;
    } else if (res == 'h') {
      f->h_falg = 1;
    } else if (res == 's') {
      f->s_flag = 1;
    } else if (res == 'f') {
      strcpy(f->filename[f->f_flag], optarg);
      f->f_flag += 1;
    } else if (res == 'o') {
      f->o_flag = 1;
    } else {
      printf(
          "usage: s21_grep [-eivclnhsfo] [-e pattern] [-f file] [pattern] "
          "[file ...]\n");
      error = 1;
    }
  }
  return error;
}

int flag_f_prepare(Flags *f, char *fe_pattern) {
  char *str = NULL;
  str = (char *)malloc(BUF_SIZE * sizeof(char));
  int error = 0, first = 0, length_pat = 0,
      length_str = 0;  //, j = f->fe_count_pattern;
  FILE *f_file;
  for (int i = 0; i < f->f_flag && !error; i++) {
    f_file = fopen(f->filename[i], "r");
    if (!f_file) {  // если файл в -f не найден
      fprintf(stderr, "s21_grep: %s: No such file or directory\n",
              f->filename[i]);
      error = 1;
    } else {
      while (fgets(str, BUF_SIZE, f_file) !=
             NULL && !error) {  // запись строк из файла в массив паттерн
        length_pat = strlen(fe_pattern);
        length_str = strlen(str);
        if (BUF_SIZE_4 - length_pat < length_str) {
          error = 1;
          fprintf(stderr, "ERR: Exceeding the allowed size\n");
          continue;
          // error = re_alloc(&fe_pattern, length_pat, length_str);
        }
        if (*str != '\n') str[strcspn(str, "\n")] = '\0';
        if (f->e_flag || first) strcat(fe_pattern, "|");
        if (!error) strcat(fe_pattern, str);
        first = 1;
        f->fe_count_pattern++;
        // j++;
      }
      fclose(f_file);
    }
  }
  free(str);
  str = NULL;
  return error;
}

void realise_grep(Flags *f, int argc, char *argv[], char fe_pattern[]) {
  int cflag = REG_EXTENDED, error = 0, comp = 0,
      cur_file = ((f->e_flag || f->f_flag) ? optind : optind + 1);
  if (f->f_flag) error = flag_f_prepare(f, fe_pattern);
  if (!error) {
    FILE *fp;
    regex_t template;
    if (f->i_flag) {
      cflag = REG_ICASE;
    }
    if (!f->e_flag && !f->f_flag) {
      comp = regcomp(&template, argv[optind], cflag);
    } else {
      comp = regcomp(&template, fe_pattern, cflag);
    }
    while (cur_file < argc && !comp) {
      int file_match = 0, match_line_count = 0, no_match_line_count = 0;
      fp = fopen(argv[cur_file], "rb+");
      if (!fp) {
        if (!f->s_flag)
          fprintf(stderr, "%s: %s: No such file or directory\n", argv[0],
                  argv[cur_file]);
      } else {
        char *str = NULL;
        str = (char *)malloc(BUF_SIZE * sizeof(char));
        int cur_line_number = 1;
        size_t nmatch = 1;
        regmatch_t pmatch[1024];
        while (fgets(str, BUF_SIZE, fp) != NULL) {
          str[strcspn(str, "\n")] = '\n';
          str[strcspn(str, "\n") + 1] = '\0';
          int val = REG_NOMATCH;
          // if (f->e_flag != 0 || f->f_flag != 0) {
          //   template = e_opt(f, &val, template, cflag, str, nmatch, pmatch);
          // } else {
          val = regexec(&template, str, nmatch, pmatch, 0);
          // }
          reading_file(f, template, argc, argv, cur_file, &match_line_count,
                       &file_match, &no_match_line_count, nmatch, pmatch, val,
                       str, cur_line_number);
          cur_line_number++;
          // if (f->fe_count_pattern) regfree(&template);
        }
        lc_opt(argc, argv, f, cur_file, no_match_line_count, file_match,
               match_line_count);
        if (str != NULL) free(str);
        fclose(fp);
      }
      cur_file++;
    }
    if (!comp) regfree(&template);
    if (comp) fprintf(stderr, "ERR %d: regcomp error\n", comp);
  }
}

void reading_file(Flags *f, regex_t template, int argc, char *argv[],
                  int cur_file, int *match_line_count, int *file_match,
                  int *no_match_line_count, size_t nmatch, regmatch_t pmatch[],
                  int val, char *str, int cur_line_number) {
  if (val == 0) {
    if (f->n_flag && !f->c_flag && !f->l_flag && !f->v_flag) {
      if ((argc - (f->fe_count_pattern ? optind : optind + 1)) > 1 &&
          !f->h_falg) {
        printf("%s:%d:", argv[cur_file], cur_line_number);
      } else {
        printf("%d:", cur_line_number);
      }
    }
    if (!f->v_flag && !f->c_flag && !f->l_flag && !f->o_flag) {
      if ((argc - (f->fe_count_pattern ? optind : optind + 1)) > 1 &&
          !f->h_falg && !f->n_flag) {
        printf("%s:%s", argv[cur_file], str);
      } else {
        printf("%s", str);
      }
    }
    if (f->o_flag && !f->v_flag && !f->c_flag && !f->l_flag) {
      o_opt(str, f, template, argc, argv, cur_file, nmatch, pmatch);
    }
    *match_line_count += 1;
    *file_match = 1;
  } else {
    *no_match_line_count += 1;
  }
  if (val == REG_NOMATCH && f->v_flag && !f->c_flag && !f->l_flag) {
    v_opt(argc, argv, str, cur_line_number, cur_file, f);
  }
}

// regex_t e_opt(Flags *f, int *val, regex_t template, int cflag, char *str,
//               size_t nmatch, regmatch_t pmatch[]) {
//   for (int i = 0; i < f->fe_count_pattern && *val != 0; i++) {
//     regcomp(&template, f->fe_pattern[i], cflag);
//     *val = regexec(&template, str, nmatch, pmatch, 0);
//     regfree(&template);
//   }
//   return template;
// }

void n_opt(int argc, char *argv[], int cur_line_number, int cur_file,
           Flags *f) {
  if ((argc - (f->fe_count_pattern ? optind : optind + 1)) > 1 && !f->h_falg) {
    printf("%s:%d:", argv[cur_file], cur_line_number);
  } else {
    printf("%d:", cur_line_number);
  }
}

void o_opt(char *str, Flags *f, regex_t template, int argc, char *argv[],
           int cur_file, size_t nmatch, regmatch_t pmatch[]) {
  char *str_o;
  str_o = str;
  // int count = f->fe_count_pattern;
  // for (int k = 0; k < (count ? count : 1); k++) {
  int no_match_o = 0, i = 0, f_eo = 0;
  // if (count) regcomp(&template, f->fe_pattern[k], cflag);
  while (no_match_o != REG_NOMATCH) {
    no_match_o = regexec(&template, str_o, nmatch, pmatch, 0);
    if (no_match_o) continue;
    i = pmatch->rm_so;
    f_eo = pmatch->rm_eo;
    if ((argc - (f->fe_count_pattern ? optind : optind + 1)) > 1 &&
        !f->n_flag && !f->h_falg)
      printf("%s:", argv[cur_file]);
    for (; i < f_eo; i++) {
      printf("%c", str_o[i]);
    }
    printf("\n");
    str_o = (str_o + i);
    // }
    // if (count) regfree(&template);
  }
}

void v_opt(int argc, char *argv[], char *str, int cur_line_number, int cur_file,
           Flags *f) {
  if ((argc - (f->fe_count_pattern ? optind : optind + 1)) > 1 && f->n_flag &&
      !f->h_falg) {
    printf("%s:%d:%s", argv[cur_file], cur_line_number, str);
  } else if ((argc - (f->fe_count_pattern ? optind : optind + 1)) > 1 &&
             !f->h_falg) {
    printf("%s:%s", argv[cur_file], str);
  } else if (f->n_flag) {
    printf("%d:%s", cur_line_number, str);
  } else {
    printf("%s", str);
  }
}

void lc_opt(int argc, char *argv[], Flags *f, int cur_file,
            int no_match_line_count, int file_match, int match_line_count) {
  if (f->c_flag) {
    if ((argc - (f->fe_count_pattern ? optind : optind + 1)) > 1 &&
        !f->h_falg && !f->l_flag) {
      printf("%s:%d\n", argv[cur_file],
             (f->v_flag ? no_match_line_count : match_line_count));
    } else if ((argc - (f->fe_count_pattern ? optind : optind + 1)) > 1 &&
               !f->h_falg && f->l_flag) {
      printf("%s:%d\n", argv[cur_file], (f->v_flag ? 1 : file_match));
    } else if (!f->l_flag) {
      printf("%d\n", (f->v_flag ? no_match_line_count : match_line_count));
    } else {
      printf("%d\n", (f->v_flag ? 1 : file_match));
    }
  }
  if (f->l_flag) {
    if (file_match && !f->v_flag) printf("%s\n", argv[cur_file]);
    if (no_match_line_count > 0 && f->v_flag) printf("%s\n", argv[cur_file]);
  }
}
