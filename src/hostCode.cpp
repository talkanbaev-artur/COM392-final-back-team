#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include <thread>

#include "hostCode.h"
#include "gpuCode.h"
#include "randLib.h"
#include "animate.h"

/******************************************************************************/
// return information about CUDA CPU devices on this machine
int probeHost(){

  // PRINT CPU CORES INFO
  unsigned int numCores = std::thread::hardware_concurrency();
  printf("\nnum CPU cores on this machine: %d\n\n", numCores);

  // maybe add some stuff about type of CPU, etc..

  return(0);
}

/*******************************************************************************
                       PROGRAM FUNCTIONS - multithreading CPU
*******************************************************************************/
int runVS(AParams* PARAMS){
  seed_randoms();

  PARAMS->sizePopulation = 1048576; // number of people in city (1024^2)

  GPU_Palette P1 = initPopulation();

  CPUAnimBitmap animation(PARAMS->width, PARAMS->height, &P1);
  cudaMalloc((void**) &animation.dev_bitmap, animation.image_size());
  animation.initAnimation();

  // initialize population with susceptibility and virus stage,
  // let's just do it on the CPU and copy to GPU:
  float* suscMap = (float*) malloc(PARAMS->sizePopulation * sizeof(float));
  int* stageMap = (int*) malloc(PARAMS->sizePopulation * sizeof(int));
  int* mingMap = (int*) malloc(PARAMS->sizePopulation * sizeof(int));
  for(long i = 0; i < PARAMS->sizePopulation; i++){
    suscMap[i] = rand_frac(); // everyone gets a susceptibility score
    stageMap[i] = 0; // set everyone to be at stage 0 (not infected)
    mingMap[i] = round(rand_frac() * 500); // a person mingles with
          // this number of other people on average per day
    }
  // infect some initial people
  int numInitialInfections = 5; // make this a user param
  for(int i = 0; i < numInitialInfections; i++){
    long randPerson = round(rand_frac()*1024*1024);
    stageMap[randPerson] = 1; // set person to infectious stage of virus
    }
	cudaMemcpy(P1.susc, suscMap, P1.memSize, cH2D);
	cudaMemcpy(P1.stage, stageMap, P1.memIntSize, cH2D);
	cudaMemcpy(P1.ming, mingMap, P1.memIntSize, cH2D);

  // simulation runs for 10 years, updating population once per day:
  for(int day = 0; day < 3650; day++){
     int err = updatePopulation(&P1, PARAMS, day);
     animation.drawPalette(PARAMS->width, PARAMS->height);
     // return number of newly infected and deaths per day
   	}

  // return number of people who died after ten years
}