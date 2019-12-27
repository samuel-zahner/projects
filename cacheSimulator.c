#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


struct block{
	 unsigned long long int data;
	 unsigned long long int tag;
	 int validBit;
	 int dirtyBit;
	 int set;
	 int counter;
};

int getLargestCounter(struct block ** cache, struct block current,  int numOfBlocks);
int getBlockOfLeastUsed(struct block ** cache, struct block current,  int numOfBlocks);
int calculateSetOffset(double setBits);
int calculatethelogBaseTwo( int num);
struct block initializeBlock( unsigned long long int address,  int blockSize,  int numOfSets);
bool search(char *policy, struct block, struct block **,  int, int);
bool checkifPowerOfTwo(int number);
struct block ** initializeCache( int cacheSize,  int blockSize, int numOfBlocks, int numOfSets);
struct block ** updateCache(char *policy, struct block **, struct block,  int numOfBlocks, int blockSize,  int numOfSets, int addressCount);


int done = 0;


int main(int argc, char** argv){

	if(argc != 7){
		printf("insufficient arguments\n");
		exit(0);
	}

	//S = num of sets A = num of blocks per set 
	//cacheSize = C

	int cacheSize = atoi(argv[1]);
	if(!checkifPowerOfTwo(cacheSize)){
		printf("error: not power of 2\n");
		exit(0);
	}

	//blockSize = B
	int blockSize = atoi(argv[2]);
	if(!checkifPowerOfTwo(blockSize)){
		printf("error: not power of 2\n");
		exit(0);
	}

	char* policy = malloc(sizeof(argv[3]));
	policy = argv[3];

	char* associativity = malloc(sizeof(argv[4]));
	associativity = argv[4];
 
	int numOfBlocks=0;
	int numOfSets=0;
	if(strcmp(associativity, "direct") == 0){
		//simulate direct A=1(1 block)
		numOfBlocks = 1;
		numOfSets = cacheSize/blockSize;
	}
	else if(strcmp(associativity,"assoc") == 0){
		//simulate fully associated S=1 A=C/B
		numOfBlocks = cacheSize/blockSize;
		numOfSets = 1;
	}
	else{
		//find n ---> check power of 2 S>1, A>1
		  int n = associativity[6] - '0';
		if(!checkifPowerOfTwo(n)){
			printf("error: not power of 2\n");
			exit(0);
		}
		numOfBlocks = n;
		numOfSets = cacheSize/(blockSize*numOfBlocks);
	}

	int prefetchSize = atoi(argv[5]);

	struct block **cache = initializeCache(cacheSize, blockSize, numOfBlocks, numOfSets);

	FILE *fp = fopen(argv[6], "r");
	if(fp == NULL){
		printf("file does not exist\n");
		exit(0);
	}
	unsigned long long int address=0;

	char readWrite;
	//NO Prefetch

	  int mem_reads = 0;
	  int mem_writes = 0;
	  int cache_hits = 0;
	  int cache_misses = 0;
          struct block currentBlock;
  
   	

  	  int addressCount =0;
	
	
	 while((fscanf(fp, "%c %llx", &readWrite, &address) != EOF) && (done != 1)){
		if(readWrite == '\n'){
			continue;
		}
		if(readWrite =='#'){
			done = 1;
			break;
		}
		addressCount++;// used for lru counter - running time stamp of last use


		//initialize block based on given address
		currentBlock = initializeBlock(address, blockSize, numOfSets);

		//checks R or W then checks if it is in cache ----> incrementing pointers accordingly
		if(readWrite == 'R'){
			if(search(policy,currentBlock, cache, numOfBlocks, addressCount)){
				cache_hits++;
         		}
			else{
				mem_reads++;
				cache_misses++;
				//adds to cache
				cache = updateCache(policy, cache, currentBlock, numOfBlocks, blockSize, numOfSets, addressCount);
			}

		}
		else if(readWrite == 'W'){
			if(search(policy,currentBlock, cache, numOfBlocks, addressCount)){
				cache_hits++;
				mem_writes++;
  			}
			else{
				cache_misses++;
				mem_reads++;
				mem_writes++;
 
				//adds to cache
				cache = updateCache(policy, cache, currentBlock, numOfBlocks, blockSize, numOfSets, addressCount);
			}
		}
		
	}
	printf("no-prefetch\n");
	printf("Memory reads: %d\n", mem_reads);
	printf("Memory writes: %d\n", mem_writes);
	printf("Cache hits: %d\n", cache_hits);
	printf("Cache misses: %d\n", cache_misses);

	

	//WITH PREFETCH --> need to prefetch on every miss(R or W), add to cache if not there, inc mem_reads if not there
	//new cache so there is no left over from previous
	struct block **cache2 = initializeCache(cacheSize, blockSize, numOfBlocks, numOfSets);
	rewind(fp);

	mem_reads = 0;
	mem_writes = 0;
	cache_hits = 0;
	cache_misses = 0;
	unsigned long long int address2 = 0;
	struct block currentPrefetch;
	done = 0;
        addressCount = 0;

	while((fscanf(fp, "%c %llx", &readWrite, &address) != EOF) && (done != 1)){
		if(readWrite == '\n'){
			continue;
		}
		if(readWrite == '#'){
			done = 1;
			break;
		}
		addressCount++;
		
		
		currentBlock = initializeBlock(address, blockSize, numOfSets);
		if(readWrite == 'R'){
			if(search(policy,currentBlock, cache2, numOfBlocks, addressCount)){
				cache_hits++;
			}
			else{
				mem_reads++;
				cache_misses++;

				cache2 = updateCache(policy, cache2, currentBlock, numOfBlocks, blockSize, numOfSets, addressCount);

				//prefetching
				  int i = 0;
				address2 = address;
				while(i < prefetchSize){
					//increases address by blocksize to return next address/block
					address2 = address2 + blockSize;
					currentPrefetch = initializeBlock(address2, blockSize, numOfSets);
					//if present do nothing
					if(search(policy,currentPrefetch, cache2, numOfBlocks, addressCount)){
						
						//continue;
					}
					else{
						//if not present increment reads and put into cache
						mem_reads++;

						cache2 = updateCache(policy, cache2, currentPrefetch, numOfBlocks, blockSize, numOfSets, addressCount);
					}
					i++;
				}

					
			}

		}
		else if(readWrite == 'W'){
			if(search(policy,currentBlock, cache2, numOfBlocks, addressCount)){
				cache_hits++;
				mem_writes++;
			}
			else{
				cache_misses++;
				mem_reads++;
				mem_writes++;

				cache2 = updateCache(policy, cache2, currentBlock, numOfBlocks, blockSize, numOfSets, addressCount);

				//prefetching
				  int j = 0;
				address2 = address;
				while(j < prefetchSize){
					address2 = address2 + blockSize;
					currentPrefetch = initializeBlock(address2, blockSize, numOfSets);
					if(search(policy, currentPrefetch, cache2, numOfBlocks, addressCount)){
						//continue;
					}
					else {
						mem_reads++;
						cache2 = updateCache(policy, cache2, currentPrefetch, numOfBlocks, blockSize, numOfSets, addressCount);

					}
					j++;
				}
				
			}
		}
		
	}
	printf("with-prefetch\n");
	printf("Memory reads: %d\n", mem_reads);
	printf("Memory writes: %d\n", mem_writes);
	printf("Cache hits: %d\n", cache_hits);
	printf("Cache misses: %d\n", cache_misses);

	return 0;	
}




