#include"Ranking.h"

Ranking::Ranking()
{
    hiScore = 0;
    // �z���0�ŏ�����
    for (int i = 0; i < 5; ++i) 
    {
        scores[i] = 0;
    }
}

Ranking::~Ranking()
{
}

void Ranking::AddScore(int score)
{

}