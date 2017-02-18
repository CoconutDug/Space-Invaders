#include "Scoreboard.h"
#include <algorithm>
#include <functional>

Scoreboard::Scoreboard(char* filename)
	:
	filename(filename)
{
	vScores = LoadScoresFromFile("hiscore.txt");
}

Scoreboard::~Scoreboard()
{}

void Scoreboard::AddScore(int inScore)
{
	vScores = LoadScoresFromFile("hiscore.txt");
	Score score(inScore);
	vScores.push_back(score);
}


std::vector<Score> Scoreboard::GetScoreboardVector()
{
	return vScores;
}

std::wstring Scoreboard::PrintScore(int i)
{
	return vScores[i].Print(i);
}

int Scoreboard::GetnScores()
{
	if (nScores >= 10)
	{
		nScores = 10;
	}
	return nScores;
}

void Scoreboard::SortVector()
{
	std::sort(vScores.begin(), vScores.end());
}

std::vector<Score> Scoreboard::LoadScoresFromFile(char * fileName) //Function that returns vector of scores from a given file
{
	std::vector<Score> scores; //Vector of scores to be returned by this function
	std::ifstream is(fileName, std::ios::in | std::ios::binary); //create input file stream
	if (is.is_open()) //if file is open
	{
		while (!is.eof()) //while not at end of file
		{
			
			int score; //int to store score value
			char stime[20]; //char buffer to store time data
			
			is.read((char*)&score, sizeof(int)); //Read int
			
			if (is.eof()) //If the end of file has been reached after reading score value (should not happen, but if it does break out of while loop)
				break;
			is.read(stime, sizeof(char) * 20); //read the time stamp stored as 20 byte char buffer

			Score newScoreEntry(score, stime); //Create new score entry to store in vector
			scores.push_back(newScoreEntry); //Add score to vector of scores
			std::sort(scores.begin(), scores.end());
			nScores++;
		}
	}
	is.close();//close file
	
	return scores;
}

bool Scoreboard::WriteScoresToFile(char * fileName, std::vector<Score> scores) //Function that returns true if successful / false if failure to write scores to file
{
	std::ofstream os(fileName, std::ios::binary | std::ios::out);
	if (os.is_open()) //If file was opened successfully for writing
	{
		for (int i = 0; i < (int)scores.size(); i++) //for each score in vector<Score>
		{
			os.write((char*)&scores[i].GetScore(), sizeof(int)); //Write score
			os.write(scores[i].GetTime(), sizeof(char) * 20); //Write time
		}
		os.close(); //Close score file after writing completed
		return true; //Return true, scores successfully written
	}
	else //If file was not successfuly opened for writing
	{
		return false; //Return false, failed to write successfuly
	}
}
