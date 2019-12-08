#pragma once
#include <vector>
#include<direct.h>
#include<iostream>
#include<string>
#include<queue>
#include<unordered_map>
#include<map>
#include<fstream>
#include<bitset>
#include<conio.h>
using namespace std;

struct node
{
	char ch;
	int freq;
	node *left, *right;
};

struct compare
{
	bool operator()(node* l, node* r)
	{
		return l->freq > r->freq;
	}
};


node* getNode(char ch, int freq, node* left, node* right); // tạo 1 node mới có chứa các thông tin: kí tự, tần số, nút con trái-phải

void encode(node* root, string str, unordered_map<char, string> &huffmanCode);	// mã hóa các kí tự xuất hiện trong file thành mã huffman

void compress(string text, priority_queue<node*, vector<node*>, compare> &pq, string &code, string &out_bin_path);

void decode(node* root, string code, ofstream & out);			// giải mã mã huffman thành các kí tự bằng cách duyệt cây

// Hàm nén thư mục ghi ra file nhị phân
void folder_compress(string folder_name, string out_bin_path, vector<node*> &root, vector<priority_queue<node*, vector<node*>, compare>> pq);

// Hàm thực thi chương trình
void folderCompressionExecute();
