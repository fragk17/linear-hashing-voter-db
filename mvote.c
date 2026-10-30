#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hashtable_interface.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Use this to run properly: %s -f <input_file> -b <bucket_size(int)>\n", argv[0]);
        return 1;
    }

    char *input_file;
    int bucket_size;

    for(int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) { //we know that after "-f" we have our input file name and store it in a variable
            if (i + 1 < argc) {
                input_file = argv[i + 1];
                i++;
            }
        } else if(strcmp(argv[i], "-b") == 0) { //we know that after "-b" we have our input file and store its name in a variable
            if (i + 1 < argc) {
                bucket_size = atoi(argv[i + 1]);
                i++;
            }
        }
    }

    
    // opening the CSV file for reading
    FILE *file = fopen(input_file, "r");
    if (file == NULL) {
        printf("%s could not be opened\n", input_file);
        return 1;
    }

    char line[500];
    int pin;
    char lastname[50];
    char firstname[50];
    int zip;
    int mode = HASH;
    

    HashTable H;
    Zip_List Z;
    Z = make_ziplist(Z);
    H = make_table(H);
    



    //inseting the content of the input(CSV) file to the hashtable
    while (fgets(line, 500, file)!= NULL) {
        sscanf(line, "%d %s %s %d", &pin, firstname, lastname, &zip);

        H = insert(H,pin,firstname,lastname,zip,'N',bucket_size,HASH);

    }
    fclose(file);

    // printHashTable(H); //was used for testing

    char userInput[200]; // maximum input length is 199 characters
    fgets(userInput, sizeof(userInput), stdin);

    while (strcmp(userInput, "exit\n") != 0) {
        int pin;
        char lastname[50];
        char firstname[50];
        int zip;
        char *word = strtok(userInput, " ");

        if (strcmp(word, "l") == 0) {
            word = strtok(NULL, " ");

            while (word != NULL) {
                pin = atoi(word); //pin = 0 if atoi fails to convert string to integer
                if (pin != 0 || strcmp(word, "0") == 0) { //if pin is not 0 and the string that was given was not "0" then an integer was given

                    if (search_pin(H,pin)==1) print_pin(H,pin);
                    else printf("Participant %d not in cohort\n",pin);

                } else {
                    printf("Malformed Pin\n");
                }
                word = strtok(NULL, " ");
            }
        }
        else if (strcmp(word, "i") == 0){
            word = strtok(NULL, " ");

            int counter = 0;
            while (word != NULL) {
                if(counter == 0) {
                    pin = atoi(word);
                    if(!(pin != 0 || strcmp(word, "0") == 0)) {
                        counter = -1;
                        break;
                    }
                } else if (counter == 1) {
                    strcpy(lastname,word);
                    if(strlen(word) == 0) {
                        counter = -1;
                        break;
                    }
                } else if (counter == 2) {
                    strcpy(firstname,word);
                    if(strlen(word) == 0) {
                        counter = -1;
                        break;
                    }
                } else if (counter == 3) {
                    zip = atoi(word);
                    if(!(zip != 0 || strcmp(word, "0") == 0)) {
                        counter = -1;
                        break;
                    }
                }
                counter++;
                word = strtok(NULL, " ");
            }
            if(counter == -1) printf("Malformed Input\n");
            else if (search_pin(H,pin)==1)
            {
                printf("%d already exists\n",pin);
            }
            else {
                insert(H,pin,firstname,lastname,zip,'N',bucket_size,HASH);
                printf("Inserted %d %s %s %d\n", pin, firstname, lastname, zip);
            }
        }
        else if(strcmp(word, "m") == 0) {
            word = strtok(NULL, " ");

            while (word != NULL) {
                pin = atoi(word); //pin = 0 if atoi fails to convert string to integer
                if (pin != 0 || strcmp(word, "0") == 0) { //if pin is not 0 and the string that was given was not "0" then an integer was given

                    if (search_pin(H,pin)==1) {
                        if(mark_yes(H,pin,Z)==1) {
                            printf("%d Marked Voted\n",pin);
                            voted_participants++;
                        }
                    }
                    else printf("%d does not exist\n",pin);

                } else printf("Malformed Input\n");

                word = strtok(NULL, " ");
            }
        }
        else if(strcmp(word, "bv") == 0) {
            word = strtok(NULL, " ");
            char *newline = strchr(word, '\n'); //finding the position of the '\n' 
            
            if (newline != NULL) {// Replacing the '\n' from the back of the input string
                *newline = '\0';
            }
            int temp;
            FILE *key_file = fopen(word, "r");
            if (key_file == NULL) {
                printf("%s could not be opened\n", word);
            }
            else {
                while(fgets(line, 500, key_file)!= NULL) {
                    temp = sscanf(line, "%d", &pin);

                    if (temp == 1) { //sscanf rerurns 1 if the conversion is succesful
                        temp = mark_yes(H,pin,Z);
                        if (temp == 1) {
                            printf("%d Marked Voted\n",pin);
                            voted_participants++;
                        }
                        else if (temp == -1) printf("%d does not exist\n",pin);
                    } else printf("Malformed Input\n");
                }
            }
            fclose(key_file);
        }
        else if(strcmp(word, "v\n") == 0) {
            printf("Voted So Far %d\n",voted_participants);
        }
        else if(strcmp(word, "perc\n") == 0) {
            double temp = (double)voted_participants/num_of_keys;
            printf("%f\n",temp);
        }
        else if(strcmp(word, "t\n") == 0) { //printing the total number of voters , not aknowledging if they already have voted or not
            printf("Total Voters %d\n",num_of_keys);
        }
        // else if(strcmp(word, "z") == 0) {
        //     word = strtok(NULL, " ");

        //     while (word != NULL) {
        //         zip = atoi(word);
        //         print_zipcode_voters(Z,zip);
        //     }
        // }
        else printf("Wrong input - Please try again!\n");
        fgets(userInput, sizeof(userInput), stdin);
    }

    if(strcmp(userInput, "exit\n") != 0) free(H);

    return 0;
}