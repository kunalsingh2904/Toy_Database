a.out : am.o amfns.o misc.o amsearch.o aminsert.o amstack.o amglobals.o main.o amscan.o amprint.o
	cc am.o misc.o amfns.o amsearch.o aminsert.o  amstack.o amglobals.o pflayer.out btree.o amscan.o amprint.o 

amlayer.o : am.o amfns.o amsearch.o aminsert.o amstack.o amglobals.o amscan.o amprint.o
	ld -r am.o amfns.o amsearch.o aminsert.o  amstack.o amglobals.o amscan.o amprint.o  -o amlayer.o

am.o : am.c am.h pf.h
	cc -c am.c

amfns.o : amfns.c am.h pf.h
	cc -c amfns.c

amsearch.o : amsearch.c am.h pf.h
	cc -c amsearch.c

aminsert.o : aminsert.c am.h pf.h
	cc -c aminsert.c

amscan.o : amscan.c am.h pf.h testam.h
	cc -c amscan.c

amstack.o : amstack.c am.h pf.h
	cc -c amstack.c

amglobals.o : amglobals.c am.h
	cc -c amglobals.c

amprint.o : amprint.c am.h pf.h 
	cc -c amprint.c
	
main.o : btree.c am.h pf.h 
	cc -c btree.c

misc.o : misc.c
	cc -c misc.c

