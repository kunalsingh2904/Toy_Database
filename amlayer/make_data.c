#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>


int main() {
	FILE *fp;
	fp = fopen("testdata1.txt", "w");
	srand(time(NULL));
	for(int i=0; i<5000; i++) {
		fprintf(fp, "%d\n", rand()%5000);
	}
	return 0;
}
