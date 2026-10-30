#include <stdio.h>
#include <stdlib.h>
#include "ziplist_interface.h"


Zip_List make_ziplist(Zip_List Z)
{
    Z = (Zip_List)malloc(sizeof(struct ZipList));
    Z->head = NULL;
    return Z;
}

void print_zipcode_voters(Zip_List Z,int zip) {
    struct zipnode *temp = Z->head;
    struct voter_node *temp1;
    
    if(temp->zip == zip) {
        printf("%d voted in %d\n",temp->size,zip);
        temp1 = temp->point;
        while (temp1 != NULL) {
            printf("%d\n",temp1->pin);
            temp1 = temp1->next;
        }

    } else {
        while(temp->zip != zip) { //finding the zipcode so we can print the voter from it
            temp = temp->next;
        }
        printf("%d voted in %d\n",temp->size,zip);
        temp1 = temp->point;
        while (temp1 != NULL) {
            printf("%d\n",temp1->pin);
            temp1 = temp1->next;
        }
    }
}

void insert_voters_node(struct zipnode *zip_node,int pin) {
    struct voter_node *temp = zip_node->point;

    if (temp == NULL) { //if this zip code doesnt have any voters
        struct voter_node *newnode = (struct voter_node *)malloc(sizeof(struct voter_node));
        newnode->pin = pin;
        newnode->next = NULL;
        zip_node->point = newnode;
    } else {
        while(temp != NULL) { //if it already has voters 
            if(temp->next == NULL) break;
            temp = temp->next;
        }
        struct voter_node *newnode = (struct voter_node *)malloc(sizeof(struct voter_node));
        newnode->pin = pin;
        newnode->next = NULL;
        temp->next = newnode;
    }
}

void insert_to_ziplist(int pin,int zip, Zip_List Z) {
    
    struct zipnode *temp = Z->head;
    if (temp == NULL) {
        struct zipnode *newnode = malloc(sizeof(struct zipnode));
        newnode->zip = zip;
        newnode->next = NULL;
        newnode->size = 1;
        insert_voters_node(newnode,pin);
        Z->head = newnode;
    } else {
        while(temp != NULL) {
            if(temp->zip == zip) {
                insert_voters_node(temp,pin);
                temp->size++;
                return;
            } else if(temp->next == NULL) {
                struct zipnode *newnode = malloc(sizeof(struct zipnode));
                newnode->zip = zip;
                newnode->next = NULL;
                newnode->size = 1;
                insert_voters_node(newnode,pin);
                temp->next = newnode;
                return;
            }
            temp = temp->next;
        }
    }
}