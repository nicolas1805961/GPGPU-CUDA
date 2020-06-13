#include "graphcut_cpu.hpp"


// Tant qu'il existe un noeud actif et tant que ce noeud a un excess flow,
// je push et je relabel (je relabel si je ne peux pas push)

void graphcut_cpu(Graph &graph)
{
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
}
