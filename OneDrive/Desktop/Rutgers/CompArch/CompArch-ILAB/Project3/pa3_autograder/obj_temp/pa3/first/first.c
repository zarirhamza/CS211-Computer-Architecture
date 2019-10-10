#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
struct Address {
	long long tag;
	long long set;
	long long time;
}; 

struct Cache{
	struct Address* line;
} ;

long long bitExtracted(long long value, long long begin, long long end)
{
    long long one = 1;	
    long long mask = (one << (end - begin)) - one;
    //printf("%llu \n", mask);
    return (value >> begin) & mask;
}

int compareString(char* a, char* b){
	if(strcmp(a,b) == 0)
		return 1;
	return 0;

}
int powerof2(int num){
	double check = log2(num);
	if (ceil(check) == check && floor(check) == check)
		return 1;
	return 0;
}

int checkString (char* str){
	for(int a = 0; a < strlen(str); a++){
		if(isdigit(str[a]) == 0){
			return 0;}
	}
	return 1;
}

int main (int argc, char** argv){
	FILE* fp = fopen(argv[5], "r");
	char command[5];
	long long address;
	long long pc = 0;
	

	char* test = calloc(strlen(argv[4]) + 1, sizeof(char));
	strcpy(test, argv[4]);	
	//check if input is ok
        if(powerof2(atoi(argv[1])) && powerof2(atoi(argv[2])) && (compareString(argv[3],"fifo") || compareString(argv[3],"lru"))){
                if(compareString(argv[4], "direct") || compareString(argv[4], "assoc"))
                        ;
                else if(strncmp(argv[4], "assoc:", 6) == 0){
			strtok(test, ":");
			test = strtok(NULL, ":");
                        if(powerof2(atoi(test)) && checkString(test)){
                                //printf("%s \n", (test));
                        }
                        else{
                                printf("error\n");
                                return 0;
                         }
                }
                else{
                        printf("error\n");
                        return 0;
                }
        }
        else {printf("error\n"); return 0;}

	struct Cache* cache = (struct Cache*)malloc(sizeof(struct Cache*) * (atoi(argv[1])/atoi(argv[2])));
	for(int a = 0; a < (atoi(argv[1])/atoi(argv[2]));a++){
		cache[a].line = NULL;
	}
	//direct
	//associative
	//assoc:n
	
	double mread = 0;
	double mwrite = 0;
	double chit = 0;
	//double errors = 0;
	//ASK IF MREAD = CMISS
	
	
	fscanf(fp, "%s ", command);
	if(strcmp(argv[4],"direct") == 0){
		double offset = log2(atoi(argv[2]));
		double set = log2(atoi(argv[1])/atoi(argv[2]));
		double tag = 48 - offset - set;	
		
		while(strcmp(command, "#eof") != 0){
			fscanf(fp,"%llx",&address);
			//first t bits are tag
			//next s bits are set
			//ignore last couple
			struct Address* temp = (struct Address*)malloc(sizeof(struct Address));
			temp->tag = bitExtracted(address,48-tag,48);
			temp->set = bitExtracted(address,offset,48-tag);
			temp->time = pc;
			pc++;			

			if(strcmp(command,"R") == 0){ // assume direcT
				if(cache[temp->set].line == NULL || cache[temp->set].line->tag != temp->tag){
					mread++;
					cache[temp->set].line = temp;
				}
				else{ 
					chit++;
				}				
			}
			else if (strcmp(command, "W") == 0){
				if(cache[temp->set].line == NULL || cache[temp->set].line->tag != temp->tag){
					mread++;
					mwrite++;
					cache[temp->set].line = temp;
				}
				else{
					chit++;
					mwrite++;
				}
			}
			fscanf(fp, "%s", command);
		}	
	}
	else if(strcmp(argv[4],"assoc") == 0){
		double offset = log2(atoi(argv[2]));
		double set = log2(atoi(argv[1])/atoi(argv[2]));
		double tag = 48 - offset;
		
		int found = 0;
		double error = 0;
		double pos = 0;
		
		while(strcmp(command, "#eof") != 0){
			fscanf(fp, "%llx", &address);
			
			struct Address* temp = (struct Address*)malloc(sizeof(struct Address));                                                                                                                         temp->tag = bitExtracted(address,48-tag,48);
                        temp->set = bitExtracted(address,offset,48-tag);
                        temp->time = pc;
			pc++;
			
			error = pc;
			pos = 0;
			if(strcmp(command, "R") == 0){
				for(int a = 0; a < pow(2,set); a++){
					if(cache[a].line == NULL){
						if(error != -1){
						error =  -1;
						pos = a;
						}
					}
					else if(cache[a].line->time < error){
						error = cache[a].line->time;
						pos = a;
					}

					if(cache[a].line!= NULL && cache[a].line->tag == temp->tag && found == 0){
						chit++;
						found++;
						if(strcmp(argv[3],"lru") == 0){
							cache[a].line->time = temp->time;
						}
					}
				
				}
				if(found)
					found--;
				else{
					mread++;
					cache[(int)pos].line = temp;
					found = 0;	
				}
			}
			else if(strcmp(command, "W") == 0){
				for(int a = 0; a < pow(2,set); a++){
					if(cache[a].line == NULL){
						if(error != -1){
						error =  -1;
						pos = a;
						}
					}
					else if(cache[a].line->time < error){
						error = cache[a].line->time;
						pos = a;
					}

					if(cache[a].line!= NULL && cache[a].line->tag == temp->tag && found == 0){
						chit++;
						mwrite++;
						found++;
						if(strcmp(argv[3],"lru") == 0){
							cache[a].line->time = temp->time;
						}
						
					}
					
				}
				if(found)
					found--;
				else{
					mread++;
					mwrite++;
					cache[(int)pos].line = temp;
					found = 0;
				}
			}
			

			fscanf(fp, "%s", command);
		}
	}
	else if(strncmp(argv[4], "assoc:", 6) == 0){
		 double offset = log2(atoi(argv[2]));   
		 double set = log2(atoi(argv[1])/atoi(argv[2])/atoi(test));
		 //printf("%0.0lf here\n", set);
                 double tag = 48 - offset - set;
		 int found = 0;
		 double error = 0;
		 int pos = 0;

				                                                                                                                                                                                 while(strcmp(command, "#eof") != 0){  
			

			fscanf(fp, "%llx", &address);                                                                                                                                        
			struct Address* temp = (struct Address*)malloc(sizeof(struct Address));
			temp->tag = bitExtracted(address,48-tag,48);
			temp->set = bitExtracted(address, offset, 48-tag)*atoi(test);
			temp->time = pc;
		        pc++;	
			
			//printf("%lld, %lld, %lld, %lld \n", address, temp->tag, temp->set, temp->time);	
			
			if(strcmp(command,"R") == 0){
				error = pc;
				pos = 0;
				for(int a = 0; a < atoi(test); a++){
					if(cache[temp->set + a].line == NULL){
						if(error != -1){
						error =  -1;
						pos = a;
						}
					}
					else if(cache[temp->set + a].line->time < error){
						error = cache[temp->set + a].line->time;
						pos = a;
					}

					if(cache[temp->set+a].line!= NULL && cache[temp->set+a].line->tag == temp->tag && found == 0){
						chit++;
						found++;
						
						if(strcmp(argv[3],"lru") == 0){
							cache[temp->set+a].line->time = temp->time;
						}
					}
				}
				if(found)
					found = 0;
				else{
					mread++;
					cache[temp->set + pos].line = temp;
					found = 0;	
				}
				//printf("pos is: %d, set is: %lld \n", pos, temp->set);
				
				
			}
			else if (strcmp(command, "W") == 0){
				error = pc;
				pos = 0;
				for(int a = 0; a < atoi(test); a++){
					if(cache[temp->set + a].line == NULL){
						if(error != -1){
						error =  -1;
						pos = a;
						}
					}
					else if(cache[temp->set + a].line->time < error){
						error = cache[temp->set + a].line->time;
						pos = a;
					}

					if(cache[temp->set+a].line!= NULL && cache[temp->set+a].line->tag == temp->tag && found == 0){
						chit++;
						mwrite++;
						found++;
						
						if(strcmp(argv[3],"lru") == 0){
							cache[temp->set + a].line->time = temp->time;
						}
					}
				}
				if(found)
					found = 0;
				else{
					mread++;
					cache[temp->set + pos].line = temp;
					mwrite++;
					found = 0;	
				}
				 //printf("pos is: %d, set is: %lld \n", pos, temp->set);	
			}
			fscanf(fp, "%s", command);
		}
		//printf("%0.0lf,%0.0lf,%0.0lf, %s\n", tag,set,offset,test);
	}

	printf("Memory reads: %0.0lf\n", mread);
	printf("Memory writes: %0.0lf\n", mwrite);
	printf("Cache hits: %0.0lf\n", chit);
	printf("Cache misses: %0.0lf\n", mread);
	return 0;
}
