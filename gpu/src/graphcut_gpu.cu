#include "Image.hpp"
#include "Graph_gpu.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <stdio.h>

#define cudaCheckError() {                                                                       \
  cudaError_t e=cudaGetLastError();                                                        \
  if(e!=cudaSuccess) {                                                                     \
      printf("Cuda failure %s:%d: '%s'\n",__FILE__,__LINE__,cudaGetErrorString(e));        \
      exit(EXIT_FAILURE);                                                                  \
  }                                                                                        \
}

__global__ void display(GraphGPU* graph)
{
    int x = blockIdx.x * 256 + threadIdx.x;

    printf("%d", graph->m_excessFlow[x]);
}

//function added to check if the graph was copied successfully on GPU
void count_active_cpu(GraphGPU graph)
{
    int count = 0;
    for (int i = 0; i < graph.m_maxHeight; i++)
    {
        if (graph.m_excessFlow[i] > 0 && graph.m_heights[i] < graph.m_maxHeight)
            count++;
    }
    std::cout << "cpu_count = " << count << "\n";
}

//Copy a cpu array onto the device
void copy_pointer(int** gpu_ptr, int* cpu_ptr, size_t size)
{
    //allocate array on the device
    int *host_ptr;
    cudaMalloc((void **) &host_ptr, size);
    cudaCheckError();
    //copy the cpu array on the device
    cudaMemcpy(host_ptr, cpu_ptr, size, cudaMemcpyHostToDevice);
    cudaCheckError();
    //copy the gpu array adress on our pointer
    cudaMemcpy(gpu_ptr, &host_ptr, sizeof(int*), cudaMemcpyHostToDevice);
    cudaDeviceSynchronize();
    cudaCheckError();
}

//Copy the CPU graph to the GPU
void copy_graph(GraphGPU *gpu_graph, GraphGPU cpu_graph)
{
    //copy the structure
    cudaMemcpy(gpu_graph, &cpu_graph, sizeof(GraphGPU), cudaMemcpyHostToDevice);
    cudaCheckError();

    size_t matrix_size = cpu_graph.m_maxHeight * sizeof(int);

    //copy every matrices used
    copy_pointer(&(gpu_graph->m_excessFlow), cpu_graph.m_excessFlow, matrix_size);
    copy_pointer(&(gpu_graph->m_heights), cpu_graph.m_heights, matrix_size);
    copy_pointer(&(gpu_graph->m_topNeighbourCapacity), 
        cpu_graph.m_topNeighbourCapacity, matrix_size);
    copy_pointer(&(gpu_graph->m_bottomNeighbourCapacity),
        cpu_graph.m_bottomNeighbourCapacity, matrix_size);
    copy_pointer(&(gpu_graph->m_rightNeighbourCapacity),
        cpu_graph.m_rightNeighbourCapacity, matrix_size);
    copy_pointer(&(gpu_graph->m_leftNeighbourCapacity),
        cpu_graph.m_leftNeighbourCapacity, matrix_size);
    copy_pointer(&(gpu_graph->m_sinkCapacity),
        cpu_graph.m_sinkCapacity, matrix_size);
    copy_pointer(&(gpu_graph->m_sourceCapacity),
        cpu_graph.m_sourceCapacity, matrix_size);
}

//copy the graph heights onto the swap
__global__ void graph_to_swap(GraphGPU *graph, int *swap)
{
    int x = blockIdx.x * 256 + threadIdx.x;
    if (x >= graph->m_maxHeight)
        return;

    swap[x] = graph->m_heights[x];
}

//copy the swap heights onto the graph heights
__global__ void swap_to_graph(GraphGPU *graph, int *swap)
{
    int x = blockIdx.x * 256 + threadIdx.x;
    if (x >= graph->m_maxHeight)
        return;

    graph->m_heights[x] = swap[x];
}


void graphcut_gpu(GraphGPU graph)
{
    cudaDeviceProp device;
    cudaGetDeviceProperties(&device,0);

    int blockSize = 256;
    int numBlocks = (graph.m_maxHeight + blockSize - 1) / blockSize;

    //allocate graph on GPU and copy the one initialize on CPU
    GraphGPU* g;
    cudaMalloc((void **) &g, sizeof(GraphGPU));
    cudaCheckError();
    copy_graph(g, graph);

    //setup CPU counter
    int c = 0;
    int* count = &c;

    //setup GPU counter
    int* gpu_count;
    cudaMalloc((void **)&gpu_count, sizeof(int));
    cudaCheckError();
    cudaDeviceSynchronize();
    cudaMemcpy(gpu_count, count, sizeof(int), cudaMemcpyHostToDevice);
    cudaCheckError();
    cudaDeviceSynchronize();

    //count active nodes
    count_active<<<numBlocks, blockSize>>>(g, gpu_count);
    cudaCheckError();
    cudaDeviceSynchronize();

    //copy the counter from GPU to CPU
    cudaMemcpy(count, gpu_count, sizeof(int), cudaMemcpyDeviceToHost);

    //initialize swap heights
    int* swap_heights;
    cudaMalloc((void **)&swap_heights, graph.m_maxHeight * sizeof(int));
    cudaCheckError();

    //copy the graph heights onto the swap
    graph_to_swap<<<numBlocks, blockSize>>>(g, swap_heights);

    while(*count > 0)
    {
        push<<<numBlocks, blockSize>>>(g);
        cudaDeviceSynchronize();

        //update swap heights
        relabel<<<numBlocks, blockSize>>>(g, swap_heights);
        cudaDeviceSynchronize();

        //copy the updated heights to the graph
        swap_to_graph<<<numBlocks, blockSize>>>(g, swap_heights);
        cudaDeviceSynchronize();

        //set gpu counter to 0
        cudaMemset(gpu_count,0,sizeof(int));
        count_active<<<numBlocks, blockSize>>>(g, gpu_count);
        cudaCheckError();
        cudaDeviceSynchronize();

        //copy GPU counter onto CPU counter for the while loop
        cudaMemcpy(count, gpu_count, sizeof(int), cudaMemcpyDeviceToHost);
        std::cout << *count << "\n";
    }
}
