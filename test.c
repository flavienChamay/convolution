#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int comparaisonMedian(const void * a, const void * b) {
	return (int)(*(unsigned char*)a - *(unsigned char*)b);
}

//The median is supposed to be 

int main(){
	unsigned char * median = (unsigned char *)malloc(sizeof(unsigned char) * 3 * 3);
	median[0] = 99;
	median[1] = 23;
	median[2] = 56;
	median[3] = 126;
	median[4] = 121;
	median[5] = 120;
	median[6] = 97;
	median[7] = 12;
	median[8] = 9;
	//~ qsort(median, 9, sizeof(unsigned char), comparaisonMedian);
	//~ for(int i = 0; i < 9; i++){
		//~ printf("median[%d] = %d \n", i, median[i]);
	//~ }
	int H[256];
	int C[256];
	
	//filling H and C with zeros
	for(int i = 0; i < 256; i++){
		H[i] = 0;
		C[i] = 0;
	}
	//compute H
	for(int i = 0; i < 9; i++){
		H[median[i]]++;
	}
	//compute C and stop when we are on the median
	C[0] = H[0];
	int temp = 0;
	int i = 1;
	while(temp == 0){
		C[i] = C[i-1] + H[i];
		if(C[i] >= (9 / 2) + 1){
			break;
		}
		i++;
	}
	printf("indice i = %d", i);
	printf("median = %d", i);
	return 0;
}
