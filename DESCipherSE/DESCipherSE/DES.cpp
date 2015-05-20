/*A program to implement Encryption/decryption using DES Method
			Matthew de Neef		| 	212503024
			   ENEL4SE Security & Encryption

NOTE: Compiled using VS2013, don't use Dev C++  to compile	*/
#include <iostream>
#include <array>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
using namespace std;

class DES_Cipher	
{
public:
	DES_Cipher();

	~DES_Cipher();
	/*Menu Prototypes*/
	void progLoad();				//Menu: Load program
	void showMenu();				//Menu: show
	void menuSelection();			//Menu: make a selection
	void menuError();				//Menu: error in choice

	/*DES Key Generation Actions*/
	void keyGeneration();			//generates the round keys for use in DES
	void asciiToBin(int);			//converts ASCII int value to binary
	/*DES Plaintext Modifying Actions*/
	void enterPlaintext();			//receives user plaintext to encrypt
	/*Process Actions*/

	/*DES Key Permuted Choice Matrices*/
	int PC1[56] = { 57, 49, 41, 33, 25, 17, 9,
				  1, 58, 50, 42, 34, 26, 18, 
				  10, 2, 59, 51, 43, 35, 27, 
				  19, 11, 3, 60, 52, 44, 36, 
				  63, 55, 47, 39, 31, 23, 15, 
				  7, 62, 54, 46, 38, 30, 22, 
				  14, 6, 61, 53, 45, 37, 29, 
				  21, 13, 5, 28, 20, 12, 4 };

	int PC2[56] = { 14, 17, 11, 24, 1, 5, 3, 28,
				  15, 6, 21, 10, 23, 19, 12, 4,
				  26, 8, 16, 7, 27, 20, 13, 2,
				  41, 54, 31, 37, 47, 55, 30, 40,
				  51, 45, 33, 48, 44, 49, 39, 56, 
				  34, 53, 46, 42, 50, 36, 29, 32 };
	/*--------------------------------*/
	/*DES Key Generation Matrices*/
	int C_n[28];	//left and right-half matrices of key generation respectively
	int D_n[28];
	int C_n_LS[28];	//Cn and Dn SHIFTED counterpart matrices (for storing temp shifted value)
	int D_n_LS[28];
	
	int user_key_int[8];	//user entered key if integers chosen
	char user_key_char[8];	//user entered key if characters chosen

	int init_key[17][56];	//initial key before permutation
	int key_64bit[64];		//64-bit key matrix
	int final_key[16][48];	//Round Keys after permutation and shifting
	/*--------------------------------*/
	/*Service Variables*/
	int bin[8];				//binary representation of ASCII char
	/*--------------------------------*/
	/*DES Plaintext Input Matrices*/
	int IP[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
		60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6,
		64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17, 9, 1,
		59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5,
		63, 55, 47, 39, 31, 23, 15, 7 };

	int init_P[100][64];			//Initial text stored in 8 char blocks (max 100 letters input)
	int final_P[100][64];			//P after permutation with the IP

	vector<int> user_text_int;		//user plaintext if choosing to enter integers
	string user_text_char;	//user plaintext if choosing to enter chars

};

DES_Cipher::~DES_Cipher()
{
}

void DES_Cipher::asciiToBin(int quot){		//Convert integer value of ASCII char to binary
	int i = 8;
	while (quot != 0){
		bin[--i] = quot % 2;		//Continually Divide by 2 and Record Remainder
		quot = quot / 2;
	}
}

/*------------------------------Menu Options-----------------------------*/
void DES_Cipher::progLoad(void)
{
	system("cls");
	cout << "------------------------------\n" << endl;
	cout << "\tSecurity and Encryption ENEL4SE\n" << endl;
	cout << "\tDES Cipher\n" << endl;
	cout << "--------------------------------\n" << endl;
	cout << "Matthew de Neef | 212503024\n" << endl;
	cout << "------------------------------\n" << endl;
	system("pause");
}


void DES_Cipher::showMenu(void)
{
	system("cls");
	cout << "------------------------\n" << endl;
	cout << "\tMain Menu \n" << endl;
	cout << "------------------------\n" << endl;
	cout << "<Instruction:> Choose an option below." << endl;
	cout << "<Note:>\n Perform these in order to get correct results\n" << endl;
	cout << "\n   1) Enter text" << endl;
	cout << "\n   2) Enter Key" << endl;
	cout << "\n   3) Encrypt" << endl;
	cout << "\n   4) Decrypt" << endl;
	cout << "\n   5) Exit" << endl;

}

void DES_Cipher::menuSelection(void)
{
	int sel = 0;
	cout << "\n------------------------\n" << endl;
	cout << "Your selection:\n" << endl;
	cin >> sel;

	switch (sel)
	{
	case 1: system("cls"); enterPlaintext(); break;
	case 2: system("cls"); keyGeneration(); break;
	case 3: system("cls"); /*Encrypt()*/; break;
	case 4: system("cls"); /*Decrypt()*/; break;
	case 5: cout << "Quitting..." << endl; system("pause"); exit(0);
	default: menuError(); break;
	}
	showMenu(); menuSelection();
}

