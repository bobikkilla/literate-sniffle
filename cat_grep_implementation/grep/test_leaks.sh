#!/bin/bash
SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""

s21_command=(
    "./s21_grep"
    )
sys_command=(
    "grep"
    )

flags=(
    "v"
    "c"
    "l"
    "n"
    "h"
    "o"
    "i"
    "s"
)

tests=(
"s test_files/test_0_grep.txt FLAGS"
"for s21_grep.c s21_grep.h Makefile FLAGS"
"for s21_grep.c FLAGS"
"-e for -e ^int s21_grep.c s21_grep.h Makefile FLAGS"
"-e for -e ^int s21_grep.c FLAGS"
"FLAGS no_file.txt"
"-e intel -e int FLAGS test_files/test_7_grep.txt"
"-e int -e intel FLAGS test_files/test_7_grep.txt"
)

manual=(
"-n for test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-n for test_files/test_1_grep.txt"
"-n -e ^\} test_files/test_1_grep.txt"
"-ce ^int test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-e ^int test_files/test_1_grep.txt"
"-nivh = test_files/test_1_grep.txt test_files/test_2_grep.txt" 
"-e"
"-ie INT test_files/test_5_grep.txt"
"-echar test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-ne = -e out test_files/test_5_grep.txt"
"-iv int test_files/test_5_grep.txt"
"-in int test_files/test_5_grep.txt"
"-c -l aboba test_files/test_1_grep.txt test_files/test_5_grep.txt"
"-v test_files/test_1_grep.txt -e ank"
"-noe ) test_files/test_5_grep.txt"
"-l for test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-o -e int test_files/test_4_grep.txt"
"-e = -e out test_files/test_5_grep.txt"
"-noe ing -e as -e the -e not -e is test_files/test_6_grep.txt"
"-e ing -e as -e the -e not -e is test_files/test_6_grep.txt"
"-c -e . test_files/test_1_grep.txt -e '.'"
"-l for no_file.txt test_files/test_2_grep.txt"
"-e int -si no_file.txt s21_grep.c no_file2.txt s21_grep.h"
)

run_test() {
    param=$(echo "$@" | sed "s/FLAGS/$var/")
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose -q --log-file="${s21_command[@]}".log ./"${s21_command[@]}" $param > /dev/null
    leak=$(grep -ic -A10000 "LEAK SUMMARY:" "${s21_command[@]}".log || true)
    leak2=$(grep -ic -A10000 "ERROR SUMMARY: [^0]" "${s21_command[@]}".log || true)
    let "COUNTER++"
    if [ "$leak" -eq "0" ] && [ "$leak2" -eq "0" ]
    then
        let "SUCCESS++"
        printf "\033[92mSuccess $param\033[0m\n"
    else
        let "FAIL++"
        printf "\033[91mFAIL $param\033[0m\n"
        #exit 1
    fi
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}

echo "^^^^^^^bobiksdoh^^^^^^^"
echo "TESTS WITH NORMAL FLAGS"
echo "^^^^^^^^^^^^^^^^^^^^^^^"
printf "\n"
echo "#######################"
echo "MANUAL TESTS"
echo "#######################"
printf "\n"

for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done

printf "\n"
echo "#######################"
echo "AUTOTESTS"
echo "#######################"
printf "\n"
echo "======================="
echo "1 PARAMETER"
echo "======================="
printf "\n"

count1parameter=0
for var1 in "${flags[@]}"
do
    for i in "${tests[@]}"
    do
        if [ $count1parameter -lt 10 ]; then
            var="-$var1"
            run_test "$i"
            ((count1parameter++))
        else
            break 2
        fi
    done
done

printf "\n"
echo "======================="
echo "2 PARAMETERS"
echo "======================="
printf "\n"
count2parameter=0

for var1 in "${flags[@]}"
do
    if [ $count2parameter -lt 10 ]; then
        for var2 in "${flags[@]}"
        do
            if [ $var1 != $var2 ]; then
                for i in "${tests[@]}"
                do
                    if [ $count2parameter -lt 10 ]; then
                        var="-$var1 -$var2"
                        run_test "$i"
                        ((count2parameter++))
                    else
                        break 3  
                    fi
                done
            fi
        done
    fi
done
printf "\n"
echo "======================="
echo "3 PARAMETERS"
echo "======================="
printf "\n"

count3parameter=0
for var1 in "${flags[@]}"
do
    if [ $count3parameter -lt 10 ]; then
        for var2 in "${flags[@]}"
        do
            for var3 in "${flags[@]}"
            do
                if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]; then
                    for i in "${tests[@]}"
                    do
                        if [ $count3parameter -lt 10 ]; then
                            var="-$var1 -$var2 -$var3"
                            run_test "$i"
                            ((count3parameter++))
                        else
                            break 4
                        fi
                    done
                fi
            done
        done
    fi
done
printf "\n"
echo "======================="
echo "4 PARAMETERS"
echo "======================="
printf "\n"

count4parameter=0
for var1 in "${flags[@]}"
do
    if [ $count4parameter -lt 10 ]; then
        for var2 in "${flags[@]}"
        do
            for var3 in "${flags[@]}"
            do
                for var4 in "${flags[@]}"
                do
                    if [ $var1 != $var2 ] && [ $var2 != $var3 ] \
                       && [ $var1 != $var3 ] && [ $var1 != $var4 ] \
                       && [ $var2 != $var4 ] && [ $var3 != $var4 ]; then
                        for i in "${tests[@]}"
                        do
                            if [ $count4parameter -lt 10 ]; then
                                var="-$var1 -$var2 -$var3 -$var4"
                                run_test "$i"
                                ((count4parameter++))
                            else
                                break 5  
                            fi
                        done
                    fi
                done
            done
        done
    fi
done

printf "\n"
echo "#######################"
echo "AUTOTESTS"
echo "#######################"
printf "\n"
echo "======================="
echo "DOUBLE PARAMETER"
echo "======================="
printf "\n"
count_double_parameter=0
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ "$var1" != "$var2" ]; then
            for i in "${tests[@]}"
            do
                if [ $count_double_parameter -lt 10 ]; then
                    var="-$var1$var2"
                    run_test "$i"
                    ((count_double_parameter++))
                else
                    break 3
                fi
            done
        fi
    done
done

printf "\n"
echo "#######################"
echo "AUTOTESTS"
echo "#######################"
printf "\n"
echo "======================="
echo "TRIPLE PARAMETER"
echo "======================="
printf "\n"

count_triple_parameter=0
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            if [ "$var1" != "$var2" ] && [ "$var2" != "$var3" ] && [ "$var1" != "$var3" ]; then
                for i in "${tests[@]}"
                do
                    if [ $count_triple_parameter -lt 10 ]; then
                        var="-$var1$var2$var3"
                        run_test "$i"
                        ((count_triple_parameter++))
                    else
                        break 4
                    fi
                done
            fi
        done
    done
done

printf "\n"
echo "FAILED: $FAIL"
echo "SUCCESSFUL: $SUCCESS"
echo "ALL: $COUNTER"
printf "\n"
##############################
