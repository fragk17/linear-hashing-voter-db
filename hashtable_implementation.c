#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "hashtable_interface.h"

int num_of_keys = 0;
double load = 0.0;
int level = 0;
int p = 0;
int buckets = INITIAL_NUM_OF_BUCKETS;
int buckets_to_be_exceeded = INITIAL_NUM_OF_BUCKETS;
int voted_participants = 0;

int hash(int pin,int level) {
    int hash = pin % ((int)pow(2, level) * 2);
    return hash;
}

HashTable extend_table(HashTable H) {
    int i;
    int result = INITIAL_NUM_OF_BUCKETS * (int)pow(2, level+1);
    
    H=realloc(H,(result)*sizeof(struct Bucket));

    for(i=(INITIAL_NUM_OF_BUCKETS * (int)pow(2, level));i<(INITIAL_NUM_OF_BUCKETS * (int)pow(2, level+1));i++){
        H[i].head=NULL;
        H[i].overflow=NULL;
        H[i].size=0;
    }

    return H;
}

HashTable make_table(HashTable H) {
    int i;
    
    H = (HashTable)malloc(INITIAL_NUM_OF_BUCKETS*sizeof(struct Bucket));

    for(i=0;i<INITIAL_NUM_OF_BUCKETS;i++){

        H[i].head=NULL;
        H[i].overflow=NULL;
        H[i].size=0;
    }
    return H;
}

void insert_to_list(struct list *list,struct voter *info) {

    struct list_node* newNode = (struct list_node *)malloc(sizeof(struct list_node));
    if(newNode != NULL) {
        newNode->pin = info->pin;
        newNode->info = info;
        newNode->next = list->head;
        list->head = newNode;
    }
}

void printHashTable(HashTable H);

void rehash(HashTable H,int bucket_size){

    struct list_node *temp,*temp1;
    struct Bucket *temp2,*temp3,*temp4;
    int counter = 0;


    struct list *list = (struct list*)malloc(sizeof(struct list));
    list->head = NULL;

    temp2 = &H[p];

    while(temp2 != NULL) {
        temp = temp2->head;

        while (temp != NULL)
        {
            temp1 = temp;
            insert_to_list(list,temp->info);
            temp = temp->next;
            free(temp1);

            if (counter < 1) {
                temp2->head = NULL;
                temp2->size = 0;
            }
        }
        temp3 = temp2;
        temp2 = temp2->overflow;
        if(counter < 1) temp3->overflow = NULL; 
        if(counter > 0) free(temp3);
        counter++;
    }

    while (list->head != NULL) {
        struct list_node *temp = list->head;
        insert(H,list->head->pin,list->head->info->firstname,list->head->info->lastname,list->head->info->zip,list->head->info->hasVoted,bucket_size,REHASH);
        list->head = list->head->next;
        free(temp);
    }

    free(list);
}

// int voters_count(HashTable H) {
//     int count = 0;
    
//     for(int i=0 ; i < buckets ;i++ ){
//         count+= H[i].size;
//     }
//     return count;
// }


struct Bucket *makebucket()
{
	struct Bucket *bucket = (struct Bucket *)malloc(sizeof(struct Bucket));
	bucket->head = NULL;
    bucket->overflow = NULL;
	bucket->size = 0;
	return bucket;
}

