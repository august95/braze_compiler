#pragma once
//stores location in a file

#include <string>

class filePosition
{
public:
	filePosition(std::string filename);
	void incrementLine();
	void incrementCol();
	void printFileLocation();
	std::string getLocationString();

private:	
	std::string m_filename;
	int m_line;
	int m_col;
};
