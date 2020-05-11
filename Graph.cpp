#include "Graph.hpp"
#include <iostream>

Graph::Graph(Image const& image, Image const& imageHelper): m_maxHeight(image.getWidth() * image.getHeight()), m_width(image.getWidth()), m_height(image.getHeight())
{
    m_heights = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_excessFlow = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_bottomNeighbourCapacity = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_topNeighbourCapacity = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_leftNeighbourCapacity = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_rightNeighbourCapacity = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_sourceCapacityToNodes = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 1));
    m_sinkCapacityFromNodes = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 1));
    m_sinkCapacityToNodes = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_sourceCapacityFromNodes = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    /*m_leftNeighbourFlow = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_rightNeighbourFlow = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_topNeighbourFlow = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_bottomNeighbourFlow = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_sinkFlowFromNodes = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));
    m_sourceFlowToNodes = std::vector<std::vector<unsigned int>>(m_height, std::vector<unsigned int>(m_width, 0));*/
    //int max = 0;
    unsigned int x = 0;
    unsigned int y = 0;
    for (size_t j = 0; j < m_width; j++)
    {
        for (size_t i = 0; i < m_height - 1; i++)
        {
            x = sqrt(pow((image.getImage()[m_width * i * 3 + (j * 3)] - image.getImage()[m_width * ((i + 1) * 3) + (j * 3)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 1] - image.getImage()[m_width * ((i + 1) * 3) + (j * 3) + 1]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 2] - image.getImage()[m_width * ((i + 1) * 3) + (j * 3) + 2]), 2));
            y = ((x / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3);
            /*if (y > max)
                max = y;*/
            m_bottomNeighbourCapacity[i][j] = y;
        }
        for (size_t i = 1; i < m_height; i++)
        {

            x = sqrt(pow((image.getImage()[m_width * i * 3 + (j * 3)] - image.getImage()[m_width * ((i - 1) * 3) + (j * 3)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 1] - image.getImage()[m_width * ((i - 1) * 3) + (j * 3) + 1]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 2] - image.getImage()[m_width * ((i - 1) * 3) + (j * 3) + 2]), 2));
            y = ((x / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3);
            /*if (y > max)
                max = y;*/
            m_topNeighbourCapacity[i][j] = y;
        }
    }
    for (size_t i = 0; i < m_height; i++)
    {
        for (size_t j = 1; j < m_width; j++)
        {
            x = sqrt(pow((image.getImage()[m_width * i * 3 + (j * 3)] - image.getImage()[m_width * i * 3 + ((j * 3) - 3)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 1] - image.getImage()[m_width * i * 3 + ((j * 3) - 2)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 2] - image.getImage()[m_width * i * 3 + ((j * 3) - 1)]), 2));
            y = ((x / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3);
            /*if (y > max)
                max = y;*/
            m_leftNeighbourCapacity[i][j] = y;
        }
        for (size_t j = 0; j < m_width - 1; j++)
        {
            x = sqrt(pow((image.getImage()[m_width * i * 3 + (j * 3)] - image.getImage()[m_width * i * 3 + ((j * 3) + 3)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 1] - image.getImage()[m_width * i * 3 + ((j * 3) + 4)]), 2) + pow((image.getImage()[m_width * i * 3 + (j * 3) + 2] - image.getImage()[m_width * i * 3 + ((j * 3) + 5)]), 2));
            y = ((x / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3);
            /*if (y > max)
                max = y;*/
            m_rightNeighbourCapacity[i][j] = y;
        }
    }
    std::vector<unsigned int> white;
    std::vector<unsigned int> black;
    unsigned int sumIntensityForegroundRed = 0;
    unsigned int sumIntensityForegroundGreen = 0;
    unsigned int sumIntensityForegroundBlue = 0;
    unsigned int sumIntensityBackgroundRed = 0;
    unsigned int sumIntensityBackgroundGreen = 0;
    unsigned int sumIntensityBackgroundBlue = 0;
    auto ptr = image.getImage();
    auto ptrHelper = imageHelper.getImage();
    for (int i = 0; i < imageHelper.getHeight() * imageHelper.getWidth(); i++)
    {
        if (ptrHelper[(i * 3)] == ptrHelper[(i * 3) + 1] && ptrHelper[(i * 3)] == ptrHelper[(i * 3) + 2] && ptrHelper[(i * 3)] == 255)
        {
            white.push_back(i * 3);
            m_sourceCapacityToNodes[i / m_width][i % m_width] = 1000000; //sqrt(pow(255, 2) * 3);
            m_sinkCapacityFromNodes[i / m_width][i % m_width] = 0;
        }
        else if (ptrHelper[(i * 3)] == ptrHelper[(i * 3) + 1] && ptrHelper[(i * 3)] == ptrHelper[(i * 3) + 2] && ptrHelper[(i * 3)] == 0)
        {
            black.push_back(i * 3);
            m_sinkCapacityFromNodes[i / m_width][i % m_width] = 1000000; //sqrt(pow(255, 2) * 3);
            m_sourceCapacityToNodes[i / m_width][i % m_width] = 0;
        }
    }
    /*std::vector<float> histogamRedForeground(256, 0);
    std::vector<float> histogamGreenForeground(256, 0);
    std::vector<float> histogamBlueForeground(256, 0);
    std::vector<float> histogamRedBackground(256, 0);
    std::vector<float> histogamGreenBackground(256, 0);
    std::vector<float> histogamBlueBackground(256, 0);*/
    /*for (size_t i = 0; i < 256; i++)
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
    float averageForegroundRed = sumIntensityForegroundRed / white.size();
    float averageForegroundGreen = sumIntensityForegroundGreen / white.size();
    float averageForegroundBlue = sumIntensityForegroundBlue / white.size();
    float averageBackgroundRed = sumIntensityBackgroundRed / black.size();
    float averageBackgroundGreen = sumIntensityBackgroundGreen / black.size();
    float averageBackgroundBlue = sumIntensityBackgroundBlue / black.size();

    for (size_t i = 0; i < m_height; i++)
    {
        for (size_t j = 0; j < m_width; j++)
        {
            uint8_t r = image.getImage()[m_width * (i * 3) + (j * 3)];
            uint8_t g = image.getImage()[m_width * (i * 3) + (j * 3) + 1];
            uint8_t b = image.getImage()[m_width * (i * 3) + (j * 3) + 2];

            /*float pf = (histogamRedForeground[r] + histogamGreenForeground[g] + histogamBlueForeground[b]) / 3;
            float pb = (histogamRedBackground[r] + histogamGreenBackground[g] + histogamBlueBackground[b]) / 3;*/

            float df = sqrt(pow(r - averageForegroundRed, 2) + pow(g - averageForegroundGreen, 2) + pow(b - averageForegroundBlue, 2));
            float db = sqrt(pow(r - averageBackgroundRed, 2) + pow(g - averageBackgroundGreen, 2) + pow(b - averageBackgroundBlue, 2));

            /*float pf = ((df / (sqrt(pow(255, 2) * 3))) - 1) * -1;
            float pb = ((db / (sqrt(pow(255, 2) * 3))) - 1) * -1;*/
            //float pf = df / (df + db);
            //float pb = db / (df + db);
            if (m_sourceCapacityToNodes[i][j] == 1)
                m_sourceCapacityToNodes[i][j] = ((df / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3); //-log(pf);;
            if (m_sinkCapacityFromNodes[i][j] == 1)
                m_sinkCapacityFromNodes[i][j] = ((db / sqrt(pow(255, 2) * 3)) - 1) * -sqrt(pow(255, 2) * 3); //-log(1 - pf);
        }
    }

    for (size_t i = 0; i < m_height; i++)
    {
        for (size_t j = 0; j < m_width; j++)
        {
            //m_excessFlow[i][j] = m_sourceCapacityToNodes[i][j] - m_sinkCapacityFromNodes[i][j];
            //m_sourceFlowToNodes[i][j] = m_sourceCapacityToNodes[i][j];
            m_excessFlow[i][j] = m_sourceCapacityToNodes[i][j];
            m_sourceCapacityFromNodes[i][j] = m_sourceCapacityToNodes[i][j];
            m_sourceCapacityToNodes[i][j] = 0;
        }
    }
    /*for (size_t i = 0; i < m_height; i++)
    {
        for (size_t j = 0; j < m_width; j++)
        {
            if (m_sourceCapacityToNodes[i][j] != 0)
                std::cout << m_sourceCapacityToNodes[i][j] << "\n";
        }
    }*/
    //exit(0);
}

