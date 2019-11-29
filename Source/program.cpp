#include"Header.h"

int main()
{

	string c;
	cout << "Text-file path: ";	
	cin >> c;
	ifstream fp;
	fp.open(c, ios::in);
	if (fp.fail())
	{
		cout << "CANNOT OPEN FILE!";
		_getch();
		return 0;
	}
	string txt;
	int count = 0;
	char a;
	while (!fp.eof())
	{
		fp.get(a);
		txt.push_back(a);
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
		return 0;
	}
	string s = "";
	while(!readbin.eof())
	{
		bitset<8> set;
		readbin.read((char*)&set, sizeof(char));
		cout << set.to_string().length() << endl;
		s.append(set.to_string());
	}
	readbin.close();
	
	string decomp;
	cout << "Decompressed File Path: ";
	cin >> decomp;
	ofstream decompress(decomp);
	if (decompress.fail())
	{
		cout << "CANNOT OPEN FILE TO WRITE!";
		return 0;
	}

	int index = -1;
	cout << "\tDecompressing...";
	while (index < (int)s.size() - 2) {
		decode(r, s, decompress, index);
	}
	cout << "Done!";
	decompress.close();
	delete r;

	system("pause");
	return 0;
}