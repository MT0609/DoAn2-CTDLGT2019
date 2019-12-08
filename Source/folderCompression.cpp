#include "folder.h"

void getDir(const char* d, vector<string> & f)
{
	FILE* pipe = NULL;
	string pCmd = "dir /B /S " + string(d);
	char buf[256];

	if (NULL == (pipe = _popen(pCmd.c_str(), "rt")))
	{
		cout << "..." << endl;
		return;
	}

	while (!feof(pipe))
	{
		if (fgets(buf, 256, pipe) != NULL)
		{
			f.push_back(string(buf));
		}

	}

	_pclose(pipe);
}

string get_fileName(string path)
{
	string file = "";
	int i = path.length() - 1;
	while ((int)path[i] != 92)
		--i;
	for (int idx = i + 1; idx < path.length(); ++idx)
		file.push_back(path[idx]);
	return file;
}

node* getNode(char ch, int freq, node* left, node* right)
{
	node* Node = new node();

	Node->ch = ch;
	Node->freq = freq;
	Node->left = left;
	Node->right = right;

	return Node;
}

void encode(node* root, string str, unordered_map<char, string> &huffmanCode)
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

void compress(string text, priority_queue<node*, vector<node*>, compare> &pq, string &code, string &out_bin_path)
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
		node *left = pq.top(); pq.pop();
		node *right = pq.top();	pq.pop();
		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
		// thực hiện đến khi hàng đợi chỉ còn 1 node chính (không tính con)
	}

	// root stores pointer to root of Huffman Tree
	node* root = pq.top();

	// duyệt cây huffman và tạo bảng kí tự -> mã huffman
	unordered_map<char, string> huffmanCode;
	encode(root, "", huffmanCode);

	for (int i = 0; i < text_len; ++i)
		code += huffmanCode[text[i]];			

	ofstream fp;
	fp.open(out_bin_path, ios::binary | ios::app);
	int n = code.length();
	int times = n / 8;
	int j = 0;

	fp.write((char*)&n, sizeof(int));
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

void decode(node* root, string code, ofstream & out)
{
	node* pos = root;
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

void folder_compress(string folder_name, string out_bin_path, vector<node*> &root, vector<priority_queue<node*, vector<node*>, compare>> pq)
{
	vector<string> filenames;

	getDir((char*)folder_name.c_str(), filenames);
	pq.resize(filenames.size());
	root.resize(filenames.size());
	
	ifstream read_txt;
	for (int i = 0; i < filenames.size(); ++i)
	{
		filenames[i].resize(filenames[i].size() - 1);
		read_txt.open(filenames[i]);
		string txt;
		char a;
		while (!read_txt.eof())
		{
			read_txt.get(a);
			if (a != 13)
				txt.push_back(a);
		};
		
		read_txt.close();
		string code = "";
		compress(txt, pq[i], code, out_bin_path);		// create huffman code and compress file; save priority queue (pq), code, filepath for later use
		root[i] = pq[i].top();
	}
}

void folderCompressionExecute()
{
	// NÉN (THƯ MỤC)
	string out_bin_path;
	cout << "Enter compressed file path: ";				// GHI TÊN FILE NÉN
	cin >> out_bin_path;

	string folder_path;
	cout << "Enter folder path (D:\\folder\\): ";					// THƯ MỤC CẦN NÉN
	cin >> folder_path;
	vector<string> files;
	getDir((char*)folder_path.c_str(), files);

	vector<priority_queue<node*, vector<node*>, compare>> pq;
	vector<node*> root;
	folder_compress(folder_path, out_bin_path, root, pq);

	ifstream readbin(out_bin_path, ios::binary);
	if (readbin.fail())
	{
		cout << "CANNOT OPEN COMPRESSED FILE!";
		_getch();
		return;
	}

	//	GIẢI NÉN
	string out_path;
	cout << "Enter out-folder path (D:\\folder\\): ";
	cin >> out_path;
	string copy(out_path);
	_mkdir((char*)out_path.c_str());

				// ĐỌC FILE NÉN CÓ CHỨA CÁC FILE CŨ ĐÃ NÉN (CHUNG 1 FILE BIN)
	int k = 0;
	while(!readbin.eof())
	{	
		int code_length(0);
		readbin.read(reinterpret_cast<char*>(&code_length), sizeof(int));
		string s = "";
		bitset<8> set;
		for (int i = 0; i < code_length / 8; ++i)
		{
			readbin.read((char*)&set, sizeof(char));
			s.append(set.to_string());
		}
		if (code_length % 8 != 0)
		{
			readbin.read((char*)&set, sizeof(char));
			s.append(set.to_string());
		}
		if (s.length() == 0) break;
		s.erase(s.length() - 8, 8);
		int i = s.length() - 8;
		while (s[i] == '0')
		{
			s.erase(i, 1);
		}
		
		string outfile = "";
		outfile = out_path + get_fileName(files[k]);
		outfile.resize(outfile.size() - 1);
		ofstream decompress;
		decompress.open(outfile);
		cout << "\tDecompressing file " << k + 1 << "..." << endl;
		decode(root[k], s, decompress);
		++k;
	}
}