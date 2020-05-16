#pragma once

#include "Image.hpp"
#include <vector>
#include <cstdint>
#include <cmath>
#include <cmath>
#include <algorithm>
#include <memory>

class Graph
{
private:
    std::vector<std::vector<unsigned int>> m_excessFlow;
    std::vector<std::vector<unsigned int>> m_heights;
    std::vector<std::vector<unsigned int>> m_leftNeighbourCapacity;
    std::vector<std::vector<unsigned int>> m_rightNeighbourCapacity;
    std::vector<std::vector<unsigned int>> m_topNeighbourCapacity;
    std::vector<std::vector<unsigned int>> m_bottomNeighbourCapacity;
    std::vector<std::vector<unsigned int>> m_sourceCapacityToNodes;
    std::vector<std::vector<unsigned int>> m_sourceCapacityFromNodes;
    std::vector<std::vector<unsigned int>> m_sinkCapacityFromNodes;
    std::vector<std::vector<unsigned int>> m_sinkCapacityToNodes;
    /*std::vector<std::vector<unsigned int>> m_sinkFlowFromNodes;
    std::vector<std::vector<unsigned int>> m_sourceFlowToNodes;
    std::vector<std::vector<unsigned int>> m_leftNeighbourFlow;
    std::vector<std::vector<unsigned int>> m_rightNeighbourFlow;
    std::vector<std::vector<unsigned int>> m_topNeighbourFlow;
    std::vector<std::vector<unsigned int>> m_bottomNeighbourFlow;*/
    unsigned int m_maxHeight;
    unsigned int m_width;
    unsigned int m_height;

public:
    Graph(Image const& image, Image const& imageHelper);
    bool push(unsigned int i, unsigned int j);
    void relabel(unsigned int i, unsigned int j);
    std::shared_ptr<std::pair<unsigned int, unsigned int>> isActive();
    std::vector<std::vector<unsigned int>> getSinkCapacityToNodes();
    std::vector<std::vector<unsigned int>> getSourceCapacityToNodes();
};
