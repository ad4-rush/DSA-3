#include "../include/common.h"

#include "pa1.h"

// don't remove these globals

static struct record *record_arr = NULL;

static size_t max_capacity = 0;

static size_t num_records = 0;

// don't modify this function

struct record *get_record_arr()

{

    return record_arr;
}

// don't modify this function

// compare two uids

// return -1 if uid1 < uid2

// return 0 if uid1 == uid2

// return 1 if uid1 > uid2

static int cmp_uid(char *uid1, char *uid2)

{

    int i;

    for (i = 0; i < MAX_LEN; i++)
    {

        if (uid1[i] > uid2[i])
        {

            return 1;
        }

        else if (uid1[i] < uid2[i])
        {

            return -1;
        }
    }

    return 0;
}

// don't modify this function

// compare two records when uid is key

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
                            

// return the total number of records in the

// record_arr

size_t get_num_records() {
    return num_records;
}


// insert record r at the end of the record array

// if the record array is full, allocate

// a new buffer of double size, copy data

// from old buffer to the new buffer

// delete the old buffer

// make record_arr point to the new buffer



void insert_record(struct record r)

{
    if (num_records==max_capacity)
    {
        struct record *t;
        if (num_records==0){
            record_arr = allocate_memory(sizeof(struct record));
            record_arr[num_records] = r;
            max_capacity++;
            num_records++;
        }
        else{
            t=(struct record *)allocate_memory(2*get_num_records()*sizeof(struct record));
            max_capacity*=2;
        }
        for(int i=0;i<num_records;i++)
        {
            t[i]=record_arr[i];
        }
        t[num_records]=r;
        num_records++;
        free_memory(record_arr);
        record_arr=t;
    }
    else
    {
        record_arr[num_records]=r;
        num_records++;
    }
}
// return the record corresponding to the uid

// use the linear search algorithm

// you can also use cmp_uid routine defined earlier

// if none of the records contain the input uid

// return a dummy record with status == -1

struct record search_record_linear(char uid[MAX_LEN])

{
    for (int i = 0; i < get_num_records(); i++) {
        if (cmp_uid(record_arr[i].uid, uid) == 0) {
            return record_arr[i];
        }
    }
    struct record dummy_record;
    dummy_record.status = -1;
    return dummy_record;
}

// return the record corresponding to the uid


 struct record search_record_binary(char uid[MAX_LEN]) {
    int s=0;
    int e=num_records-1;
    int mid;
    while(s<=e) {
        mid = s + (e-s)/2;
        if(cmp_uid(record_arr[mid].uid, uid) == 0) {
            return record_arr[mid];
        }
        else if(cmp_uid(record_arr[mid].uid, uid)==1) {
            e=mid-1;
        }
        else {
            s=mid+1;
        }
    }
    // If the UID is not found, return an empty record
    struct record dummy_record;
    dummy_record.status = -1;
    return dummy_record;
}

struct record delete_record(char uid[MAX_LEN]) {
    int found = 0;
    int index = -1;
    for (int i = 0; i < num_records; i++) {
        if (cmp_uid(record_arr[i].uid, uid) == 0) {
            found = 1;
            index = i;
            break;
        }
    }
    if (!found) {
        struct record dummy_record;
        dummy_record.status = -1;
        return dummy_record;
    }
    struct record deleted_record = record_arr[index];
    for (int i = index; i < num_records - 1; i++) {
        record_arr[i] = record_arr[i + 1];
    }
    num_records--;
    if (num_records <= max_capacity / 4) {
        int new_capacity = max_capacity / 2;
        struct record *new_arr = (struct record *)allocate_memory(sizeof(struct record) * ((max_capacity) / 2));
        for (int i = 0; i < num_records; i++) {
            new_arr[i] = record_arr[i];
        }
        free_memory(record_arr);
        record_arr = new_arr;
        max_capacity = new_capacity;
    }
    
    return deleted_record;
}



void swap(struct record arr[], int a, int b)

{
    struct record temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}
void delete_all_records()

{

    free_memory(record_arr);

    max_capacity = 0;

    num_records = 0;
}



