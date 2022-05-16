#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include "search.h"
#include "helper.h"

void search_text (char* text, int num_lines, int line_length, char* search_string, int *occurences)
{
  /*
    Counts occurences of substring "search_string" in "text". "text" contains multiple lines and each line
    has been placed at text + line_length * num_lines since line length in the original text file can vary.
    "line_length" includes space for '\0'.

    Writes result at location pointed to by "occurences".


    *************************** PARALLEL VERSION **************************

    NOTE: For the parallel version, distribute the lines to each processor. You should only write
    to "occurences" from the root process and only access the text pointer from the root (all other processes
    call this function with text = NULL)
  */

  // Write your parallel solution here

//  Getting rank and number of processes
  int rank, num_procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  // Distibuting text over number of processes
  int size = (num_lines/num_procs);

   int running_count = 0;
  // int lineIndexBuffer = 0;
  // char* lineTextBuffer = (char*)malloc(sizeof(char)  * (line_length) );
  char* TextBuffer = (char*)malloc(sizeof(char) * size * (line_length) );

  if(rank == 0){
    printf("size = %d\n",size);
      for(int k = 1;k < num_procs;k++){
        TextBuffer = text + (k*(size-1)*line_length);
        // printf("original text %s\n",text + (k*size)*line_length);
        // printf("copied text %s\n",TextBuffer + line_length);
        MPI_Send(TextBuffer, line_length*size, MPI_CHAR, k, 0, MPI_COMM_WORLD);
      }
  }else{
    MPI_Recv(TextBuffer, line_length*size, MPI_CHAR , 0, 0, MPI_COMM_WORLD , MPI_STATUS_IGNORE);
     //printf("line 1 %s from process %d\n",TextBuffer + line_length,rank);
  }

  for(int i = 0; i < size ; i++){
    if(rank == 0){
      running_count += count_occurences(text + i * line_length, search_string);
    }else{
      running_count += count_occurences(TextBuffer + (i * line_length), search_string);
    }
  }
  // for (int i = rank*size; i < (rank * size ) + size; i++)
  //   {
  //     //printf("line %s from process %d\n",TextBuffer+(i*line_length),rank );
  //     if(rank == 0){
  //       running_count += count_occurences(text + i * line_length, search_string);
  //       //printf("line is %s\n",text + i * line_length);
  //         // for(int k = 1;k < num_procs;k++){
  //         //   // MPI_Recv(&lineIndexBuffer, 1, MPI_INT , k, 0, MPI_COMM_WORLD , MPI_STATUS_IGNORE);
  //         //   // lineTextBuffer = text + (lineIndexBuffer * line_length);
  //         //   //
  //         //   // MPI_Send(lineTextBuffer, line_length, MPI_CHAR, k, 0, MPI_COMM_WORLD);
  //         //   // lineTextBuffer = text + (i * line_length);
  //         //
  //         // }
  //     }else{
  //       // lineIndexBuffer = i;
  //       // MPI_Send(&lineIndexBuffer, 1, MPI_INT , 0, 0, MPI_COMM_WORLD);
  //       // MPI_Recv(lineTextBuffer, line_length, MPI_CHAR , 0, 0, MPI_COMM_WORLD , MPI_STATUS_IGNORE);
  //       running_count += count_occurences(TextBuffer + (i * line_length), search_string);
  //     }
  //      //running_count += count_occurences(lineTextBuffer, search_string);
  //
  //   }
    printf("running count %d from process %d\n",running_count,rank);
    *occurences = 0;
    MPI_Reduce(&running_count , occurences , 1, MPI_INT , MPI_SUM , 0, MPI_COMM_WORLD);

}
