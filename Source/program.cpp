#include"Header.h"
#include"folder.h"

int main()
{
Menu:
	cout << "1 - File compression (.txt, .c, .cpp, .h)" << endl;
	cout << "2 - Folder compression (including files with .txt, .c, .cpp, .h)" << endl;
	cout << "Your option: ";
	int option;
	cin >> option;
	switch (option)
	{
	case 1:
		fileCompressExecute();
		break;
	case 2:
		folderCompressionExecute();
		break;
	default:
		goto Menu;
	}

	system("pause");
	return 0;
}