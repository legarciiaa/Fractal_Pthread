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
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    int** image;
};

//start the list
struct BagTask *work= NULL;
struct BagTask *result = NULL;
int size= 0;
int max_iter = 100;

void mandelbrot(void *fractal)
{
   //http://libxbgi.sourceforge.net/mandelbrot.c
   //http://www.codewithc.com/how-to-include-graphics-h-in-codeblocks/
   /*
   struct FractalIndex *index = fractal;
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
  }*/
}

void workFractal(){
    //https://gist.github.com/andrejbauer/7919569
    //Thread consumer, work in fractal and remove task in queue
    struct Node *node=NULL;
    //use while() (exemplo righi)
    while(work->first != NULL){
      node = work->first;
      deleteQueue(work,node);
      mandelbrot(&node);
    }

}

void drawFractal(){
    //Draw fractal (search lib to draw)
    //use while() (exemplo righi)
}

void insertWork(int size){
    //https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_in_c.htm --LINKED LIST
    int i=0;
    int count=0;
    struct FractalIndex fractal[size/20];
    struct Node node[size/20];

    //add to list of task

    while (size > 0 ){
       printf("\nCreate work: %d \nSIZE: %d",count,size);
       fractal[count].xmin=i;
       fractal[count].ymin=i;

       if ((size-1) > i){
           i=i+20;
           size = size-20;
       }else{
           i=size;
           size = 0;
       }
       fractal[count].xmax=i;
       fractal[count].ymax=i;

       node[count].index = &fractal[count];

       //FIFO order
       if(work == NULL){
          work->first = &node[count];
          work->last = &node[count];
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
