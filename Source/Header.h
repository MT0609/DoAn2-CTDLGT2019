#include<iostream>
#include<string>
#include<string.h>
#include<queue>
#include<unordered_map>
#include<fstream>
#include<bitset>
#include<conio.h>
using namespace std;


// A Tree node
struct Node
{
	char ch;
	int freq;
	Node *left, *right;
};

// Comparison object to be used to order the heap
struct comp
{
	bool operator()(Node* l, Node* r)
	{
		// highest priority item has lowest frequency
		return l->freq > r->freq;
	}
};	


// Function to allocate a new tree node
Node* getNode(char ch, int freq, Node* left, Node* right);

// traverse the Huffman Tree and store Huffman Codes
// in a map.
void encode(Node* root, string str, unordered_map<char, string> &huffmanCode);

// traverse the Huffman Tree and decode the encoded string
void decode(Node* root, string code, ofstream &decompress, int& index);

// Builds Huffman Tree and decode given input text
void compress(string text, priority_queue<Node*, vector<Node*>, comp> &pq, string &code, string &path);