void DES_Cipher::menuError()
{
	system("cls");
	cout << "\n\nERROR: No such option. Please try again.\n\n";
	fflush(stdin);
	system("pause");
	system("cls");
}

/*------------------------------Plaintext Entry & Permutes-----------------*/

void DES_Cipher::enterPlaintext()
{
	int choice;
	system("cls");
	cout << "How do you wish to enter the plaintext?" << endl;
	cout << "1. Characters\t2.Integers" << endl;
	cout << "Choice: ";
	cin >> choice;

	if (choice == 1){
		cout << "\n Please enter characters [no spaces]: "; getline(cin, user_text_char);
		if ((user_text_char.size() % 8) !=0 )			//blovk won't be perfectly divisible by 8
		{
			cout << "Text won't fit in 8-bit blocks! Adding 'x' as filler char" << endl;
			user_text_char.append("x");
		}
	}
	if (choice == 2)
	{
		cout << "\n Please enter 8 Integers:" << endl;
		for (int i = 0; i < 8; i++)
		{
			cout << "Int " << i << ": " << endl;
			cin >> user_key_int[i];
		}
	}
}

/*------------------------------Key Generation-----------------------------*/
void DES_Cipher::keyGeneration()
{
	int choice;
	int ascii, j;
	system("cls");
	cout << "How do you wish to enter the key?" << endl;
	cout << "1. Characters\t2.Integers" << endl;
	cout << "Choice: ";
	cin >> choice;

	if (choice == 1){
		cout << "\n Please enter 8 characters [no spaces]: " << endl;
		cin >> user_key_char;
	}
	if (choice == 2)
	{
		cout << "\n Please enter 8 Integers:" << endl;
		for (int i = 0; i < 8; i++)
		{
			cout << "Int " << i << ": "<<endl;
			cin >> user_key_int[i];
		}
	}
	user_text_int.push_back()

	for (int i = 0; i < 8; i++)
	{
		if (choice == 1)
		{
			ascii = (int)user_key_char[i];		//if using chars for key, cast to int & save temp var
		}
		else ascii = user_key_int[i];			//else if using int for key, save as int
		asciiToBin(ascii);					//convert to binary, result in bin[]
		
		for (int b = 0; b < 8; b++)
		{
			key_64bit[j] = bin[b];			//note how j not reset, keeps incrementing to append to array
			j++;
		}

		/* PC1 PERMUTATION STEP
		Process: Permuting K0 through PC1 dropping each 8th bit. Results in 56 bit permute.
				 Note how -1th index used as PC1 does not use 0th based indexing. 
				 This key is not the final either, LS and PC2 must still occur.
		}
		*/
		for (int i = 0; i < 56; i++)
		{
			init_key[0][i] = key_64bit[PC1[i] - 1];	
		}

		/* LEFT SHIFT STEP
		Process: The Cn and Dn half matrices of 28bits each from k56-bit permuted key are
				 left shifted in bit position. this occurs once on array ends or twice 
				 otherwise. Clockwise wraparound occurs on end bits if needed.*/
		}

		for (int k = 1; k <= 16; k++)			//for 16 rounds (using familiar 1th notation)
		{
			for (int i = 0; i < 28; i++)		//for each half of 56 bit key (in C & D halves)
			{
				C_n[i] = init_key[k - 1][i];			//c0 = K0_0 etc
				D_n[i] = init_key[k - 1][i + 28];
			}

			if (k == 1||k==2||k==9||k==16)
			{
				C_n_LS[0] = C_n[27];					//left shifting the leftmost array value loops around
				D_n_LS[0] = D_n[27];

				for (int i = 1; i < 28; i++)		//for the rest, each half of 56 bit key (in C & D halves)
				{
					C_n_LS[i] = C_n[i - 1];			//shift value 1 left if k at column ends
					D_n_LS[i] = D_n[i - 1];			//C & D SHIFTED = the value to the immediate left in C/D
				}

			}
			else
			{
				C_n_LS[0] = C_n[26];
				C_n_LS[1] = C_n[27];			//left shifting the leftmost array value loops around

				D_n_LS[0] = D_n[26];			//not LS TWO bits around clockwise as above
				D_n_LS[1] = D_n[27];

				for (int i = 2; i < 28; i++)		//for the rest, each half of 56 bit key (in C & D halves)
				{
					C_n_LS[i] = C_n[i - 2];			//for the rest...shift value 2 left if k not at column ends
					D_n_LS[i] = D_n[i - 2];
				}
			}

			for (int i = 0; i < 28; i++)		//concating Cn and Dn to the 56bit key each round
			{
				init_key[k][i] = C_n_LS[i];
				init_key[k][i + 28] = D_n_LS[i];
			}

		}

		/* PC2 PERMUTATION STEP
		Process: Permuting each 56-bit shifted key through PC2. Results in 48 bit permute of subarrays.
		This process generates the final 48bit round key set of 16 to be used during the Feistel process
		
		*/

		for (int i = 0; i<16; i++)
		{
			for (int j = 0; j<48; j++)
			{
				final_key[i][j] = init_key[i + 1][PC2[j] - 1];
			}
		}
}



