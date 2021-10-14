#!/bin/bash
g++ test_gen.cpp -o gen
g++ A2.cpp -o my
#g++ naive.cpp -o sol

#./gen > in && ./my < in > out1 && ./sol < in > out2
./gen > in && ./my < in
#declare -i i=0
#DIFF=$(diff out1 out2)

while [ $? -eq 0 ] 
do
	echo 'Test #'$i passed!
	i=$((i+1))
    #./gen > in && ./my < in > out1 && ./sol < in > out2
	./gen > in && ./my < in
	#DIFF=$(diff out1 out2)
done

#echo $DIFF
echo "Your test is found :)"
