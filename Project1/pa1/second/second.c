#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Node{
	struct Node* next;
	int val;
};

struct hashtableItem{
	struct Node *head;
};


int main (int argc, char** argv) {
	if (argc != 2) {
        printf("Unexpected number of arguments(%d)\n", argc);
	//input file name
    	exit(0);
	}
		
	
	FILE* fp = fopen(argv[1], "r"); //read file
	char action[5];
	int number;
	struct hashtableItem *hashtable = (struct hashtableItem*)malloc(sizeof(struct hashtableItem) * 11000);
	
	if(hashtable == NULL){
		printf("FAIL");
		exit(0);
	}
	for(int a = 0; a<10000;a++){
		hashtable[a].head = NULL;
	}
	while(fscanf(fp, "%s\t%d",action,&number) != EOF){
		int negativeNumber = number;
		if(number < 0){ 
			int r = number % 10000;
   		        negativeNumber = r < 0 ? r + 10000 : r;
		}
		if(strcmp(action,"s") == 0){
			struct Node* temp = (struct Node*)hashtable[negativeNumber%10000].head;
			while(temp != NULL){
				if(temp->val == number){
					printf("present\n");
					break;
				}
				else
					temp = temp->next;
			}
			if(temp == NULL)
				printf("absent\n");
		}
		else if (strcmp(action,"i") == 0){
			struct Node* temp = (struct Node*)hashtable[negativeNumber%10000].head;
			if(temp == NULL){
				struct Node* n = (struct Node*) malloc(sizeof(n)*2);
				n->val = number;
				n->next = NULL;
				(hashtable[negativeNumber%10000]).head = n;
				printf("inserted\n"); 
			}
			else{
				int found = 0;
				if(temp->val == number){
					printf("duplicate\n");
					found = 1;
				}
				while(temp->next != NULL && found == 0){
					if(temp->val == number){
						printf("duplicate\n");
						found = 1;
						break;
					}
					//printf("temp is: %d",temp->val);
					temp = temp->next;
				}
				if(found != 1){
					struct Node* n = (struct Node*) malloc(sizeof(n)*2);
					n->val = number;
					n->next = NULL;
					temp->next = n;
					printf("inserted\n");
				}
			}
		}
	//printf("%s,%d\n", action, number);
	}
	//printf("%d", (-5)%10000);
	free(hashtable);
	fclose(fp);
	return 0;	
}
