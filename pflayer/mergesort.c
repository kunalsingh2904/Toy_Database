#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "pf.h"
#include <string.h>
#include <math.h>
#include "pftypes.h"

#define MERGESORTFILE "mergesortfile"
#define filename "inputcharfile.txt"

int MAXRECSPERPAGE = (PF_PAGE_SIZE - si) / (ATTRSIZE + si);

void create_input_pages() {
    int totalRecsAdded;
    int error;
    
    if ((error=PF_CreateFile(MERGESORTFILE))!= PFE_OK){
        PF_PrintError( "Error in creating mergesortfile");
        exit(1);
    }
    // printf("mergesortfile created\n");
    
    int fd,pagenum;
    char *buf;
    
    pagenum = -1;
    
    if ((fd=PF_OpenFile(MERGESORTFILE))<0){
        PF_PrintError("Error in opening mergesortfile");
        exit(1);
    }
    //-printf("opened %s\n", MERGESORTFILE);
    
    if ((error=PF_AllocPage(fd,&pagenum,&buf))!= PFE_OK) {
        PF_PrintError("ERROR: failed allocating buffers in create_input_pages\n");
        exit(1);
    }
    printf("allocated page %d in create_input_pages\n",pagenum);
    // printf("TEst0\n");
    FILE *file = fopen ( filename, "r" );
    // printf("TEst1\n");
    //printf("f4f42");
    fflush(file);
    if ( file != NULL ) {
        char line[MAXATTRSIZE];
        
        int recsAdded = 0;
        totalRecsAdded = 0;
        while ( fgets ( line, ATTRSIZE, file ) != NULL ) {
            //puts(line);
            //printf("%d \n", strlen(line));
            if(recsAdded == MAXRECSPERPAGE) {
                if ((error=PF_UnfixPage(fd, pagenum, TRUE))!= PFE_OK) {
                    PF_PrintError("Error in disposing page of mergesortfile");
                    exit(1);
                }
                printf("deallocated page %d in create_input_pages\n",pagenum);
                if ((error=PF_AllocPage(fd,&pagenum,&buf))!= PFE_OK) {
                    PF_PrintError("ERROR: failed allocating buffers in create_input_pages\n");
                    exit(1);
                }
                printf("allocated page %d in create_input_pages\n",pagenum);
                recsAdded = 0;
                bcopy(&recsAdded, buf, si);
            }
            bcopy(line, buf + si + recsAdded * (ATTRSIZE + si), ATTRSIZE);
            
            totalRecsAdded++;
            bcopy((char*)&totalRecsAdded, buf + si + recsAdded * (ATTRSIZE + si) + ATTRSIZE, si);
            int abc=recsAdded+1;
            bcopy(&abc, buf, si);
                        
            // printf("%s ", line);
            // bcopy(buf + si + recsAdded * (ATTRSIZE + si), line, ATTRSIZE);
            // bcopy(buf + si + recsAdded * (ATTRSIZE + si) + ATTRSIZE, (char*)&totalRecsAdded, si);
            // bcopy(buf, &recsAdded, si);
            recsAdded++;
//            printf("%s %d %d\n", line, totalRecsAdded, recsAdded);
            
        }
       //- printf("Totally %d records were added\n", totalRecsAdded);
        fclose ( file );
    }
    else {
        perror ( filename ); /* why didn't the file open? */
    }
    if ((error=PF_UnfixPage(fd, pagenum, TRUE))!= PFE_OK){
        PF_PrintError("Error in disposing page of mergesortfile");
        exit(1);
    }
    printf("deallocated page %d in create_input_pages\n", pagenum);
    if ((error=PF_CloseFile(fd))!= PFE_OK) {
        PF_PrintError("Error in closing mergesortfile---");
        exit(1);
    }
    //-printf("mergesortfile closed\n");
    
    
}


readfile(fname)
char *fname;
{
    int error;
    char *buf;
    int pagenum;
    int fd;
    
    // printf("opening %s\n",fname);
    if ((fd=PF_OpenFile(fname))<0){
        // printf("%d\n", fd);
        PF_PrintError("open file");
        exit(1);
    }
    printfile(fd);
    if ((error=PF_CloseFile(fd))!= PFE_OK){
        PF_PrintError("close file");
        exit(1);
    }
}

