#pragma once
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class Score
{
public:
	Score(int score);
	Score(int score, char * time);
	~Score();
	int& GetScore();
	char* GetTime();
	std::wstring Print(int pos);
private:
	int score;
	char stime[20];
};

bool operator<(Score score1, Score score2);
