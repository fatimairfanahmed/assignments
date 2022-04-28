/*
Fatima Irfan
9th April 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"

struct ppm_pixel** sub_image(struct ppm_pixel* palette, struct ppm_pixel** result, int size, float xmin, float xmax, float ymin, float ymax, int maxIterations){
  for (int r = 0; r < size; r++){
    for (int c = 0; c < size; c++){
      float row = r;
      float col = c;
      float xfrac = col/size;
      float yfrac = row/size;
      float x0 = xmin + xfrac*(xmax - xmin);
      float y0 = ymin + yfrac*(ymax - ymin);
      float x = 0;
      float y = 0;
      int iter = 0;
      while (iter < maxIterations && x*x + y*y < 2*2){
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      if (iter < maxIterations) {
        result[r][c] = palette[iter];
      }
      else {
        result[r][c].red = 0;
        result[r][c].green = 0;
        result[r][c].blue = 0;
      }
    }
  }
  return result;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int MAX = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  srand(time(0));

  struct ppm_pixel** result = (struct ppm_pixel**)malloc(sizeof(struct ppm_pixel*)*(size));
  for (int i = 0; i < size; i++) { 
    result[i] = malloc(sizeof(struct ppm_pixel)*(size)); 
  } 

  struct ppm_pixel* palette = (struct ppm_pixel*)(malloc(sizeof(struct ppm_pixel)*MAX));
  for (int j = 0; j < MAX; j++){
    palette[j].red = rand() % 255;
    palette[j].green = rand() % 255;
    palette[j].blue = rand() % 255;
  }

  double timer;
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);

  result = sub_image(palette,result,size,xmin,xmax,ymin,ymax,MAX);

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

}