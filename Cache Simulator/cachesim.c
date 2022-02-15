#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    int age;
    long int tag;
    struct node* next;
};

struct node* createNode(int number, long int t, struct node* nex){
    struct node* temp = malloc(sizeof(struct node));

    temp->age = number;
    temp->tag = t;
    temp->next = nex;

    return temp;
}

void freeNodes(struct node** head, int Sets){
	
	struct node* temp;
	struct node* ptr;	
	
	for (int i = 0; i < Sets; i++){
		temp = head[i];
		while (temp!=NULL){
			ptr = temp;
			temp = temp->next;
			free(ptr);
		}
	}
	
	free(head);
}

int logTwo(int temp){
    int i = 0;
    
    if (temp == 1) return 0;

    while (temp > 1){
        if (temp % 2 != 0) return -1;
        temp = temp/2;
        i++;
    }

    return i;
}

void incrementAge(struct node* head){
	struct node* ptr = head;
	
	while (ptr != NULL){
		ptr->age++;
		ptr = ptr->next;
	}
}

//Checks if address is loaded into the cache, returns 0 if it is
//If address is not loaded into the cache, returns 1 and loads it.
int Lookup(struct node** head, long int address, int replacement, int associativity, int power_Block, int size_Sets, int pre, int lines, int size_Block){	
	
	long int tag, index, blockOffset;
	blockOffset = address%size_Block;
	long int t = (address - blockOffset)%(size_Sets*size_Block);
	index = t/size_Block;
	
	if (index < 0 || index > size_Sets) return 1;

	tag = address-blockOffset-(index*size_Sets*size_Block);

	struct node* ptr = head[index]; // create a pointer to traverse this list
	struct node* temp = ptr;
	
	int n = 0; //this will keep track of the number of nodes in head[index], used later in replacement

	while (ptr != NULL){
		if (ptr->tag == tag){
			if (replacement == 0){
				return 0;
			}
			else if (pre != 1) {	//since loading an existing piece of memory from the prefethcing technique does not increase the relative age of nodes
				incrementAge(head[index]);
				ptr->age = 0;		//We must increase the relative age of each node, since this one was just accessed we set this node's age to 0
				return 0;
			}
			else {		//prefetch and we're using LRU which doesn't count as access
				return 0;
			} 
		}
		n++;
		ptr = ptr->next;
	}
	
	//At this point, our address is not within the cache simulator, we need to load it in
	//FIFO is trivial and our new node will delete the first node in head[index] and add this one to the end.
	//LRU will sequentially search head[index] for the largest relative node age and replace itself, setting the new node age to 0.
	
	struct node* newNode = createNode(0, tag, NULL);
	ptr = head[index];
	temp = head[index];
	
	if (ptr == NULL){
		head[index] = newNode;
		return 1;
	}
	
	if (replacement == 0){
		if (associativity == 1){
			head[index] = newNode;
			free(temp);
		}
		else {	//delete the first node and add the new one to the end, only if there is not enough space in the list
			
			if (n < lines){	//still space left, add this new node to the end
				while (ptr->next != NULL){
					ptr = ptr->next;
				}
				ptr->next = newNode;
			}
			else if (n == lines){
				temp = ptr;
				ptr = ptr->next;
				free(temp);
				head[index] = ptr;
			
			while (ptr->next != NULL) ptr = ptr->next;
			
				ptr->next = newNode;
			}
			else {
				exit(0);
			}
		}
	}
	else {
		if (associativity == 1){
			head[index] = newNode;
			free(temp);
		}
		else {
			if (n < lines){	//add the new node to the end of the list and increment all ages
				incrementAge(head[index]);
				while(ptr->next != NULL) ptr = ptr->next;
				ptr->next = newNode;
			}
			else if (n == lines){	//locate the node with the largest age and replace
				while (ptr != NULL){
					if (ptr->age > temp->age) temp = ptr;
					ptr = ptr->next;
				}
				
				incrementAge(head[index]);
				temp->age = 0;
				temp->tag = tag;
				free(newNode);
			}
			else {
				exit(0);
			}
		}
	}
	
	return 1;
}

