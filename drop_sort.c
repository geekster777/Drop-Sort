#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define TOTAL_NUMS 100

//A linked list head, holding a node and the length of the list
struct linked_list {
  struct node * head;
  int length;
} linked_list;

//a node in the linked list
struct node {
  int val;
  struct node * next;
} node;

void drop_sort(struct linked_list * list) {
  int iterations=0;
  struct linked_list * sorted = malloc(sizeof(linked_list));
  struct linked_list * result = malloc(sizeof(linked_list));
  struct linked_list * dropped = malloc(sizeof(linked_list));
  struct node * filler = malloc(sizeof(node));
  sorted->length=0;
  sorted->head=NULL;

  while(list->length>0) {
    iterations++;
    dropped->length=0;
    dropped->head=NULL;

    struct node * current = list->head;
    while(current->next!=NULL) {
      //checks if the following number is smaller than the current one
      if(current->val > current->next->val) {
        //drops the number that is out of order, and adds it to the dropped
        struct node * drop = current->next;
        current->next = drop->next;
        drop->next=dropped->head;
        dropped->head=drop;
        dropped->length++;
        list->length--;
      }
      else {
        //proceeds if the number is in order
        current=current->next;
      }
    }

    //The numbers inside list are now entirely in order. Now to merge them.
    result->length=0;
    current=filler;
    result->head=current;
    current->next=NULL;
    
    while(list->length>0 || sorted->length>0) {
      if(list->length==0) {
        current->next=sorted->head;
        result->length+=sorted->length;
        sorted->length=0;
      }
      else if(sorted->length==0) {
        current->next=list->head;
        result->length+=list->length;
        list->length=0;
      }
      else if(list->head->val<sorted->head->val) {
        struct node * store_node = list->head;
        list->head=list->head->next;
        list->length--;
        current->next=store_node;
        current=store_node;
        result->length++;
      }
      else {
        struct node * store_node = sorted->head;
        sorted->head=sorted->head->next;
        sorted->length--;
        current->next=store_node;
        current=store_node;
        result->length++;
      }
    }
    result->head=result->head->next;

    sorted->head=result->head;
    sorted->length=result->length;

    list->head=dropped->head;
    list->length=dropped->length;
  }

  list->head=sorted->head;
  list->length=sorted->length;
  free(sorted);
  free(result);
  free(dropped);
  free(filler);
}

int main() {
  srand(time(NULL));
  
  //randomizes a list of numbers
  int nums[TOTAL_NUMS];
  for(int i=0; i<TOTAL_NUMS; i++)
    nums[i]=rand()%1000;
  
  //declares our initial linked list
  struct linked_list * list = malloc(sizeof(linked_list)); 
  list->length=0;
  struct node * last = NULL;
  
  //puts all of the values into the linked list backwards (more efficient)
  for(int i=0; i<TOTAL_NUMS; i++) {
    list->length++;
    struct node * current = malloc(sizeof(node));
    current->next=last;
    current->val = nums[i];
    last=current;
    nums[i]=0;
  }
  
  //links the list to the head
  list->head=last;
  drop_sort(list); 
  
  struct node * current = list->head;

  //puts all the values back into the array
  int i=0;
  while(current!=NULL && i<TOTAL_NUMS) {
    nums[i++]=current->val;
    struct node * last = current;
    current=current->next;
    free(last);
  }
  
  //displays the array
  for(int i=0; i<TOTAL_NUMS; i++)
    printf("%d ",nums[i]);
  printf("\n");
      
  return 0;
}
