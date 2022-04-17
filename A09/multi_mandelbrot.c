#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"

void sub_image(struct ppm_pixel* palette, struct ppm_pixel* result, int size, int start_row, int start_col, int xmin, int xmax, int ymin, int ymax, int maxIterations){
  int end_row = start_row + size/2;
  int end_col = start_col + size/2;
  for (int r = start_row; r < end_row; r++){
    for (int c = start_col; c < end_col; c++){
      float row = r, col = c;
      float xfrac = col/size;
      float yfrac = row/size;
      float x0 = xmin + xfrac*(xmax - xmin);
      float y0 = ymin + yfrac*(ymax - ymin);
      float x = 0, y = 0;
      int iter = 0;
      while (iter < maxIterations && x*x + y*y < 2*2){
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }
      if (iter < maxIterations) {
        int cols = size/2;
        result[r*cols + c] = palette[iter];
      }
      else {
        int cols = size/2;
        result[r*cols + c].red = 0;
        result[r*cols + c].green = 0;
        result[r*cols + c].blue = 0;
      }
    }
  }
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

  struct ppm_pixel* palette = (struct ppm_pixel*)(malloc(sizeof(struct ppm_pixel)*maxIterations));
  for (int j = 0; j < maxIterations; j++){
    palette[j].red = rand() % 255;
    palette[j].green = rand() % 255;
    palette[j].blue = rand() % 255;
  }

  int shared_memory = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel)*size*size, 0644 | IPC_CREAT);
  if (shared_memory == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }
  struct ppm_pixel* result = shmat(shared_memory, NULL, 0);
  if (result == (void*) -1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  } 

  pid_t child_1, child_2, child_3, child_4;
  child_1 = fork();          //child process 1
  if (child_1 > 0 ) {
    child_2 = fork();       //child process 2
    if (child_2 > 0){
      child_3 = fork();     //child process 3
      if (child_3 > 0) {
        child_4 = fork();   //child process 4
        if (child_4 == 0){
          int start_row = 0;
          int start_col = 0;
          int end_row = start_row + size/2;
          int end_col = start_col + size/2;
          printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", getpid(), start_col, end_col, start_row, end_row);
          printf("Launched child process: %d\n", getpid());
          sub_image(palette,result,size,0,0,xmin,xmax,ymin,ymax,maxIterations);
          exit(0);
        }
      }
      else {
        int start_row = size/2;
        int start_col = 0;
        int end_row = start_row + size/2;
        int end_col = start_col + size/2;
        printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", getpid(), start_col, end_col, start_row, end_row);
        printf("Launched child process: %d\n", getpid());
        sub_image(palette,result,size,size/2,0,xmin,xmax,ymin,ymax,maxIterations);
        exit(0);
      }
    }
    else {
      int start_row = 0;
      int start_col = size/2;
      int end_row = start_row + size/2;
      int end_col = start_col + size/2;
      printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", getpid(), start_col, end_col, start_row, end_row);
      printf("Launched child process: %d\n", getpid());
      sub_image(palette,result,size,0,size/2,xmin,xmax,ymin,ymax,maxIterations);
      exit(0);
    }
  }
  else {
    int start_row = size/2;
    int start_col = size/2;
    int end_row = start_row + size/2;
    int end_col = start_col + size/2;
    printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", getpid(), start_col, end_col, start_row, end_row);
    printf("Launched child process: %d\n", getpid());
    sub_image(palette,result,size,size/2,size/2,xmin,xmax,ymin,ymax,maxIterations);
    exit(0);
  }

  for (int i = 0; i < 4; i++) {
    int status;
    int pid = wait(&status);
    printf("Child process complete: %d\n", pid);
  }

  struct ppm_pixel** final_result = (struct ppm_pixel**)malloc(sizeof(struct ppm_pixel*)*(size));
  for (int i = 0; i < size; i++) { 
    final_result[i] = malloc(sizeof(struct ppm_pixel)*(size)); 
  } 

  for (int i = 0; i < size; i++){
    for (int j = 0; j < size; j++){
      final_result[i][j] = result[i*size + j];
    }
  }

  char newFile[50];
  int timeNow = time(0);
  sprintf(newFile, "temp_mandelbrot-%d-%d.ppm", size, timeNow);
  printf("Writing file: %s\n", newFile);
  write_ppm(newFile, final_result, size , size);

  if (shmdt(result) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }

  if (shmctl(shared_memory, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }  
   
  free(palette);
  palette = NULL;
  result = NULL;
  for (int i = 0; i < size; i++){
    free(final_result[i]);
  } 
  free(final_result);
  final_result = NULL;
}
