rm -f output_from_btreescan.txt
rm -f output_from_mergesort.txt
cd ./pflayer
rm -f output_from_mergesort.txt
./run_mergesort.sh
cp output_from_mergesort.txt ..
cd ../amlayer
rm -f output_from_btreescan.txt
./run_btree_scan.sh
cp output_from_btreescan.txt ..
