#include<iostream>
#include<string>
#include<queue>
#include<unordered_map>
#include<map>
#include<fstream>
#include<bitset>
#include<conio.h>
using namespace std;
#pragma once

struct Node
{
	char ch;
	int freq;
	Node *left, *right;
};

struct comp
{
	bool operator()(Node* l, Node* r)
	{
		return l->freq > r->freq;
	}
};	


Node* getNode(char ch, int freq, Node* left, Node* right);			// tạo 1 node mới có chứa các thông tin: kí tự, tần số, nút con trái-phải

void encode(Node* root, string str, map<char, string> &huffmanCode);		// mã hóa các kí tự xuất hiện trong file thành mã huffman

void decode(Node* root, string code, ofstream & out);			// giải mã mã huffman thành các kí tự bằng cách duyệt cây

void compress(string text, priority_queue<Node*, vector<Node*>, comp> &pq, string &code, string &path);		// Hàm nén tệp ghi ra file nhị phân

void fileCompressExecute();		// Hàm thực thi chương trình