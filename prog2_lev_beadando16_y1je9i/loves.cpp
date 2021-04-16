#include "Loveszet.h"
using namespace std;

enum Opciok {
	lekerdez=1,
	kilep
};

int menukiiras();
bool szamstring(const string &s);

int main() {
	char infajlnev[] = "verseny.txt";
	char outfajlnev[] = "sorrend.txt";
	Loveszet L(infajlnev);
	clock_t kezd, veg;
	kezd = clock();

	// 2.) Azon versenyz�k rajtsz�m�t, akiknek egym�s ut�n k�t (vagy t�bb) l�v�se is tal�lt
	cout << endl << "2. feladat:\nAz egymast kovetoen tobbszor talalo versenyzok: " << L.min_ket_talalat_kivalogat() << endl;

	// 3.) A legt�bb l�v�st lead� versenyz� (azonos l�v�ssz�m eset�n a legalacsonyabb rajtsz�m�)
	cout << endl << "3. feladat:\nA legtobb lovest leado versenyzo rajtszama: " << L.legtobb_loves() << endl;

	// 5.) Versenyz� sorsz�m�nak bek�r�se, �s a sorsz�mhoz tartoz� inform�ci�k ki�r�sa a k�perny�re
	cout << endl << "5. feladat";
	bool fkilep = false;
	int valasztas = lekerdez;
	int bekeres = 0;
	string sbekeres;
	
	while (!fkilep) {
		do {
			cout << endl << "Adjon meg egy rajtszamot (1-" << L.getLetszam() << ")! ";			
			cin >> sbekeres;
			cin.ignore();
			if (szamstring(sbekeres)) {
				bekeres = stoi(sbekeres);
			}
			else {
				bekeres = 0;
			}
			if (bekeres < 1 || bekeres > L.getLetszam()) {
				cout << endl << "A szam erteke 1 es " << L.getLetszam() << " kozott legyen!";
			}
		} while (bekeres < 1 || bekeres > L.getLetszam());

		cout << endl << "5a. feladat: Celt ero lovesek: " << L.talalatok_sorszamai(bekeres);
		cout << endl << "5b. feladat: Az eltalalt korongok szama: " << L.ossztalalt(bekeres);
		cout << endl << "5c. feladat: A leghosszabb hibatlan sorozat hossza: " << L.leghosszabb_sorozat(bekeres);
		cout << endl << "5d. feladat: A versenyzo pontszama: " << L.getPontszam(bekeres);

		valasztas = menukiiras();

		switch (valasztas) {
			case lekerdez:
				break;
			case kilep:
				cout << endl << "Kilepes a feladatbol...";
				fkilep = true;
				break;
			default:
				cout << "Hibas parameter! Kilepes a feladatbol..." << endl;
				fkilep = true;
				break;
		}
	}
	
	// 6.) rangsor kiexport�l�sa f�jlba
	L.adatexport(outfajlnev);

	cout << endl << endl;
	veg = clock();
	cout << "A program futasi ideje: " << (float)(veg - kezd) / CLK_TCK << endl;
	system("pause");
	return EXIT_SUCCESS;
}

int menukiiras() {
	int opcio;
	int cimhossz, opcmaxhossz;
	const string cim = "\t********    OPCIOK    ********";
	const string opcstr_lekerdez = "(1) On altal valasztott versenyzo eredmenyeinek lekerdezese";
	const string opcstr_kilep = "(2) Kilepes ebbol a funkciobol es a program folytatasa";
	string szokozok;

	cimhossz = cim.length();
	if (opcstr_kilep.length() > opcstr_lekerdez.length()) opcmaxhossz = opcstr_kilep.length();
	else opcmaxhossz = opcstr_lekerdez.length();

	for (int i = 0; i < (opcmaxhossz / 2 - cimhossz / 2); i++) szokozok += " ";

	cout << endl << endl << endl;
	cout.setf(ios::left);
	cout << szokozok << cim << endl << opcstr_lekerdez << endl << opcstr_kilep << endl;
	cout << "Valasszon egy opciot a fentiek kozul: ";
	cin >> opcio;

	return opcio;
}

bool szamstring(const string &s) {
	string::const_iterator it = s.begin();	// a main f�ggv�nyb�l kapott string v�ltoz� �rt�k�b�l iter�tort gener�l
	while (it != s.end() && isdigit(*it)) {	// am�g nem �rt�nk a string v�g�re, megn�zz�k az iter�tor mem�riahely�n sz�mjegy van-e
		++it;
	}
	return !s.empty() && it == s.end();		// ha nem �res a string, �s a v�g�re �rve is csak sz�mjegyet tal�ltunk: true, else false
}