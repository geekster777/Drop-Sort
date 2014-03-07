#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define TOTAL_NUMS 100

//A linked list head, holding a node and the length of the list
typedef struct {
  struct node_t * head;
  int length;
} linked_list;

//a node in the linked list
struct node_t {
  int val;
  struct node_t * next;
} node_t;

typedef struct node_t node;

//the heap datatype
typedef struct {
  struct heap_node_t * head;
  int length;
} heap_list;

//an individual node in the heap that holds a linked list
struct heap_node_t {
  struct node_t * val;
  struct heap_node_t * left;
  struct heap_node_t * right;
  struct heap_node_t * parent;
} heap_node_t;

typedef struct heap_node_t heap_node;

/**
 * Helper function to switch the values to two nodes 
 * Returns the second node sent
 */
heap_node * switch_nodes(heap_node * n1, heap_node * n2) {
  struct node_t * store_val = n1->val;
  n1->val = n2->val;
  n2->val = store_val;
  return n2;
}

/**
 * Adds a node into the heap. Then bubbles the node up the heap so that
 * the heap is in order
 *
 * list - the heap of linked lists. This list MUST be balanced, or the
 * node will not be added properly
 *
 * val - the node that you will be inserting into the heap
 */
void heap_add_node(heap_list * list, node * val) {
  //makes the node larger
  list->length++;

  //uses the length as an identifier of where to put the node
  int pos=list->length;
  heap_node * current = list->head;

  //initializes the node
  heap_node * new_node = malloc(sizeof(heap_node)); 
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
  //This is more efficient, and makes a balanced tree. 
  while(pos>3) {
    if(pos%2)
      current = current->right;
    else
      current = current->left;
    pos=pos/2;
  }

  //places the node.
  if(pos==2) 
    current->left = new_node;
  else
    current->right = new_node;
  new_node->parent = current;

  //bubbles the node up the heap so that it is in order;
  while(new_node->parent!=NULL && 
    new_node->parent->val->val > new_node->val->val) {
    
    new_node = switch_nodes(new_node,new_node->parent);
  }
}

/**
 * Pops the top value off of the heap, then re-orders the heap. It will
 * remove linked lists from the heap as they empty.
 *
 * heap - the heap containing ordered linked lists for elements. 
 */
node * pop_heap(heap_list * heap) {

  //the current node.
  heap_node * current = heap->head;

  //the value that will be returned
  node * returner = current->val;

  //pushes the linked list forward an element
  current->val=current->val->next;
  
  //bubble the head to the bottom of the list, and remove it.
  if(current->val==NULL) {
    
    //bubble the current position to the bottom of the heap
    while(current->left!=NULL || current->right!=NULL) {
      if(current->left == NULL) {
        current = switch_nodes(current, current->right);
      }
      else if(current->right == NULL) {
        current = switch_nodes(current, current->left);
      }
      else if(current->left->val->val < current->right->val->val) {
        current = switch_nodes(current, current->left);
      }
      else {
        current = switch_nodes(current, current->right);
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
      node * store_val = current->val;

      //checks if the lefthand node is the smallest value
      if(current->left == NULL) {
        current = switch_nodes(current, current->right);
      }
      else if(current->right == NULL) {
        current = switch_nodes(current, current->left);
      }
      else if(current->left->val->val < store_val->val &&
        current->left->val->val < current->right->val->val) {

        current = switch_nodes(current, current->left);
      }
      else if(current->right->val->val < store_val->val) {
        current = switch_nodes(current, current->right);
      }
      else
        return returner;
    }
  }

  return returner;
}

/**
 * Sorts the numbers using a modified drop sort that puts a set of ordered
 * linked lists into a heap. Then uses that heap to pop them off in order
 * 
 * list - the linked of numbers for the sort
 */
void drop_sort(linked_list * list) {
  //research value
  int iterations=0;

  //some datastructures to help with the sort
  linked_list * dropped = malloc(sizeof(linked_list));
  heap_list * heap = malloc(sizeof(heap_list));

  //initializes the heap
  heap->length=0;
  heap->head=NULL;

  //loops until no more numbers are left to be dropped
  while(list->length>0) {

    //counts how many times the list must be iterated
    iterations++;

    //resets the list of dropped items
    dropped->length=0;
    dropped->head=NULL;

    //gets the first node in the list. There is guaranteed to be one
    node * current = list->head;

    //loops through the list, dropping any numbers that are out of order.
    //This will leave you with a sorted linked list
    while(current->next!=NULL) {
      //checks if the following number is smaller than the current one
      if(current->val > current->next->val) {
        
        //drops the number that is out of order, and adds it to the dropped
        node * drop = current->next;
        current->next = drop->next;
        drop->next=dropped->head;
        dropped->head=drop;

        //readjusts the lengths of moving a node
        dropped->length++;
        list->length--;
      }
      else {
        //proceeds if the number is in order
        current=current->next;
      }
    }
    
    //places that sorted linked list into the heap
    heap_add_node(heap,list->head);
    printf("%d - ",list->length);
    node * cur = list->head;
    while(cur!=NULL) {
      printf("%d ",cur->val);
      cur=cur->next;
    }
    printf("\n");
    //puts the dropped items back into the list
    list->head=dropped->head;
    list->length=dropped->length;
  }
 
  //Pops the first element off the heap
  node * current = pop_heap(heap);
  list->head=current;
  list->length=1;
  
  //pops all the elements off the heap, and puts them into the linked list.
  while(heap->length>0) {
    current->next=pop_heap(heap);
    current=current->next;
    list->length++;
  }

  //null terminates the linked list
  current->next=NULL;
  
  //frees up the list we created
  free(dropped);
  printf("%d\n",iterations);
}

int main() {
  srand(time(NULL));
  
  //randomizes a list of numbers
  int nums[TOTAL_NUMS];
  for(int i=0; i<TOTAL_NUMS; i++)
    nums[i]=rand()%100;
  
  //declares our initial linked list
  linked_list * list = malloc(sizeof(linked_list)); 
  list->length=0;
  node * last = NULL;
  
  //puts all of the values into the linked list backwards (more efficient)
  for(int i=0; i<TOTAL_NUMS; i++) {
    list->length++;
    node * current = malloc(sizeof(node));
    current->next=last;
    current->val = nums[i];
    last=current;
    nums[i]=0;
  }
  
  //links the list to the head
  list->head=last;
  drop_sort(list); 
  
  node * current = list->head;

  //puts all the values back into the array
  int i=0;
  while(current!=NULL && i<TOTAL_NUMS) {
    nums[i++]=current->val;
    node * last = current;
    current=current->next;
    free(last);
  }
  
  //displays the array
  for(int i=0; i<TOTAL_NUMS; i++) {
    if(i>0 && nums[i]<nums[i-1])
      printf("* ");
    printf("%d ",nums[i]);
  }
  printf("\n");
      
  return 0;
}
