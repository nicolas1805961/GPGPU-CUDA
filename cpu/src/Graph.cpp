#include "Graph.hpp"
#include <iostream>

Graph::Graph(Image const& image, Image const& imageHelper): m_maxHeight(5/*image.getWidth() * image.getHeight()*/), m_width(image.getWidth()), m_height(image.getHeight())
{
    //initialisation des matrices
    m_heights = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    m_excessFlow = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    m_bottomNeighbourCapacity = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    m_topNeighbourCapacity = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    m_leftNeighbourCapacity = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    m_rightNeighbourCapacity = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 0));
    m_sourceCapacity = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 1));
    m_sinkCapacity = std::vector<std::vector<int>>(m_height, std::vector<int>(m_width, 1));
    // Ici je label les arrêtes entre les noeuds avec la distance en rgb entre chaque pixel (noeud = pixel)
    for (int i = 0; i < m_height - 1; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            int redDistance = pow((image.getImageRgb()[(m_width * i + j) * 3] - image.getImageRgb()[(m_width * (i + 1) + j) * 3]), 2);
            int greenDistance = pow((image.getImageRgb()[((m_width * i + j) * 3) + 1] - image.getImageRgb()[((m_width * (i + 1) + j) * 3) + 1]), 2);
            int blueDistance = pow((image.getImageRgb()[((m_width * i + j) * 3) + 2] - image.getImageRgb()[((m_width * (i + 1) + j) * 3) + 2]), 2);
            int distance = redDistance + greenDistance + blueDistance;
            m_bottomNeighbourCapacity[i][j] = ((sqrt(distance) / sqrt(pow(255, 2))) - 1) * -sqrt(pow(255, 2));
        }
    }
    for (int i = 1; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            int redDistance = pow((image.getImageRgb()[(m_width * i + j) * 3] - image.getImageRgb()[(m_width * (i - 1) + j) * 3]), 2);
            int greenDistance = pow((image.getImageRgb()[((m_width * i + j) * 3) + 1] - image.getImageRgb()[((m_width * (i - 1) + j) * 3) + 1]), 2);
            int blueDistance = pow((image.getImageRgb()[((m_width * i + j) * 3) + 2] - image.getImageRgb()[((m_width * (i - 1) + j) * 3) + 2]), 2);
            int distance = redDistance + greenDistance + blueDistance;
            m_topNeighbourCapacity[i][j] = ((sqrt(distance) / sqrt(pow(255, 2))) - 1) * -sqrt(pow(255, 2));
        }
    }
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 1; j < m_width; j++)
        {
            int redDistance = pow((image.getImageRgb()[(m_width * i + j) * 3] - image.getImageRgb()[((m_width * i + j) * 3) - 3]), 2);
            int greenDistance = pow((image.getImageRgb()[((m_width * i + j) * 3) + 1] - image.getImageRgb()[((m_width * i + j) * 3) - 2]), 2);
            int blueDistance = pow((image.getImageRgb()[((m_width * i + j) * 3) + 2] - image.getImageRgb()[((m_width * i + j) * 3) - 1]), 2);
            int distance = redDistance + greenDistance + blueDistance;
            m_leftNeighbourCapacity[i][j] = ((sqrt(distance) / sqrt(pow(255, 2))) - 1) * -sqrt(pow(255, 2));
        }
        for (int j = 0; j < m_width - 1; j++)
        {
            int redDistance = pow((image.getImageRgb()[(m_width * i + j) * 3] - image.getImageRgb()[((m_width * i + j) * 3) + 3]), 2);
            int greenDistance = pow((image.getImageRgb()[((m_width * i + j) * 3) + 1] - image.getImageRgb()[((m_width * i + j) * 3) + 4]), 2);
            int blueDistance = pow((image.getImageRgb()[((m_width * i + j) * 3) + 2] - image.getImageRgb()[((m_width * i + j) * 3) + 5]), 2);
            int distance = redDistance + greenDistance + blueDistance;
            m_rightNeighbourCapacity[i][j] = ((sqrt(distance) / sqrt(pow(255, 2))) - 1) * -sqrt(pow(255, 2));
        }
    }
    //contient les indices des pixels noirs et blanc
    std::vector<int> white;
    std::vector<int> black;
    int sumIntensityForegroundRed = 0;
    int sumIntensityForegroundGreen = 0;
    int sumIntensityForegroundBlue = 0;
    int sumIntensityBackgroundRed = 0;
    int sumIntensityBackgroundGreen = 0;
    int sumIntensityBackgroundBlue = 0;
    auto ptr = image.getImageRgb();
    auto ptrHelper = imageHelper.getImageRgb();
    // ici je label les arrêtes entre la source et les noeuds appartenant au foreground d'une part, et d'autre part entre les noeuds appartenant au background et le puit. Les noeuds dont on est sur qu'ils sont du foreground ou background sont les pixels blanc et noir respectivement. On les etiquettes avec une très grande valeur car c'est sur qu'ils appartiennent au background/foreground.
    for (int i = 0; i < imageHelper.getHeight() * imageHelper.getWidth(); i++)
    {
        if (ptrHelper[(i * 3)] == ptrHelper[(i * 3) + 1] && ptrHelper[(i * 3)] == ptrHelper[(i * 3) + 2] && ptrHelper[(i * 3)] == 255)
        {
            white.push_back(i * 3);
            m_sourceCapacity[i / m_width][i % m_width] = std::numeric_limits<int>::max(); //sqrt(pow(255, 2) * 3);
            m_sinkCapacity[i / m_width][i % m_width] = 0;
        }
        else if (ptrHelper[(i * 3)] == ptrHelper[(i * 3) + 1] && ptrHelper[(i * 3)] == ptrHelper[(i * 3) + 2] && ptrHelper[(i * 3)] == 0)
        {
            black.push_back(i * 3);
            m_sinkCapacity[i / m_width][i % m_width] = std::numeric_limits<int>::max(); //sqrt(pow(255, 2) * 3);
            m_sourceCapacity[i / m_width][i % m_width] = 0;
        }
    }
    
    for (size_t i = 0; i < white.size(); i++)
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
    if (white.size() == 0 || black.size() == 0)
        std::cerr << "!!! ERROR !!! : Are you sure images are loaded properly ?" << std::endl;
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
            uint8_t r = image.getImageRgb()[m_width * (i * 3) + (j * 3)];
            uint8_t g = image.getImageRgb()[m_width * (i * 3) + (j * 3) + 1];
            uint8_t b = image.getImageRgb()[m_width * (i * 3) + (j * 3) + 2];

            float df = sqrt(pow(r - averageForegroundRed, 2) + pow(g - averageForegroundGreen, 2) + pow(b - averageForegroundBlue, 2));
            float db = sqrt(pow(r - averageBackgroundRed, 2) + pow(g - averageBackgroundGreen, 2) + pow(b - averageBackgroundBlue, 2));

            if (m_sourceCapacity[i][j] == 1)
                m_sourceCapacity[i][j] = ((df / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3);
            if (m_sinkCapacity[i][j] == 1)
                m_sinkCapacity[i][j] = ((db / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3);
        }
    }
    //ici j'initialise l'excess flow en saturant les arrêtes(capacity) partant de la source. Et je créer la capacité dans le sens inverse.
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            m_excessFlow[i][j] = m_sourceCapacity[i][j] - m_sinkCapacity[i][j];
        }
    }
}
//fonction push. Semblable à la slide nvidia en rajoutant la possibilité de push vers le puit et la source. On cherche à push un max vers le puit donc le puit est prioritaire. A l'inverse on cherche à push le moins possible vers la source. Donc la source arrive en dernier recours. Chaque push doit augmenter la capacité dans le sens inverse (residual arc). On ne peut push que si la height du noeud voisin est inférieur de 1 au noeud selectionné.
bool Graph::push(int i, int j) 
{
    if (m_excessFlow[i][j] <= 0 || m_heights[i][j] >= m_maxHeight)
        return false;

    bool ok = false;
    //pour débug décommenter cette ligne
    //std::cout << i << " " << j << " " << m_excessFlow[i][j] << " " << m_heights[i][j] << "\n";
    if (j > 0 && m_heights[i][j - 1] == m_heights[i][j] - 1)
    {
        int flow = std::min(m_leftNeighbourCapacity[i][j], m_excessFlow[i][j]);
        m_excessFlow[i][j] -= flow;
        m_excessFlow[i][j - 1] += flow;
        m_leftNeighbourCapacity[i][j] -= flow;
        m_rightNeighbourCapacity[i][j - 1] += flow;
        ok = true;
    }
    if (j < m_width - 1 && m_heights[i][j + 1] == m_heights[i][j] - 1)
    {
        int flow = std::min(m_rightNeighbourCapacity[i][j], m_excessFlow[i][j]);
        m_excessFlow[i][j] -= flow;
        m_excessFlow[i][j + 1] += flow;
        m_rightNeighbourCapacity[i][j] -= flow;
        m_leftNeighbourCapacity[i][j + 1] += flow;
        ok = true;
    }
    if (i > 0 && m_heights[i - 1][j] == m_heights[i][j] - 1)
    {
        int flow = std::min(m_topNeighbourCapacity[i][j], m_excessFlow[i][j]);
        m_excessFlow[i][j] -= flow;
        m_excessFlow[i - 1][j] += flow;
        m_topNeighbourCapacity[i][j] -= flow;
        m_bottomNeighbourCapacity[i - 1][j] += flow;
        ok = true;
    }
    if (i < m_height - 1 && m_heights[i + 1][j] == m_heights[i][j] - 1)
    {
        int flow = std::min(m_bottomNeighbourCapacity[i][j], m_excessFlow[i][j]);
        m_excessFlow[i][j] -= flow;
        m_excessFlow[i + 1][j] += flow;
        m_bottomNeighbourCapacity[i][j] -= flow;
        m_topNeighbourCapacity[i + 1][j] += flow;
        ok = true;
    }
    return ok;
}
// fonction relabel en ajoutant la possibilité de relabel au dessus de maxheight (cf example wikipedia). On cherche à relabel le noeud actif d'une unité de plus que le voisin ayant la "height" la plus basse et dont l'arc à une capacité supérieur à 0.
void Graph::relabel(unsigned int i, unsigned int j) 
{
    if (m_excessFlow[i][j] > 0 && m_heights[i][j] < m_maxHeight)
    {
        auto myHeight = m_maxHeight;
        if (m_leftNeighbourCapacity[i][j] > 0)
            myHeight = std::min(myHeight, m_heights[i][j - 1] + 1);
        if (m_rightNeighbourCapacity[i][j] > 0)
            myHeight = std::min(myHeight, m_heights[i][j + 1] + 1);
        if (m_topNeighbourCapacity[i][j] > 0)
            myHeight = std::min(myHeight, m_heights[i - 1][j] + 1);
        if (m_bottomNeighbourCapacity[i][j] > 0)
            myHeight = std::min(myHeight, m_heights[i + 1][j] + 1);
        m_heights[i][j] = myHeight;
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
            if (m_excessFlow[i][j] > 0 && m_heights[i][j] < m_maxHeight)
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

int Graph::count_active()
{
    int count = 0;
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            if (m_excessFlow[i][j] > 0 && m_heights[i][j] < m_maxHeight)
            {
                count++;
            }
        }
    }
    std::cout << count << "\n";
    return count;
}

