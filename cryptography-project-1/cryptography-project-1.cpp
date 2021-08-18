//Corey Fitzpatrick
//Cryptography 8/17/2021

#include <iostream>
#include <string>
#include<vector>
#include <fstream>
#include <set>
#include <algorithm>

std::string encrypt(int key, std::string plainText);
std::string decrypt(int key, std::string cipherText);
void printCipher(std::string cipherText);
void printPlainText(std::string plainText);
std::string parseString(std::string text);
void displayOptions();
std::vector<std::string> getPossibleSolutions(std::string userCipher);
std::string bestGuess(std::vector<std::string> plainTextList);

struct Dictionary {
	std::set<std::string> words;

	Dictionary() {
		std::string line;
		std::ifstream myfile("wordList.txt");
		if (myfile.fail())
			exit(1);
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				std::transform(line.begin(), line.end(), line.begin(), std::toupper);
				words.insert(line);
			}
			myfile.close();
		}
	}
};

//==============================================================================
int main()
{
	int selection;
	int key;
	std::string plainText;
	std::string cipherText;

	displayOptions();
	std::cin >> selection;

	while (selection != 4) {
		switch (selection) {
		case 1:
			std::cout << "Enter a key (integer between 0-25): ";
			std::cin >> key;
			while (key > 25 || key < 0) {
				std::cout << "Enter a key (integer between 0-25): ";
				std::cin >> key;
			}
			std::cout << "Enter a plain text message to encode: ";
			std::cin.ignore();
			std::getline(std::cin, plainText);
			cipherText = encrypt(key, plainText);
			printCipher(cipherText);
			break;
		case 2:
			std::cout << "Enter a key (integer between 0-25): ";
			std::cin >> key;
			while (key > 25 || key < 0) {
				std::cout << "Enter a key (integer between 0-25): ";
				std::cin >> key;
			}
			std::cout << "Enter a cipher text message to decode: ";
			std::cin.ignore();
			std::getline(std::cin, cipherText);
			plainText = decrypt(key, cipherText);
			printPlainText(plainText);
			break;
		case 3:
			std::cout << "Enter a cipher text and I will try and guess what the plain-text is: \n";
			std::cin.ignore();
			std::getline(std::cin, cipherText);
			std::vector<std::string> possibleSolutions = getPossibleSolutions(cipherText);
			std::cout << "\nMy best guess is: " + bestGuess(possibleSolutions) + "\n\n";
			break;
		}

		displayOptions();
		std::cin >> selection;
	}
}
//==============================================================================
std::string encrypt(int key, std::string plainText) {
	std::string cipherText = "";
	char cipherArray[26];
	int index;
	plainText = parseString(plainText);
	for (int i = 0; i < 26; i++) {
		cipherArray[i] = 'A' + i;
	};
	for (int i = 0; i < plainText.length(); i++) {
		index = (toupper(plainText[i]) - 'A' + key) % 26;
		cipherText += cipherArray[index];
	}

	return cipherText;
}
//==============================================================================

std::string decrypt(int key, std::string cipherText) {
	std::string plainText = "";
	char plainTextArray[26];
	int index;

	cipherText = parseString(cipherText);
	for (int i = 0; i < 26; i++) {
		plainTextArray[i] = 'A' + i;
	};
	for (int i = 0; i < cipherText.length(); i++) {
		index = toupper(cipherText[i]) - 'A' - key;
		if (index < 0)
			index += 26;
		plainText += plainTextArray[index];
	}

	return plainText;
}
//==============================================================================
//prints in all uppercase, and prints in blocks of 5 characters.
void printCipher(std::string cipherText) {
	std::cout << "\nYour encrypted message is: \n";
	for (int i = 1; i <= cipherText.length(); i++) {
		std::cout << cipherText[i - 1];
		if (i % 5 == 0)
			std::cout << " ";
	}
	std::cout << "\n\n";
}
//==============================================================================
//prints plain text in a single string in uppercase.
void printPlainText(std::string plainText) {
	std::cout << "\nYour decrypted message is: \n";
	for (char letter : plainText) {
		std::cout << letter;
	}
	std::cout << "\n\n";
}
//==============================================================================
//utility function that removes any white space and periods.  Returns formatted string.
std::string parseString(std::string text) {
	for (int i = 0; i < text.length(); i++) {
		if (text[i] == ' ' || text[i] == '.' || text[i] == ',') {
			text.erase(text.begin() + i);
			i--;
		}
	}

	return text;
}
//==============================================================================

void displayOptions() {
	std::cout << "Enter 1 to encrypt a plain-text. \nEnter 2 to decrypt a cipher-text. \nEnter 3 to enter a cipher and I will try to solve it. \nEnter 4 to exit.\n";
}
//==============================================================================
//returns a vector containing all 26 possible solutions to the ciphertext, one of them must contain the original plaintext message
std::vector<std::string> getPossibleSolutions(std::string userCipher) {
	std::vector<std::string> plainTextList;
	char cipherArray[26];
	std::string cipher = "";
	for (int i = 0; i < 26; i++) {
		plainTextList.push_back(decrypt(i, userCipher));
	};
	std::cout << "The possible Solutions are: \n";
	for (auto i = plainTextList.begin(); i != plainTextList.end(); ++i)
		std::cout << *i << "\n";

	return plainTextList;
}
//==============================================================================
//searches a set of 10000 most commonly used english words and searches all 26 possible solutions for the one that
//has the most occurrence of the most used words.
//returns the plaintext string that has the most occurrences of the most frequently used words in english.
//this can be highly optimized, mainly by eliminating low frequency words, and using simply most often used word endings like 'ing', 'ed', 'ly', etc.
std::string bestGuess(std::vector<std::string> plainTextList) {
	Dictionary dict;
	int count = 0;
	int numWords[26] = { 0 };
	std::string word;

	for (auto i = plainTextList.begin(); i != plainTextList.end(); ++i) {
		for (auto j : dict.words) {
			word = *i;
			if (word.find(j) != std::string::npos) {
				numWords[i - plainTextList.begin()]++;
			}
		}
	}

	int maxElement = *std::max_element(std::begin(numWords), std::end(numWords));
	int index;
	for (int i = 0; i < 26; i++) {
		if (numWords[i] == maxElement)
			index = i;
	}
	return plainTextList.at(index);
}