printfile(fd)
int fd;
{
    int error;
    char *buf;
    int pagenum;
    
    // printf("reading file\n");
    pagenum = -1;
    while ((error=PF_GetNextPage(fd,&pagenum,&buf))== PFE_OK){
        int recsInPage;
        bcopy(buf,(char *)&recsInPage,si);
        //printf("%d\n", recsInPage);
        char key[MAXATTRSIZE];
        int value;
        
        int k;
        for(k=0; k<recsInPage; k++) {
            bcopy(buf + si + k * (ATTRSIZE + si), key, ATTRSIZE);
            bcopy(buf + si + k * (ATTRSIZE + si) + ATTRSIZE, &value, si);
            // printf("%s\n", key);
        }
        
        //-printf("got page %d, %d\n",pagenum,recsInPage);
        
        if ((error=PF_UnfixPage(fd,pagenum,FALSE))!= PFE_OK){
            PF_PrintError("unfix");
            exit(1);
        }
    }
    if (error != PFE_EOF){
        PF_PrintError("not eof\n");
        exit(1);
    }
    //-printf("eof reached\n");
}

struct Record {
    char key[ATTRSIZE];
    int value;
};

int stringcmp(const void *a, const void *b)
{
    const char **ia = (const char **)((struct Record*)a)->key;
    const char **ib = (const char **)((struct Record*)b)->key;
    //return 1;
    //printf("%s %s\n", ((struct Record*)a)->key, ((struct Record*)b)->key);
    return strcmp(((struct Record*)a)->key, ((struct Record*)b)->key);
}

create_runs(fname, no_of_runs, pages_in_last_run)
char *fname;
int *no_of_runs;
int *pages_in_last_run;

{
    int error;
    char *buf;
    int pagenum;
    int fd;
    
    
    // printf("opening %s\n",fname);
    if ((fd=PF_OpenFile(fname))<0){
        PF_PrintError("open file");
        exit(1);
    }
   
    pagenum = -1;
    *no_of_runs = 0;
    
    int done;
    done = 0;
    // printf("test5\n");
    while(1) {
        *pages_in_last_run = 0;
        (*no_of_runs)++;
        
        
        struct Record recsInMem[(PF_MAX_BUFS-1)*MAXRECSPERPAGE];
        int pagesInMem[(PF_MAX_BUFS-1)];
        
        int i, totRecNo;
        totRecNo = 0;
        for(i=0; i<(PF_MAX_BUFS-1); i++) {
            
            error=PF_GetNextPage(fd,&pagenum,&buf);
            //-printf("Page being read is %d\n", pagenum);
            if(error!=PFE_OK) {
                done = 1;
                // printf("No more pages to read\n");
                break;
            }
            // printf("ok1\n");
            
            
            
            pagesInMem[i] = pagenum;
            int recsInPage;
            bcopy(buf,(char *)&recsInPage,si);
            // printf("%d records in current page\n", recsInPage   );
            char key[ATTRSIZE];
            int value;
            int j;
            for(j=0; j<recsInPage; j++) {
                bcopy(buf+si+(ATTRSIZE + si)*j,key,ATTRSIZE);
                bcopy(buf+si+(ATTRSIZE + si)*j+ATTRSIZE,&value,si);
                //printf("%s, %d\n", key, value);
                strcpy(recsInMem[totRecNo].key, key);
                recsInMem[totRecNo].value = value;
                //recsInMem[totRecNo] = (struct Record) {.key=key, .value=value };
                totRecNo++;
            }
            (*pages_in_last_run)++;
            // printf("pages_in_last_run are %d \n", *pages_in_last_run);
        }
        // printf("ok2\n");
        qsort(recsInMem, totRecNo, sizeof(recsInMem[0]), stringcmp);

        // testing sort
        // int k;
        // for(k=0; k<totRecNo; k++) {
        //     printf("%d %s %d \n", totRecNo, recsInMem[k].key, recsInMem[k].value);
        // }

        for(int k=0; k<*pages_in_last_run; k++) {
            if ((error=PF_UnfixPage(fd,pagesInMem[k],FALSE))!= PFE_OK){
                // printf("%d", k);
                PF_PrintError("unfix");
                exit(1);
            }
            // printf("deallocated page %d %d \n",k, *pages_in_last_run);
        }
        // printf("ok3\n");
    
        char run_name[20];
        sprintf(run_name, "pass1.run%d", (*no_of_runs)-1);
    
        if ((error=PF_CreateFile(run_name))!= PFE_OK){
            PF_PrintError( "Error in creating run_name");
            exit(1);
        }
        // printf("run_name%d created\n", *no_of_runs);
        
        int fd2;
        
        //-printf("opening %s\n",run_name);
        if ((fd2=PF_OpenFile(run_name))<0){
            PF_PrintError("open file");
            exit(1);
        }
        // printf("ok4\n");
        int no_of_records_to_be_added;
        no_of_records_to_be_added = totRecNo;
        // printf("ok5\n");
        int m;
        char *buf2;
        int pagenum2;
        for(m=0; m<(*pages_in_last_run); m++) {
            if ((error=PF_AllocPage(fd2,&pagenum2,&buf2))!= PFE_OK) {
                PF_PrintError("ERROR: failed allocating buffers in create_input_pages\n");
                exit(1);
                // printf("test4\n");;
            }
            // printf("allocated page %d in create_runs\n",pagenum2);
            
            int z;
            for(z=0; z<MAXRECSPERPAGE; z++) {
//                printf("%d %d %d %d %d just printed %s\n", z+m*MAXRECSPERPAGE, z, m*MAXRECSPERPAGE, m, MAXRECSPERPAGE,  recsInMem[z+m*MAXRECSPERPAGE].key);
                bcopy(recsInMem[z+m*MAXRECSPERPAGE].key, buf2 + si + z * (ATTRSIZE + si), ATTRSIZE);
                bcopy(&recsInMem[z+m*MAXRECSPERPAGE].value, buf2 + si + z * (ATTRSIZE + si) + ATTRSIZE, si);
                
            }
            bcopy(&z, buf2, si);
            if ((error=PF_UnfixPage(fd2, pagenum2, TRUE))!= PFE_OK){
                PF_PrintError("Error in disposing page of mergesortfile");
                exit(1);
            }
            //-printf("deallocated page %d in create_runs\n",pagenum2);
        }
        
        
        if ((error=PF_CloseFile(fd2))!= PFE_OK){
            PF_PrintError("close file");
            exit(1);
        }
        //-printf("closed %s\n",run_name);
        
        if(done) {
            if(*pages_in_last_run==0) *no_of_runs = *no_of_runs - 1;
            break;
        }
        
    }

    if ((error=PF_CloseFile(fd))!= PFE_OK){
        PF_PrintError("close file");
        exit(1);
    }
    // printf("bye1\n");
}

