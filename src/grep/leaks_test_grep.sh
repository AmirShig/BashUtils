#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
RESULT=0
DIFF_RES=""


declare -a tests=(
"while s21_grep.c s21_grep.h"
"-i regex s21_grep.c"
"-v int s21_grep.c"
"-c int s21_grep.c"
"-l int s21_grep.c s21_grep.h Makefile"
"-n int s21_grep.c"
"-o int s21_grep.c"
"-h while s21_grep.c s21_grep.h"
"text -s no_file.txt"
"-f test_grep/test_ptrn_grep.txt s21_grep.c"
"-in regex s21_grep.c"
"-cv s s21_grep.c"
"-iv s s21_grep.c"
"-lv s s21_grep.c s21_grep.c Makefile"
"-ho while s21_grep.c s21_grep.h"
"-nf test_grep/test_ptrn_grep.txt s21_grep.c"
"-ie INT test_grep/test_5_grep.txt"
"-echar test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-ne = -e out test_grep/test_5_grep.txt"
"-iv int test_grep/test_5_grep.txt"
"-in int test_grep/test_5_grep.txt"
"-c -l arbacadabra test_grep/test_1_grep.txt test_grep/test_5_grep.txt"
"-v test_grep/test_1_grep.txt -e ank"
"-noe ) test_grep/test_5_grep.txt"
"-l for test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-o -e int test_grep/test_4_grep.txt"
"-e = -e out test_grep/test_5_grep.txt"
"-noe ing -e as -e the -e not -e is test_grep/test_6_grep.txt"
"-e ing -e as -e the -e not -e is test_grep/test_6_grep.txt"
"-l for no_file.txt test_grep/test_2_grep.txt"
"-f test_grep/test_3_grep.txt test_grep/test_5_grep.txt"
)

testing()
{
    t=$(echo $@)
    leaks -quiet -atExit -- ./s21_grep $t > test_s21_grep.log
    leak=$(grep leaks test_s21_grep.log)
    (( COUNTER++ ))
    if [[ $leak == *"0 leaks for 0 total leaked bytes"* ]]
    then
      (( SUCCESS++ ))
      echo "== $COUNTER == \033[32m[SUCCESS]\033[0m grep $t"
    else
      (( FAIL++ ))
      echo "== $COUNTER == \033[31m[FAIL]\033[0m grep $t"
    fi
    rm test_s21_grep.log
}


for i in "${tests[@]}"
do
    testing $i
done

echo "\n\tALL: $COUNTER || \033[31mFAIL: $FAIL\033[0m || \033[32mSUCCESS: $SUCCESS\033[0m"
