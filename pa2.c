#include "../include/common.h"
#include "pa2.h"


// don't remove these globals.
static struct record *bst_root = NULL;
static struct record *avl_root = NULL;
static size_t num_bst_nodes = 0;
static size_t num_avl_nodes = 0;

// don't modify this function
struct record *get_bst_root()
{
  return bst_root;
}

// don't modify this function
struct record *get_avl_root()
{
  return avl_root;
}

// don't modify this function
// compare two uids
// return -1 if uid1 < uid2
// return 0 if uid1 == uid2
// return 1 if uid1 > uid2
static int cmp_uid(char *uid1, char *uid2)
{
  int i;

  for (i = 0; i < MAX_LEN; i++) {
    if (uid1[i] > uid2[i]) {
      return 1;
    }
    else if (uid1[i] < uid2[i]) {
      return -1;
    }
  }
  return 0;
}

// don't modify this function
// compare two records
// we assume that uid is unique and
// there can't be two records with the 
// same uid
// return -1 if record1 < record2
// return 0 if record1 == record2
// return 1 if record1 > record2
static int cmp_record(struct record *r1, struct record *r2)
{
  return cmp_uid(r1->uid, r2->uid);
}

// don't modify this function
static void *allocate_memory(size_t size)
{
  return __mymalloc(size);
}

// don't modify this function
static void free_memory(void *ptr)
{
  __myfree(ptr);
}

// ---------------- Add your code below this line ----------------- //

// Return the total number of records in the
// BST rooted at bst_root.
size_t get_num_bst_records_supporter(struct record* h){
  if (h == NULL) 
    return 0;
  else
    return 1+get_num_bst_records_supporter(bst_root->left)+get_num_bst_records_supporter(bst_root->right);
}
size_t get_num_bst_records()
{
  return num_bst_nodes;
}

// Return the total number of records in the
// AVL tree rooted at avl_root.
size_t get_num_avl_records()
{
  return num_avl_nodes;
}

// Insert record r in the BST rooted at bst_root.
void insert_record_bst(struct record r){
  num_bst_nodes++;
  if (bst_root == NULL) {
    bst_root = allocate_memory(sizeof(struct record));
    *bst_root = r;
    bst_root->parent = NULL;
    bst_root->left = NULL;
    bst_root->right=NULL;
    return;
  }
  struct record *previos = bst_root;
  int flag=0;
    while(previos)
    {
        int c = cmp_uid(r.uid, previos->uid);
        if (c == -1)
        { 
          if(!previos->left){
            flag=1;
            break;
          }
            previos = previos->left;
        }
        else if (c == 1)
        { 
          if(!previos->right){
            break;
          }
            previos= previos->right;
        }
        
    }

  struct record *new_node = (struct record *)allocate_memory(sizeof(struct record));
  *new_node = r;
  new_node->parent = previos;

  if (flag) {
    previos->left = new_node;
  } else{
    previos->right = new_node;
  }
 
}

// Insert record r in the AVL tree rooted at avl_root.
///insert record avl poora sir kaa code hai to chances hai ki sabka same ho
//aur koi tarika/option nhi tha

int tree_balance(struct record* n) {
    int r_height = (n->right) ? n->right->height : -1;
    int l_height = (n->left) ? n->left->height : -1;
    return l_height - r_height;
}

void set_height(struct record* n) {
    int r_height = (n->right) ? n->right->height : -1;
    int l_height = (n->left) ? n->left->height : -1;
    n->height = (l_height > r_height) ? l_height + 1 : r_height + 1;
}
void set_height2(struct record* n,struct record* n2){
    set_height(n);
    set_height(n2);
}
void set_height3(struct record* n,struct record* n2,struct record* n3){
    set_height(n);
    set_height(n2);
    set_height(n3);
}
struct record* LL(struct record* n) {
    struct record* new_root = n->left;
    n->left = new_root->right;
    new_root->right = n;
    set_height2(n,new_root);
    return new_root;
}

struct record* RR(struct record* n) {
    struct record* new_root = n->right;
    n->right = new_root->left;
    new_root->left = n;
    set_height2(n,new_root);
    return new_root;
}

