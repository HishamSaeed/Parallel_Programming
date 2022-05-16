#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<string.h>
#include"helper.h"
#include"life.h"
#include"gui.h"


void copy_temp(int height,int width,int grid[height][width],int temp[width]){
    for(int i = 0; i < width ; i++)
      {
        grid[height][i] = temp[i];
      }
}
void simulate(int height, int width, int grid[height][width], int num_iterations)
{
  /*
    Write your parallel solution here. You first need to distribute the data to all of the
    processes from the root. Note that you cannot naively use the evolve function used in the
    sequential version of the code - you might need to rewrite it depending on how you parallelize
    your code.

    For more details, see the attached readme file.
  */

  char const  filename1[] = {"chunk1_par"};
  char const  filename2[] = {"chunk2_par"};
  char const  filename3[] = {"chunk3_par"};
  char const  filename4[] = {"chunk17_par"};
  // Calculating needed parameters for communication
  int rank, num_procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

   int chunk_size = (height-2)/num_procs;
   int (*chunk)[width] = malloc ( sizeof( int[chunk_size+2][width] ));;
   // int *temp = malloc(sizeof(int)*width);

   int *send_count = malloc(sizeof(int)*num_procs);
   int *send_displs = malloc(sizeof(int)*num_procs);

   // int *receive_count = malloc(sizeof(int)*num_procs);
   // int *receive_displs = malloc(sizeof(int)*num_procs);
   for(int i = 0;i < num_procs;i++){
     send_count[i] = (chunk_size + 1)*width;
     send_displs[i] = (i*chunk_size);
   }
    if(rank == 0){
     // send_count[0] = (chunk_size+1)*width;
     // send_displs[0] = 0;
     // receive_count[0] = chunk_size*width;
     // receive_displs[0] = 0;
     // for(int i = 0;i < num_procs;i++){
     //   send_count[i] = (chunk_size + 1)*width;
     //   send_displs[i] = (i*chunk_size);
     //
     //   // receive_count[i] = (chunk_size)*width;
     //   // receive_displs[i] = (i*chunk_size) + 1;
     // }
     // send_count[num_procs-1] = (chunk_size+1)*width;
     // send_displs[num_procs-1] = ((num_procs-1)*chunk_size);
     // for(int l = 0;l < width;l++){
     //   printf("%d",grid[send_displs[1]][l]);
     // }
     // printf("\n");
     // receive_count[num_procs-1] = chunk_size*width;
     // receive_displs[num_procs-1] = ((num_procs-1)*chunk_size)-1;
   }
   // for(int l = 0;l < num_procs;l++){
   //   printf("%d ",send_displs[l]);
   // }
   // printf("\n");
  if(rank == 0 || rank == num_procs-1){
    MPI_Scatterv(grid,send_count,send_displs,MPI_INT,chunk,(chunk_size+1)*width,MPI_INT,0,MPI_COMM_WORLD);
    if(rank == 0){
      save_to_file(chunk_size+1,width,chunk,filename1);
    }

    if(rank == 3){
      save_to_file(chunk_size+2,width,chunk,filename4);
    }
  }else{
    MPI_Scatterv(grid,send_count,send_displs,MPI_INT,chunk,send_count[rank],MPI_INT,0,MPI_COMM_WORLD);
    if(rank == 1){
      save_to_file(chunk_size+1,width,chunk,filename2);
    }
    if(rank == 2){
      save_to_file(chunk_size+2,width,chunk,filename3);
    }

  }

  // for(int i = 0; i < num_iterations; i++){
  //   if(rank == 0){
  //     evolve_par(chunk_size + 1,width,chunk);
  //
  //     MPI_Recv(temp,width,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  //     MPI_Send(chunk + 1,width,MPI_INT,(num_procs-1),0,MPI_COMM_WORLD);
  //     copy_temp(chunk_size + 1,width,chunk,temp);
  //
  //     MPI_Recv(temp,width,MPI_INT,(num_procs-1),0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  //     MPI_Send(chunk + chunk_size ,width,MPI_INT,1,0,MPI_COMM_WORLD);
  //     copy_temp(0,width,chunk,temp);
  //   }else{
  //     if(rank == num_procs-1){
  //       // Sending upper bound of the domain(chunk+1 first row after padding),then receiving lower bound and save it at the lower padding row
  //       evolve_par(chunk_size + 1,width,chunk);
  //       MPI_Send(chunk+1,width,MPI_INT,(rank-1)%num_procs,0,MPI_COMM_WORLD);
  //       MPI_Recv(temp,width,MPI_INT,(rank+1)%num_procs,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  //       copy_temp(chunk_size+1,width,chunk,temp);
  //
  //       // Sending lower bound of the domain(chunk + chunk_size last row before padding),then receiving upper padding
  //       MPI_Send(chunk + chunk_size,width,MPI_INT,(rank+1)%num_procs,0,MPI_COMM_WORLD);
  //       MPI_Recv(temp,width,MPI_INT,(rank-1)%num_procs,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  //       copy_temp(0,width,chunk,temp);
  //     }else{
  //       evolve_par(chunk_size + 2,width,chunk);
  //       MPI_Send(chunk+1,width,MPI_INT,(rank-1)%num_procs,0,MPI_COMM_WORLD);
  //       MPI_Recv(temp,width,MPI_INT,(rank+1)%num_procs,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  //       copy_temp(chunk_size+1,width,chunk,temp);
  //
  //       MPI_Send(chunk + chunk_size,width,MPI_INT,(rank+1)%num_procs,0,MPI_COMM_WORLD);
  //       MPI_Recv(temp,width,MPI_INT,(rank-1)%num_procs,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  //       copy_temp(0,width,chunk,temp);
  //     }
  //
  //   }
//  }

  // if(rank == 0){
  //   MPI_Gatherv(chunk,(chunk_size)*width,MPI_INT,grid,receive_count,receive_displs,MPI_INT,0,MPI_COMM_WORLD);
  //   //save_to_file(height,width,grid,filename);
  // }else{
  //   if(rank == num_procs-1){
  //       MPI_Gatherv(chunk + 1,(chunk_size)*width,MPI_INT,grid,receive_count,receive_displs,MPI_INT,0,MPI_COMM_WORLD);
  //   }else
  //   {
  //     MPI_Gatherv(chunk + 1,(chunk_size)*width,MPI_INT,grid,receive_count,receive_displs,MPI_INT,0,MPI_COMM_WORLD);
  //   }
  // }


    free(chunk);
    // free(temp);
    free(send_count);
    free(send_displs);
    // free(receive_count);
    // free(receive_displs);
}
