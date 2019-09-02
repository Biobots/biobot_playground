#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
	ostringstream sstream;
	fstream file;
	file.open("../io/iotest.txt", ios::in);
	//file.seekg(0, file.end);
    //size_t srcsize = file.tellg();
	sstream << file.rdbuf();
	file.close();
	string str(sstream.str());
	const char* content = str.c_str();
	printf("%s\n", content);
	return 0;
}