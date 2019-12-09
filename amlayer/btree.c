#include <stdio.h>
#include "am.h"
#include "testam.h"
#include "pf.h"

#define MAX_FNAME_LENGTH 80	/* max length for file name */

main()
{
int id0; /* index descriptor */
int sd0; /* scan descriptors */
int i;
RecIdType recid;	/* record id */
char buf[NAMELENGTH]; /* buffer to store chars */
char fnamebuf[MAX_FNAME_LENGTH];	/* file name buffer */
int recnum;	/* record number */
int numrec;		/* # of records retrieved*/

	/* init */
	PF_Init();
    
    FILE *fp;
    fp = fopen("inputcharfile.txt", "r");
    fflush(fp);
    char line[512];
    
	/* create index on the both field of the record*/
	//printf("creating indices\n");
	xAM_CreateIndex(RELNAME,RECNAME_INDEXNO,CHAR_TYPE,NAMELENGTH);
	

	/* open the index */
	//printf("opening indices\n");
	sprintf(fnamebuf,"%s.%d",RELNAME,RECNAME_INDEXNO);
	id0 = xPF_OpenFile(fnamebuf);
	
	/* insert into index on character */
	//printf("inserting into index on char\n");
	while ( fgets ( line, NAMELENGTH, fp ) != NULL ) {
		xAM_InsertEntry(id0,CHAR_TYPE,NAMELENGTH,line,IntToRecId(recnum));
    }

	sd0 = xAM_OpenIndexScan(id0,CHAR_TYPE,NAMELENGTH,EQ_OP,NULL);
	numrec = 0;
    char nextKey[512];

	while((recnum=RecIdToInt(xAM_FindNextKey(sd0, nextKey)))>= 0){
		printf("%s\n",nextKey);
		numrec++;
	}

	/* destroy everything */
	xAM_CloseIndexScan(sd0);
	xPF_CloseFile(id0);
	xAM_DestroyIndex(RELNAME,RECNAME_INDEXNO);

    printf("Number of seeks required in btree scan for reads: %d\n", nReadSeeks);
    printf("Number of seeks required in btree scan for writes: %d\n", nWriteSeeks);
    printf("Number of transfers required in btree scan for reads: %d\n", nReadTransfers);
    printf("Number of transfers required in btree scan for writes: %d\n", nWriteTransfers);
    
	//printf("test1 done!\n");
}
