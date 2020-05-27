#include "Graph.hpp"
#include <iostream>

Graph::Graph(Image const& image, Image const& imageHelper): m_maxHeight(image.getWidth() * image.getHeight()), m_width(image.getWidth()), m_height(image.getHeight())
{
    //initialisation des matrices
    m_heights = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    m_excessFlow = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    m_bottomNeighbourCapacity = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    m_topNeighbourCapacity = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    m_leftNeighbourCapacity = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    m_rightNeighbourCapacity = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    m_sourceCapacityToNodes = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    m_sinkCapacityFromNodes = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    m_sinkCapacityToNodes = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    m_sourceCapacityFromNodes = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    /*m_leftNeighbourFlow = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_rightNeighbourFlow = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_topNeighbourFlow = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_bottomNeighbourFlow = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_sinkFlowFromNodes = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_sourceFlowToNodes = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));*/
    //int max = 0;
    int x = 0;
    int y = 0;
    /*for (size_t j = 0; j < m_width; j++)
    {
        for (size_t i = 0; i < m_height - 1; i++)
        {
            x = sqrt(pow((image.getImage()[m_width * i * 3 + (j * 3)] - image.getImage()[m_width * ((i + 1) * 3) + (j * 3)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 1] - image.getImage()[m_width * ((i + 1) * 3) + (j * 3) + 1]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 2] - image.getImage()[m_width * ((i + 1) * 3) + (j * 3) + 2]), 2));
            y = ((x / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3);*/
            /*if (y > max)
                max = y;*/
            /*m_bottomNeighbourCapacity[i][j] = y;
        }
        for (size_t i = 1; i < m_height; i++)
        {

            x = sqrt(pow((image.getImage()[m_width * i * 3 + (j * 3)] - image.getImage()[m_width * ((i - 1) * 3) + (j * 3)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 1] - image.getImage()[m_width * ((i - 1) * 3) + (j * 3) + 1]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 2] - image.getImage()[m_width * ((i - 1) * 3) + (j * 3) + 2]), 2));
            y = ((x / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3);*/
            /*if (y > max)
                max = y;*/
            /*m_topNeighbourCapacity[i][j] = y;
        }
    }
    for (size_t i = 0; i < m_height; i++)
    {
        for (size_t j = 1; j < m_width; j++)
        {
            x = sqrt(pow((image.getImage()[m_width * i * 3 + (j * 3)] - image.getImage()[m_width * i * 3 + ((j * 3) - 3)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 1] - image.getImage()[m_width * i * 3 + ((j * 3) - 2)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 2] - image.getImage()[m_width * i * 3 + ((j * 3) - 1)]), 2));
            y = ((x / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3);*/
            /*if (y > max)
                max = y;*/
            /*m_leftNeighbourCapacity[i][j] = y;
        }
        for (size_t j = 0; j < m_width - 1; j++)
        {
            x = sqrt(pow((image.getImage()[m_width * i * 3 + (j * 3)] - image.getImage()[m_width * i * 3 + ((j * 3) + 3)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 1] - image.getImage()[m_width * i * 3 + ((j * 3) + 4)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 2] - image.getImage()[m_width * i * 3 + ((j * 3) + 5)]), 2));
            y = ((x / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3);*/
            /*if (y > max)
                max = y;*/
            /*m_rightNeighbourCapacity[i][j] = y;
        }
    }*/
    // Ici je label les arrêtes entre les noeuds avec la distance en rgb entre chaque pixel (noeud = pixel)
    for (int i = 0; i < m_height - 1; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            x = sqrt(pow((image.getImage()[m_width * i * 3 + (j * 3)] - image.getImage()[m_width * ((i + 1) * 3) + (j * 3)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 1] - image.getImage()[m_width * ((i + 1) * 3) + (j * 3) + 1]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 2] - image.getImage()[m_width * ((i + 1) * 3) + (j * 3) + 2]), 2));
            y = ((x / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3);
            /*if (y > max)
                max = y;*/
            m_bottomNeighbourCapacity[i][j] = y;
        }
    }
    for (int i = 1; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {

            x = sqrt(pow((image.getImage()[m_width * i * 3 + (j * 3)] - image.getImage()[m_width * ((i - 1) * 3) + (j * 3)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 1] - image.getImage()[m_width * ((i - 1) * 3) + (j * 3) + 1]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 2] - image.getImage()[m_width * ((i - 1) * 3) + (j * 3) + 2]), 2));
            y = ((x / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3);
            /*if (y > max)
                max = y;*/
            m_topNeighbourCapacity[i][j] = y;
        }
    }
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 1; j < m_width; j++)
        {
            x = sqrt(pow((image.getImage()[m_width * i * 3 + (j * 3)] - image.getImage()[m_width * i * 3 + ((j * 3) - 3)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 1] - image.getImage()[m_width * i * 3 + ((j * 3) - 2)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 2] - image.getImage()[m_width * i * 3 + ((j * 3) - 1)]), 2));
            y = ((x / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3);
            /*if (y > max)
                max = y;*/
            m_leftNeighbourCapacity[i][j] = y;
        }
        for (int j = 0; j < m_width - 1; j++)
        {
            x = sqrt(pow((image.getImage()[m_width * i * 3 + (j * 3)] - image.getImage()[m_width * i * 3 + ((j * 3) + 3)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 1] - image.getImage()[m_width * i * 3 + ((j * 3) + 4)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 2] - image.getImage()[m_width * i * 3 + ((j * 3) + 5)]), 2));
            y = ((x / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3);
            /*if (y > max)
                max = y;*/
            m_rightNeighbourCapacity[i][j] = y;
        }
    }
    //contient les indices des pixels noirs et blanc
    std::vector<int> white;
    std::vector<int> black;
    /*int sumIntensityForegroundRed = 0;
    int sumIntensityForegroundGreen = 0;
    int sumIntensityForegroundBlue = 0;
    int sumIntensityBackgroundRed = 0;
    int sumIntensityBackgroundGreen = 0;
    int sumIntensityBackgroundBlue = 0;*/
    //auto ptr = image.getImage();
    auto ptrHelper = imageHelper.getImage();
    // ici je label les arrêtes entre la source et les noeuds appartenant au foreground d'une part, et d'autre part entre les noeuds appartenant au background et le puit. Les noeuds dont on est sur qu'ils sont du foreground ou background sont les pixels blanc et noir respectivement. On les etiquettes avec une très grande valeur car c'est sur qu'ils appartiennent au background/foreground.
    for (int i = 0; i < imageHelper.getHeight() * imageHelper.getWidth(); i++)
    {
        if (ptrHelper[(i * 3)] == ptrHelper[(i * 3) + 1] && ptrHelper[(i * 3)] == ptrHelper[(i * 3) + 2] && ptrHelper[(i * 3)] == 255)
        {
            white.push_back(i * 3);
            m_sourceCapacityToNodes[i / m_width][i % m_width] = 100000000; //sqrt(pow(255, 2) * 3);
            //m_sinkCapacityFromNodes[i / m_width][i % m_width] = 0;
        }
        else if (ptrHelper[(i * 3)] == ptrHelper[(i * 3) + 1] && ptrHelper[(i * 3)] == ptrHelper[(i * 3) + 2] && ptrHelper[(i * 3)] == 0)
        {
            black.push_back(i * 3);
            m_sinkCapacityFromNodes[i / m_width][i % m_width] = 100000000; //sqrt(pow(255, 2) * 3);
            //m_sourceCapacityToNodes[i / m_width][i % m_width] = 0;
        }
    }
    /*std::vector<float> histogamRedForeground(256, 0);
    std::vector<float> histogamGreenForeground(256, 0);
    std::vector<float> histogamBlueForeground(256, 0);
    std::vector<float> histogamRedBackground(256, 0);
    std::vector<float> histogamGreenBackground(256, 0);
    std::vector<float> histogamBlueBackground(256, 0);*/
    /*for (int i = 0; i < 256; i++)
    {
        for (size_t j = 0; j < white.size(); j++)
        {
            if (ptr[white[j]] == i)
                histogamRedForeground[i]++;
            if (ptr[white[j] + 1] == i)
                histogamGreenForeground[i]++;
            if (ptr[white[j] + 2] == i)
                histogamBlueForeground[i]++;
        }
        for (size_t j = 0; j < black.size(); j++)
        {
            if (ptr[black[j]] == i)
                histogamRedBackground[i]++;
            if (ptr[black[j] + 1] == i)
                histogamGreenBackground[i]++;
            if (ptr[black[j] + 2] == i)
                histogamBlueBackground[i]++;
        }
        histogamRedForeground[i] /= white.size();
        histogamGreenForeground[i] /= white.size();
        histogamBlueForeground[i] /= white.size();
        histogamRedBackground[i] /= black.size();
        histogamGreenBackground[i] /= black.size();
        histogamBlueBackground[i] /= black.size();
    }*/
    
    /*for (size_t i = 0; i < white.size(); i++)
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
            uint8_t r = image.getImage()[m_width * (i * 3) + (j * 3)];
            uint8_t g = image.getImage()[m_width * (i * 3) + (j * 3) + 1];
            uint8_t b = image.getImage()[m_width * (i * 3) + (j * 3) + 2];

            float pf = (histogamRedForeground[r] + histogamGreenForeground[g] + histogamBlueForeground[b]) / 3;
            float pb = (histogamRedBackground[r] + histogamGreenBackground[g] + histogamBlueBackground[b]) / 3;*/

            /*float df = sqrt(pow(r - averageForegroundRed, 2) + pow(g - averageForegroundGreen, 2) + pow(b - averageForegroundBlue, 2));
            float db = sqrt(pow(r - averageBackgroundRed, 2) + pow(g - averageBackgroundGreen, 2) + pow(b - averageBackgroundBlue, 2));*/

            /*float pf = ((df / (sqrt(pow(255, 2) * 3))) - 1) * -1;
            float pb = ((db / (sqrt(pow(255, 2) * 3))) - 1) * -1;*/
            //float pf = df / (df + db);
            //float pb = db / (df + db);
            /*if (m_sourceCapacityToNodes[i][j] == 1)
                m_sourceCapacityToNodes[i][j] = ((df / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3); //-log(pf);;
            if (m_sinkCapacityFromNodes[i][j] == 1)
                m_sinkCapacityFromNodes[i][j] = ((db / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3); //-log(1 - pf);
        }
    }*/
    //ici j'initialise l'excess flow en saturant les arrêtes(capacity) partant de la source. Et je créer la capacité dans le sens inverse.
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            //m_excessFlow[i][j] = m_sourceCapacityToNodes[i][j] - m_sinkCapacityFromNodes[i][j];
            //m_sourceFlowToNodes[i][j] = m_sourceCapacityToNodes[i][j];
            m_excessFlow[i][j] = m_sourceCapacityToNodes[i][j];
            m_sourceCapacityFromNodes[i][j] = m_sourceCapacityToNodes[i][j];
            m_sourceCapacityToNodes[i][j] = 0;
        }
    }
    /*for (int i = 0; i < m_height; i++)
    {
        for (size_t j = 0; j < m_width; j++)
        {
            if (m_sourceCapacityToNodes[i][j] != 0)
                std::cout << m_sourceCapacityToNodes[i][j] << "\n";
        }
    }*/
    //exit(0);
}
//fonction push. Semblable à la slide nvidia en rajoutant la possibilité de push vers le puit et la source. On cherche à push un max vers le puit donc le puit est prioritaire. A l'inverse on cherche à push le moins possible vers la source. Donc la source arrive en dernier recours. Chaque push doit augmenter la capacité dans le sens inverse (residual arc). On ne peut push que si la height du noeud voisin est inférieur de 1 au noeud selectionné.
bool Graph::push(unsigned int i, unsigned int j) 
{
    bool ok = false;
    //pour débug décommenter cette ligne
    //std::cout << i << " " << j << " " << m_excessFlow[i][j] << " " << m_heights[i][j] << "\n";
    if (m_excessFlow[i][j] > 0 && m_sinkCapacityFromNodes[i][j] > 0 && m_heights[i][j] == 1)
    {
        int flow = std::min(m_sinkCapacityFromNodes[i][j], m_excessFlow[i][j]);
        m_excessFlow[i][j] -= flow;
        m_sinkCapacityFromNodes[i][j] -= flow;
        m_sinkCapacityToNodes[i][j] += flow;
        ok = true;
    }
    if (m_excessFlow[i][j] > 0 && m_leftNeighbourCapacity[i][j] > 0 && m_heights[i][j - 1] == m_heights[i][j] - 1)
    {
        int flow = std::min(m_leftNeighbourCapacity[i][j], m_excessFlow[i][j]);
        m_excessFlow[i][j] -= flow;
        m_excessFlow[i][j - 1] += flow;
        m_leftNeighbourCapacity[i][j] -= flow;
        m_rightNeighbourCapacity[i][j - 1] += flow;
        ok = true;
    }
    if (m_excessFlow[i][j] > 0 && m_rightNeighbourCapacity[i][j] > 0 && m_heights[i][j + 1] == m_heights[i][j] - 1)
    {
        int flow = std::min(m_rightNeighbourCapacity[i][j], m_excessFlow[i][j]);
        m_excessFlow[i][j] -= flow;
        m_excessFlow[i][j + 1] += flow;
        m_rightNeighbourCapacity[i][j] -= flow;
        m_leftNeighbourCapacity[i][j + 1] += flow;
        ok = true;
    }
    if (m_excessFlow[i][j] > 0 && m_topNeighbourCapacity[i][j] > 0 && m_heights[i - 1][j] == m_heights[i][j] - 1)
    {
        int flow = std::min(m_topNeighbourCapacity[i][j], m_excessFlow[i][j]);
        m_excessFlow[i][j] -= flow;
        m_excessFlow[i - 1][j] += flow;
        m_topNeighbourCapacity[i][j] -= flow;
        m_bottomNeighbourCapacity[i - 1][j] += flow;
        ok = true;
    }
    if (m_excessFlow[i][j] > 0 && m_bottomNeighbourCapacity[i][j] > 0 && m_heights[i + 1][j] == m_heights[i][j] - 1)
    {
        int flow = std::min(m_bottomNeighbourCapacity[i][j], m_excessFlow[i][j]);
        m_excessFlow[i][j] -= flow;
        m_excessFlow[i + 1][j] += flow;
        m_bottomNeighbourCapacity[i][j] -= flow;
        m_topNeighbourCapacity[i + 1][j] += flow;
        ok = true;
    }
    if (m_excessFlow[i][j] > 0 && m_sourceCapacityFromNodes[i][j] > 0 && m_maxHeight + 1 == m_heights[i][j])
    {
        int flow = std::min(m_sourceCapacityFromNodes[i][j], m_excessFlow[i][j]);
        m_excessFlow[i][j] -= flow;
        m_sourceCapacityFromNodes[i][j] -= flow;
        m_sourceCapacityToNodes[i][j] += flow;
        ok = true;
    }
    return ok;
}
// fonction relabel en ajoutant la possibilité de relabel au dessus de maxheight (cf example wikipedia). On cherche à relabel le noeud actif d'une unité de plus que le voisin ayant la "height" la plus basse et dont l'arc à une capacité supérieur à 0.
void Graph::relabel(unsigned int i, unsigned int j) 
{
    if (m_excessFlow[i][j] > 0 /*&& m_heights[i][j] < m_maxHeight*/)
    {
        auto myHeight = std::numeric_limits<int>::max();
        if (m_sinkCapacityFromNodes[i][j] > 0)
            myHeight = std::min(myHeight, 0);
        if (m_leftNeighbourCapacity[i][j] > 0)
            myHeight = std::min(myHeight, m_heights[i][j - 1]);
        if (m_rightNeighbourCapacity[i][j] > 0)
            myHeight = std::min(myHeight, m_heights[i][j + 1]);
        if (m_topNeighbourCapacity[i][j] > 0)
            myHeight = std::min(myHeight, m_heights[i - 1][j]);
        if (m_bottomNeighbourCapacity[i][j] > 0)
            myHeight = std::min(myHeight, m_heights[i + 1][j]);
        if (m_sourceCapacityFromNodes[i][j] > 0)
            myHeight = std::min(myHeight, m_maxHeight);
        m_heights[i][j] = myHeight + 1;
    }
}
//selection du noeud actif : celui qui est actif et qui a la height maximum(ce dernier point n'est pas necessaire). On renvoit une paire d'indices d'un noeud actif.
std::shared_ptr<std::pair<unsigned int, unsigned int>> Graph::isActive() 
{
    std::pair<unsigned int, unsigned int> pair;
    unsigned int count = 0;
    int maxHeight = -1;
    int row = 0;
    int column = 0;
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            if (m_excessFlow[i][j] > 0 /*&& m_heights[i][j] < m_maxHeight*/)
            {
                if (m_heights[i][j] > maxHeight)
                {
                    maxHeight = m_heights[i][j];
                    row = i;
                    column = j;
                }
                count++;
            }
        }
    }
    //pour le débug
    std::cout << count << "\n";
    pair = std::make_pair(row, column);
    if (count > 0)
        return std::make_shared<std::pair<unsigned int, unsigned int>>(pair);
    else
        return nullptr;
}

std::vector<std::vector<int>> Graph::getTop() 
{
    return m_topNeighbourCapacity;
}

std::vector<std::vector<int>> Graph::getBottom() 
{
    return m_bottomNeighbourCapacity;
}

std::vector<std::vector<int>> Graph::getLeft() 
{
    return m_leftNeighbourCapacity;
}

std::vector<std::vector<int>> Graph::getRight() 
{
    return m_rightNeighbourCapacity;
}

std::vector<std::vector<int>> Graph::getHeights() 
{
    return m_heights;
}

std::vector<std::vector<int>> Graph::getExcessFlow() 
{
    return m_excessFlow;
}