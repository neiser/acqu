#!/bin/sh
# this setups the trigger such that it outputs the experiment trigger
# on debug output no 4, where the TRB3 is connected
# it should return the value 122 as hex = 0x7a
ssh acqu@a2cbtrigger 'cd /home/acqu/VUPROM && ./vmeext.sh 0xea002e40 122 w'
