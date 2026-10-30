#include "ziplist_interface.h"
#include "globals.h"


#define HASH 1
#define REHASH 0
#define INITIAL_NUM_OF_BUCKETS 2
#define LOADLIMIT 0.75

struct voter {
    int pin;
    char lastname[50];
    char firstname[50];
    int zip;
    char hasVoted;
};

struct list_node {
	int pin;
    struct voter *info;
	struct list_node *next;
};

struct Bucket {
	struct list_node *head;
	struct Bucket *overflow;
	int size;
};

struct list {
    struct list_node *head;
};

typedef struct Bucket *HashTable;


int hash(int pin,int level);
HashTable extend_table(HashTable H);
HashTable make_table(HashTable H);
void insert_to_list(struct list *list,struct voter *info);
void rehash(HashTable H,int bucket_size);
struct Bucket *makebucket();
HashTable insert(HashTable H, int pin, char *firstname, char *lastname, int zip, char hasVoted, int bucket_size,int mode);
void printIndexList(struct Bucket *bucket);
void printHashTable(HashTable H);
int search_pin(HashTable H,int pin);
void print_pin(HashTable H,int pin);
int mark_yes(HashTable H,int pin,Zip_List ziplist);