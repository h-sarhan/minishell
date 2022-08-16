PURPLE="\033[0;35m"
GREEN="\033[0;32m"
BLUE="\033[0;36m"
YELLOW="\033[0;33m"
RESET="\033[0m"
ERROR="\033[0;31m"

run_test() {
	echo -e "${PURPLE}${1}${RESET}"
	echo "./pipex ${2} ${3} ${4} ${5}"
	echo
	./pipex "${2}" "${3}" "${4}" "${5}" 2> my_err
	echo $? > my_ret
	cat "${5}" > my_out
	(< ${2} ${3} | ${4} > ${5}) 2> bash_err
	echo $? > correct_ret
	cat "${5}" > correct_out
	if cmp -s my_out correct_out
	then
		echo -e "${GREEN}Correct Output${RESET}"
	else
		echo -e "${ERROR}Wrong output${RESET}"
		echo "My Output:"
		cat my_out
		echo "Correct Output:"
		cat correct_out
		echo 
	fi
	if cmp -s my_ret correct_ret
	then
		echo -e "${GREEN}Correct Return Value${RESET}"
	else
		echo -e "${ERROR}Wrong return${RESET}"
		echo "My Return:"
		cat my_ret
		echo "Correct Return:"
		cat correct_ret
		echo 
	fi
	echo -e "${YELLOW}My error${RESET}"
	cat my_err
	echo -e "${YELLOW}Bash error${RESET}"
	cat bash_err
	echo
	rm -f correct_ret bash_err my_err my_ret correct_out my_out
}

echo -e "${BLUE}TEST 1${RESET}"
run_test "Test with valid parameters" "infile" "grep a1" "wc -w" "outfile" 

echo -e "${BLUE}TEST 2${RESET}"
run_test "Test with invalid infile" "infiled" "grep a1" "wc -w" "outfile"

echo -e "${BLUE}TEST 3${RESET}"
run_test "Test with invalid cmd_1" "infile" "gerp a1" "wc -w" "outfile"

echo -e "${BLUE}TEST 4${RESET}"
run_test "Test with invalid cmd_2" "infile" "grep a1" "wqc -w" "outfile"

echo -e "${BLUE}TEST 5${RESET}"
touch readonlyfile
chmod 0444 readonlyfile
run_test "Test with readonly outfile" "infile" "grep a1" "wc -w" "readonlyfile" 

echo -e "${BLUE}TEST 6${RESET}"
touch nonreadablefile
chmod u-r nonreadablefile
chmod g-r nonreadablefile
chmod o-r nonreadablefile
run_test "Test with nonreadable infile" "nonreadablefile" "grep a1" "wc -w" "outfile" 

echo -e "${BLUE}TEST 7${RESET}"
run_test "Test with invalid infile and invalid cmd_1" "infiled" "gerp a1" "wc -w" "outfile" 

echo -e "${BLUE}TEST 8${RESET}"
run_test "Test with readonly outfile and invalid cmd_2" "infile" "grep a1" "wqc -w" "readonlyfile" 

echo -e "${BLUE}TEST 9${RESET}"
run_test "Test with readonly outfile and invalid cmd_1" "infile" "gerp a1" "wc -w" "readonlyfile" 

echo -e "${BLUE}TEST 10${RESET}"
run_test "Test with readonly outfile and invalid infile" "infiled" "grep a1" "wc -w" "readonlyfile" 

rm -f readonlyfile
rm -f nonreadablefile
rm -f outfile