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
	Loveszet(char *filenev);
	~Loveszet();

	int getLetszam() const { return v; }
	int getPontszam(int rajtszam) const { return tmb[rajtszam - 1].pontszam; }

	void kiir();
	void adatexport(char *filenev);

	string min_ket_talalat_eldontessel();
	string min_ket_talalat_stringfinddal();
	string talalatok_sorszamai(int rajtszam);

	int legtobb_loves();
	int loertek(char *sor, int hossz);
	int ossztalalt(int rajtszam);
	int leghosszabb_sorozat(int rajtszam);

private:

	int v;
	Versenyzo *tmb;
	Versenyzo *rangsor;
	const string LOVES_JELEK = "+-";
	const char PLUSZ = 43;
	const char MINUSZ = 45;

	bool ervenyes_szoveg(const string &rekord, string erv_karakterek = "+-");
	bool ervenyes_szoveg(char *rekord, int h);
	void rangsorol(Versenyzo *tmb);

};