#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>

using namespace std;

void readAllBytes(string fileName, vector<char> &result);
void writeToFile(string fileName, vector<int> &compressCode);
void compress(vector<char> &uncompressInfo, vector<int> &compressCode);
void decompress(vector<int> op, string fileName);

int main(char argc, char *argv[])
{
	argc = 4;
	argv[1] = (char*)"123.txt";
	argv[2] = (char*)"234.lzw";
	argv[3] = (char*)"decompress123.txt";
	if (argc != 4)
	{
		printf("Wrong number of arguments!!!");
		system("pause");
		return 0;
	}


	string fileInputName = argv[1];
	string fileOutputName = argv[2];
	string fileDecompressName = argv[3];

	vector<char> uncompressInformation;
	vector<int> compressCode;

	readAllBytes(fileInputName, uncompressInformation);
	compress(uncompressInformation, compressCode);
	writeToFile(fileOutputName, compressCode);
	decompress(compressCode, fileDecompressName);
	system("pause");
	return 0;
}

void writeToFile(string fileName, vector<int> &compressCode)
{
	ofstream file(fileName, ios::binary);
	if (file.is_open())
	{
		file.write((char*)&compressCode[0], compressCode.size() * sizeof(int));
	}
	else
	{
		cout << "Unable to open file" << endl;
	}
	file.close();

}

void readAllBytes(string fileName, vector<char> &result)
{
	ifstream file(fileName, ios::binary | ios::ate);
	if (file.is_open())
	{
		ifstream::pos_type pos = file.tellg();
		result.resize(pos);
		file.seekg(0, ios::beg);
		file.read(&result[0], pos);
	}
	else
	{
		cout << "Unable to open file" << endl;
	}
	file.close();
}

void compress(vector<char> &uncompressInfo, vector<int>& compressCode)
{
	map<string, int> table;
	for (int i = 0; i <= 255; i++) {
		string ch = "";
		ch += char(i);
		table[ch] = i;
	}

	string p = "", c = "";
	p += uncompressInfo[0];
	int code = 256;
	vector<int> output_code;
	for (int i = 0; i < uncompressInfo.size(); i++) 
	{
		if (i != uncompressInfo.size() - 1)
			c += uncompressInfo[i + 1];
		if (table.find(p + c) != table.end()) 
		{
			p = p + c;
		}
		else 
		{
			compressCode.push_back(table[p]);
			table[p + c] = code;
			code++;
			p = c;
		}
		c = "";
	}
	compressCode.push_back(table[p]);
	for (auto it : compressCode)
		cout << it << endl;
}

void  decompress(vector<int> compressCode, string fileName)
{
	ofstream file(fileName);
	map<int, string> table;
	for (int i = 0; i <= 255; i++) {
		string ch = "";
		ch += char(i);
		table[i] = ch;
	}
	int old = compressCode[0], n;
	string s = table[old];
	string c = "";
	c += s[0];
	file << s;
	int count = 256;
	for (int i = 0; i < compressCode.size() - 1; i++) 
	{
		n = compressCode[i + 1];
		if (table.find(n) == table.end()) 
		{
			s = table[old];
			s = s + c;
		}
		else 
		{
			s = table[n];
		}
		file << s;
		c = "";
		c += s[0];
		table[count] = table[old] + c;
		count++;
		old = n;
	}
	file.close();
}