void sort_records_q(struct record arr[], int low, int high) {
    if (low < high) {
        int p=partition(arr,low,high);
        sort_records_q(arr,low,p - 1);
        sort_records_q(arr,p+1,high);
    }
}
int partition(struct record arr[],int low,int high) {
    struct record pivot=arr[high];
    int i=low-1;

    for (int j=low;j<high;j++) {
        if (cmp_uid(arr[j].uid,pivot.uid)<=0) {
            i++;
            struct record temp=arr[i];
            arr[i]=arr[j];
            arr[j]=temp;
        }
    }

    struct record temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i+1;
}


void quicksort(struct record arr[], int lo, int hi)

{

    if (lo >= hi)

    {

        return;
    }

    int p = partition(arr, lo, hi);

    quicksort(arr, lo, p - 1);

    quicksort(arr, p + 1, hi);
}

void sort_records_quick()

{

    quicksort(record_arr, 0, num_records - 1);
}


// sort the record_arr using merge sort

// use cmp_record implementaion to compare two records

void merge(struct record *arr,struct record *tmp, int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = 0;
    
    while (i <= mid && j <= right) {
        if (cmp_record(&arr[i], &arr[j]) <= 0) {
            tmp[k++] = arr[i++];
        } else {
            tmp[k++] = arr[j++];
        }
    }
    
    while (i <= mid) {
        tmp[k++] = arr[i++];
    }
    
    while (j <= right) {
        tmp[k++] = arr[j++];
    }
    
    for (int p = 0; p < k; p++) {
        arr[left + p] = tmp[p];
    }
}
void sort_records_merge(){
	sort_records_quick (record_arr,0,num_records-1);
}



// sort the record_arr using selection sort

// use cmp_record implementaion to compare two records

void sort_records_selection()

{
    for (int i = 0; i < num_records - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < num_records; j++) {
             if (cmp_uid(record_arr[j].uid, record_arr[min_idx].uid) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(record_arr,i,min_idx);
        }
    }
}
size_t get_num_records_with_name_linear(char name[MAX_LEN])

{

    int counter = 0;

    for (int i = 0; i < num_records; i++)

    {

        if (strcmp(record_arr[i].name, name) == 0)

        {

            counter++;
        }}
    return counter;
}
int asd(char name[MAX_LEN]){
return get_num_records_with_name_linear(name);}



int partition(struct record arr[], int lo, int hi)

{

    struct record pivot = arr[lo];

    int left = lo + 1;

    int right = hi;

    while (left <= right)

    {

        while (left <= right && cmp_uid(arr[left].name, pivot.name) <= 0)

        {

            left++;
        }

        while (left <= right && cmp_uid(arr[right].name, pivot.name) > 0)

        {

            right--;
        }

        if (left <= right)

        {

            swap(arr, left, right);

            left++;

            right--;
        }
    }

    swap(arr, lo, right);

    return right;
}

void sort2(struct record arr[], int lo, int hi)

{

    if (lo >= hi)

    {

        return;
    }

    int p = parti(arr, lo, hi);

    sort2(arr, lo, p - 1);

    sort2(arr, p + 1, hi);
}

void rearrange_data()

{

    sort2(record_arr, 0, num_records - 1);
}



size_t get_num_records_with_name_binary(char name[MAX_LEN])

{
size_t count = 0;  
    size_t low = 0;  
    size_t high = num_records - 1; 
    while (low <= high) {
        size_t mid = low + (high - low) / 2; 
        if (cmp_uid(record_arr[mid].name, name) == 0) {
            count++; 
            size_t i = mid - 1;
            while (i >= low && cmp_uid(record_arr[i].name, name) == 0) {
                count++;
                i--;
            }
            i = mid + 1;
            while (i <= high && cmp_uid(record_arr[i].name, name) == 0) {
                count++;
                i++;
            }
            return count;
        } else if (cmp_uid(record_arr[mid].name, name) < 0) {
            low = mid + 1;  
        } else {
            high = mid - 1; 
        }
    }
    return count;

}