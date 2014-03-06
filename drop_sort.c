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

struct heap_list {
  struct heap_node * head;
  int length;
} heap_list;

struct heap_node {
  struct node * val;
  struct heap_node * left;
  struct heap_node * right;
  struct heap_node * parent;
} heap_node;

void heap_add_node(struct heap_list * list, struct node * val) {
  list->length++;
  int pos=list->length;
  struct heap_node * current = list->head;
  struct heap_node * new_node = malloc(sizeof(heap_node));
  
  new_node->left=NULL;
  new_node->right=NULL;
  new_node->val=val;
  
  //creates the initial element if the heap is empty
  if(pos==1) {
    new_node->parent=NULL;
    list->head=new_node;
    return;
  }
  
  //finds the current position in the heap to add the new item. 
  while(pos>3) {
    if(pos%2)
      current = current->right;
    else
      current = current->left;
    pos=pos/2;
  }
  if(pos==2) 
    current->left = new_node;
  else
    current->right = new_node;
  new_node->parent = current;

  //bubbles the node up the heap so that it is in order;
  while(new_node->parent!=NULL && 
    new_node->parent->val->val > new_node->val->val) {
    struct node * store_val = new_node->val;
    new_node->val=new_node->parent->val;
    new_node=new_node->parent;
    new_node->val=store_val;
  }
}

struct node * pop_heap(struct heap_list * heap) {
  struct heap_node * current = heap->head;
  struct node * returner = current->val;
  current->val=current->val->next;
  
  //bubble the head to the bottom of the list, and remove it.
  if(current->val==NULL) {
    
    //bubble the current position ot the bottom of the heap
    while(current->left!=NULL || current->right!=NULL) {
      if(current->left == NULL) {
        current->val = current->right->val;
        current=current->right;
        current->val = NULL;
      }
      else if(current->right == NULL) {
        current->val = current->left->val;
        current=current->left;
        current->val = NULL;
      }
      else if(current->left->val->val < current->right->val->val) {
        current->val = current->left->val;
        current=current->left;
        current->val = NULL;
      }
      else {
        current->val = current->right->val;
        current=current->right;
        current->val = NULL;
      }
    }

    //removes the node from the heap
    if(current->parent != NULL && current == current->parent->left)
      current->parent->left=NULL;
    else if(current->parent != NULL)
      current->parent->right=NULL;
    else
      heap->length=1;
    free(current);
    
    heap->length--;
  }
  
  //bubbles up any nodes with smaller values than the head.
  else {
    while(current->left!=NULL || current->right!=NULL) {
      struct node * store_val = current->val;
      //checks if the lefthand node is the smallest value
      if(current->left == NULL) {
        current->val = current->right->val;
        current=current->right;
        current->val = store_val;
      }
      else if(current->right == NULL) {
        current->val = current->left->val;
        current=current->left;
        current->val = store_val;
      }
      else if(current->left->val->val < store_val->val &&
        current->left->val->val < current->right->val->val) {

        current->val = current->left->val;
        current = current->left;
        current->val = store_val;
      }
      else if(current->right->val->val < store_val->val) {
        current->val = current->right->val;
        current = current->right;
        current->val = store_val;
      }
      else
        return returner;
    }

  }

  return returner;
}

void drop_sort(struct linked_list * list) {
  int iterations=0;
  struct linked_list * sorted = malloc(sizeof(linked_list));
  struct linked_list * result = malloc(sizeof(linked_list));
  struct linked_list * dropped = malloc(sizeof(linked_list));
  struct heap_list * heap = malloc(sizeof(heap_list));
  heap->length=0;
  heap->head=NULL;
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
    
    heap_add_node(heap,list->head);
    list->head=dropped->head;
    list->length=dropped->length;
  }
  
  struct node filler;
  node.val=NULL;
  node.next=NULL;
  struct node * current = &node;
  sorted->head=current;
  sorted->length=0;
  printf("\n");
  while(heap->length>0) {
    current->next=pop_heap(heap);
    current=current->next;
    sorted->length++;
  }
  sorted->head=sorted->head->next;
  
  list->head=sorted->head;
  list->length=sorted->length;
  free(sorted);
  free(result);
  free(dropped);
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
