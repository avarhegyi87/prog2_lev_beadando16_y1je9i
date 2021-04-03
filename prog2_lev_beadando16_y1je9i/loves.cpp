#include "Loveszet.h"
using namespace std;

enum Opciok {
	lekerdez=1,
	kilep
};

int menukiiras();

int main() {
	char infajlnev[] = "verseny.txt";
	char outfajlnev[] = "sorrend.txt";
	Loveszet L(infajlnev);
	clock_t kezd, veg;
	kezd = clock();

	// 2.) Azon versenyzõk rajtszámát, akiknek egymás után két (vagy több) lövése is talált
	cout << endl << "2. feladat:\nAz egymast kovetoen tobbszor talalo versenyzok: " << L.min_ket_talalat_eldontessel() << endl;

	// 3.) A legtöbb lövést leadó versenyzõ (azonos lövésszám esetén a legalacsonyabb rajtszámú)
S	cout << endl << "3. feladat\nA legtobb lovest leado versenyzo rajtszama: " << L.legtobb_loves() + 1 << endl;

	// 5.) Versenyzõ sorszámának bekérése, és a sorszámhoz tartozó információk kiírása a képernyõre
	cout << endl << "5. feladat";
	bool fkilep = false;
	int valasztas = lekerdez;
	int bekeres;
	
	while (!fkilep) {
		do {
			cout << endl << "Adjon meg egy rajtszamot! ";			
			cin >> bekeres;
			if (bekeres < 0 || bekeres > L.getLetszam()) {
				cout << endl << "A szam erteke 0 es " << L.getLetszam() << " legyen!";
			}
		} while (bekeres < 0 || bekeres > L.getLetszam());

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
	
	// 6.) rangsor kiexportálása fájlba
	L.adatexport(outfajlnev);

	cout << endl << endl;
	veg = clock();
	cout << "A program futasi ideje: " << (float)(veg - kezd) / CLK_TCK << endl;
	system("pause");
	return EXIT_SUCCESS;
}

int menukiiras() {
	int valasztas;

	cout << endl << endl;
	cout << "****\tOPCIOK\t****" << endl;
	cout << "(1) On altal valasztott versenyzo eredmenyeinek lekerdezese" << endl;
	cout << "(2) Kilepes ebbol a funkciobol es a program folytatasa" << endl;
	cout << "Valasszon egy opciot a fentiek kozul: ";
	cin >> valasztas;

	return valasztas;
}