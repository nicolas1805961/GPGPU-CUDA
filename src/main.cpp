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
    // tant qu'il existe un noeud actif et tant que ce noeud à un excess flow je push et je relabel (je relabel si je ne peux pas push)
    while (graph.count_active() > 0)
    {

        for (int i = 0; i < graph.getHeight(); i++)
        {
            for (int j = 0; j < graph.getWidth(); j++)
            {
                    graph.relabel(i,j);
            }
        }
        for (int i = 0; i < graph.getHeight(); i++)
        {
            for (int j = 0; j < graph.getWidth(); j++)
            {
                    graph.push(i,j);
            }
        }
    }
    std::cout << "Graph cut done\n";
    std::vector<std::vector<int>> out = std::vector<std::vector<int>>(graph.getHeight(), std::vector<int>(graph.getWidth(), 0));
    //auto visited = graph.dfs();
    for (int i = 0; i < graph.getHeight(); i++)
    {
        std::cout << i << " / " << graph.getHeight() << "\n";
        for (int j = 0; j < graph.getWidth(); j++)
        {
            if (graph.getHeights()[i][j] > 0)
                out[i][j] = 1;
        }
    }
    
    std::ofstream ofs ("results/out.ppm", std::ios::binary);
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