int calculatethelogBaseTwo(  int num){
	return (log(num)/log(2));
}

bool checkifPowerOfTwo(  int number){
	if(number == 1){
		return true;
	}
	else if(number%2 == 1){
		return false;
	}
	while(number > 0){
		if(number%2 == 0){
			number = number/2;
		}
		else
		{
			return false;
		}

		if(number == 1){
			return true;
		}
	}

	return false;
	
}

//uses matrix to initialize cache
struct block ** initializeCache(  int cacheSize,   int blockSize,   int numOfBlocks,   int numOfSets){
	struct block** cache = malloc(sizeof(struct block*)*numOfSets);
	for( int i = 0; i < numOfSets; i++){
		cache[i] = malloc(sizeof(struct block)* numOfBlocks);

	}
	//initial block to populate cache
	struct block current;
	current.tag = 0;
	current.data = 0;
	current.validBit=0;
	current.dirtyBit=0;
	current.counter = 0;
	for(int i = 0; i < numOfSets; i++){
		for(int j = 0; j< numOfBlocks; j++){
			current.set = i;
			cache[i][j] = current;
		}
	}
	return cache;

}

struct block initializeBlock(unsigned long long int address,   int blockSize,   int numOfSets){
	struct block current;
	current.data = address;
	current.validBit = 0;
	//offset bits
	  int offsetBits = calculatethelogBaseTwo(blockSize);
	  int setBits = calculatethelogBaseTwo(numOfSets);
	// and it with all 1s bits
	double setBitsDouble = setBits;
	//set offset is the number of set bits having all 1s in binary ---> 3 bits = 1 1 1
	  int setOffset = calculateSetOffset(setBitsDouble);
	//shift by the offsetbits and & that with the setoffset to get the setIndex
	current.set = (address >> offsetBits) & setOffset;
	current.dirtyBit = 0;
	//shift address by offset and set to get tag
	current.tag = address >> (offsetBits + setBits);
        current.counter  =0;
	return current;


}

