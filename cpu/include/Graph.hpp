#pragma once

#include "Image.hpp"
#include <vector>
#include <cstdint>
#include <cmath>
#include <cmath>
#include <algorithm>
#include <memory>
#include <stack>
#include <list>

class Graph
{
private:
    std::vector<std::vector<int>> m_excessFlow;
    std::vector<std::vector<int>> m_heights;
    std::vector<std::vector<int>> m_leftNeighbourCapacity;
    std::vector<std::vector<int>> m_rightNeighbourCapacity;
    std::vector<std::vector<int>> m_topNeighbourCapacity;
    std::vector<std::vector<int>> m_bottomNeighbourCapacity;
    std::vector<std::vector<int>> m_sinkCapacity;
    std::vector<std::vector<int>> m_sourceCapacity;
    int m_maxHeight;
    int m_width;
    int m_height;
    uint8_t *m_rgbImage;
    uint8_t *m_rgbImageHelper;

public:
    Graph(Image const& image, Image const& imageHelper);
    bool push(int i, int j);
    void relabel(unsigned int i, unsigned int j);
    int count_active();
    std::shared_ptr<std::pair<unsigned int, unsigned int>> isActive();
    std::vector<std::vector<int>> getHeights();
    std::vector<std::vector<int>> getExcessFlow();
    std::vector<std::vector<int>> getLeftNeighbourCapacity();
    std::vector<std::vector<int>> getRightNeighbourCapacity();
    std::vector<std::vector<int>> getTopNeighbourCapacity();
    std::vector<std::vector<int>> getBottomNeighbourCapacity();
    std::vector<std::vector<bool>> dfs();
    int getWidth();
    int getHeight();
};
