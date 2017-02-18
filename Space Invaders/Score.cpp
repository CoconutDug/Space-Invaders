#include "Score.h"

Score::Score(int score)
	:
	score(score)
{
	time_t curTime = time(NULL); //get current time
	strftime(stime, 20, "%X/%x", localtime(&curTime)); //Get time and store it in "char stime[20]"
}
Score::Score(int score, char * time)
	:
	score(score)
{
	memcpy(stime, time, sizeof(char) * 20); //Copy time argument to stime buffer using memcpy
}
Score::~Score()
{

}
int& Score::GetScore()
{
	return score;
}
char* Score::GetTime()
{
	return stime;
}
std::wstring Score::Print(int pos)
{
	std::wstringstream ss;
	if (pos < 9)
	{
		ss << pos + 1 << "		" << score << "			" << stime;
	}
	else if (pos == 9)
	{
		ss << pos + 1 << "		" << score << "			" << stime;
	}
	return ss.str();
}

bool operator<(Score score1, Score score2)
{
	return (score1.GetScore() > score2.GetScore());
}