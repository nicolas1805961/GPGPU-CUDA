#include "Image.hpp"
#include "Graph.hpp"
#include <iostream>
#include <vector>
#include <cmath>

int main()
{
    Image image("inputs/3096.jpg");
    Image imageHelper("inputs/3096_LI.jpg");
    Graph graph(image, imageHelper);
    while (auto indices = graph.isActive())
    {
        std::cout << (*indices).first << ", " << (*indices).second << std::endl;
        if (!graph.push((*indices).first, (*indices).second))
            graph.relabel((*indices).first, (*indices).second);
    }
    return 0;
}
