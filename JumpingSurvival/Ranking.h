#pragma once
#include"DxLib.h"
#include <algorithm>

class Ranking
{
public:

    Ranking();
    ~Ranking();

    void AddScore(int score);
    void Draw();

private:
    int hiScore;
    int scores[5];
    bool isHighScore;
};