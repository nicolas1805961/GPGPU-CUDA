#pragma once

#include "Image.hpp"
#include <cmath>

class Graph
{
public:
    int *m_excessFlow;
    int *m_heights;
    int *m_leftNeighbourCapacity;
    int *m_rightNeighbourCapacity;
    int *m_topNeighbourCapacity;
    int *m_bottomNeighbourCapacity;
    int *m_sourceCapacityToNodes;
    int *m_sourceCapacityFromNodes;
    int *m_sinkCapacityFromNodes;
    int *m_sinkCapacityToNodes;

    int m_maxHeight;
    int m_width;
    int m_height;
    uint8_t *m_rgbImage;
    uint8_t *m_grayImage;
    uint8_t *m_rgbImageHelper;
    uint8_t *m_grayImageHelper;

    Graph(Image const& image, Image const& imageHelper);
};

__global__ void push(Graph* g);
__global__ void relabel(Graph* g, int* swap_heights);
__global__ void count_active(Graph* g, int* count);
