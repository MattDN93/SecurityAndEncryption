/*A program to implement Encryption/decryption using Feistel Method
Matthew de Neef		| 	212503024
ENEL4SE Security & Encryption

NOTE: Compiled using VS2013, don't use Dev C++  to compile								*/

#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <random>

using namespace std;

class Feistel
{
public:
	void progLoad();				//Menu: Load program
	void showMenu();				//Menu: show
	void menuSelection();			//Menu: make a selection
	void menuError();				//Menu: error in choice

	void queryRounds();				//Ask user how many rounds to do
	void enterPlaintext();			//Ask user ro enter plaintext
	void enterKey();				//generate Keys in keyschedule

	void Encrypt();					//Run the encryption
	void Decrypt();					//Run the encryption
	void FeistelCipher(vector<vector<int>>);	//Fesitel Cipher progress

	void asciiToBinary(int);		//convert ASCII integer to Binary
	void binaryToAscii();		//convert binary to ASCII integer

	void populateKeys();			//populate with random numbers


private:
	int N;							//# Feistel rounds to perform
	int length=0;					//length is  of 1/2 text input!!
	string plaintext;				//plaintext input
	vector<vector<int>> Left;		//Left half-block [N+1][length * 8bits]
	vector<vector<int>> Right;		//right half-block [N+1][length * 8bits]
	vector<vector<int>> Key;		//the key of size [N+1][length * 8bits]
	vector <char> temp_key;			//temporary vector to hold partial keys
	int bin[8];						//8-bit binary rep from ASCII
	char *btoa_ptr;						//represent ASCII from binary
	vector <int> btoa;				//temp array to store ASCII equivalents
	char temp_ascii[50];
};


//--------------------------------MENU SYSTEM--------------------------//

void Feistel::progLoad(void)
{
	system("cls");
	cout << "------------------------------\n" << endl;
	cout << "\tSecurity and Encryption ENEL4SE\n" << endl;
	cout << "\tFeistel Cipher\n" << endl;
	cout << "--------------------------------\n" << endl;
	cout << "Matthew de Neef | 212503024\n" << endl;
	cout << "------------------------------\n" << endl;
	system("pause");
}


void Feistel::showMenu(void)
{
	system("cls");
	cout << "------------------------\n" << endl;
	cout << "\tMain Menu \n" << endl;
	cout << "------------------------\n" << endl;
	cout << "<Instruction:> Choose an option below." << endl;
	cout << "<Note:>\n Perform these in order to get correct results\n" << endl;
	cout << "\n\n   1) Enter number of rounds" << endl;
	cout << "\n   2) Enter text" << endl;
	cout << "\n   3) Enter Key" << endl;
	cout << "\n   4) Encrypt" << endl;
	cout << "\n   5) Decrypt" << endl;
	cout << "\n   6) Exit" << endl;

}

void Feistel::menuSelection(void)
{
	int sel = 0;
	cout << "\n------------------------\n" << endl;
	cout << "Your selection:\n" << endl;
	cin >> sel;

	switch (sel)
	{
	case 1: system("cls"); queryRounds(); break;
	case 2: system("cls"); enterPlaintext(); break;
	case 3: system("cls"); enterKey(); break;
	case 4: system("cls"); Encrypt(); break;
	case 5: system("cls"); cout << "Disclaimer: Using the indicated method in class to decrypt a Feistel encrypted code does not work. This code follows the method given but WILL NOT produce correct results. Press enter to agree..." << endl; system("pause"); Decrypt(); break;
	case 6: cout << "Quitting..." << endl; system("pause"); exit(0);
	default: menuError(); break;
	}
	showMenu(); menuSelection();
}

void Feistel::menuError()
{
	system("cls");
	cout << "\n\nERROR: No such option. Please try again.\n\n";
	fflush(stdin);
	system("pause");
	system("cls");
}

void Feistel::binaryToAscii()
{
	btoa.resize(plaintext.size());		//resize it for storage
	int incr=0,k=7;							//allows to access the next 8 bits in word

	//Binary bits are weighted according to their position and decoded as follows:
	/*
			1		0		0		1		0		1		1		1
			2^7 +  0^6	+  0^5	+	2^4	+	0^3	+	2^2	+	2^1	+	2^0 = 151
			2^k where k=7
			Decrement k each time and make the sum
			use MODULO 2^(k%8) so viz. 2^18 = 2^2 for weighting blocks beyond the first one
	*/

	for (int i = 0; i < length; i++)
	{
		for (int j = 0 + incr; j < 8 + incr; j++)
		{
			btoa[i] = btoa[i] + Left[N][j] * pow(2, k % 8);
			btoa[i + length] = btoa[i + length] + Right[N][j] * pow(2, k % 8);
			k--;
		}
		incr += 8;							//if one 'word' of 8-bits is done, skip to the next one
	}

}

