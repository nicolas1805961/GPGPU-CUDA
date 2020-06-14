#pragma once

#include "Image.hpp"
#include <cmath>

class GraphGPU
{
public:
    int *m_excessFlow;
    int *m_heights;
    int *m_leftNeighbourCapacity;
    int *m_rightNeighbourCapacity;
    int *m_topNeighbourCapacity;
    int *m_bottomNeighbourCapacity;
    int *m_sourceCapacity;
    int *m_sinkCapacity;

    int m_maxHeight;
    int m_width;
    int m_height;
    uint8_t *m_rgbImage;
    uint8_t *m_grayImage;
    uint8_t *m_rgbImageHelper;
    uint8_t *m_grayImageHelper;

    GraphGPU(Image const& image, Image const& imageHelper);
};

__global__ void push(GraphGPU* g);
__global__ void relabel(GraphGPU* g, int* swap_heights);
__global__ void count_active(GraphGPU* g, int* count);
__global__ void setImage(GraphGPU* graph, int *out);
