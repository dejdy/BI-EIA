rm -rf in.txt;
max=100000;
maxNum=10000;
for (( i=0; i<max; i++))
do 
	echo $((RANDOM % maxNum+1))>>"in.txt"
done
