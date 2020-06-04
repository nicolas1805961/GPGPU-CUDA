#include "Image.hpp"
#include "Graph.hpp"
#include <iostream>
#include <vector>
#include <cmath>

int main()
{
    Image image("inputs/12003.jpg");
    Image imageHelper("inputs/12003_modified.jpg");
    Graph graph(image, imageHelper);
    /*auto activeNodes = graph.isActive();
    while (activeNodes.size() > 0)
    {
        for (auto const &it : activeNodes)
            graph.relabel(it.first, it.second);
        for (auto const &it : activeNodes)
            graph.push(it.first, it.second);
        activeNodes = graph.isActive();
    }*/
    // tant qu'il existe un noeud actif et tant que ce noeud à un excess flow je push et je relabel (je relabel si je ne peux pas push)
    while (auto indices = graph.isActive())
    {
        while (graph.getExcessFlow()[(*indices).first][(*indices).second] > 0)
        {
            if (!graph.push((*indices).first, (*indices).second))
                graph.relabel((*indices).first, (*indices).second);
        }
    }
    std::cout << "sinkCapacityFromNodes = " << graph.getSinkCapacityFromNodes()[0][0] << "\n";
    std::cout << "sinkCapacityToNodes = " << graph.getSinkCapacityToNodes()[0][0] << "\n";
    std::cout << "sourceCapacityFromNodes = " << graph.getSourceCapacityFromNodes()[0][0] << "\n";
    std::cout << "sourceCapacityToNodes = " << graph.getSourceCapacityToNodes()[0][0] << "\n";
    return 0;
}