#include "../include/common.h"
#include "pa4.h"
#include <stdbool.h>

// don't remove these globals
struct heap_elem *min_heap_arr = NULL;

// don't modify this function
void initialize_min_heap_arr(struct heap_elem *arr)
{
  min_heap_arr = arr;
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


// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// return r->friends.
struct list_records* get_friends_list(struct record *r)
{
  return r->friends;
}

// Make r1 and r2 friends of each other if they aren't already friends.
// The friends field in "struct record" stores the
// head of the linked list of friends of a given user.
// Return 1 if r1 and r2 are already friends before this call.
// Otherwise, make r1 a friend of r2 and return 0.
// To make r1 a friend of r2, 
// insert r1 into the list r2->friends and insert r2 into the list r1->friends.
bool are_they_already_friend(struct list_records *friend,struct list_records *friends, struct record *r2, struct record *r1){
  for (;friend->next != NULL;friend = friend->next)
    {
        if ((friend->record==r2))
        {
            return true; // Friendship already there
        }
        
    }
  return (friend->record==r2);
}
int make_friends(struct record *r1, struct record *r2)
{   
    int x=0;
    if(r1->friends&&r2->friends){
    if (are_they_already_friend(r1->friends,r2->friends,r2,r1)){
      x = 1;
    }}
    if(!x){
    struct list_records *aa = r1->friends;
    struct list_records *bb = r2->friends;

    // struct list_records* new_record = (struct list_records*)allocate_memory(sizeof(struct list_records));
    // if (new_record == NULL) {
    //     return -1; 
    // }
    // new_record->record = r2;
    // // new_record->next = NULL;
    
    // if (aa == NULL) {
    //     aa = new_record;
    // } else {
    //     struct list_records* current = aa;
    //     while (current->next != NULL) {
    //         current = current->next;
    //     }
    //     current->next = new_record;
    // }

  //   struct list_records* new_record2 = (struct list_records*)allocate_memory(sizeof(struct list_records));
  //   if (new_record2 == NULL) {
  //       return -1; 
  //   }
  //   new_record2->record = r1;
  //   // new_record2->next = NULL;
    
  //   if (bb == NULL) {
  //       bb = new_record2;
  //   } else {
  //       struct list_records* current2 = bb;
  //       while (current2->next != NULL) {
  //           current2 = current2->next;
  //       }
  //       current2->next = new_record2;
  //   }
  // return 0;
  struct list_records *new_record = allocate_memory(sizeof(struct list_records));
  struct list_records *new_record2 = allocate_memory(sizeof(struct list_records));
  struct list_records *temp = r1->friends;
  struct list_records *tempo = r2->friends;
    new_record->record = r2;
    new_record2->record = r1;
    r1->friends = new_record;
    r2->friends = new_record2;
    if (temp) {
      r1->friends->next=temp;
    }
    if (tempo) {
      r2->friends->next=tempo;
    }}
    return x;
}   


// remove all nodes from r->friends and release the corresponding memory'
void delete_friends_list_recursive(struct list_records *r)
{
  if (r->next){
    delete_friends_list_recursive(r->next);
  }
    free_memory(r);
}
void delete_friends_list(struct record *r)
{
  if(r->friends)
    delete_friends_list_recursive(r->friends);
}

// Each friends f of r is connected via an undirected
// edge whose weight is the distance between r and f.
// compute the shortest distance and a shortest path to 
// all other vertices reachable from r.
// initially, the status fields in all records are set to zero.
// distance and pred fields may contain grabage values.
// you need to use the distance function to compute the distance
// between two records.
// you can use min_heap_arr to implement a min heap or
// a linear scan to find a vertex with the minimum distance.
// In addition to distance and pred, you can also update the
// status field in your implementation.
// Don't modify other fields in struct record.
// At the end of this procedure, distance field in each record
// reachable via r, must contain the minimum distance from r;
// and the pred field must contain the predecessor on a shorest
// path.




#include<limits.h>
struct record* find_minimum(int idx)
{
  double max_dist = INT_MAX;
  int max_index = 0,i = 0;
  struct record * p;
  
  for (;i <= idx - 1 ; i++ )
  {
    if (max_dist > min_heap_arr[i].r->distance)
    {
      max_index = i;
      max_dist = min_heap_arr[i].r->distance;
      
    }
  }
  for (int j = 0; j < idx; j++)
  {
    if (min_heap_arr[j].r->distance == max_dist)
    {
      i = j;
      break;
    }
  }
  p = min_heap_arr[max_index].r;
  min_heap_arr[max_index].r = min_heap_arr[--(idx)].r; 
  // *idx=0;
  return p;
}
void DFS(struct list_records  *friends , int *idx)
{
  for(;friends;friends = friends->next)
  {
    int *x;
    x=&(friends->record->status);
    if (!(*x))
    {
      friends->record->pred = NULL;
      friends->record->distance = INFINITY;
      *x = 1;
      min_heap_arr[*idx].r = friends->record;
      (*idx)++;
      struct list_records  *forwarding_pointer=friends->record->friends;
      DFS(forwarding_pointer , idx);
    }
    
  }
}
// double distance(struct location *loc1, struct location *loc2)
// {
//   double dist;

//   dist = (sin(deg_to_rad(loc1->lat)) * sin(deg_to_rad(loc2->lat))) +
//     (cos(deg_to_rad(loc1->lat)) * cos(deg_to_rad(loc2->lat)) *
//      cos(deg_to_rad(loc1->lon - loc2->lon)));
//   dist = acos(dist) * 6371;
//   return dist;
// }

void compute_sssp(struct record *r)
{
  r->status = 1;
  r->distance = 0;
  r->pred = NULL;
  int index = 1;
  min_heap_arr[0].r = r;
  DFS(r->friends , &index);
  int indexx=index;
  for(;indexx;indexx--)
  {
    struct record *u = find_minimum(indexx++);
    struct list_records *f = u->friends;
    for (;f;f = f->next)
    {
      double dist = u->distance + distance(&(u->loc) , &(f->record->loc));
      if (f->record->distance > u->distance + distance(&u->loc, &f->record->loc))
      {
        f->record->distance = u->distance + distance(&u->loc, &f->record->loc);
        f->record->pred = u;
      }
    }
    indexx--;
  }
}