//searches whether block is in the cache(used for both R and Ws)
bool search(char *policy, struct block currentBlock, struct block ** cache,   int numOfBlocks, int addressCount){
	//we know which set the block is in(currentBlock.set) so we just loop through the blocks in that set in the cache
	for(int i = 0; i < numOfBlocks; i++){
		if(cache[currentBlock.set][i].tag == currentBlock.tag){
			if(strcmp(policy, "lru") == 0){
                        	cache[currentBlock.set][i].counter = addressCount;
			}
			return true;
		}
	}
	return false;
}

//updates cache by inserting block
struct block ** updateCache(char *policy, struct block ** cache, struct block current,   int numOfBlocks,  int blockSize,   int numOfSets, int addressCount){
		int blockToReplace = 0;

//if validbit is 0 it means theres nothing there so we insert the block and turn valid bit to one
		if(strcmp(policy, "lru") == 0){
			current.counter = addressCount;
		}
		if(numOfBlocks == 1){
			cache[current.set][0] = current;
			return cache;
		}
			
			
		for(int i = 0; i < numOfBlocks; i++){
			if(cache[current.set][i].validBit == 0){
				current.validBit = 1;
   				cache[current.set][i] = current;
				return cache;
			}
			if(strcmp(policy, "fifo") == 0){
				current.counter = current.counter+1;
			}
		}
		
			
		current.validBit = 1;
		if(strcmp(policy, "lru") == 0){
			blockToReplace = getBlockOfLeastUsed(cache, current, numOfBlocks);
			//puts it in at index current.counter%(mod)numOfBlocks 
			cache[current.set][blockToReplace] = current;
		}  
		else //fifo
		{
			//gets the largest counter that is in the specific set and increments by 1
			current.counter = getLargestCounter(cache, current, numOfBlocks);
			//puts it in at index current.counter%(mod)numOfBlocks 
			cache[current.set][current.counter%numOfBlocks] = current;
		}	


		return cache;
}

//calculates set offset 3 bits = 2^3 -1 = 7 = 1 1 1
  int calculateSetOffset(double setBits){
	double setOffsetDouble = 0;
		setOffsetDouble = pow(2, setBits)-1;
		
	  int setOffset = setOffsetDouble;
	return setOffset;
}

//returns largest counter +1
int getLargestCounter(struct block ** cache, struct block current,   int numOfBlocks){
	int counter = 0;
	for(int i = 0; i < numOfBlocks;i++){
			//only updates largest counter if it is greater
			if(cache[current.set][i].counter > counter){
				counter = cache[current.set][i].counter;
				
			}
	} 
	//increments once more
	counter++;
	
	return counter;

}


int getBlockOfLeastUsed(struct block ** cache, struct block current,   int numOfBlocks){
	int counter = 0x0fffffff;
        int block = 0;
	for(int i = 0; i < numOfBlocks;i++){
 		//only updates largest counter if it is greater
		if(cache[current.set][i].counter < counter){
			counter = cache[current.set][i].counter;
                        block = i;
		}
	} 
	return block;
}


