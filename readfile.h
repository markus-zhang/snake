//A specific file processor for Snake
//Example of lines:
//100 100
//110 100
//120 100
//Each line consists of two words. If the first one is not a number,
//we know that this line is for indication.
//If the first one is a number, simple push it into a line.

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;

struct line	{
public:
	string firstStr;
	string secondStr;
	int firstNum;
	int secondNum;
	line(): firstNum(0), secondNum(0)	{}
};

class lines	{
private:
	vector<line> lineList;
	int numElement;
	ifstream data;
	vector<string> words;
	string word;
public:
	//Constructor
	lines()	{
		numElement = 0;
		words.reserve(100);
	}
	//Utilities
	void lines_push(line newLine)	{
		lineList.push_back(newLine);
		numElement += 1;
	}
	vector<line> lines_getlist()	{
		return lineList;
	}
	int lines_getNum()	{
		return numElement;
	}
	bool lines_read(const string&);
	bool lines_debug();
	bool lines_debug2();
	void lines_reset()	{
		lineList.clear();
		words.clear();
		lineList.reserve(100);
		words.reserve(100);
	}
};

bool lines::lines_read(const string& fileName)	{
	data.open(fileName);
	if (!data.good())	{
		cout << "File not exist or corrupted!" << endl;
		return false;
	}

	while (data >> word)	{
		//cout << word << endl;
		words.push_back(word);
	}
	//lines_debug2();

	lineList.resize(words.size() / 2 + 1);
	//cout << words.size() / 2 + 1 << endl;
	int j = 0;	
	for (int i = 0; i <= words.size() - 2; i += 2)	{
		lineList[j].firstStr = words[i];
		lineList[j].secondStr = words[i + 1];
		istringstream(lineList[j].firstStr) >> lineList[j].firstNum;
		istringstream(lineList[j].secondStr) >> lineList[j].secondNum;
		j += 1;
		//cout << j << " ";
		numElement += 1;
	}
	return false;
}

bool lines::lines_debug()	{
	if (numElement <= 0)	{
		return false;
	}

	for (int i = 0; i <= numElement - 1; i ++)	{
		cout << lineList[i].firstNum << " " << lineList[i].secondNum << endl;
	}

	return true;
}

bool lines::lines_debug2()	{
	for (int i = 0; i <= words.size() - 1; i ++)	{
		cout << words[i] << " ";
	}
	return true;
}
