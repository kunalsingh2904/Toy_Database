rm -f pass*
rm -f file1
rm -f file2
rm -f *.o
rm -f testpf
rm -f testpf1
rm -f mergesortfile
make
# make testpf.o
# make testpf
# make testpf1.o
# make testpf1
# make testing.o
make mergesort
./mergesort > output_from_mergesort.txt
rm -f ../amlayer/pflayer.out
cp pflayer.o ../amlayer/pflayer.out
rm -f pass*
rm -f file1
rm -f file2
rm -f *.o
rm -f testpf
rm -f testpf1
rm -f mergesortfile

