#include <stdlib.h>
#include <stdio.h>

void swap(int *xp, int *yp){
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

int main (int argc, char** argv) {
	if (argc != 2) {
        printf("Unexpected number of arguments(%d)\n", argc);
		//input file name
    	}
	
	FILE* fp = fopen(argv[1], "r"); //read file
    int num;
	fscanf(fp, "%d", &num);
	//printf("size is: %d\n", num);
	
	int *arrNum = (int *)malloc(sizeof(int)*(num+1));
	int n = 0,even = 0, odd = 0;
	while(fscanf(fp, "%d", &num) != EOF){
		arrNum[n] = num;
		n++;
		//printf("num is: %d\n", arrNum[n-1]);
		if(arrNum[n-1] % 2 == 0)
			even++;
		else
			odd++;
	}
	
	int *arrNumOdd = (int *)malloc((odd+1)*sizeof(int));
	int *arrNumEven = (int *)malloc((even+1)*sizeof(int));
	
	even = 0;
	odd = 0;
	for(int a = 0; a < n; a++){
		if(arrNum[a]%2 == 0){
			arrNumEven[even] = arrNum[a];
			even++;
			//printf("even is: %d\t", arrNumEven[even-1]);
		}
		else{
			arrNumOdd[odd] = arrNum[a];
			odd++;
			//printf("odd is %d\t", arrNumOdd[odd-1]);
		}
	}
	
	free(arrNum);
	//printf("odd is: %d, even is %d\n", odd, even);
	//printf("\n");
	
	int ct = 0,ct2 = 0,hold = 0;
	for(ct = 0; ct < odd-1; ct++){
		hold = ct;
		for(ct2 = ct+1; ct2 < odd; ct2++){
			if(arrNumOdd[ct2] < arrNumOdd[hold]){
				hold = ct2;
			}
		}
		printf("%d\t",arrNumOdd[hold]);
		swap(&arrNumOdd[hold], &arrNumOdd[ct]);
	}
		printf("%d\t", arrNumOdd[odd-1]);
	
	for(ct = 0; ct < even-1; ct++){
		hold = ct;
		for(ct2 = ct+1; ct2 < even; ct2++){
			if(arrNumEven[ct2] > arrNumEven[hold])
				hold = ct2;
		}
	
		printf("%d\t",arrNumEven[hold]);
		swap(&arrNumEven[hold], &arrNumEven[ct]);
	}

		printf("%d", arrNumEven[even-1]);
	
	printf("\n");
	free(arrNumEven);
	free(arrNumOdd);
	fclose(fp);
	return 0;	
}
