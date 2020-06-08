#include "Image.hpp"
#include "Graph.hpp"
#include <iostream>
#include <fstream>
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
    std::vector<std::vector<int>> out = std::vector<std::vector<int>>(graph.getHeight(), std::vector<int>(graph.getWidth(), 0));
    for (int i = 0; i < graph.getHeight(); i++)
    {
        for (int j = 0; j < graph.getWidth(); j++)
        {
            if (graph.getHeights()[i][j] > 0)
                out[i][j] = 1;
            //std::cout << graph.getHeights()[i][j] << "\n";
            /*std::cout << "i = " << i << ", j = " << j << " left = " << graph.getSourceCapacityToNodes()[i][j] << ", right = " << graph.getSinkCapacityToNodes()[i][j] << ", top = " << graph.getTopNeighbourCapacity()[i][j] << ", bottom = " << graph.getBottomNeighbourCapacity()[i][j] << "\n";*/
        }
    }
    std::ofstream ofs ("image.pbm", std::ios::binary);
    ofs << "P4\n" << graph.getWidth() << " " << graph.getHeight() << "\n";
    for (int i = 0; i < graph.getHeight(); i++)
    {
        for (int j = 0; j < graph.getWidth(); j++)
        {
            char b = (char)(out[i][j]);  
            ofs << b;
        }
    }
    ofs.close();
    return 0;
}