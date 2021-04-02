#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

struct Versenyzo {
	string slovesek;		// l�v�srekord - string t�pussal
	char clovesek[40] = "";	// l�v�srekord - char t�pussal
	int pontszam = 0;		// pontsz�m
	int l = 0;				// leadott l�v�sek sz�ma
	int helyezes = 0;		// helyez�s
	int rajtszam = 0;		// rajtsz�m
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