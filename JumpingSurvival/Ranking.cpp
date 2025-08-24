#include"Ranking.h"

Ranking::Ranking()
{
    hiScore = 0;
    // ”z—ñ‚ğ0‚Å‰Šú‰»
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