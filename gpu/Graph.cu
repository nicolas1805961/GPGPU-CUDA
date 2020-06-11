#include "Graph.hpp"
#include <limits>
#include <stdio.h>

__device__ int artificial_height = 10000;
//Same as CPU initialization
Graph::Graph(Image const& image, Image const& imageHelper)
{
    m_maxHeight = image.getWidth() * image.getHeight();
    m_width = image.getWidth();
    m_height = image.getHeight();

    m_excessFlow = (int*)std::malloc(m_maxHeight * sizeof(int));
    m_heights = (int*)std::malloc(m_maxHeight * sizeof(int));
    m_topNeighbourCapacity = (int*)std::malloc(m_maxHeight * sizeof(int));
    m_leftNeighbourCapacity = (int*)std::malloc(m_maxHeight * sizeof(int));
    m_rightNeighbourCapacity = (int*)std::malloc(m_maxHeight * sizeof(int));
    m_bottomNeighbourCapacity = (int*)std::malloc(m_maxHeight * sizeof(int));
    m_sourceCapacityToNodes = (int*)std::malloc(m_maxHeight * sizeof(int));
    m_sinkCapacityToNodes = (int*)std::malloc(m_maxHeight * sizeof(int));
    m_sourceCapacityFromNodes = (int*)std::malloc(m_maxHeight * sizeof(int));
    m_sinkCapacityFromNodes = (int*)std::malloc(m_maxHeight * sizeof(int));

    m_grayImage = image.getImageGray();

    // Ici je label les arrêtes entre les noeuds avec la distance en rgb entre chaque pixel (noeud = pixel)
    for (int i = 0; i < m_height - 1; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            int distance = pow((m_grayImage[m_width * i + j] - m_grayImage[m_width * (i + 1) + j]), 2);
            m_bottomNeighbourCapacity[m_width * i + j] = ((sqrt(distance) / sqrt(pow(255, 2))) - 1) * -sqrt(pow(255, 2));
        }
    }
    for (int i = 1; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            int distance = pow((m_grayImage[m_width * i + j] - m_grayImage[m_width * (i - 1) + j]), 2);
            m_topNeighbourCapacity[m_width * i + j] = ((sqrt(distance) / sqrt(pow(255, 2))) - 1) * -sqrt(pow(255, 2));
        }
    }
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 1; j < m_width; j++)
        {
            int distance = pow((m_grayImage[m_width * i + j] - m_grayImage[m_width * i + j - 1]), 2);
            m_leftNeighbourCapacity[m_width * i + j] = ((sqrt(distance) / sqrt(pow(255, 2))) - 1) * -sqrt(pow(255, 2));
        }
        for (int j = 0; j < m_width - 1; j++)
        {
            int distance = pow((m_grayImage[m_width * i + j] - m_grayImage[m_width * i + j + 1]), 2);
            m_rightNeighbourCapacity[m_width * i + j] = ((sqrt(distance) / sqrt(pow(255, 2))) - 1) * -sqrt(pow(255, 2));
        }
    }
    //contient les indices des pixels noirs et blanc
    std::vector<int> white;
    std::vector<int> black;
    int sumIntensityForegroundRed = 0;
    int sumIntensityForegroundGreen = 0;
    int sumIntensityForegroundBlue = 0;
    int sumIntensityBackgroundRed = 0;
    int sumIntensityBackgroundGreen = 0;
    int sumIntensityBackgroundBlue = 0;
    auto ptr = image.getImageRgb();
    auto ptrHelper = imageHelper.getImageRgb();
    // ici je label les arrêtes entre la source et les noeuds appartenant au foreground d'une part, et d'autre part entre les noeuds appartenant au background et le puit. Les noeuds dont on est sur qu'ils sont du foreground ou background sont les pixels blanc et noir respectivement. On les etiquettes avec une très grande valeur car c'est sur qu'ils appartiennent au background/foreground.
    for (int i = 0; i < imageHelper.getHeight() * imageHelper.getWidth(); i++)
    {
        if (ptrHelper[(i * 3)] == ptrHelper[(i * 3) + 1] && ptrHelper[(i * 3)] == ptrHelper[(i * 3) + 2] && ptrHelper[(i * 3)] == 255)
        {
            white.push_back(i * 3);
            m_sourceCapacityToNodes[i + i % m_width] = std::numeric_limits<int>::max(); //sqrt(pow(255, 2) * 3);
            m_sinkCapacityFromNodes[i + i % m_width] = 0;
        }
        else if (ptrHelper[(i * 3)] == ptrHelper[(i * 3) + 1] && ptrHelper[(i * 3)] == ptrHelper[(i * 3) + 2] && ptrHelper[(i * 3)] == 0)
        {
            black.push_back(i * 3);
            m_sinkCapacityFromNodes[i + i % m_width] = std::numeric_limits<int>::max(); //sqrt(pow(255, 2) * 3);
            m_sourceCapacityToNodes[i + i % m_width] = 0;
        }
    }

    for (size_t i = 0; i < white.size(); i++)
    {
        sumIntensityForegroundRed += ptr[white[i]];
        sumIntensityForegroundGreen += ptr[white[i] + 1];
        sumIntensityForegroundBlue += ptr[white[i] + 2];
    }
    for (size_t i = 0; i < black.size(); i++)
    {
        sumIntensityBackgroundRed += ptr[black[i]];
        sumIntensityBackgroundGreen += ptr[black[i] + 1];
        sumIntensityBackgroundBlue += ptr[black[i] + 2];
    }
    float averageForegroundRed = sumIntensityForegroundRed / white.size();
    float averageForegroundGreen = sumIntensityForegroundGreen / white.size();
    float averageForegroundBlue = sumIntensityForegroundBlue / white.size();
    float averageBackgroundRed = sumIntensityBackgroundRed / black.size();
    float averageBackgroundGreen = sumIntensityBackgroundGreen / black.size();
    float averageBackgroundBlue = sumIntensityBackgroundBlue / black.size();

    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            uint8_t r = image.getImageRgb()[m_width * (i * 3) + (j * 3)];
            uint8_t g = image.getImageRgb()[m_width * (i * 3) + (j * 3) + 1];
            uint8_t b = image.getImageRgb()[m_width * (i * 3) + (j * 3) + 2];

            float df = sqrt(pow(r - averageForegroundRed, 2) + pow(g - averageForegroundGreen, 2) + pow(b - averageForegroundBlue, 2));
            float db = sqrt(pow(r - averageBackgroundRed, 2) + pow(g - averageBackgroundGreen, 2) + pow(b - averageBackgroundBlue, 2));

            if (m_sourceCapacityToNodes[m_width * i + j] == 1)
                m_sourceCapacityToNodes[m_width * i + j] = ((df / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3); //-log(pf);;
            if (m_sinkCapacityFromNodes[m_width * i + j] == 1)
                m_sinkCapacityFromNodes[m_width * i + j] = ((db / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3); //-log(1 - pf);
        }
    }
    //ici j'initialise l'excess flow en saturant les arrêtes(capacity) partant de la source. Et je créer la capacité dans le sens inverse.
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            m_excessFlow[m_width * i + j] = m_sourceCapacityToNodes[m_width * i + j] - m_sinkCapacityFromNodes[m_width * i + j];
            m_sourceCapacityFromNodes[m_width * i + j]= m_sourceCapacityToNodes[m_width * i + j];
            m_sourceCapacityToNodes[m_width * i + j] = 0;
        }
    }
}

