#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>


using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::to_string;
using std::ofstream;
using std::ifstream;

const int KEY_LENGTH = 29;


int mod(int a, int b, int c);
void encode(int key1, int key2, char *charKey, string fileName);
string decode(int key1, int key2,  char *charkey, string fileName);

int main() {
	char input = '0';
	char characterKey[KEY_LENGTH] = { '~', '~', 'a','b', 'c',  'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
							'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' , ' '};
	
	
	string fileName;
	int key1;
	int key2;
	cout << "enter the first part of your public key" << endl << "->>> ";
	cin >> key1;

	cout << "enter the second part of your public key" << endl << "->>> ";
	cin >> key2;

	cin >> input;

	if (input == 'e') {
	cout << "enter file name: " << endl << "-->> ";
	cin >> fileName;
	encode(key1, key2, characterKey, fileName);
		}
	else if (input == 'd') {
		cout << "file name: " << endl;
		cout << "-->> ";
		cin >> fileName;
		decode(key1, key2, characterKey, fileName);
	}
	return 0;
}

int mod(int a, int b, int c) {
	//solves a^b mod(c)
	
	int result;
	int a2 = a * a;
	int a1 = a;


	if (b % 2 == 1){
		result = a % c;
		b--;
		result = result % c;
	}
	else {
		result = a2 % c;
		b -= 2;
		result = result % c;
	}
	while (b != 0) {
		result *= a2 % c;
		b -= 2;
		result = result % c;
	}

	return result % c;
}

void encode(int key1, int key2, char *charKey, string fileName) {
	string input;

	ifstream infile(fileName);

	std::getline(infile, input);
	infile.close();

	int *encoded = new int[input.length()];

	//create an array of numbers corresponding to the equivalent letters
	//to be encoded
	for (int i = 0; i < input.length(); i++) {
		for (int j = 0; j < KEY_LENGTH; j++) {
			if (input[i] == charKey[j]) {
				encoded[i] = j;

			}
		}

	}



	//encode message
	for (int i = 0; i < input.length(); i++) {
		encoded[i] = mod(encoded[i], key1, key2);
	}

	ofstream outfile("incrypted.txt");


	//print encoded message in a whitespace separeted list
	for (int i = 0; i < input.length(); i++) {
		outfile << encoded[i];
		if (i != input.length() - 1)
			outfile << ' ';
	}
	outfile.close();

	cout << "\"incrypted.txt\"" << endl;
}

string decode(int key1, int key2,  char *charKey, string fileName) {

	int prime[14] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43}; //allows the calculation of N's up to 1763
	string message;

	ifstream infile(fileName);

	std::getline(infile, message);
	infile.close();
	
	//break N and find d
	int p;
	int q;

	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 14; j++) {
			if (prime[i] * prime[j] == key2) {
				p = prime[i];
				q = prime[j];
			}

		}
	}

	int phi = (p - 1) * (q - 1);

	//find d
	int d = 2;
	while ((d * key1) % phi != 1)
		d++;


	int termNumber = 0;
	string decoded;

	//add one space to the end of message
	message.append(" ");

	//find the amount of terms in the message
	for (int i = 0; i < message.length(); i++) {
		if (message[i] == ' ')
			termNumber++;
	}

	int *terms = new int[termNumber];

	//take each term and separte into string then convert to int and deposit in terms
	int termIndex = 0;
	int index1 = 0;
	int index2;
	
	for (int i = 0; i < message.length(); i++) {
		if (message[i] == ' ') {
			index2 = i;
			terms[termIndex] = std::stoi(message.substr(index1, index2), nullptr, 10);
			termIndex++;
			index1 = i + 1;
		}

	}
	//decode

	for (int i = 0; i < termIndex; i++) {
		terms[i] = mod(terms[i], d, key2);
	}

	cout << "p = " << p << ", q = " << q << ", \"decrypted.txt\"" << endl;

	ofstream outfile("decrypted.txt");
	//turn decoded numbers into chars and append them to decoded
	decoded = charKey[(int)terms[0]];
	string toAppend;
	for (int i = 1; i < termIndex; i++) {
		toAppend = charKey[(int)terms[i]];
		decoded.append(toAppend);
	}
	outfile << decoded;
	outfile.close();
	
	return decoded;
}
