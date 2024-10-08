#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"VAR test_cat/test_case_cat.txt"
)

declare -a extra=(
"test_cat/test_case_cat.txt"
"-b test_cat/test_case_cat.txt"
"-e test_cat/test_case_cat.txt"
"-n test_cat/test_case_cat.txt"
"-n test_cat/test_2_cat.txt"
"-s test_cat/test_case_cat.txt"
"-s test_cat/test_case_cat.txt"
"-t test_cat/test_case_cat.txt"
"-s test_cat/test_1_cat.txt"
"-b -e -n -s -t -v test_cat/test_1_cat.txt"
"-t test_cat/test_3_cat.txt"
"-n test_cat/test_2_cat.txt"
"no_file.txt"
"-n -b test_cat/test_1_cat.txt"
"-s -n -e test_cat/test_4_cat.txt"
"-n test_cat/test_1_cat.txt"
"-n test_cat/test_1_cat.txt test_cat/test_2_cat.txt"
"-v test_cat/test_5_cat.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_cat $t > test_s21_cat.log
    cat $t > test_sys_cat.log
    DIFF_RES="$(diff -s test_s21_cat.log test_sys_cat.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_cat.log and test_sys_cat.log are identical" ]
    then
      (( SUCCESS++ ))
        echo "\033[31m F: $FAIL \033[0m||\033[32m S: $SUCCESS \033[0m|| $COUNTER => \033[32m[SUCCESS]\033[0m cat $t"
    else
      (( FAIL++ ))
        echo "\033[31m F: $FAIL \033[0m||\033[32m S: $SUCCESS \033[0m|| $COUNTER => \033[31m[FAIL]\033[0m cat $t"
    fi
    rm test_s21_cat.log test_sys_cat.log
}

for i in "${extra[@]}"
do
    testing $i
done

# 2 параметра
for var1 in b e n s t v
do
    for var2 in b e n s t v
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

echo "\nALL: $COUNTER || \033[31mFAIL: $FAIL\033[0m || \033[32mSUCCESS: $SUCCESS\033[0m"
