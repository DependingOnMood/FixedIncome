#!/bin/sh

echo "start server"
# start server here in backgound (append with &)
./myserver&

a=1
while [ $a -le 100 ]
do
	echo "pass: $a"
	# call the student's client inside this loop
    ./myclient
	a=`expr $a + 1`
done
