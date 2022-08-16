
echo "sgfefeqd qwdqwdddafasd s+64 65 30 9 2 92 ad asd lim asd as d asd\n asdasd \nlim\n a dasd sd \n asdasd asdasdsbvfsbvsdd asd asd\n afdsvdsvasdqpkqwd ew edfsb\nlim2\nfdbdfbdfbfsbdfbqckqwxqwx qw \n qfqwodeqm domw	m	\tefeqc eqew cewx ewc ewc ewc wc ew" > in_file
echo "sgfefeqd qwdqwdddafasd s+64 65 30 9 2 92 ad asd lim asd as d asd\n asdasd " > in_file1
echo "sgfefeqd qwdqwdddafasd s+64 65 30 9 2 92 ad asd lim asd as d asd\n asdasd \nlim\n a dasd sd \n asdasd asdasdsbvfsbvsdd asd asd\n afdsvdsvasdqpkqwd ew edfsb" > in_file2

timeout_2() {
	( < in_file $1 "$2" "$3" "$4" "$5" "$6" "$7"  "$8" "$9" &> /dev/null ) & pid=$!
	( sleep 8 && kill -HUP $pid ) 2>/dev/null & watcher=$!
	if wait $pid 2>/dev/null; then
		status="finished"
		pkill -HUP -P $watcher
		wait $watcher
	else
		status="interrupted"
	fi
	}


timeout_2 "$1" here_doc lim "cat" "wc -cl" "grep 0" "sort" "cat" "outfile"
< in_file1 cat | wc -cl |  grep 0  | sort | cat >> outfile2
different=`diff outfile outfile2`
if [ "$status" = "finished" ] && [ "$different" = "" ]
	then
	echo "${GREEN}[OK]${RESET}\n"
else
	echo "${RED}[KO]${RESET}\n"
fi