void Feistel::asciiToBinary(int ascii_val)
{
	//_itoa_s(ascii_val, temp_ascii, 8, 2);
	
	int b = 8;
	while (ascii_val != 0)
	{
		bin[--b] = ascii_val % 2;		//Continually Divide by 2 and Record Remainder
		ascii_val /= 2;
	}
	
	for (int i = 0; i < 8; i++)			//display to user
	{
		cout << bin[i] << " ";
	}
	
}

void Feistel::queryRounds(){		//get rounds to perform
	cout << "Please enter the number of rounds to perform: ";
	cin >> N;
}

void Feistel::enterPlaintext(){
	fflush(stdin);
	cout << "Please enter the plaintext to encrypt: "; getline(cin, plaintext);
	transform(plaintext.begin(), plaintext.end(), plaintext.begin(), ::toupper);

	if (plaintext.size() % 2 != 0)		//force even-valued string length
	{
		cout << "Error, odd numbered plaintext, added filler chars\n";
		plaintext.append("X");
	}
	cout << "New text string: " << plaintext << endl;

	length = plaintext.size() / 2;		//block length = half plaintext string length
	
	Left.resize(length);		//set vector sizes, both dimensions; of left half
	Right.resize(length);		//set vector sizes, both dimensions; of right half

	for (int i = 0; i < length; i++)	//vector size Left[rows 0 to 1/2 plaintext length]
										//vector size left[columns 8 * 1/2 plaintext length]
	{
		Left[i].resize(8*length);
		Right[i].resize(8*length);
	}


	int ascii,k=0;						//temp ASCII holding variable
	cout << "Left: " << endl;
	
	for (int i = 0; i < length; i++)	//LEFT HALF OF STRING
	{
		cout << "Char #" << i << ": ";
		ascii = plaintext[i];
		asciiToBinary(ascii);			//pass to be converted to ASCII -> Binary

		for (int b = 0; b < 8; b++)		//populate left half of codeword
		{
			Left[0][k] = bin[b];
			k++;
		}
		cout << endl;
	}

	k = 0;
	cout << "Right: " << endl;
	for (int i = length; i < plaintext.size(); i++)	//RIGHT HALF OF STRING
	{
		cout << "Char #" << i << ": ";
		ascii = plaintext[i];
		asciiToBinary(ascii);			//pass to be converted to ASCII -> Binary

		for (int b = 0; b < 8; b++)		//populate left half of codeword
		{
			Right[0][k] = bin[b];
			k++;
		}
		cout << endl;
	}
	system("pause");
}

void Feistel::enterKey(void)			//Capture Key Schedule 
{
	srand(time(NULL));					//seed random number generator
	Key.resize(N);					//set vector sizes, both dimensions; of key schedule, 8xn where n is block size
	for (int i = 0; i < N; i++)
	{
		Key[i].resize(8*length);
	}

	temp_key.resize(length);			//set temporary vector key size
	int ascii;

	system("cls");
	cout << "Generating key schedule of size " << length << " randomly!" << endl;

	for (int rnd = 0; rnd < N; rnd++)			//for required user-defined number of rounds
	{
		int incr = 0;
		cout << "\nROUND KEY " << rnd + 1 << endl;

		for (int j = 0; j < length; j++)	//for the length of a block
		{
			int random_ascii = rand() % 128;	//extent of the standard ASCII table
			temp_key[j] = (char)random_ascii;	//store ASCII char equivalent into temporary key vector
			cout << "\t" << random_ascii << "  " << (char) random_ascii <<" ";				//print this key
		}

		cout << endl;
		for (int i = 0; i < length; i++)
		{
			ascii = (int)temp_key[i];			//Get ascii value of character
			asciiToBinary(ascii);				//Convert to binary
			for (int bit = 0; bit < 8; bit++)
			{									//Save bits in KEY vector
				Key[rnd][incr] = bin[bit];
				incr++;
			}
			cout << " | ";
		}

	}

	cout << "\nSaved into key matrix ready for encryption." << endl;
	cout << "IMPORTANT: ASCII chars 0-32 are non-printable and may not be displayed in the output" << endl;
	cout << "Non-text key chars should be noted for decryption at the recipient end." << endl;
	system("pause");
}

