rm -rf in.txt;
max=10000000;
for (( i=0; i<max; i++))
do 
	echo $((max-i))>>"in.txt"
done

