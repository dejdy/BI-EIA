rm -rf in.txt;
max=100000;
for (( i=0; i<max; i++ ))
do
	echo $((i))>>"in.txt"
done

