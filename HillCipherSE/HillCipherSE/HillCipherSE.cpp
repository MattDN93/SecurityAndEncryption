#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#define HT 3
#define WDTH 3
using namespace std;

class HillCipher
{
private:
	vector<vector<double>> keyMatrix;	//a vector-of-a-vector creates a 2D array
	string text;						//text representation of the plaintext/ciphertext
	string alphabet;					//English alphabet
	vector<int> numerical;				//vector of integer representations of the letters
	vector<char> chartext;				//vector of individual encrypted chars
	vector<vector<int>> result; //{ { 0, 0, 0 },{ 0, 0, 0 },{0, 0, 0 } };		//multiplication result vector initialized
	float determinant;

public:
	void captureParagraph();			//actions for the process
	void captureMatrix();
	void doAction();

	void progLoad();					//menu processes
	void mainMenu();
	void menuSelection();
	void menuError();

	void storeMatrix(vector<vector<double>>);					//copies from user input to private variables
	void displayMatrix(vector<vector<double>>);
	vector<vector<double>> returnMatrix();		//returns private variables from class
	void displayCipherPlain(vector<vector<int>>);					//show result of en/decryption

	void convertToNumbers(string);						//converts plaintext chars to integers
	void setAlphabet(string);					//set alphabet


};

//--------------------------------MENU SYSTEM--------------------------//

void HillCipher::progLoad(void)
{
	system("cls");
	cout << "------------------------------\n" << endl;
	cout << "\tSecurity and Encryption ENEL4SE\n" << endl;
	cout << "\tHill Cipher Practical\n" << endl;
	cout << "--------------------------------\n" << endl;
	cout << "Matthew de Neef | 212503024\n" << endl;
	cout << "------------------------------\n" << endl;
	system("pause");
}


void HillCipher::mainMenu(void)
{
	system("cls");
	cout << "------------------------\n" << endl;
	cout << "\tMain Menu \n" << endl;
	cout << "------------------------\n" << endl;
	cout << "<Instruction:> Choose an option below." << endl;
	cout << "<Note:>\n Perform these in order to get correct results\n" << endl;
	cout << "\n\n   1) Capture paragraph to edit" << endl;
	cout << "\n   2) Enter matrix encryption key" << endl;
	cout << "\n   3) Encrypt/Decrypt" << endl;
	cout << "\n   4) Exit" << endl;

}

void HillCipher::menuSelection(void)
{
	int sel = 0;
	cout << "\n------------------------\n" << endl;
	cout << "Your selection:\n" << endl;
	cin >> sel;

	switch (sel)
	{
	case 1: system("cls"); cout << "Enter a paragraph to encrypt/decrypt: " << endl; captureParagraph(); break;
	case 2: system("cls"); cout << "Enter matrix encryption key. \nIf you wish to decrypt this matrix will automatically be inversed." << endl; captureMatrix(); break;
	case 3: system("cls"); cout << "Perform (E)ncryption or (D)ecryption? (E/D): " << endl; doAction(); /*displayCipherPlain();*/ break;
	case 4: cout << "Quitting..." << endl; system("pause"); exit(0);
	default: menuError(); break;
	}
	mainMenu(); menuSelection();
}

void HillCipher::menuError()
{
	system("cls");
	printf("\n\nERROR: No such option. Please try again.\n\n");
	fflush(stdin);
	system("pause");
	system("cls");
}


//--------------------------------COMPUTATIONAL ACTIONS--------------------------//

void HillCipher::doAction()									//Hill encryption action
{
	int temp = 0;
	int n = numerical.size() / 3;
	while (n>0){
		result[0][0] = 0;			//Initialise multiplication result to 0
		result[1][0] = 0;
		result[2][0] = 0;
		cout << "this loop is running" << endl;
		for (int i = 0; i<3; i++)
		{

			for (int cols = 0; cols < 3; cols++)
			{
				int inc = 0;														//performs matrix multiplication on the key with blocks
				inc = cols + temp;
				result[i][0] = result[i][0] + (keyMatrix[i][cols] * numerical[inc]);

			}
			cout << "ran for one block to give " << result[i][0] << endl;
			if (result[i][0] >= 26)
			{
				result[i][0] %= 26;														//get answer MOD 26 for conversion to 0-25 alphabet range
			}
		}n--;																			//one less run left
		temp += 3;																		//because this is a linear vector the pointer must jump to the next block

		for (int i = 0; i<HT; i++)
		{
			//displayCipherPlain(result);														//pass to display routine
			cout << result[i][0] << "-";
			cout << alphabet[result[i][0]] << "-" << endl;										//show the ASCII result of the computation 
		}
		system("pause");
	}


}

