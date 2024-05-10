#!/bin/bash

tf_dir="testfiles/"

# флаги
#options=("" "-b" "-e" "-n" "-s" "-t" "--" "-q" "test" "b")
#options=("" "-b" "-e" "-n" "-s" "-t")
#options=("--" "-q" "test")
#options=("-b")
#options=("-b" "-e" "-be")
options=("-e")
#options=("-s")
#options=("-be")
#options=("-b" "-e" "-n" "-s" "-t" "-be" "-bn" "-bs" "-bt" "-en" "-es" "-et" "-ns" "-nt" "-st" "-ben" "-bes" "-bet" "-bns" "-bnst" "-bnt" "-ens" "-enst" "-ent" "-est" "-bens" "-benst" "-best" "-bnst")

GREEN=$(tput setaf 2)
RED=$(tput setaf 1)
RESET=$(tput sgr0)


for opt in "${options[@]}"
do
    echo "TESTING '$opt' option"
    COUNTER=1
    for testfile in "${tf_dir}"*
    do

        filename=$(basename "$testfile")


        cat_res="cat_res_${opt}_${COUNTER}.txt"
        s21_cat_res="s21_cat_res_${opt}_${COUNTER}.txt"


        cat $opt "$testfile" &> "$cat_res"
        .././s21_cat $opt "$testfile" &> "$s21_cat_res"



        if cmp --silent "$cat_res" "$s21_cat_res"
        then echo "    $filename ${GREEN}OK${RESET}"
        else echo "    $filename ${RED}FAILED${RESET}"
        fi

        rm $cat_res $s21_cat_res


        COUNTER=$((COUNTER + 1))
    done
done
