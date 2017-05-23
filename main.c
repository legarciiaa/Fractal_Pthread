#include <pthread.h>
#include <math.h>
#include <stdlib.h>

//queue to bag of task
typedef struct Node{
    struct FractalIndex *index;
    struct Node *next;
} BagTask;

struct FractalIndex{
    double xmin;
    double xmax;
    double ymin;
    double ymax;
};

BagTask *first = NULL;
BagTask *last = NULL;

void workFractal(){
    //https://gist.github.com/andrejbauer/7919569
    //Thread consumer, work in fractal and remove task in queue

    //use while() (exemplo righi)

}

void drawFractal(){
    //Draw fractal (search lib to draw)
    //use while() (exemplo righi)

}

void insertQueue(int size){
    //https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_in_c.htm --LINKED LIST
    int i=0;
    BagTask *current= (BagTask*)malloc(sizeof(BagTask));
    current ->next =NULL;

    //add to list of task
    while (size > 0 ){
       current->index->xmin=i;
       current->index->ymin=i;

       if (size < i+20){
           i=i+20;
           size = size-20;
       }else{
           i=size;
           size = 0;
       }
       current->index->xmax=i;
       current->index->ymax=i;
       }

       //FIFO order
       if(first== NULL){
          first= last= current;
       }else{
          last->next=current;
          last=current;
       }
}

void deleteQueue(BagTask *task){
    //start from the first link
   struct BagTask* current = first;
   struct BagTask* previous = NULL;

   if(first != NULL) {
      //navigate through list
      while(current != task) {
        if(current->next == NULL) {
           return NULL;
        } else {
          previous = current;
          current = current->next;
        }
     }
      //found a match, update the link
      if(current == task) {
         firsts= first->next;
       } else {
         previous->next = current->next;
       }

   return current;
}

int main(int argc, char *argv[]) {
    int nColor = argv[1];
    int nThreads = argv[2];
    int size= argv[3]; //fractal size total

    //add tasks
    insertQueue(size);

    //create the thread to work
    workFractal(nThreads);

    //create the thread to draw
    drawFractal(nThreads);
}