void Feistel::Encrypt()							//runs the encryption mechanism for N rounds
{
	FeistelCipher(Key);
}

void Feistel::Decrypt()							//runs the encryption mechanism for N rounds
{
	vector<vector<int>> De_Key;
	De_Key.resize(N);					//set vector sizes, both dimensions; of key schedule, 8xn where n is block size
	for (int i = 0; i < N; i++)
	{
		De_Key[i].resize(8 * length);
	}
	
	int incr = N;
	for (int rnd = 0; rnd < N; rnd++)		//simply store the values from the key in reverse order into a new 2D vector and pass this...
	{
		incr--;
		for (int j = 0; j < 8 * length; j++)
		{
			De_Key[rnd][j] = Key[incr][j];
		}
		
	}

	FeistelCipher(De_Key);					//...to the same Feistel Cipher
}

void Feistel::FeistelCipher(vector<vector<int>> FKey)			//Capture Key Schedule 
{
	//FKey.resize(N);					//set vector sizes, both dimensions; of key schedule, 8xn where n is block size


	cout << "ROUND 0" << endl;
	cout << "\nL0 : \t" << endl;
	for (int i = 0; i < 8 * length; i++)	//L0 printed out; initial so nothing changed
		{
			cout << Left[0][i];
		}
		cout << " | ";


	cout << "\nR0 : \t" << endl;

		for (int i = 0; i < 8* length; i++)	//R0 printed out; initial so nothing changed
		{
			cout << Right[0][i];
		}
		cout << " | ";

	//We must enlarge the vectors ass new rounds are completed
	Left.resize(N+1);		//set vector sizes, both dimensions; of left half
	Right.resize(N+1);		//set vector sizes, both dimensions; of right half

	for (int i = 1; i < N+1; i++)		//create bigger vector for upcoming encryption rounds
	{
		Left[i].resize(8*length);
		Right[i].resize(8 * length);
	}


	for (int rnd = 0; rnd < N; rnd++)		//for N rounds specified
	{
		
		for (int b = 0; b < 8*length; b++)
		{
			Left[rnd + 1][b] = Right[rnd][b];		//L1=R0 (or Ln = Rn-1)
			Right[rnd + 1][b] = Left[rnd][b] ^ Key[rnd][b];		//R1 = L0 XOR Key(n) -> Rn = Ln-1 XOR Key(n)
		}

		cout << "\n\nROUND "<< rnd + 1 <<":";		//Display rnd Keys and Ln and Rn (in binary)
		cout << "\nL" << rnd + 1 << ":\t";
		
			for (int i = 0; i < 8*length ; i++)	//Ln printed out; follows Feistel formula
			{
				cout << Left[rnd+1][i];
			}
			cout << " | " << " = R" << rnd;

		cout << "\nR" << rnd + 1 << ":\t";

			for (int i = 0; i < 8*length; i++)	//Rn printed out; follows Feistel formula
			{
				cout << Right[rnd+1][i];
			}
			cout << " | "<< " = L" << rnd << " XOR Key" <<rnd;


		cout << "\nKEY" << rnd << " :\t";		//Key printed out; follows Feistel formula, used by THAT round for finding the Rn above it

			for (int i = 0; i < 8*length; i++)
			{
				cout << Key[rnd][i];
			}
			cout << " | ";


	}

	cout << "\n-------------------------------"<<endl;
	cout << "\n\nBinary final concatenated L"<<N<<" R"<<N<<endl;		//Show the concatenated binary LN/RN after N rounds
	for (int i = 0; i<length * 8; i++)
	{
		cout << Left[N][i];
	}
	cout << "  ";
	
	for (int i = 0; i<length * 8; i++)
	{
		cout << Right[N][i];
	}

	binaryToAscii();							//Convert Binary LN RN to ASCII characters
	
	cout << "\n-------------------------------" << endl;
	cout << "\n\nThe ASCII numerical value is:  ";					//Display ASCII Integer values
	for (int i = 0; i<2 * length; i++)
	{
		cout << "   "<< btoa[i];
	}
	cout << "\n-------------------------------" << endl;
	cout << "\n\nCharacters:  ";									//Display ASCII characters
	for (int i = 0; i<2 * length; i++)
	{
		cout << "   "<< (char)btoa[i];
	}
	cout << "\n-------------------------------" << endl;
	system("pause");
}


int main()
{
	Feistel newFC;
	newFC.progLoad();
	newFC.showMenu();
	newFC.menuSelection();
}

