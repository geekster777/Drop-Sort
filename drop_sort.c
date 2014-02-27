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

int main() {
  srand(time(NULL));
  
  //randomizes a list of numbers
  int nums[TOTAL_NUMS];
  for(int i=0; i<TOTAL_NUMS; i++)
    nums[i]=rand()%100;
  
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
  }
  
  //links the list to the head
  list->head=last;
  
  struct node * current = list->head;

  //puts all the values back into the array
  int i=0;
  while(current && i<TOTAL_NUMS) {
    nums[i]=current->val;
    current=current->next;
  }
  
  //displays the array
  for(int i=0; i<TOTAL_NUMS; i++)
    printf("%d ",nums[i]);
  
  printf("\n");
    
  return 0;
}