merge_runs(cur_pass, cur_blocks_per_run, cur_no_of_runs)
int *cur_pass;
int *cur_blocks_per_run;
int *cur_no_of_runs;
{
    int error;
    int i;
    for(i=0; i<(*cur_no_of_runs/ (PF_MAX_BUFS-1)); i++) {
    //for(i=0; i<1; i++) {
        char run_name[20];
        sprintf(run_name, "pass%d.run%d", *cur_pass, i);
        
        if ((error=PF_CreateFile(run_name))!= PFE_OK){
            PF_PrintError( "Error in creating run_name");
            exit(1);
        }
        //-printf("run-%d created\n", i);
        
        int fd;
        
        //-printf("opening %s\n",run_name);
        if ((fd=PF_OpenFile(run_name))<0){
            PF_PrintError("open file");
            exit(1);
        }
        
        int j;
        //int fileDescArr[PF_MAX_BUFS-1];
        int *fileDescArr = malloc(sizeof(int)*(PF_MAX_BUFS-1));
        char *buf3[PF_MAX_BUFS-1];
        int pagenos[PF_MAX_BUFS-1];
        
        int recsRead[PF_MAX_BUFS-1];
        
        for(j=0; j<PF_MAX_BUFS-1; j++) {
            pagenos[j] = -1;
            recsRead[j]=0;
        }
        
        for(j=0; j<PF_MAX_BUFS-1;j++) {
            

            
            char run_name[20];
            sprintf(run_name, "pass%d.run%d", (*cur_pass)-1, j+i*(PF_MAX_BUFS-1));
            //-printf("opening %s\n",run_name);
            if ((fileDescArr[j]=PF_OpenFile(run_name))<0){
                PF_PrintError("open file f34wg34");
                exit(1);
            }
            
            
            if ((error=PF_GetNextPage(fileDescArr[j],&pagenos[j],&buf3[j]))!= PFE_OK) {
                PF_PrintError("ERROR: failed getting next page\n");
                exit(1);
            }
            
            //printf("%d %d %d \n ", j, fileDescArr[j], pagenos[j]);
            

        }
        
        for(j=0; j<(*cur_blocks_per_run)*(PF_MAX_BUFS-1); j++) {
            //printf("ercver %d \n", j);
            char *buf4;
            int page;
            if ((error=PF_AllocPage(fd,&page,&buf4)!= PFE_OK)) {
                PF_PrintError("ERROR: failed allocating buffers in create_input_pages\n");
                exit(1);
            }
//            printf("allocated %d\n", j);
            int k;
            for(k=0; k<MAXRECSPERPAGE; k++) {
                //-printf("wrote rec %d\n", k);
                int l;
                char smallestKey[MAXATTRSIZE] = "zzzzzzzzz";
                int value_assoc_with_smallest_key;
                
                char *tempKey = malloc(ATTRSIZE*sizeof(char));
                int temp_value_assoc_with_smallest_key;
                int index_with_smallest_key = 0;
                
                for(l=0; l<PF_MAX_BUFS-1; l++) {
                    // printf("%d -- %d \n", pagenos[l]!=(*cur_blocks_per_run) || recsRead[l]!=MAXRECSPERPAGE, l);
                    if(pagenos[l]!=((*cur_blocks_per_run)-1) || recsRead[l]!=MAXRECSPERPAGE) {
                        bcopy(buf3[l] + si + recsRead[l] * (ATTRSIZE + si), tempKey, ATTRSIZE);
                        bcopy(buf3[l] + si + recsRead[l] * (ATTRSIZE + si) + ATTRSIZE, &temp_value_assoc_with_smallest_key, si);
                    
                        int comp = strcmp(tempKey, smallestKey);
                        if(comp<0) {
                            strcpy(smallestKey, tempKey);
                            value_assoc_with_smallest_key = temp_value_assoc_with_smallest_key;
                            index_with_smallest_key = l;
                        }
                    }
                }

                recsRead[index_with_smallest_key]++;
                //printf("%d %d %d %d %s\n", k, index_with_smallest_key, recsRead[index_with_smallest_key], pagenos[index_with_smallest_key], smallestKey);
    
                
                bcopy(smallestKey, buf4 + si + k * (ATTRSIZE + si), ATTRSIZE);
                bcopy(&value_assoc_with_smallest_key, buf4 + si + k * (ATTRSIZE + si) + ATTRSIZE, si);
                
                
                
                if(recsRead[index_with_smallest_key]==MAXRECSPERPAGE) {
                    if ((error=PF_UnfixPage(fileDescArr[index_with_smallest_key], pagenos[index_with_smallest_key], TRUE))!= PFE_OK){
                        //printf("index with smallest key %d %d %d", index_with_smallest_key, pagenos[index_with_smallest_key], fileDescArr[index_with_smallest_key]);
                        PF_PrintError("Error in disposing page of mergesortfile, error here\n");
                        exit(1);
                    }
                    
                    if ((error=PF_GetNextPage(fileDescArr[index_with_smallest_key], &pagenos[index_with_smallest_key],&buf3[index_with_smallest_key]))!= PFE_OK) {
                        //printf("%d EOF", index_with_smallest_key);
                    }
                    else {
                        //printf("I WAS HERE for %d -- %d -- %d -- %d \n", index_with_smallest_key, pagenos[index_with_smallest_key], j, fileDescArr[index_with_smallest_key]);
                        recsRead[index_with_smallest_key] = 0;
                    }
                }
                

            }
            
            bcopy(&k, buf4, si);
        
            if ((error=PF_UnfixPage(fd, page, TRUE))!= PFE_OK){
                PF_PrintError("Error in disposing page of mergesortfile");
                exit(1);
            }
            
            
            //printf("cf4rc %d", page);
            
        }
        
        
        
        for(j=0; j<PF_MAX_BUFS-1;j++) {

        // if ((error=PF_UnfixPage(fileDescArr[j], pagenos[j], TRUE))!= PFE_OK){
        //     PF_PrintError("Error in disposing page of mergesortfile");
        //     exit(1);
        // }

            
            if ((error=PF_CloseFile(fileDescArr[j]))!= PFE_OK){
                PF_PrintError("close file");
                exit(1);
            }
        }
        
        if ((error=PF_CloseFile(fd))!= PFE_OK){
            PF_PrintError("close file");
            exit(1);
        }
     //-   printf("closed %s\n",run_name);


    }

}