//same as CPU push, added atomic operations to avoid random states
__global__ void push(Graph* graph)
{
    int x = blockIdx.x * 256 + threadIdx.x;
    int i = x / graph->m_width;
    int j = x % graph->m_width;

    int* m_excessFlow = graph->m_excessFlow;
    int* m_heights = graph->m_heights;
    int* m_leftNeighbourCapacity = graph->m_leftNeighbourCapacity;
    int* m_rightNeighbourCapacity = graph->m_rightNeighbourCapacity;
    int* m_topNeighbourCapacity = graph->m_topNeighbourCapacity;
    int* m_bottomNeighbourCapacity = graph->m_bottomNeighbourCapacity;
    int m_maxHeight = graph->m_maxHeight;
    int m_height = graph->m_height;
    int m_width = graph->m_width;

    if (x >= m_maxHeight)
        return;

    //artificially changed maxHeight to try to have a faster result
    m_maxHeight = artificial_height;

    if (m_excessFlow[i*m_width+j] > 0 && m_heights[i*m_width+j] < m_maxHeight)
    {
        if (j > 0 && m_heights[i*m_width + j - 1] == m_heights[i*m_width+j] - 1)
        {
            int flow = fminf(m_leftNeighbourCapacity[i*m_width+j], m_excessFlow[i*m_width+j]);
            //m_excessFlow[i*m_width+j] -= flow;
            atomicSub(&m_excessFlow[i*m_width+j],flow);
            //m_excessFlow[i*m_width+j - 1] += flow;
            atomicAdd(&m_excessFlow[i*m_width+j - 1],flow);
            //m_leftNeighbourCapacity[i*m_width+j] -= flow;
            atomicSub(&m_leftNeighbourCapacity[i*m_width+j], flow);
            //m_rightNeighbourCapacity[i*m_width+j - 1] += flow;
            atomicAdd(&m_rightNeighbourCapacity[i*m_width+j-1],flow);
        }
        if (j < m_width - 1 && m_heights[i*m_width+j + 1] == m_heights[i*m_width+j] - 1)
        {
            int flow = fminf(m_rightNeighbourCapacity[i*m_width+j], m_excessFlow[i*m_width+j]);
            //m_excessFlow[i*m_width+j] -= flow;
            atomicSub(&m_excessFlow[i*m_width+j],flow);
            //m_excessFlow[i*m_width+j + 1] += flow;
            atomicAdd(&m_excessFlow[i*m_width+j + 1],flow);
            //m_rightNeighbourCapacity[i*m_width+j] -= flow;
            atomicSub(&m_rightNeighbourCapacity[i*m_width+j], flow);
            //m_leftNeighbourCapacity[i*m_width+j + 1] += flow;
            atomicAdd(&m_leftNeighbourCapacity[i*m_width+j+1],flow);
        }
        if (i > 0 && m_heights[(i - 1)*m_width+j] == m_heights[i*m_width+j] - 1)
        {
            int flow = fminf(m_topNeighbourCapacity[i*m_width+j], m_excessFlow[i*m_width+j]);
            //m_excessFlow[i*m_width+j] -= flow;
            atomicSub(&m_excessFlow[i*m_width+j],flow);
            //m_excessFlow[(i-1)*m_width+j] += flow;
            atomicAdd(&m_excessFlow[(i-1)*m_width+j],flow);
            //m_topNeighbourCapacity[i*m_width+j] -= flow;
            atomicSub(&m_topNeighbourCapacity[i*m_width+j], flow);
            //m_bottomNeighbourCapacity[(i-1)*m_width+j] += flow;
            atomicAdd(&m_bottomNeighbourCapacity[(i-1)*m_width+j],flow);
        }
        if (i < m_height - 1 && m_heights[(i + 1)*m_width+j] == m_heights[i*m_width+j] - 1)
        {
            int flow = fminf(m_bottomNeighbourCapacity[i*m_width+j], m_excessFlow[i*m_width+j]);
            //m_excessFlow[i*m_width+j] -= flow;
            atomicSub(&m_excessFlow[i*m_width+j],flow);
            //m_excessFlow[(i+1)*m_width + j] += flow;
            atomicAdd(&m_excessFlow[(i+1)*m_width+j],flow);
            //m_bottomNeighbourCapacity[i*m_width+j] -= flow;
            atomicSub(&m_bottomNeighbourCapacity[i*m_width+j], flow);
            //m_topNeighbourCapacity[(i + 1)*m_width+j] += flow;
            atomicAdd(&m_topNeighbourCapacity[(i+1)*m_width+j],flow);
        }
    }
    //printf("i=%d j=%d\n",i, j);
}

