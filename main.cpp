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
    int x = graph.getHeight() * graph.getWidth();
    std::vector<std::vector<int>> out = std::vector<std::vector<int>>(graph.getHeight(), std::vector<int>(graph.getWidth(), 0));
    //auto visited = graph.dfs();
    for (int i = 0; i < graph.getHeight(); i++)
    {
        for (int j = 0; j < graph.getWidth(); j++)
        {
            std::cout << x - (i * j) << "\n";
            if (graph.getHeights()[i][j] > 0)
                out[i][j] = 1;
        }
    }
    
    std::ofstream ofs ("out.ppm", std::ios::binary);
    ofs << "P6\n" << graph.getWidth() << " " << graph.getHeight() << "\n255\n";
    for (int i = 0; i < graph.getHeight(); i++)
    {
        for (int j = 0; j < graph.getWidth(); j++)
        {
            char r = (char)(255 * out[i][j]); 
            char g = (char)(255 * out[i][j]); 
            char b = (char)(255 * out[i][j]); 
            ofs << r << g << b;
        }
    }
    ofs.close();
    return 0;
}