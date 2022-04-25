#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "stdbool.h"
#include "math.h"

pthread_mutex_t mutex;
pthread_barrier_t barrier;

struct thread_data {
  struct ppm_pixel* palette;
  struct ppm_pixel** result;
  bool** member;
  int** count;
  int start_col;
  int start_row;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
  int sub_size;
  int size;
  int id;
  int max_count;
  pthread_mutex_t mutex;
};

void* sub_image(void* userData) {
  struct thread_data* data = (struct thread_data*)userData;
  int end_row = data->start_row + data->sub_size;
  int end_col = data->start_col + data->sub_size;
  printf("Thread  %lu) sub-image block: cols (%d, %d) to rows (%d, %d)\n", pthread_self(), data->start_col, end_col, data->start_row, end_row);

  //Part 1
  for (int r = data->start_row; r < end_row; r++){
    for (int c = data->start_col; c < end_col; c++){
      float row = r;
      float col = c;
      float xfrac = row/data->size;
      float yfrac = col/data->size;
      float x0 = data->xmin + xfrac*(data->xmax - data->xmin);
      float y0 = data->ymin + yfrac*(data->ymax - data->ymin);
      float x = 0;
      float y = 0;
      int iter = 0;
      while (iter < data->maxIterations && x*x + y*y < 2*2){
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }
      if (iter < data->maxIterations) {
        data->member[r][c] = false;
      }
      else {
        data->member[r][c] = true;
      }
    }
  }

  //Part 2
  for (int r = data->start_row; r < end_row; r++){
    for (int c = data->start_col; c < end_col; c++){
      if (data->member[r][c] == true) {
        continue;
      }
      else {
        float row = r;
        float col = c;
        float xfrac = row/data->size;
        float yfrac = col/data->size;
        float x0 = data->xmin + xfrac*(data->xmax - data->xmin);
        float y0 = data->ymin + yfrac*(data->ymax - data->ymin);
        float x = 0;
        float y = 0;
        while (x*x + y*y < 2*2) {
          float xtmp = x*x - y*y + x0;
          y = 2*x*y + y0;
          x = xtmp;
          int yrow = (int)round(data->size*(y - data->ymin)/(data->ymax - data->ymin));
          int xcol = (int)round(data->size*(x - data->xmin)/(data->xmax - data->xmin));
          if (yrow < 0 || yrow >= data->size) continue; // out of range
          if (xcol < 0 || xcol >= data->size) continue; // out of range
          pthread_mutex_lock(&data->mutex);
          data->count[yrow][xcol] = data->count[yrow][xcol] + 1;
          if (data->count[yrow][xcol] > data->max_count) {
              data->max_count = data->count[yrow][xcol];
          }
          pthread_mutex_unlock(&data->mutex);
        }
      }
      
    }  
  }
  
  //Part 3
  float gamma = 0.681;
  float factor = 1.0/gamma;
  pthread_barrier_wait(&barrier);
  for (int r = data->start_row; r < end_row; r++){
    for (int c = data->start_col; c < end_col; c++){
      float value = 0;
      if (data->count[r][c] > 0){
        value = log((float)data->count[r][c])/log(data->max_count);
        value = pow(value, factor);
      }
      data->result[r][c].red = value * 255;
      data->result[r][c].green = value * 255;
      data->result[r][c].blue = value * 255;
    }
  }
  printf("Thread  %lu) finished\n", pthread_self());
  return (void*)NULL; 
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  struct ppm_pixel** result = (struct ppm_pixel**)malloc(sizeof(struct ppm_pixel*)*(size));
  for (int i = 0; i < size; i++) { 
    result[i] = malloc(sizeof(struct ppm_pixel)*(size)); 
  } 

  bool** member = (bool**)malloc(sizeof(bool*)*(size));
  for (int i = 0; i < size; i++) { 
    member[i] = malloc(sizeof(bool)*(size)); 
  } 
  
  int** count = (int**)malloc(sizeof(int*)*(size));
  for (int i = 0; i < size; i++) { 
    count[i] = malloc(sizeof(int)*(size)); 
  }

  struct ppm_pixel* palette = (struct ppm_pixel*)(malloc(sizeof(struct ppm_pixel)*maxIterations));
  for (int j = 0; j < maxIterations; j++){
    palette[j].red = rand() % 255;
    palette[j].green = rand() % 255;
    palette[j].blue = rand() % 255;
  }
  
  printf("Test with 4 threads\n");
  pthread_t threads[4];
  struct thread_data data[4];
  int subsize = size/2; 

  srand(time(0));
  double timer;
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);

  int max_count = 0;

  int barr = pthread_barrier_init(&barrier, NULL, 4);
  if (barr) {
    printf("ERROR: pthread_barrier_init failed\n");
    exit(0);
  }

  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);
  for (int i = 0; i < 4; i++) {
    data[i].palette = palette;
    data[i].result = result;
    data[i].count = count;
    data[i].member = member;
    data[i].xmin = xmin;
    data[i].xmax = xmax;
    data[i].ymin = ymin;
    data[i].ymax = ymax;
    data[i].mutex = mutex;
    data[i].maxIterations = maxIterations;
    data[i].max_count = max_count;
    data[i].size = size;
    data[i].sub_size = subsize;
    data[i].id = pthread_self();
    switch(i) {
      case 0 : data[i].start_col = 0;       data[i].start_row = 0;        break;
      case 1 : data[i].start_col = size/2;  data[i].start_row = 0;        break;
      case 2 : data[i].start_col = 0;       data[i].start_row = size/2;   break;
      case 3 : data[i].start_col = size/2;  data[i].start_row = size/2;   break;
    }
    pthread_create(&threads[i], NULL, sub_image, (void*)&data[i]);
  }

  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], NULL);
  }  

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %lf seconds\n", size, size,timer);

  char newFile[50];
  int timeNow = time(0);
  sprintf(newFile, "buddhabrot-%d-%d.ppm", size, timeNow);
  printf("Writing file: %s\n", newFile);
  write_ppm(newFile, result, size , size);

  for (int i = 0; i < size; i++){
    free(result[i]);
  } 
  free(result);

  for (int i = 0; i < size; i++){
    free(member[i]);
  } 
  free(member);

  for (int i = 0; i < size; i++){
    free(count[i]);
  } 
  free(count);

  free(palette);
  result = NULL;
  palette = NULL;
  pthread_mutex_destroy(&mutex); 
}
