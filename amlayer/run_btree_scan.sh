rm *.o
rm a.out
rm -rf testrel.0
rm -rf testrel.1
rm -f testdata1.txt
rm -f output_from_btreescan.txt
make
./a.out > output_from_btreescan.txt
rm -f *.o
rm -f a.out
rm -rf testrel.0
rm -rf testrel.1
rm -f testdata1.txt
