echo "MY OUTPUT"
./pipex infile "grep love"  "wc -l"  "awk '{printf (\"number of lines that has love is %d\", \$1)}'" outfile
echo $?
echo "SHELL OUTPUT"
< infile        grep love | wc -l | awk '{printf("number of lines that has love is %d", $1)}' > outfile2

echo $? 