void HillCipher::storeMatrix(vector<vector<double>> store)
{
	keyMatrix = store;			//stores matrix to matrix variable
}

void HillCipher::setAlphabet(string alph)
{
	alphabet = alph;			//stores matrix to matrix variable
}

vector<vector<double>> HillCipher::returnMatrix()
{
	return keyMatrix;			//returns matrix from private variable
}

void HillCipher::captureMatrix()
{
	vector<vector<double>> store;
	double val;
	// Set up sizes. (HEIGHT x WIDTH)
	store.resize(HT);					//resize vector to match array dimensions
	for (int i = 0; i < HT; ++i)
		store[i].resize(WDTH);

	//Ask user for values by iterating
	for (int i = 0; i < HT; ++i)
	{
		for (int j = 0; j < WDTH; ++j)
		{
			cout << "Enter value at row [" << i + 1 << "] column [" << j + 1 << "] : ";
			cin >> val;
			store[i][j] = val;
		}
		storeMatrix(store);				//writes data to class member variable
	}
	displayMatrix(store);				//dsplays the matrix to the user
}

void HillCipher::displayMatrix(vector<vector<double>> dispMat)
{
	system("cls");
	cout << "The matrix currently stored is: " << endl;
		for (int i = 0; i < HT; ++i)
	{
	//determinant = determinant + (dispMat[0][i]*(dispMat[1][(i+1)%3]*dispMat[2][(i+2)%3] - dispMat[1][(i+2)%3]*dispMat[2][(i+1)%3]));
	}
	
	dispMat = returnMatrix();				//get the currently stored matrix
	for (int i = 0; i < HT; ++i)
	{
		cout << endl;
		for (int j = 0; j < WDTH; ++j)
		{
			cout << "| ";
			cout << dispMat[i][j];
			cout << "\t";
		}
		cout << "| ";
		cout << endl;
		cout << "---------------------------";
	}
	cout << endl;
	system("pause");
}

void HillCipher::captureParagraph()
{
	fflush(stdin);
	cout << "Enter the paragraph to encrypt: "; getline(cin, text);
	transform(text.begin(), text.end(), text.begin(), ::toupper);

	if (text.size() % 3 != 0)			//adds filler X 'n' times if char length not a multiple of 3 (3x3 matrix)
	{
		cout << "String not correct length. Adding fillers to allow for blocks of 3." << endl;
		int numfiller = 3 - (text.size() % 3);
		for (int i = 0; i < numfiller; i++)
		{
			text.append("X");
		}

	}

	int y = text.size();
	for (int i = 0; i < y; i += 3)
	{
		text.insert(i, " ");
		y = text.size();
		i++;

	}

	cout << "New modified text: " << text << endl;
	system("pause");
	convertToNumbers(text);
	cout << "New modified text with numberical values: ";

	for (int i = 0; i < numerical.size(); i++)
	{
		cout << numerical[i] << "-";

	}
	cout << endl;
	system("pause");

}

void HillCipher::convertToNumbers(string input)
{
	for (int i = 0; i < input.size(); i++)
	{
		for (int j = 0; j < alphabet.size(); j++)
		{
			if (input[i] == alphabet[j])
			{
				int newnum;
				newnum = j;
				numerical.push_back(newnum);				//matches the letters in plaintext string to the alphabet & substitutes them for a number eg. A = 0; B = 1 etc.
			}
		}

	}
}

void HillCipher::displayCipherPlain(vector<vector<int>> result_vector)
{
//	system("cls");
//	cout << "The matrix determinant currently stored is: " << endl;
//	
//	//dispMat = returnMatrix();				//get the currently stored matrix
//	for (int i = 0; i < HT; ++i)
//	{
//		cout << endl;
//		for (int j = 0; j < WDTH; ++j)
//		{
//			cout << "| ";
//			cout << dispMat[i][j];
//			cout << "\t";
//		}
//		cout << "| ";
//		cout << endl;
//		cout << "---------------------------";
//	}
//	cout << endl;
//	system("pause");
}

int main()
{

	HillCipher newHill;										//instantiate class object
	newHill.setAlphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	newHill.progLoad();										//load the user menus
	newHill.mainMenu();
	newHill.menuSelection();
	return 0;

}