HashTable insert(HashTable H, int pin, char *firstname, char *lastname, int zip, char hasVoted, int bucket_size,int mode) {
    int result;
    struct list_node *temp;
    struct Bucket *temp1; 
    
    if(mode == HASH){
        num_of_keys++;
        load = (double)num_of_keys/(buckets*2);
        result = hash(pin,level);
       if(result<p)
          result = hash(pin,level+1);
    }
    else{
        result = hash(pin,level+1); //rehashing
    }

    temp1 = &H[result];
    while (temp1->overflow!= NULL) {
        temp1 = temp1->overflow;
    }

    if(H[result].head == NULL) {
        struct list_node *NewNode = (struct list_node *)malloc(sizeof(struct list_node));
        struct voter *NewVoter = (struct voter *)malloc(sizeof(struct voter));

        H[result].head = NewNode;
        H[result].size += 1;

        NewVoter->pin = pin;
        strcpy(NewVoter->firstname, firstname);
	    strcpy(NewVoter->lastname, lastname);
	    NewVoter->zip = zip;
	    NewVoter->hasVoted = hasVoted;

        NewNode->pin = pin;
        NewNode->info = NewVoter;
        NewNode->next = NULL;

    } else if(H[result].head != NULL && (H[result].size < bucket_size)){

        temp = H[result].head;
        while(temp->next !=NULL){              /*traverse to the end of the bucket*/
            temp = temp->next;
        }
        

        struct list_node *NewNode = (struct list_node *)malloc(sizeof(struct list_node));
        struct voter *NewVoter = (struct voter *)malloc(sizeof(struct voter));
        temp->next = NewNode; 

        H[result].size += 1;

        NewVoter->pin = pin;
        strcpy(NewVoter->firstname, firstname);
	    strcpy(NewVoter->lastname, lastname);
	    NewVoter->zip = zip;
	    NewVoter->hasVoted = hasVoted;

        NewNode->pin = pin;
        NewNode->info = NewVoter;
        NewNode->next = NULL;

    } else if(H[result].head != NULL && (H[result].size == bucket_size) && (H[result].overflow==NULL)) {

        struct Bucket *overflow_bucket = makebucket();
        H[result].overflow = overflow_bucket;

        struct list_node *NewNode = (struct list_node *)malloc(sizeof(struct list_node));
        struct voter *NewVoter = (struct voter *)malloc(sizeof(struct voter));

        overflow_bucket->head = NewNode;
        overflow_bucket->size++;

        NewVoter->pin = pin;
        strcpy(NewVoter->firstname, firstname);
	    strcpy(NewVoter->lastname, lastname);
	    NewVoter->zip = zip;
	    NewVoter->hasVoted = hasVoted;

        NewNode->pin = pin;
        NewNode->info = NewVoter;
        NewNode->next = NULL;

    }
    else {
        temp1 = &H[result];
        while (temp1->overflow!= NULL) {
            temp1 = temp1->overflow;
        }
        if (temp1->size < bucket_size) {

            temp = temp1->head;
            while (temp->next !=NULL) {
                temp = temp->next;
            }
            
            struct list_node *NewNode = (struct list_node *)malloc(sizeof(struct list_node));
            struct voter *NewVoter = (struct voter *)malloc(sizeof(struct voter));
            temp->next = NewNode; 

            temp1->size += 1;

            NewVoter->pin = pin;
            strcpy(NewVoter->firstname, firstname);
            strcpy(NewVoter->lastname, lastname);
            NewVoter->zip = zip;
            NewVoter->hasVoted = hasVoted;

            NewNode->pin = pin;
            NewNode->info = NewVoter;
            NewNode->next = NULL;
        } else if(temp1->size == bucket_size) {
            struct Bucket *overflow_bucket = makebucket();
            temp1->overflow = overflow_bucket;

            struct list_node *NewNode = (struct list_node *)malloc(sizeof(struct list_node));
            struct voter *NewVoter = (struct voter *)malloc(sizeof(struct voter));

            overflow_bucket->head = NewNode;
            overflow_bucket->size++;

            NewVoter->pin = pin;
            strcpy(NewVoter->firstname, firstname);
            strcpy(NewVoter->lastname, lastname);
            NewVoter->zip = zip;
            NewVoter->hasVoted = hasVoted;

            NewNode->pin = pin;
            NewNode->info = NewVoter;
            NewNode->next = NULL;
        }
    }




    if(mode == HASH){
        if(load>LOADLIMIT){                    //rehashing
            if(p == 0) {
                H=extend_table(H);
            }
            buckets++;
            rehash(H,bucket_size);
            p++;  //moving p

            if(p>buckets_to_be_exceeded-1) {         //finishing level
                buckets_to_be_exceeded = buckets_to_be_exceeded*2;
                level++;      //changing level/round
                p=0;
            }
        }
    }

    return H;
}

void printIndexList(struct Bucket *bucket) {
    struct list_node *temp;
    struct Bucket *temp1;
    int counter = 0;
    temp1 = bucket;
    while(temp1 != NULL) {
        if(counter > 0) printf("---Overflow bucket(%d):",counter);
        temp = temp1->head;
        if(temp == NULL) printf(" ");
        while (temp != NULL)
        {
            printf(" %d ",temp->pin);
            if(temp->next != NULL) printf(",");
            temp = temp->next;
        }
        temp1 = temp1->overflow;
        counter++;
    }
}

void printHashTable(HashTable H) {
    int i , count=0 , empty=0;
    printf("\n");

    for(i=0;i<buckets;i++){
        printf("%d:",i);
        printIndexList(&H[i]);
        printf("\n");
    }
    printf("\n");
}

int search_pin(HashTable H,int pin) {
        
    int index;
    struct list_node *temp;
    struct Bucket *temp1;

    index = hash(pin,level);
    if(index<p) {         //bucket is already rehashed
        index=hash(pin,level+1);   //searching in level +1
    }
    temp1 = &H[index];    //points at the start of the index p in which pin is located  

    while(temp1 != NULL){    
        temp = temp1->head;
        while (temp != NULL) {
            if(temp->pin==pin)
                return 1;
            temp=temp->next;
        }
        temp1 = temp1->overflow;
    }

    return 0;

}

void print_pin(HashTable H,int pin) {
        
    int index;
    struct list_node *temp;
    struct Bucket *temp1;

    index = hash(pin,level);
    if(index<p) {         //bucket is already rehashed
        index=hash(pin,level+1);   //searching in level +1
    }
    temp1 = &H[index];    //points at the start of the index p in which pin is located

    while(temp1 != NULL){
        temp = temp1->head;
        while (temp != NULL) {
            if(temp->pin==pin) { //we found the pin that will or has voted
                printf("%d %s %s %d %c\n",temp->info->pin,temp->info->lastname,temp->info->firstname,temp->info->zip,temp->info->hasVoted);
                return;
            }
            temp=temp->next;    //moving to next voter
        }
        temp1 = temp1->overflow;    //moving to an overflow bucket
    }
}

int mark_yes(HashTable H,int pin,Zip_List ziplist) { //returns 1 if the voter if the voter hadn't previously voted
        
    int index;
    struct list_node *temp;
    struct Bucket *temp1;

    index = hash(pin,level);
    if(index<p) {         //bucket is already rehashed
        index=hash(pin,level+1);   //searching in level +1
    }
    temp1 = &H[index];    //points at the start of the index p in which pin is located

    while(temp1 != NULL){       
        temp = temp1->head;
        while (temp != NULL) {
            if(temp->pin==pin) { //we found the pin that will or has voted
                if (temp->info->hasVoted == 'N') {
                    // insert_to_ziplist(temp->pin,temp->info->zip,ziplist); //didn't make it work in time
                    temp->info->hasVoted = 'Y';
                    return 1;       //just voted
                }
                return 0; //has already voted
            }
            temp=temp->next;
        }
        temp1 = temp1->overflow; //moving to an overflow bucket
    }
    return -1; //pin is not in the hashtable
}