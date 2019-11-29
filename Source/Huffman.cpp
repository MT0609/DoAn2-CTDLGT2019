#include"Header.h"

// Function to allocate a new tree node
Node* getNode(char ch, int freq, Node* left, Node* right)
{
	Node* node = new Node();

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

// Builds Huffman Tree and decode given input text
void compress(string text,  priority_queue<Node*, vector<Node*>, comp> &pq, string &code, string &path)
{
	// count frequency of appearance of each character
	// and store it in a map
	unordered_map<char, int> freq;
	for (char ch : text) {
		freq[ch]++;
	}

	// Create a priority queue to store live nodes of
	// Huffman tree;

	// Create a leaf node for each character and add it
	// to the priority queue.
	for (auto pair : freq) {
		pq.push(getNode(pair.first, pair.second, NULL, NULL));
	}

	// do till there is more than one node in the queue
	while (pq.size() != 1)
	{
		// Remove the two nodes of highest priority
		// (lowest frequency) from the queue
		Node *left = pq.top(); pq.pop();
		Node *right = pq.top();	pq.pop();

		// Create a new internal node with these two nodes
		// as children and with frequency equal to the sum
		// of the two nodes' frequencies. Add the new node
		// to the priority queue.
		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}

	// root stores pointer to root of Huffman Tree
	Node* root = pq.top();

	// traverse the Huffman Tree and store Huffman Codes
	// in a map. Also prints them
	unordered_map<char, string> huffmanCode;
	encode(root, "", huffmanCode);

	cout << endl << "Encoded Table: " << endl;
	for (auto pair : huffmanCode) {
		cout << pair.first << " " << pair.second << '\n';
	}

	// print encoded string
	for (char ch : text) {
		code.append(huffmanCode[ch]);
	}

	cout << "Compressed file path: ";
	cin >> path;
	ofstream fp;
	fp.open(path, ios::out | ios::binary);
	int n = code.length();
	int times = n / 8;
	int j = 0;
	for (int i = 0; i < times; ++i)
	{
		char *s;
		int len = 8;
		s = new char[len];
		for (int k = 0; k < 8; ++k)
		{
			s[k] = code[j];
			++j;
		}
		s[len] = '\0';	

		bitset<8> set(s);
		const char toWrite = (unsigned char)((unsigned int)set.to_ulong());
		fp.write(&toWrite, sizeof(char));
	}

	if (n % 8 != 0)
	{
		char *s; int len = n % 8;
		s = new char[len];
		for (int k = 0; k < len; ++k)
		{
			//s = (char*)realloc(s, ++k * sizeof(char));
			s[k] = code[j];
			++j;
		}
		s[len] = '\0';

		bitset<8> set(s);
		fp.write((char*)&set, sizeof(char));
	}

	fp.close();
}

// traverse the Huffman Tree and decode the encoded string
void decode(Node* root, string code, ofstream &decompress, int& index)
{
	//static int index = -1;
	if (root == NULL) {
		return;
	}

	// found a leaf node
	if (root->left == NULL && root->right == NULL)
	{
		decompress << root->ch;
		return;
	}

	index++;

	if (code[index] == '0')
		decode(root->left, code, decompress, index);
	else if (code[index] == '1')
		decode(root->right, code, decompress, index);
}


// traverse the Huffman Tree and store Huffman Codes
// in a map.
void encode(Node* root, string str, unordered_map<char, string> &huffmanCode)
{
	if (root == nullptr)
		return;

	// found a leaf node
	if (root->left == NULL && root->right == NULL) {
		huffmanCode[root->ch] = str;
	}

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}