bool Graph::push(unsigned int i, unsigned int j) 
{
    bool ok = false;
    bool notSource = false;
    if (m_heights[i][j] == 1 && m_sinkCapacityFromNodes[i][j] > 0)
    {
        int flow = std::min(m_sinkCapacityFromNodes[i][j], m_excessFlow[i][j]);
        m_excessFlow[i][j] -= flow;
        m_sinkCapacityFromNodes[i][j] -= flow;
        m_sinkCapacityToNodes[i][j] += flow;
        notSource = true;
        ok = true;
    }
    if (j >= 1 && m_heights[i][j - 1] == m_heights[i][j] - 1 && m_leftNeighbourCapacity[i][j] > 0)
    {
        int flow = std::min(m_leftNeighbourCapacity[i][j], m_excessFlow[i][j]);
        m_excessFlow[i][j] -= flow;
        m_excessFlow[i][j - 1] += flow;
        m_leftNeighbourCapacity[i][j] -= flow;
        m_rightNeighbourCapacity[i][j - 1] += flow;
        notSource = true;
        ok = true;
    }
    if (j < m_width - 1 && m_heights[i][j + 1] == m_heights[i][j] - 1 && m_rightNeighbourCapacity[i][j] > 0)
    {
        int flow = std::min(m_rightNeighbourCapacity[i][j], m_excessFlow[i][j]);
        m_excessFlow[i][j] -= flow;
        m_excessFlow[i][j + 1] += flow;
        m_rightNeighbourCapacity[i][j] -= flow;
        m_leftNeighbourCapacity[i][j + 1] += flow;
        notSource = true;
        ok = true;
    }
    if (i >= 1 && m_heights[i - 1][j] == m_heights[i][j] - 1 && m_topNeighbourCapacity[i][j] > 0)
    {
        int flow = std::min(m_topNeighbourCapacity[i][j], m_excessFlow[i][j]);
        m_excessFlow[i][j] -= flow;
        m_excessFlow[i - 1][j] += flow;
        m_topNeighbourCapacity[i][j] -= flow;
        m_bottomNeighbourCapacity[i - 1][j] += flow;
        notSource = true;
        ok = true;
    }
    if (i < m_height - 1 && m_heights[i + 1][j] == m_heights[i][j] - 1 && m_bottomNeighbourCapacity[i][j] > 0)
    {
        int flow = std::min(m_bottomNeighbourCapacity[i][j], m_excessFlow[i][j]);
        m_excessFlow[i][j] -= flow;
        m_excessFlow[i + 1][j] += flow;
        m_bottomNeighbourCapacity[i][j] -= flow;
        m_topNeighbourCapacity[i + 1][j] += flow;
        notSource = true;
        ok = true;
    }
    if (m_maxHeight == m_heights[i][j] - 1 && m_sourceCapacityFromNodes[i][j] > 0 && notSource == false)
    {
        int flow = std::min(m_sourceCapacityFromNodes[i][j], m_excessFlow[i][j]);
        m_excessFlow[i][j] -= flow;
        m_sourceCapacityFromNodes[i][j] -= flow;
        m_sourceCapacityToNodes[i][j] += flow;
        ok = true;
    }
    return ok;
}