static struct record* LR(struct record* n) {
    struct record* middle = n->left;
    struct record* new_root = middle->right;
    middle->right = new_root->left;
    n->left = new_root->right;
    new_root->left = middle;
    new_root->right = n;
    set_height3(n,middle,new_root);
    return new_root;
}

static struct record* RL(struct record* n) {
    struct record* middle = n->right;
    struct record* new_root = middle->left;
    middle->left = new_root->right;
    n->right = new_root->left;
    new_root->left = n;
    new_root->right = middle;
    set_height3(n,middle,new_root);
    return new_root;
}

struct record* rotate(struct record* n) {
    int bal = tree_balance(n);
    if (bal > 1) {
        bal = tree_balance(n->left);
        if (bal > 0) {
            n = LL(n);
        }
        else {
            n = LR(n);
        }
    }
    else if (bal < -1) {
        bal = tree_balance(n->right);
        if (bal > 0) {
            n = RL(n);
        }
        else {
            n = RR(n);
        }
    }
    return n;
}

struct record* insert_supporter(struct record* root, struct record val) {
    if (root == NULL) {
        root = allocate_memory(sizeof(struct record));
        *root = val;
        return root;
    }

    else {
        if (cmp_uid(val.uid, root->uid) <= 0) {
            root->left = insert_supporter(root->left, val);
        }
        else {
            root->right = insert_supporter(root->right, val);
        }
        set_height(root);
        int bal = tree_balance(root);
        return rotate(root);
    }
}

void insert_record_avl(struct record r)
{
    num_avl_nodes++;
    avl_root = insert_supporter(avl_root, r);
}

// Search the record corresponding to uid in the BST
// rooted at bst_root.
// If the record is not present, return a dummy record
// with -1 in the status field.
struct record* pointer_search(char uid[MAX_LEN]){
    struct record dummy;
    dummy.status=-1;
    if(bst_root == NULL){
      return NULL;
    }
    struct record *previos = bst_root;
    for (;previos!=NULL;)
    {
        int c = cmp_uid(uid, previos->uid);
        if (c == -1){
            previos = previos->left;
        }
        else if (c == 1){
            previos= previos->right;
        }
        else{
            return previos;
        }
    }
    return NULL;
}
struct record* pointer_find(char uid[MAX_LEN]){
    struct record dummy;
    dummy.status=-1;
    if(avl_root == NULL){
      return NULL;
    }
    struct record *previos = avl_root;
    for (;previos!=NULL;)
    {
        int c = cmp_uid(uid, previos->uid);
        if (c == -1)
        {
            previos = previos->left;
        }
        else if (c == 1)
        {
            previos= previos->right;
        }
        else
        {
            return previos;
        }
    }
    return NULL;
}

struct record rsearch_record_bst(char uid[MAX_LEN],struct record* root){
  struct record dummy,curr;
  dummy.status=-1;
  if (root == NULL) {
    return dummy;
  }
  int cmp = cmp_uid(uid, root->uid);
  if (cmp < 0) {
    return rsearch_record_bst(uid,root->left);
  } else if (cmp >0) {
    return rsearch_record_bst(uid,root->right);
  } else {
    // r's uid already exists in the tree
    return *root;
  }
  }

struct record search_record_bst(char uid[MAX_LEN]){
  return rsearch_record_bst(uid,bst_root);
}

// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// Return the head of the linked list of friends
// of the user with uid uid in the BST rooted at bst_root.
// If the corresponding record doesn't exist, return NULL.
struct list_records *get_friends_list_bst(char uid[MAX_LEN]){ 
  return search_record_bst(uid).friends;  
}


// Delete the record (say n) corresponding to uid from the BST.
// Remove n from the lists of friends of other records
// and release the memory for the linked list nodes.
// Release memory for all the nodes in the list of friends of n.
// Return a copy of the value of the deleted node.
// If the node is not present, return a dummy record
// with -1 in the status field.

