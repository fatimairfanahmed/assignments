#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"

pthread_mutex_t mutex;

struct thread_data {
  struct ppm_pixel* palette;
  struct ppm_pixel** result;
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
};

void* sub_image(void* userData){
  pthread_mutex_lock(&mutex);
  struct thread_data* data = (struct thread_data*)userData;
  int end_row = data->start_row + data->sub_size;
  int end_col = data->start_col + data->sub_size;
  printf("Thread  %lu) sub-image block: cols (%d, %d) to rows (%d, %d)\n", pthread_self(), data->start_col, end_col, data->start_row, end_row);
  for (int r = data->start_row; r < end_row; r++){
    for (int c = data->start_col; c < end_col; c++){
      float row = r;
      float col = c;
      float xfrac = col/data->size;
      float yfrac = row/data->size;
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
        data->result[r][c] = data->palette[iter];
      }
      else {
        data->result[r][c].red = 0;
        data->result[r][c].green = 0;
        data->result[r][c].blue = 0;
      }
    }
  }
    printf("Thread  %lu) finished\n", pthread_self());
    pthread_mutex_unlock(&mutex);
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

  struct ppm_pixel** result = (struct ppm_pixel**)malloc(sizeof(struct ppm_pixel*)*(size));
  for (int i = 0; i < size; i++) { 
    result[i] = malloc(sizeof(struct ppm_pixel)*(size)); 
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

  pthread_mutex_init(&mutex, NULL);
  for (int i = 0; i < 4; i++) {
    data[i].palette = palette;
    data[i].result = result;
    data[i].xmin = xmin;
    data[i].xmax = xmax;
    data[i].ymin = ymin;
    data[i].ymax = ymax;
    data[i].maxIterations = maxIterations;
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
  sprintf(newFile, "mandelbrot-%d-%d.ppm", size, timeNow);
  printf("Writing file: %s\n", newFile);
  write_ppm(newFile, result, size , size);

  for (int i = 0; i < size; i++){
    free(result[i]);
  } 
  free(result);
  free(palette);
  result = NULL;
  palette = NULL;
  pthread_mutex_destroy(&mutex); 
}