std::vector<std::vector<int>> Graph::getHeights() 
{
    return m_heights;
}

std::vector<std::vector<int>> Graph::getExcessFlow() 
{
    return m_excessFlow;
}

std::vector<std::vector<int>> Graph::getLeftNeighbourCapacity() 
{
    return m_leftNeighbourCapacity;
}

std::vector<std::vector<int>> Graph::getRightNeighbourCapacity() 
{
    return m_rightNeighbourCapacity;
}

std::vector<std::vector<int>> Graph::getTopNeighbourCapacity() 
{
    return m_topNeighbourCapacity;
}

std::vector<std::vector<int>> Graph::getBottomNeighbourCapacity() 
{
    return m_bottomNeighbourCapacity;
}

int Graph::getWidth() 
{
    return m_width;
}

int Graph::getHeight() 
{
    return m_height;
}

std::vector<std::vector<bool>> Graph::dfs()
{
    std::vector<std::vector<bool>> visited = std::vector<std::vector<bool>>(m_height, std::vector<bool>(m_width, false));
    std::stack<std::pair<int, int>> stack;
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            if (m_sourceCapacity[i][j] > 0)
                stack.push(std::make_pair(i, j));
        }
    }
    while (!stack.empty())
    {
        auto s = stack.top(); 
        stack.pop();
        visited[s.first][s.second] = true;
        if (m_leftNeighbourCapacity[s.first][s.second] > 0 && !visited[s.first][s.second - 1])
            stack.push(std::make_pair(s.first, s.second - 1));
        if (m_rightNeighbourCapacity[s.first][s.second] > 0 && !visited[s.first][s.second + 1])
            stack.push(std::make_pair(s.first, s.second + 1));
        if (m_bottomNeighbourCapacity[s.first][s.second] > 0 && !visited[s.first + 1][s.second])
            stack.push(std::make_pair(s.first + 1, s.second));
        if (m_topNeighbourCapacity[s.first][s.second] > 0 && !visited[s.first - 1][s.second])
            stack.push(std::make_pair(s.first - 1, s.second));
        std::cout << stack.size() << "\n";
    }
    return visited;
}