struct record* find(struct record* t ,char uid[MAX_LEN])
{
  if(t==NULL){
    return NULL;
  }
  int cmp=cmp_uid(uid,t->uid);
  if(cmp==0){
    return t;
  }
  else if(cmp==-1){
    find(t->left,uid);
  }
  else{
    find(t->right,uid);
  }
}
void remove_from_friend_list_helper(struct list_records* curr, char uid[MAX_LEN])
{
    if (curr == NULL) {
        return;
    }
    
    if (!cmp_uid(curr->record->uid, uid)) {
        struct list_records* next = curr->next;
        free_memory(curr);
        remove_from_friend_list_helper(next, uid);
    } else {
        remove_from_friend_list_helper(curr->next, uid);
    }
}
void move_to_next(struct record*temp){
  temp->friends=temp->friends->next;
}

void remove_from_friend_list(struct record* r1, char uid[MAX_LEN])
{
    if (r1->friends == NULL) {
        return;
    }
    
    if (!cmp_uid(r1->friends->record->uid, uid)) {
        struct list_records* next = r1->friends->next;
        free_memory(r1->friends);
        r1->friends = next;
        return;
    }
    
    remove_from_friend_list_helper(r1->friends->next, uid);
}

void remove_from_friend_list1(struct record*temp, char uid[MAX_LEN])
{
  struct list_records* tempo= temp->friends;
  struct list_records* tempoo= temp->friends->next;
  if(!cmp_uid(temp->friends->record->uid, uid)){
    move_to_next(temp);
    free_memory(tempo);
    return;
  }
  for(;tempoo!=NULL;tempoo=tempoo->next)  {
    if (!cmp_uid(tempoo->record->uid,uid)){
      break;
    }
    tempo=tempoo;
  }
  tempo->next=tempoo->next;
  free_memory(tempoo);
}

void delete_friend_list(struct record* tempo,char uid[MAX_LEN]) {
    struct list_records* fr=tempo->friends;
    while (fr){
      struct record* temp;
      temp=find(bst_root,fr->record->uid);
      struct list_records *tempoo=fr->next;
      remove_from_friend_list1(temp, uid);
      free_memory(fr);
      fr=tempoo;
    }
}
struct record* delete_my_node(struct record* root, char uid[MAX_LEN])
{ int c=cmp_uid(root->uid,uid);
  if(c<0){
    root->right=delete_my_node(root->right,uid);
  }
  else if(c>0){
    root->left=delete_my_node(root->left,uid);
  }
  else{
    if((root->left==NULL||root->right==NULL)){
      struct record* tempo;
      if(!root->left){
        tempo=root->right;
      }
      else{
        tempo=root->left;
      }
      free_memory(root);
      return tempo;
    }
      struct record* righty = root->right;
      struct record* lefty = root->left;
      struct record* parenty = root->parent;
      struct record* min_node = root->left;
      while (min_node->right != NULL)
        min_node = min_node->right;
      *root=*min_node;
      root->left=lefty;root->parent=parenty;root->right=righty;
      root->left=delete_my_node(root->left,min_node->uid);
  }

  return root;
}

struct record delete_record_bst(char uid[MAX_LEN]) {
    struct record* tempo = find(bst_root,uid);
    if (tempo==NULL){
      struct record dummy;
      dummy.status=-1;
      return dummy;
    }
    delete_friend_list(tempo,uid);
    struct record node=*tempo;
    bst_root = delete_my_node(bst_root,uid);
    num_bst_nodes--;
  return node;
}

// Search the record corresponding to uid in the AVL tree
// rooted at avl_root.
// If the record is not present, return a dummy record 
// with -1 in the status field.
struct record search_record_avl(char uid[MAX_LEN]){
  return rsearch_record_bst(uid,avl_root);
}

// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// Return the head of the linked list of friends
// of the user with uid uid in the AVL tree rooted at avl_root.
// If the corresponding record doesn't exist, return NULL.
struct list_records *get_friends_list_avl(char uid[MAX_LEN])
{
  return search_record_avl(uid).friends;  
}


