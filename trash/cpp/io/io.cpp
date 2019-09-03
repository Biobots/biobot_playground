#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
//-------------- A -------------------
	ostringstream sstream;
	fstream file;
	file.open("../io/iotest.txt", ios::in);
	//file.seekg(0, file.end);
    //size_t srcsize = file.tellg();
	sstream << file.rdbuf();
	file.close();
	string str(sstream.str());
	const char* content = str.c_str();
	printf("A: %s\n", content);

//-------------- B -------------------
    ifstream file1;
    file1.open("../io/iotest.txt", ios::ate);
    streampos size = file1.tellg();
    char* mem = new char[size];
    file1.seekg(0, ios::beg);
    file1.read(mem, size);
    file1.close();
    printf("B: %s\n", content);
    delete [] mem;

	return 0;
}