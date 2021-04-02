#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

struct Versenyzo {
	string slovesek;		// lövésrekord - string típussal
	char clovesek[40] = "";	// lövésrekord - char típussal
	int pontszam = 0;		// pontszám
	int l = 0;				// leadott lövések száma
	int helyezes = 0;		// helyezés
	int rajtszam = 0;		// rajtszám
};

class Loveszet {
public:
	Loveszet(char* filenev);
	~Loveszet();

	int getLetszam() const { return v; }
	int getPontszam(int rajtszam) const { return tmb[rajtszam - 1].pontszam; }

	void kiir();
	string konzisztens_lovok();
	int legtobb_loves();
	int loertek(char *sor, int hossz);
	int ossztalalt(int rajtszam);
	string talalatok_sorszamai(int rajtszam);
	int leghosszabb_sorozat(int rajtszam);
	void adatexport(char *filenev);

private:

	int v;
	Versenyzo* tmb;
	Versenyzo* rangsor;
	const string loveszet_jelek="+-";

	bool ervenyes_rekord(const string &rekord, const string &erv_karakterek);
	bool ervenyes_rekord(char* rekord, int h);
	void rangsorol(Versenyzo *tmb);

};