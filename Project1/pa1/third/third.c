#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char** argv) {
	if (argc != 2) {
        printf("Unexpected number of arguments(%d)\n", argc);
	//input file name
    }
		
	
	FILE* fp = fopen(argv[1], "r"); //read file
        int num;
	fscanf(fp, "%d", &num);
	

	char word[5];
	int num1,num2;	

	while(fscanf(fp, "%s\t%d\t%d", word, &num1, &num2)!= EOF){
		//printf("%s, %d, %d\n", word, num1, num2);
		if(strcmp(word,"set") == 0){
			num ^= (-(unsigned short)num2 ^ num) & (1 << num1);
			printf("%d\n",num);
			
		}
		else if(strcmp(word,"get") == 0){
			
			printf("%d\n", (num >> num1) & 1);
		}
		else if(strcmp(word,"comp") == 0){
			num ^= 1 << num1;
			printf("%d\n", num);
		}
	}
	
	fclose(fp);
	return 0;
	
}
