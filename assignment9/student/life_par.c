#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<string.h>
#include"helper.h"
#include"life.h"
#include"gui.h"

// void copy_edges_par(int height, int width, int grid[height][width]){
//   for( int i = 1; i < height-1; i++ )
//     {    // join rows together
//       grid[i][0] = grid[i][width-2];
//       grid[i][width-1] = grid[i][1];
//     }
//       grid[0][0] = grid[0][width-2];
//       grid[height-1][width-1] = grid[height-1][1];
//       grid[0][width-1] = grid[0][1];
//       grid[height-1][0] = grid[height-1][width-2];
// }
void copy_padding(int height,int width,int grid[height][width],int temp[width],int position){
    for(int i = 0; i < width ; i++)
      {
        grid[position][i] = temp[i];
      }
}
// void evolve_par( int height, int width, int grid[height][width] )
// {
//   /*
//     Apply the game of life rules on a Torus --> grid contains shadow rows and columns
//     to simplify application of rules i.e. grid actually ranges from grid [ 1.. height - 2 ][ 1 .. width - 2]
//   */

//   // pointer to v
//   int (*temp)[width] = malloc( sizeof ( int[height][width] ) );

//   for( int i = 1; i < height - 1; i++)
//     for ( int j = 1; j < width - 1; j++ )
//       {

//         int sum = grid[i-1][j-1]  + grid[i-1][j] + grid[i-1][j+1] + 
//           grid[i][j-1] + grid[i][j+1] + 
//           grid[i+1][j-1] + grid[i+1][j] + grid[i+1][j+1];

//         if ( grid[i][j] == 0 )
//           {
//             // reproduction
//             if (sum == 3)
//               {
//                 temp[i][j] = 1;
//               }
//             else
//               {
//                 temp[i][j] = 0;
//               }
//           }
//         // alive
//         else
//           {
//             // stays alive
//             if (sum == 2 || sum == 3)
//               {
//                 temp[i][j] = 1;
//               }
//             // dies due to under or overpopulation
//             else
//               {
//                 temp[i][j] = 0;
//               }
//           }

//       }

//   copy_edges_par(height, width, temp);

//   for (int i = 0; i < height ; i++)
//     for(int j = 0; j < width ; j++)
//       {
//         grid[i][j] = temp[i][j];
//       }

//   free(temp);

// }
void simulate(int height, int width, int grid[height][width], int num_iterations)
{
  // char filename1[] = {"chunk1_par"};
  // char filename2[] = {"chunk2_par"};
  // char filename3[] = {"chunk3_par"};
  // char filename4[] = {"chunk4_par"};
  // char filename5[] = {"gridAfter_par"};

  int rank, num_procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  if(num_procs == 1){
    copy_edges(height, width, grid );

    for (int i = 0; i < num_iterations; i++)
      {
        evolve( height, width, grid );
        if (global_show_gui) gui_draw(height, width, grid[0]);
      }
      return;
  }
  int chunk_size = (height-2)/num_procs;
  int leftover = (height-2)%num_procs;
  int (*chunk)[width];

  int *padding = malloc(sizeof(int)*width);

  if(rank == 0){
    chunk  = malloc(sizeof(int[chunk_size+2+leftover][width]));

  }else{
    chunk  = malloc(sizeof(int[chunk_size+2][width]));
  }


  int *send_count = malloc(sizeof(int)*num_procs);
  int *send_displs = malloc(sizeof(int)*num_procs);

  int *receive_count = malloc(sizeof(int)*num_procs);
  int *receive_displs = malloc(sizeof(int)*num_procs);

  send_count[0] = (chunk_size+2+leftover)*width;
  send_displs[0] = 0;
  receive_count[0] = (chunk_size + leftover+1)*width;
  receive_displs[0] = 0;
  for(int i = 1;i < num_procs;i++){
    send_count[i] = (chunk_size+2)*width;
    send_displs[i] = (((i)*chunk_size)+leftover)*width;
    receive_count[i] = chunk_size*width;
    receive_displs[i] = (((i)*chunk_size)+leftover + 1)*width;;
  }

  if(rank == 0){
    MPI_Scatterv(grid,send_count,send_displs,MPI_INT,chunk,(chunk_size + 2 + leftover)*width,MPI_INT,0,MPI_COMM_WORLD);
  }else{
    MPI_Scatterv(grid,send_count,send_displs,MPI_INT,chunk,(chunk_size + 2 )*width,MPI_INT,0,MPI_COMM_WORLD);
  }

  for(int i = 0; i < num_iterations; i++){

      if(rank == 0){
        evolve_par(chunk_size + 2 + leftover,width,chunk);

        //Sending Lower Part of this domain that forms upper padding to the next process & receiving upper padding
        MPI_Recv(padding,width,MPI_INT,(num_procs-1),0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Send(chunk + chunk_size + leftover,width,MPI_INT,1,0,MPI_COMM_WORLD);
        copy_padding(chunk_size + 2 + leftover,width,chunk,padding,0);

        // Sending Upper Part of this domain that forms lower padding to the next process & receiving lower padding
        MPI_Recv(padding,width,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Send(chunk + 1 ,width,MPI_INT,(num_procs-1),0,MPI_COMM_WORLD);
        // Here where the problem lies althought i checked that the data sent is correct
        copy_padding(chunk_size + 2 + leftover,width,chunk,padding,chunk_size + leftover + 1);
      }else{

        evolve_par(chunk_size + 2,width,chunk);

        //Sending Lower Part of this domain that forms upper padding to the next process & receiving upper padding
        MPI_Send(chunk + chunk_size,width,MPI_INT,(rank+1)%num_procs,0,MPI_COMM_WORLD);
        MPI_Recv(padding,width,MPI_INT,(rank-1)%num_procs,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        copy_padding(chunk_size + 2,width,chunk,padding,0);

        MPI_Send(chunk + 1 ,width,MPI_INT,(rank-1)%num_procs,0,MPI_COMM_WORLD);
        MPI_Recv(padding,width,MPI_INT,(rank+1)%num_procs,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        // Here where the problem lies althought i checked that the data sent is correct
        copy_padding(chunk_size + 2,width,chunk,padding,chunk_size + 1);

      }
  }

  if(rank == 0){
    MPI_Gatherv(chunk,(chunk_size + leftover + 1)*width,MPI_INT,grid,receive_count,receive_displs,MPI_INT,0,MPI_COMM_WORLD);
  }else{
    MPI_Gatherv(chunk + 1,(chunk_size)*width,MPI_INT,grid,receive_count,receive_displs,MPI_INT,0,MPI_COMM_WORLD);
  }

  // if(rank == 0){
  //   save_to_file(chunk_size + 2 + leftover,width,chunk,filename1);
  //   save_to_file(height,width,grid,filename5);
  // }
  // if(rank == 1){
  //   save_to_file(chunk_size + 2,width,chunk,filename2);
  // }
  // if(rank == 2){
  //   save_to_file(chunk_size + 2,width,chunk,filename3);
  // }
  // if(rank == 3){
  //   save_to_file(chunk_size + 2,width,chunk,filename4);
  // }



}
