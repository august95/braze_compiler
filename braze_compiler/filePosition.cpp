#include "filePosition.h"
#include <iostream>

filePosition::filePosition()
	:m_line(1),
	m_col(1),
	m_filename()
{

}

void filePosition::setFileName(std::string filename)
{
	m_filename = filename;
}

void filePosition::incrementLine()
{
	m_line++;
	m_col = 1;
}

void filePosition::incrementCol()
{
	m_col++;
}

void filePosition::printFileLocation()
{
	std::cout << "file: " << m_filename << " line: " << m_line << " col: " << m_col << " ";
}

std::string filePosition::getLocationString()
{
	std::string file_position = "file: ";
	file_position += m_filename;
	file_position += " line: ";
	file_position += m_line;
	file_position += " col: ";
	file_position += m_col;
	file_position += " ";
	return file_position;
}