void Graph::relabel(unsigned int i, unsigned int j) 
{
    auto myHeight = m_maxHeight;
    if (m_sinkCapacityFromNodes[i][j] > 0)
        myHeight = 1;
    if (j >= 1 && m_leftNeighbourCapacity[i][j] > 0)
        myHeight = std::min(myHeight, m_heights[i][j - 1] + 1);
    if (j < m_width - 1 && m_rightNeighbourCapacity[i][j] > 0)
        myHeight = std::min(myHeight, m_heights[i][j + 1] + 1);
    if (i >= 1 && m_topNeighbourCapacity[i][j] > 0)
        myHeight = std::min(myHeight, m_heights[i - 1][j] + 1);
    if (i < m_height - 1 && m_bottomNeighbourCapacity[i][j] > 0)
        myHeight = std::min(myHeight, m_heights[i + 1][j] + 1);
    if (m_sourceCapacityFromNodes[i][j] > 0 && myHeight == m_maxHeight)
        myHeight = m_maxHeight + 1;
    m_heights[i][j] = myHeight;
}

std::optional<std::pair<unsigned int, unsigned int>> Graph::isActive() 
{
    std::pair<unsigned int, unsigned int> pair;
    int count = 0;
    for (unsigned int i = 0; i < m_height; i++)
    {
        for (unsigned int j = 0; j < m_width; j++)
        {
            if (m_excessFlow[i][j] > 0 /*&& m_heights[i][j] < m_maxHeight*/)
            {
                if (count == 0)
                    pair = std::make_pair(i, j);
                count++;
            }
        }
    }
    if (count > 0)
        return pair;
    return std::nullopt;
}

std::vector<std::vector<unsigned int>> Graph::getSinkCapacityToNodes()
{
    return m_sinkCapacityToNodes;
}

std::vector<std::vector<unsigned int>> Graph::getSourceCapacityToNodes()
{
    return m_sourceCapacityToNodes;
}