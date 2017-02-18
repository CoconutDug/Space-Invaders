#pragma once
#include "Score.h"

class Scoreboard
{
public:
	Scoreboard(char* filename);
	~Scoreboard();
	void AddScore(int score);
	std::vector<Score> GetScoreboardVector();
	std::wstring PrintScore(int i);
	int GetnScores();
	void SortVector();
	std::vector<Score> LoadScoresFromFile(char * fileName);
	bool WriteScoresToFile(char * fileName, std::vector<Score> scores);	
private:
	char* filename;
	std::vector<Score> vScores;
	int nScores = 0;
};



