# to debug shell script put this in first line:  #!/bin/sh -x

make -f makefile.1
clear

echo
echo "calling user is: \"`id -un`\" at: `date`"

EXECUTABLE=./myserver
ERROR_LOGFILE=error_logfile

# uncomment this to print debug statements
#SBB_DEBUG=1; export SBB_DEBUG

echo "======= spawning executable ======="
$EXECUTABLE  2> $ERROR_LOGFILE >&1
# unix shell built in environment variable for return code is $?
# standard is to return 0 if successful, positive otherwise
ret=$?

# standard unix practice is if an executable returns a non-zero code then it failed 
if [ $ret -ne 0 ]
then
	echo my program returned non-zero return code: $ret
	echo "prog failed"
	[ -s $ERROR_LOGFILE ] && cat $ERROR_LOGFILE
	exit
fi
