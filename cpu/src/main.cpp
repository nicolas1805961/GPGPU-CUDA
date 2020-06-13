#include "Image.hpp"
#include "Graph.hpp"
#include "graphcut_cpu.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>


int main()
{
    Image image("inputs/12003.jpg");
    Image imageHelper("inputs/12003_modified.jpg");
    Graph graph(image, imageHelper);

    graphcut_cpu(graph);
    std::cout << "Graph cut done\n";

    std::vector<std::vector<int>> out =
        std::vector<std::vector<int>>(
                graph.getHeight(),
                std::vector<int>(graph.getWidth(), 0)
        );
    auto visited = graph.dfs();
    for (int i = 0; i < graph.getHeight(); i++)
    {
        std::cout << i << " / " << graph.getHeight() << "\n";
        for (int j = 0; j < graph.getWidth(); j++)
        {
            /*std::cout << "i = " << i << ", j = " << j << ", bottom = " << graph.getBottomNeighbourCapacity()[i][j] << ", top = " << graph.getTopNeighbourCapacity()[i][j] << ", left = " << graph.getLeftNeighbourCapacity()[i][j] << ", right = " << graph.getRightNeighbourCapacity()[i][j] << "\n";*/
            if (visited[i][j]/*graph.getHeights()[i][j] > 0*/)
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
