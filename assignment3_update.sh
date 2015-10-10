#!/bin/bash
       
#Title           :assignment3_update.sh
#description     :This script will update the PA3 template files.
#Author		 :Swetank Kumar Saha <swetankk@buffalo.edu>
#Date            :November 03, 2014
#Version         :1.0
#===================================================================================================

# https://gist.github.com/davejamesmiller/1965569
function ask {
    while true; do
 
        if [ "${2:-}" = "Y" ]; then
            prompt="Y/n"
            default=Y
        elif [ "${2:-}" = "N" ]; then
            prompt="y/N"
            default=N
        else
            prompt="y/n"
            default=
        fi
 
        # Ask the question
        read -p "$1 [$prompt] " REPLY
 
        # Default?
        if [ -z "$REPLY" ]; then
            REPLY=$default
        fi
 
        # Check if the reply is valid
        case "$REPLY" in
            Y*|y*) return 0 ;;
            N*|n*) return 1 ;;
        esac
 
    done
}

echo
echo -n "Enter your UBIT username (without the @buffalo.edu part) and press [ENTER]: "
read ubitname

while true; do
        echo -n "Enter 1 (for C) OR 2 (for C++): "
        read -n 1 lang_choice

        if [ -z $lang_choice ]; then
                continue
        elif [ $lang_choice == "1" ]; then
            lang_option="c"
                break
        elif [ $lang_choice == "2" ]; then
            lang_option="cpp"
                break
        else
                continue
        fi
done
echo

if [ -d "./${ubitname}" ]; 
then
    echo "Directory with given UBITname exists"
else
    echo "No directory named ${ubitname} found. Try again!"
    exit 0
fi

wget mocha.cse.buffalo.edu/cse-489_589/assignment3_package.sh -O assignment3_package.sh
wget mocha.cse.buffalo.edu/cse-489_589/assignment3_tester.sh -O assignment3_tester.sh
wget mocha.cse.buffalo.edu/cse-489_589/${lang_option}/ubitname/Makefile -O ./${ubitname}/Makefile
wget mocha.cse.buffalo.edu/cse-489_589/${lang_option}/ubitname/include/logger.h -O ./${ubitname}/include/logger.h
wget mocha.cse.buffalo.edu/cse-489_589/${lang_option}/ubitname/src/logger.${lang_option} -O ./${ubitname}/src/logger.${lang_option}
