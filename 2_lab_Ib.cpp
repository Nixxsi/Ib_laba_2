#include <iostream>
#include <string>
#include <ctime>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
namespace Lib = boost::multiprecision;

class PrivateK {
public:
	int Mod;
	int d;
};

class PublicK {
public:
	int Mod;
	int e;
};

bool PrimeNum(int num);
int RandomNum();
int E(int f);
int D(int e, int f);
string Encoder(string str, PublicK& publicK);
string Decoder(string str, PrivateK& privateK);

int main() {

	srand(time(0));
	setlocale(LC_ALL, "rus");

	//Генерация простых чисел
	int P, Q, f;
	string str;
	PublicK publicK;
	PrivateK privateK;

	do {
		P = RandomNum();
		Q = RandomNum();

	} while (P * Q < 130 | !PrimeNum(P) | !PrimeNum(Q));

	//Генерация Публичного ключа
	publicK.Mod = P * Q;
	f = (P - 1) * (Q - 1);
	publicK.e = E(f);
	cout << "Public Key: " << "--E: " << publicK.e << "-- --Mod: " << publicK.Mod << "--" << endl;

	//Генерация Приватного ключа 
	privateK.Mod = P * Q;
	privateK.d = D(publicK.e, f);
	cout << "Private Key: " << "--D: " << privateK.d << "-- --Mod: " << privateK.Mod << "--" << endl;

	cout << "Enter the string..." << endl;
	getline(cin, str);
	string EncodeR = Encoder(str, publicK);
	cout << "Encode Result: " << EncodeR << endl;
	string DecodeR = Decoder(EncodeR, privateK);
	cout << "Decode Result: " << DecodeR << endl;
}

string Encoder(string str, PublicK& publicK) {

	string result;

	for (int i = 0; i < str.size(); i++) {
		Lib::cpp_int code = Lib::pow(Lib::cpp_int(str[i]), publicK.e) % publicK.Mod;
		result = result + to_string(int(code)) + " ";
	}
	return result;
}

string Decoder(string str, PrivateK& privateK) {

	Lib::cpp_int result;
	string code;
	string Decode;

	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] != ' ') {
			code.push_back(str[i]);
		}
		if (str[i] == ' ' || i == (str.size() - 1))
		{
			istringstream(code) >> result;
			result = (Lib::pow(result, privateK.d)) % privateK.Mod;
			code = int(result);
			Decode = Decode + code;

			code.clear();
		}
	}
	return Decode;
}

int D(int e, int f) {
	int d = 1;
	do {
		d++;
	} while ((d * e) % f != 1);
	return d;
}

int E(int f) {
	for (int i = 2; i < f; i++) {
		if (PrimeNum(i) && (f % i != 0)) return i;
	}
}
bool PrimeNum(int num) {

	int count = 0;
	for (int i = 1; i <= num / 2; i++) {
		if (num % i == 0) count++;
	}
	if (count == 1) return true;
	else return false;
}

int RandomNum() {
	return 1 + rand() % 100;
}