void printCache(struct node** head, int size_Sets){
	struct node* ptr;

	for (int i = 0; i < size_Sets; i++){
		ptr = head[i];

		printf("Set: %x\t", i);

		while (ptr != NULL){
			printf(" [age: %x, tag: %lx] ", ptr->age, ptr->tag);
			ptr = ptr->next;
		}
	
		printf("\n");
	}
}
int main(int argc, const char * argv[]) {
    
    if (argc != 6 || argv[5][0] == '\0'){
        printf("Invalid Input. Usage: ./cachesim size_Cache associativity replacement size_Block\n");
		printf("size_Cache: size of cache in bytes, must be a power of 2\n");
		printf("associativity: 0 if Fully-Associative, 1 if Direct-Mapped, n if n-Way-Associative\n");
		printf("replacement: 0 if First-In-First-Out, 1 if Least-Recently-Used, \n");
		printf("size_Block: block size in bytes, will always be a power of 2\n");
		return 1;
	}

    int power_Cache;
    int size_Cache; //size of cache in bytes, will always be a power of 2
    int replacement; //0 if FIFO, 1 if LRU
    int size_Block, power_Block; //block size in bytes, will always be a power of 2
    int associativity; //0 if Fully Associative, 1 if Direct Mapped, n if "n-way associative"
    int size_Sets, lines;

    size_Cache = atoi(argv[1]);
    power_Cache = logTwo(size_Cache);
    size_Block = atoi(argv[4]);
    power_Block = logTwo(size_Block);
    if (power_Cache == -1 || power_Block == -1) return 1;

    if (strcmp("fifo", argv[3]) == 0) replacement = 0;
    else replacement = 1;

    if (strcmp("direct", argv[2]) == 0){
        associativity = 1;
        lines = 1;
        size_Sets = size_Cache / size_Block;
    }
    else if (strcmp("assoc", argv[2]) == 0){
        associativity = 0;
        size_Sets = 1;
        lines = size_Cache / size_Block;
    }
    else {
        sscanf(argv[2], "assoc:%d", &associativity);
        lines = associativity;
        size_Sets = size_Cache / (size_Block * lines);            //caclulate the number of sets, equal to 1 for fully associative and size_Cache/size_Block for direct.
    }
    
    struct node ** head = malloc (sizeof(struct node *) * size_Sets);
	struct node ** prehead = malloc (sizeof(struct node *) * size_Sets);

	for (int i=0; i<size_Sets; i++) {
		head[i] = NULL;
		prehead[i] = NULL;
	}

    FILE *fp;
	fp = fopen(argv[5], "r");
	if (!fp) return 1;
	
	int hits = 0, misses = 0, reads = 0, writes = 0;
	int preHits = 0, preMisses = 0, preReads = 0, preWrites = 0;
	
	char* temp = (char*) malloc (sizeof(char) * 75);
	char instruction = 'x';
	long int address;
	
	//This loop goes through every address in the file and updates the counters. It first starts with the regular cache and then goes to one with prefetching.
	while (fscanf(fp, "%s", temp) == 1){
		if (strcmp(temp, "#eof") == 0 || strcmp(temp, "eof") == 0) break;
		if (fscanf(fp, " %c %lx", &instruction, &address) != 2) break;
		
		if (Lookup(head, address, replacement, associativity, power_Block, size_Sets, 0, lines, size_Block) == 0){
			hits++;
			if (instruction == 'W') writes++;
		}
		else {
			misses++;
			if (instruction == 'R'){
				reads++;
			}
			else {
				reads++;
				writes++;
			}
		}
		
		//Now do the same thing, but for prefetching
		
		if (Lookup(prehead, address, replacement, associativity, power_Block, size_Sets, 0, lines, size_Block) == 0){
			preHits++;
			if (instruction == 'W') preWrites++;
		}
		else { //If it is a cache miss for a prefetching cache, load address+1 as well
			preMisses++;
			if (instruction == 'R'){
				preReads++;
			}
			else {
				preReads++;
				preWrites++;
			}
			
			//Iterate throuh the address+1 block as well
			if (Lookup(prehead, address+size_Block, replacement, associativity, power_Block, size_Sets, 1, lines, size_Block)!=0) preReads ++;		
		}
	}
	
	//Finish by printing the output and then freeing all structures.
	
	printf("Prefetch 0\n");
	printf("Memory reads: %d\n", reads);
	printf("Memory writes: %d\n", writes);
	printf("Cache hits: %d\n", hits);
	printf("Cache misses: %d\n", misses);
	printf("Prefetch 1\n");
	printf("Memory reads: %d\n", preReads);
	printf("Memory writes: %d\n", preWrites);
	printf("Cache hits: %d\n", preHits);
	printf("Cache misses: %d", preMisses);
	
	free(temp);
	freeNodes(head, size_Sets);
	freeNodes(prehead, size_Sets);
	fclose(fp);

    return 0;

}