//Same as CPU relabel, but pushing values on swap and reading from actual heights
__global__ void relabel(Graph* graph, int* swap_heights)
{
    int x = blockIdx.x * 256 + threadIdx.x;
    int i = x / graph->m_width;
    int j = x % graph->m_width;

    int* m_excessFlow = graph->m_excessFlow;
    int* m_heights = graph->m_heights;
    int* m_leftNeighbourCapacity = graph->m_leftNeighbourCapacity;
    int* m_rightNeighbourCapacity = graph->m_rightNeighbourCapacity;
    int* m_topNeighbourCapacity = graph->m_topNeighbourCapacity;
    int* m_bottomNeighbourCapacity = graph->m_bottomNeighbourCapacity;
    int m_maxHeight = graph->m_maxHeight;
    int m_width = graph->m_width;

    if (x >= m_maxHeight)
        return;

    m_maxHeight = artificial_height;


    if (m_excessFlow[i*m_width + j] > 0 && m_heights[i*m_width+j] < m_maxHeight)
    {
        auto myHeight = m_maxHeight;
        /*if (m_sinkCapacityFromNodes[i][j] > 0)
            myHeight = std::min(myHeight, 0);*/
        if (m_leftNeighbourCapacity[i*m_width+j] > 0)
            myHeight = min(myHeight, m_heights[i*m_width + j - 1] + 1);
        if (m_rightNeighbourCapacity[i*m_width+j] > 0)
            myHeight = min(myHeight, m_heights[i*m_width + j + 1] + 1);
        if (m_topNeighbourCapacity[i*m_width+j] > 0)
            myHeight = min(myHeight, m_heights[(i-1)*m_width + j] + 1);
        if (m_bottomNeighbourCapacity[i*m_width+j] > 0)
            myHeight = min(myHeight, m_heights[(i+1)*m_width+j] + 1);
        /*if (m_sourceCapacityFromNodes[i][j] > 0)
            myHeight = std::min(myHeight, m_maxHeight);*/
        swap_heights[i*m_width+j] = myHeight;
    }
}

//count how many nodes are active
__global__ void count_active(Graph* graph, int* count)
{
    int x = blockIdx.x * 256 + threadIdx.x;
    /*int i = x / graph->m_width;
    int j = x % graph->m_height;*/

    int m_maxHeight = graph->m_maxHeight;

    if (x >= m_maxHeight)
        return;

    m_maxHeight = artificial_height;

    if (graph->m_excessFlow[x] > 0  && graph->m_heights[x] < m_maxHeight)
    {
        atomicAdd(count, 1);
    }
}

