#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "pf.h"

static char *rand_string(char *str) {
    int size;
    size = ATTRSIZE-1;
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    size_t n;
    for (n = 0; n < size; n++) {
        int key = rand() % (int) (sizeof charset - 1);
        str[n] = charset[key];
    }
    str[ATTRSIZE-1] = '\0';
    return str;
}

int main ( void ) {
    static const char filename[] = "inputcharfile.txt";
    FILE *file = fopen ( filename, "w" );
    srand(time(NULL));
    char s[ATTRSIZE];
    int i;//35447312
    for(i=0; i<1865648; i++) {
        rand_string(s);
        fprintf(file, "%s", s);
    }
    fclose(file);
}


//int main ( void ) {
//    static const char filename[] = "inputcharfile.txt";
//    static const char filename2[] = "output_from_mergesort.txt";
//    FILE *file = fopen ( filename, "w" );
//    FILE *file2 = fopen ( filename2, "r" );
//    srand(time(NULL));
//    char line[ATTRSIZE];
//    char line2[ATTRSIZE];
//    int i;//35447312
//    while ( fgets ( line, ATTRSIZE, file2 ) != NULL ) {
//        //rand_string(s);
//        //puts(line);
//        int r;
//        for(r=1; r<ATTRSIZE-1; r++) {line2[r]=line[r];// printf("%d %c ", r, line[r]);
//        }
//        printf("\n");
//        line2[ATTRSIZE-1] = '\0';
//        for(r=1; r<=ATTRSIZE-1; r++) {
//            printf("%c", line[r]);
//        }
//        //printf("%s", line2);
//    }
//    fclose(file2);
//    fclose(file);
//}

//   while ( fgets ( line, ATTRSIZE, file ) != NULL ) {

//    
//   FILE *file = fopen ( filename, "r" );
//   if ( file != NULL )
//   {
//      char line [ 128 ]; /* or other suitable maximum line size */
//      while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
//      {
//         fputs ( line, stdout ); /* write the line */
//      }
//      fclose ( file );
//   }
//   else
//   {
//      perror ( filename ); /* why didn't the file open? */
//   }
//   return 0;
//}
