#include <pthread.h>
#include <math.h>
#include <stdlib.h>

//queue to bag of task
struct Node{
    struct FractalIndex *index;
    struct Node *next;
};

struct BagTask{
   struct Node *first;
   struct Node *last;
};

struct FractalIndex{
    double xmin;
    double xmax;
    double ymin;
    double ymax;
};

//start the list
struct BagTask *work= NULL;
struct BagTask *result = NULL;


void workFractal(){
    //https://gist.github.com/andrejbauer/7919569
    //Thread consumer, work in fractal and remove task in queue

    //use while() (exemplo righi)

}

void drawFractal(){
    //Draw fractal (search lib to draw)
    //use while() (exemplo righi)

}

void insertWork(int size){
    //https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_in_c.htm --LINKED LIST
    int i=0;
    int count=0;
    struct Node *node=NULL;
    struct FractalIndex *fractal=NULL;

    //add to list of task
    while (size > 0 ){
       fractal[count].xmin=i;
       fractal[count].ymin=i;

       if (size < i+20){
           i=i+20;
           size = size-20;
       }else{
           i=size;
           size = 0;
       }
       fractal[count].xmax=i;
       fractal[count].ymax=i;
       node[count].index= &fractal[count];

       //FIFO order
       if(work->first== NULL){
          work->first= work->last= &node[count];
       }else{
          work->last->next= &node[count];
       }
       count++;
    }
}

void deleteQueue(struct BagTask *bag, struct Node *task){
    //start from the first link
   struct Node* current = bag->first;
   struct Node* previous = NULL;

   if(bag->first != NULL) {
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
         bag->first = bag->first->next;
       } else {
         previous->next = current->next;
       }

   return current;
}

int main(int argc, char *argv[]) {
    int nColor = atoi(argv[1]);
    int nThreads = atoi(argv[2]);
    int size= atoi(argv[3]); //fractal size total

    if (size > 20){
        //add tasks
        insertWork(size);

        //create the thread to work
        workFractal(nThreads);

        //create the thread to draw
        drawFractal(nThreads);
    }
    return 0;
}
