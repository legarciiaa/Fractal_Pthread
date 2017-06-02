#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <signal.h>

struct FractalIndex{
    int xmin;
    int xmax;
    int ymin;
    int ymax;
    int** image;
};

//queue to bag of task
struct Node{
    struct FractalIndex index;
    struct Node* next;
};

struct BagTask{
   struct Node *first;
   struct Node *last;
};

//start the list
//struct BagTask work;
struct BagTask result;
int size= 0;
int max_iter = 100;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t m;
static pthread_cond_t condWork;
static pthread_cond_t condResult;
int taskSize = 20;

void mandelbrot(void *fractal){
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

   //FIFO order
   if(result.first == NULL){
      result.first = &node;
      result.last = &node;
	} else{
	  result.last.next = &node;
	  result.last = &node;
	}
	//wake up condition
	pthread_cond_signal(&condResilt);
 */

}

void *workFractal(void *params){
    //https://gist.github.com/andrejbauer/7919569
    //Thread consumer, work in fractal and remove task in queue

	//use while() (exemplo righi)
	//struct Node *node= work.first;

    while(!condWork){
        //wait for the condition
        pthread_mutex_lock(&m);
        pthread_cond_wait(&condWork, &m);
        /*while(work.first != NULL){
        	//pthread_mutex_lock(&m);
            //printf("\nWORKING IN: Min x: %d Min y: %d Max x: %d Max y: %d", work.first->index.xmin, work.first->index.ymin, work.first->index.xmax, work.first->index.ymax);
            //printf("\nFirst work: Min x: %d Min y: %d Max x: %d Max y: %d", work.first->index.xmin, work.first->index.ymin, work.first->index.xmax, work.first->index.ymax);
            //printf("\nNEXT IN: Min x: %d Min y: %d Max x: %d Max y: %d", work.first->next->index.xmin, work.first->next->index.ymin, work.first->next->index.xmax, work.first->next->index.ymax);
            mandelbrot(work.first);
			deleteQueue(work,work.first);
          	pthread_mutex_unlock(&m);
        }*/
    }

}

void drawFractal(){
    //Draw fractal (search lib to draw)
    //use while() (exemplo righi)

   struct Node *node= result.first;
   while(condResult){
        //wait for the condition
        pthread_cond_wait(&condResult, &m);
        while(result.first != NULL){
           pthread_mutex_lock(&m);
           //draw
           node= result.first;
           pthread_mutex_lock(&m);
        }
    }
}

void insertWork(struct BagTask *work){
	int nextIndex=0;
    int sizeTask=size;
    struct Node node;
    struct Node *currentNode;
    struct FractalIndex fractal = {0, 0, 0, 0};
    
    //add work to list of tasks and remove from original size
    while (sizeTask > 0 ){
       printf("\nCreate job. Updated original size: %d Next index: %d", sizeTask, nextIndex);
       fractal.xmin=fractal.ymin=nextIndex;

       if (sizeTask > 0){
           fractal.xmax=fractal.ymax=nextIndex=nextIndex+(taskSize-1);
           sizeTask = sizeTask-taskSize;
       }else{
           fractal.xmax=fractal.ymax=sizeTask;
           sizeTask = 0;
       }
       nextIndex++;
       
       if(work->first == NULL){
            node.index = fractal;
            node.next = NULL;
       		work->first = &node;
       		currentNode = &node;
       		work->last = &node;
		} else{
			//sets the next node as the newly calculated index
			struct Node tempNode;
			tempNode.index = fractal;
			currentNode->next = &tempNode;
			currentNode = &tempNode;
			work->last = &tempNode;
		}

        //pthread_cond_signal(&condWork);
    }
    
    //wake condition condWork
	//	printf("\nWake up condition work");
	//pthread_cond_init(&condWork, NULL);
	//pthread_cond_signal(&condWork);
}

void deleteQueue(struct BagTask *bag, struct Node task){
    //start from the first link
   struct Node* current = bag->first;
   struct Node* previous = NULL;

   if(current != NULL) {
    	//navigate through list
    	while(&current != &task) {
        	if(current->next == NULL) {
           		return;
        	} else {
          		previous = current;
          		current = current->next;
        	}
        }

      	//found a match, update the link
		if(&current == &task) {
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
    int dummyArg = 0;
    struct BagTask bagTask = {NULL, NULL};
    struct BagTask *work = &bagTask;

    int threadCounter;
    pthread_t threadId[nThreads];
    pthread_mutex_init(&m, NULL);

	//if the total size of the fractal is bigger than the bucket size can perform the algorithm
    if (size > taskSize){
        //separate image in tasks and add them to bucket
        insertWork(work);
        
        //create the threads to work and draw
		for(threadCounter=0; threadCounter<nThreads; threadCounter++){
			printf("\nEnter thread for.");
			printf("\nFirst work: Min x: %d Min y: %d Max x: %d Max y: %d", bagTask.first->index.xmin, work->first->index.ymin, work->first->index.xmax, work->first->index.ymax);
			pthread_create(&threadId[threadCounter], NULL, workFractal, &dummyArg);
			//pthread_create(&threadId[threadCounter], NULL, drawFractal, NULL);
		}

	/*	for(threadCounter=0; threadCounter<nThreads; threadCounter++){
			pthread_join(threadId[threadCounter], NULL);
			//pthread_join(threadId[threadCounter], NULL);
		}*/
    }
    return 0;
}
