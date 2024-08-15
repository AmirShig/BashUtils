#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""


declare -a tests=(
"while grep.c grep.h"
"-i regex grep.c"
"-v int grep.c"
"-c int grep.c"
"-l int grep.c grep.h Makefile"
"-n int grep.c"
"-o int grep.c"
"-h while grep.c grep.h"
"text -s no_file.txt"
"-f test_grep/test_ptrn_grep.txt grep.c"
"-in regex grep.c"
"-cv s grep.c"
"-iv s grep.c"
"-lv s grep.c grep.c Makefile"
"-ho while grep.c grep.h"
"-nf test_grep/test_ptrn_grep.txt grep.c"
"-ie INT test_grep/test_5_grep.txt"
"-echar test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-ne = -e out test_grep/test_5_grep.txt"
"-iv int test_grep/test_5_grep.txt"
"-in int test_grep/test_5_grep.txt"
"-c -l arbacadabra test_grep/test_1_grep.txt test_grep/test_5_grep.txt"
"-v test_grep/test_1_grep.txt -e ank"
"-noe int test_grep/test_5_grep.txt"
"-l for test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-o -e int test_grep/test_4_grep.txt"
"-e = -e out test_grep/test_5_grep.txt"
"-e ing -e as -e the -e not -e is test_grep/test_6_grep.txt"
"-l for no_file.txt test_grep/test_2_grep.txt"
"-f test_grep/test_3_grep.txt test_grep/test_5_grep.txt"
)

testing()
{
    t=$(echo $@)
    ./grep $t > test_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo "\033[31m F: $FAIL \033[0m||\033[32m S: $SUCCESS \033[0m|| $COUNTER => \033[32m[SUCCESS]\033[0m grep $t"
    else
      (( FAIL++ ))
      echo "\033[31m F: $FAIL \033[0m||\033[32m S: $SUCCESS \033[0m|| $COUNTER => \033[31m[FAIL]\033[0m grep $t"
    fi
    rm test_grep.log test_sys_grep.log
}


for i in "${tests[@]}"
do
    testing $i
done

echo "\nALL: $COUNTER || \033[31mFAIL: $FAIL\033[0m || \033[32mSUCCESS: $SUCCESS\033[0m"
