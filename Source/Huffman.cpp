#include"Header.h"


Node* getNode(char ch, int freq, Node* left, Node* right)
{
	Node* node = new Node;

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}


// Xây dựng cây huffman sử dụng hàng đợi ưu tiên
void compress(string text,  priority_queue<Node*, vector<Node*>, comp> &pq, string &code, string &path)
{
	// đếm tần số xuất hiện của các kí tự trong file text
	map<char, int> freq;
	int text_len = text.length();
	for (int i = 0; i < text_len; ++i) {
		freq[text[i]]++;
	}

	// tạo hàng đợi ưu tiên cho các nút chứa kí tự và tần số
	for (auto i = freq.begin(); i != freq.end(); ++i)
		pq.push(getNode(i->first, i->second, NULL, NULL));

	while (pq.size() != 1)
	{
		// Loại 2 node có độ ưu tiên cao nhất (tần số bé nhất) khỏi hàng đợi và add node mới có 2 con là 2 node vừa loại
		Node *left = pq.top(); pq.pop();
		Node *right = pq.top();	pq.pop();
		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
		// thực hiện đến khi hàng đợi chỉ còn 1 node chính (không tính con)
	}
	Node* root = pq.top();

	// duyệt cây huffman và tạo bảng kí tự -> mã huffman
	map<char, string> huffmanCode;
	encode(root, "", huffmanCode);
	for (int i = 0; i < text_len; ++i)
		code += huffmanCode[text[i]];	

	cout << "Compressed file path: ";
	cin >> path;
	ofstream fp;
	fp.open(path, ios::out | ios::binary);
	int n = code.length();
	int times = n / 8;
	int j = 0;
	for (int i = 0; i < times; ++i)
	{
		string s;
		for (int k = 0; k < 8; ++k)
		{
			s += code[j];
			++j;
		}

		bitset<8> set(s);
		fp.write((char*)&set, sizeof(char));
	}

	if (n % 8 != 0)
	{
		string s;
		for (int k = 0; k < n % 8; ++k)
		{
			s += code[j];
			++j;
		}
		bitset<8> set(s);
		fp.write((char*)&set, sizeof(char));
	}

	fp.close();
}

void decode(Node* root, string code, ofstream & out)
{
	Node* pos = root;
	for (int i = 0; i < code.length(); ++i)
	{
		if (code[i] == '1')
			pos = pos->right;
		else 
			pos = pos->left;
		if (pos->left == NULL && pos->right == NULL)
		{
			out.write((char*)&pos->ch, 1);
			pos = root;
		}
	}
	delete pos;
}

void encode(Node* root, string str, map<char, string> &huffmanCode)
{
	if (root == NULL)
		return;

	if (root->left == NULL && root->right == NULL) {
		huffmanCode[root->ch] = str;
	}

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}

void fileCompressExecute()
{
	string c;
	cout << "Text-file path: ";
	cin >> c;
	ifstream fp;
	fp.open(c);
	if (fp.fail())
	{
		cout << "CANNOT OPEN FILE!";
		_getch();
		return;
	}
	string txt;
	int count = 0;
	char ch;
	while (!fp.eof())
	{
		fp.get(ch);
		if (ch != 13)
			txt += ch;
	};
	fp.close();

	priority_queue<Node*, vector<Node*>, comp> pq;
	string path;
	string code = "";
	compress(txt, pq, code, path);		// create huffman code and compress file; save priority queue (pq), code, filepath for later use
	Node* r = pq.top();

	ifstream readbin(path, ios::binary);
	if (readbin.fail())
	{
		cout << "CANNOT OPEN COMPRESSED FILE!";
		_getch();
		return;
	}

	string s = "";
	while (!readbin.eof())
	{
		bitset<8> set;
		readbin.read((char*)&set, sizeof(char));
		s.append(set.to_string());
	}

	s.erase(s.length() - 8, 8);
	int i = s.length() - 8;
	while (s[i] == '0')
	{
		s.erase(i, 1);
	}
	readbin.close();

	string decompr;
	cout << "Decompressed File Path: ";
	cin >> decompr;
	ofstream decompress(decompr);
	if (decompress.fail())
	{
		cout << endl << "\tCANNOT OPEN FILE TO WRITE!";
		_getch();
		return;
	}
	decode(r, s, decompress);

	cout << "Done!";
	decompress.close();
}