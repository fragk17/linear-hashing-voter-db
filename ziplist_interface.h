
struct zipnode {
    int zip;
    struct zipnode *next;
    int size;
    struct voter_node *point;
};

struct voter_node {
    int pin;
    struct voter_node *next;
};

struct ZipList
{
    struct zipnode *head;
};


typedef struct ZipList *Zip_List;


Zip_List make_ziplist(Zip_List Z);
void print_zipcode_voters(Zip_List Z,int zip);
void insert_voters_node(struct zipnode *zip_node,int pin);
void insert_to_ziplist(int pin,int zip, Zip_List Z);