int main() {
    // printf("test3\n");
    create_input_pages();
    int cur_no_of_runs;
    int pages_in_last_run;
    // printf("test4\n");
    
    create_runs(MERGESORTFILE, &cur_no_of_runs, &pages_in_last_run);
    
    // printf("No. of pages found were %d\n", cur_no_of_runs);
    // printf("%d\n", PF_MAX_BUFS);

    float no_of_passes = log(cur_no_of_runs)/ log(PF_MAX_BUFS-1);
    // printf("%.6f\n", no_of_passes);
    int cur_blocks_per_run;
    cur_blocks_per_run = PF_MAX_BUFS-1;
    // printf("ok7\n");
    int cur_pass;
    cur_pass = 2;
    for(float i=0; i<no_of_passes; i++) {
        // printf("going to merge\n");
        merge_runs(&cur_pass, &cur_blocks_per_run, &cur_no_of_runs);
        // printf("ok8\n");
        cur_blocks_per_run = cur_blocks_per_run * (PF_MAX_BUFS-1);
        cur_no_of_runs = cur_no_of_runs / (PF_MAX_BUFS-1);
        cur_pass = cur_pass + 1;
    }
    // printf("ok9\n");
    
    readfile("pass1.run0");

    printf("Number of seeks required in mergesort for reads: %d\n", nReadSeeks);
    printf("Number of seeks required in mergesort for writes: %d\n", nWriteSeeks);
    printf("Number of transfers required in mergesort for reads: %d\n", nReadTransfers);
    printf("Number of transfers required in mergesort for writes: %d\n", nWriteTransfers);
    return 0;
}