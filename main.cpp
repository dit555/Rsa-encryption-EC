#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cstdint>


using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::to_string;

const int KEY_LENGTH = 29;


double gcd(double a, double b);
long long int myPow(long long int x, long long int p);
void encode(double key1, double key2, char *charKey);
string decode(double key1, double key2, string message, char *charkey);

int main() {
	char input = '0';
	char characterKey[KEY_LENGTH] = { '~', '~', 'a','b', 'c',  'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
							'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' , ' '};
	string secretMessage;
	double key1 = 13;
	double key2 = 77;

	//run menu
	while (input == '0' || input == '1' || input == '2' || input == '3') {

		cout << "Menu" << endl;
		cout << "0. exit" << endl << "1. encode" << endl << "2. decode" << endl << "3. break" << endl;
		cout << "->>> ";

		cin >> input;

		if (input == '0')
			break;

		else if (input == '1')
			encode(key1, key2, characterKey);

		else if (input == '2') {
			cout << "input the first part of the decryption key" << endl;
			cout << "-->>";
			cin >> key1;

			cout << "input the second part of the decryption key" << endl;
			cout << "-->>";
			cin >> key2;

			cout << "input your encrypted message with each number separated by one space" << endl;
			cout << "-->> ";
			cin.ignore();
			std::getline(cin, secretMessage);

			cout << "your message is :" << endl;
			cout << decode(key1, key2, secretMessage, characterKey);
			cout << endl << "--------------------------------------" << endl << endl;
		}
		


	}


	return 0;
}

double gcd(double a, double b){

	//return if a or b is zero
	if (a == 0)
		return b;
	if (b == 0)
		return a;

	// base case 
	if (a == b)
		return a;

	//recurse
	if (a > b)
		return gcd(a - b, b);
	return gcd(a, b - a);
}

long long int myPow( long long int x, long long int p) {
	if (p == 0) return 1;
	if (p == 1) return x;
	return x * myPow(x, p - 1);
}

void encode(double key1, double key2, char *charKey) {
	string input;

	//variables nessicary for RSA encoding
	double prime1;
	double prime2;
	double phi;
	double publicKey1;
	double publicKey2;
	double privateKey1;
	double privateKey2;


	cout << "enter a sentence you would like to encode" << endl;
	cout << "-->> ";

	cin.ignore();
	std::getline(cin, input);

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
	

	//calculating vairables for RSA

	//get the first part of the public key
	
	/*publicKey1 = 2;
	while (publicKey1 < phi) {
		if (gcd(publicKey1, phi) == 1)
			break;
		else
			publicKey1++;
	}*/

	//give public key
	

	//generate the first part of the private key


	//encode message
	long long int temp;
	for (int i = 0; i < input.length(); i++) {
		temp = myPow ((long long int)encoded[i], (long long int)key1);
		encoded[i] = temp % (long long int)key2;
	}

	//print encoded message in a whitespace separeted list
	cout << "your encoded message: " << endl;
	for (int i = 0; i < input.length(); i++)
		cout << encoded[i] << ' ';

	cout << endl << "---------------------------------------" << endl << endl;
}

string decode(double key1, double key2, string message, char *charKey) {

	int prime[14] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43}; //allows the calculation of N's up to 1763

	//break N and find d
	int p;
	int q;

	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 14; j++) {
			if (prime[i] * prime[j] == key2) {
				p = i;
				q = j;
			}

		}
	}

	int phi = (p - 1) * (q - 1);

	//find d
	int d = 2;
	while ((phi * d + 1) / key1 != 1)
		d++;
	cout << "d is: " << d << endl;


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

	d = 37;
	__int128 temp;
	for (int i = 0; i < termIndex; i++) {
		temp = myPow((long long int)terms[i], (long long int)d);
		cout << temp << endl;
		terms[i] = (int)(temp % (long long int)key2);
	}
	//turn decoded numbers into chars and append them to decoded
	decoded = charKey[(int)terms[0]];
	string toAppend;
	for (int i = 1; i < termIndex; i++) {
		toAppend = charKey[(int)terms[i]];
		decoded.append(toAppend);
	}
	
	return decoded;
}