// Delete the record (say n) corresponding to uid from the AVL tree.
// Remove n from the lists of friends of other records
// and release the memory for the linked list nodes.
// Release memory for all the nodes in the list of friends of n.
// Return a copy of the value of the deleted node.
// If the node is not present, return a dummy record
// with -1 in the status field.
struct record delete_record_avl(char uid[MAX_LEN])
{
  return delete_record_bst(uid);
}

// Release memory for all BST nodes and their lists of friends.
// Make bst_root points to an empty tree.
void destroying(struct record* node) {
    if (node != NULL) {
        destroying(node->right);
        struct list_records* curr = node->friends;
        for (;curr != NULL;) {
            struct list_records* temp = curr;
            curr = curr->next;
            free_memory(temp);
        }
        free_memory(node);
        destroying(node->left);
    }
}

void destroy_bst() { 
  num_bst_nodes=0;
    destroying(bst_root);
    bst_root = NULL;
   
}


// Release memory for all AVL nodes and their lists of friends.
// Make avl_root points to an empty tree.
void destroy_avl()
{
 
  destroying(avl_root);
  avl_root = NULL;
  num_avl_nodes=0;
}
// Make users with uids uid1 and uid2 in the BST rooted at bst_root
// friends of each other if they aren't already friends.
// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// To make the user with record A a friend of the user with record B, 
// add A to B's list of friends and add B to A's list of friends.
// Return 1 if uid1 and uid2 are already friends before this call.
// Return 0 if they become friends during this call.
int are_they_already_friend(struct list_records *friend,struct record *b,char uid[MAX_LEN]){
  for (;friend != NULL;)
    {
        if (!cmp_record(friend->record,b))
        {
            return 1; // Friendship already exists
        }
        friend = friend->next;
    }
  return 0;
}

int make_friends_bst(char uid1[MAX_LEN], char uid2[MAX_LEN])
{
    struct record *a=pointer_search(uid1);
    struct record *b=pointer_search(uid2);
    struct list_records *friend = a->friends;
    int c=are_they_already_friend(friend,b,uid2);
    if (c){
      return 1;
    }

    struct list_records* new_record = (struct list_records*)allocate_memory(sizeof(struct list_records));
    new_record->record = b;
    new_record->next = NULL;

    if (a->friends == NULL) {
        a->friends = new_record;
    } else {
        struct list_records* current = a->friends;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_record;
    }

    struct list_records* new_record2 = (struct list_records*)allocate_memory(sizeof(struct list_records));
    new_record2->record = a;
    new_record2->next = NULL;

    if (b->friends == NULL) {
        b->friends = new_record2;
    } else {
        struct list_records* current2 = b->friends;
        while (current2->next != NULL) {
            current2 = current2->next;
        }
        current2->next = new_record2;
    }
  return 0;
}

// Make users with uids uid1 and uid2 in the AVL tree rooted at avl_root
// friends of each other if they aren't already friends.
// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// To make the user with record A a friend of the user with record B, 
// add A to B's list of friends and add B to A's list of friends.
// Return 1 if uid1 and uid2 are already friends before this call.
// Return 0 if they become friends during this call.
int make_friends_avl(char uid1[MAX_LEN], char uid2[MAX_LEN])
{
    struct record *a=pointer_find(uid1);
    struct record *b=pointer_find(uid2);
    struct list_records *friend = a->friends;
    int c=are_they_already_friend(friend,b,uid2);
    if (c){
      return 1;
    }

    struct list_records* new_record = (struct list_records*)allocate_memory(sizeof(struct list_records));
    new_record->record = b;
    new_record->next = NULL;

    if (a->friends == NULL) {
        a->friends = new_record;
    } else {
        struct list_records* current = a->friends;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_record;
    }

    struct list_records* new_record2 = (struct list_records*)allocate_memory(sizeof(struct list_records));
    new_record2->record = a;
    new_record2->next = NULL;

    if (b->friends == NULL) {
        b->friends = new_record2;
    } else {
        struct list_records* current2 = b->friends;
        while (current2->next != NULL) {
            current2 = current2->next;
        }
        current2->next = new_record2;
    }
  return 0;
}
