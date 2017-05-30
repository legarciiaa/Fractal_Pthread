#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

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
    int xmin;
    int xmax;
    int ymin;
    int ymax;
    int** image;
};

//start the list
struct BagTask *work;
struct BagTask *result;
int size= 0;
int max_iter = 100;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void mandelbrot(void *fractal)
{
   //http://libxbgi.sourceforge.net/mandelbrot.c
   //http://www.codewithc.com/how-to-include-graphics-h-in-codeblocks/
   /*
   struct Node *node= &work->first;
   struct FractalIndex *index = node->index;
   double x1 = index->xmin;
   double y1 = index->ymin;
   double x2 = index->xmax;
   double y2 = index->ymax;
   int xx, yy, counter;
   double dx, dy, x, y, a, b, tx, d;
   int maxx, maxy = size;
   dx = (x2 - x1) / maxx;
   dy = (y2 - y1) / maxy;
   x = x1;
   
   for (xx = 0; xx < maxx; xx++) {
    y = y1;
    for (yy = 0; yy < maxy; yy++) {
      counter = 0;
      a = b = 0.0;
      do {
         tx = a*a - b*b + x;
         b = 2*b*a + y;
	     a = tx;
         d = a*a + b*b;
	     counter++;
      } while (d <= 4.0 && counter < max_iter);
      index ->image[xx][yy] = counter;
      printf("\nIMAGE: %d %d COUNTERT %d ", xx,yy,counter);
       //setrgbcolor(counter);
      //_putpixel (xx, yy);
      y += dy;
    }
    x += dx;
  }
  
   //FIFO order --ARRUMAR
   if(result== NULL){
      result->first = &node;
      result->last = &node;
   }else{
      result->last->next= &node;
   }
      
  */
}

void workFractal(){
    //https://gist.github.com/andrejbauer/7919569
    //Thread consumer, work in fractal and remove task in queue
    struct Node *node= &work->first;

    //use while() (exemplo righi)
    while(node != NULL){
      pthread_mutex_lock(&m);
        deleteQueue(work,node);
        mandelbrot(node);
      pthread_mutex_lock(&m);
    }

}

void drawFractal(){
    //Draw fractal (search lib to draw)
    //use while() (exemplo righi)
    struct Node *node= &result->first;

    while(node != NULL){
      pthread_mutex_lock(&m);
        
      pthread_mutex_lock(&m);
    }
    
}

void insertWork(){
    //https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_in_c.htm --LINKED LIST
    int i=0;
    int sizeTask=size;
    struct FractalIndex fractal= {0,0,0,0};;
    struct Node node;

    //add to list of task
    while (sizeTask > 0 ){
       printf("\nCreate SIZE: %d I: %d",sizeTask,i);
       fractal.xmin=fractal.ymin=i;

       if (sizeTask > 0){
           fractal.xmax=fractal.ymax=i=i+19;
           sizeTask = sizeTask-20;
       }else{
           fractal.xmax=fractal.ymax=sizeTask;
           sizeTask = 0;
       }
       i++;

       printf("\nminx: %d miny: %d nmaxx: %d maxy: %d ",fractal.xmin, fractal.ymin,fractal.xmax, fractal.ymax);

       node.index = &fractal;
       //FIFO order --ARRUMAR
       /*if(work == NULL){
          work->first = &node;
          work->last = &node;
       }else{
          work->last->next= &node;
       }*/

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
           		return;
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
   }
}

int main(int argc, char *argv[]) {
    int nThreads = atoi(argv[1]);
    int nColor = atoi(argv[2]);
    size = atoi(argv[3]); //fractal size total

    int threadCounter;
    pthread_t threadId[nThreads];


    if (size > 20){
        //add tasks
        insertWork();

        //create the threads to work and draw
		for(threadCounter=0; threadCounter<nThreads; threadCounter++){
			pthread_create(&threadId[threadCounter], NULL, workFractal,NULL);
			pthread_create(&threadId[threadCounter], NULL, drawFractal, NULL);
		}

		for(threadCounter=0; threadCounter<nThreads; threadCounter++){
			pthread_join(threadId[threadCounter], NULL);
		}
    }
    return 0;
}
