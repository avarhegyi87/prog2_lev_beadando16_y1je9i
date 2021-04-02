#include "Loveszet.h"

Loveszet::Loveszet(char* filenev) {
	ifstream be(filenev);
	if (be.fail()) {
		cerr << "Hiba a fajl megnyitasa soran!\n\nEllenorizze az alabbiakat:\n"
			" - a fajlnev '" << filenev << "',\n - a fajl elerheto a projekt mappajaban,\n"
			"majd inditsa ujra a programot!\nHa a problema tovabbra is fennall, lepjen kapcsolatba a fejlesztovel!\n\n";
		system("pause"); exit(EXIT_FAILURE);
	}

	int i=0;
	int sorok_szama = 0;
	unsigned int fajlban_letszam;
	string sor;

	while (!be.eof()) {
		getline(be, sor);
		if (sor != "") {
			sorok_szama++;
		}
	}
	be.clear();		// kitöröljük az eof flaget
	be.seekg(0);	// visszaugrunk a fájl elejére, hogy beolvashassuk az értékeket

	be >> sor; // beolvassuk az elsõ sort, mely a versenyzõk számát jelöli
	// ha az elsõ sor numerikus, elmentjük számként, ha nem, a program kilép
	if (ervenyes_rekord(sor, "0123456789")) { 
		fajlban_letszam = stoi(sor); 
	}
	else {
		cout << "Ervenytelen fajl!\nAfajl elso soraban szamnak (a versenyzok szamanak) kell szerepelnie.\n\n";
		system("pause"); exit(EXIT_FAILURE);
	}

	// ha a fájlban szereplõ létszám nem egyezik a rekordok számával, a fájl hibás
	if (fajlban_letszam != sorok_szama - 1) {
		cerr << "Hibas fajl!\nA fajl elso soraban levo letszam nem egyezik a fajlban tarolt adatok szamaval."
			"A fajl nem feldolgozhato." << endl << endl;
		system("pause"); exit(EXIT_FAILURE);
	}

	Loveszet::v = sorok_szama - 1;
	Loveszet::tmb = new Versenyzo[v];
	int nemerv_index=0;

	// beolvasunk minden nem üres sort, és megnézzük, csak érvényes karakterek szerepelnek-e benne
	while (!be.eof() && i < v && be >> tmb[i].clovesek) {
		//cout << sizeof(tmb[i].clovesek) << endl << sizeof(tmb[i].clovesek[0]);
		tmb[i].slovesek = tmb[i].clovesek;
		tmb[i].l = tmb[i].slovesek.length();
		tmb[i].rajtszam = i + 1;
		if (!Loveszet::ervenyes_rekord(tmb[i].clovesek,tmb[i].l)) {
			cout << setw(2) << i+1 << ". versenyzo rekordja ervenytelen: " << tmb[i].slovesek << endl; nemerv_index++;
		}
		tmb[i].pontszam = Loveszet::loertek(tmb[i].clovesek,tmb[i].l);
		i++; 
	}
	if (nemerv_index) {
		cerr << endl << nemerv_index << " ervenytelen rekord a fajlban, a fajl nem feldolgozhato!\n"
			"A rekordok csak '+' es '-' karaktereket tartalmazhatnak.\n\n";
		system("pause"); exit(EXIT_FAILURE);
	}

	// fájl bezárása
	be.close();

	// kiiratjuk a versenyzok rekordjait: sorszám, lövések és elért pontszám
	Loveszet::kiir();

	// rangsoroljuk a tombot
	Loveszet::rangsor = new Versenyzo[v];
	for (int i = 0; i < v; i++) {
		rangsor[i] = tmb[i];
	}
	Loveszet::rangsorol(rangsor);
}

Loveszet::~Loveszet() {
	delete[]tmb;
}

bool Loveszet::ervenyes_rekord(const string &rekord, const string &erv_karakterek) {
	bool rosszchar = false;
	int talalat;

	for (char const &c: rekord) {
		talalat = erv_karakterek.find(c);
		if (talalat == string::npos) {
			rosszchar = true; break;
		}
	}
	return not rosszchar;
}

bool Loveszet::ervenyes_rekord(char* rekord, int h) {
	int i = 0;
	while (((rekord[i] == 43) || (rekord[i] == 45)) && i < h) i++;
	return i == h;
}

void Loveszet::kiir() {
	cout.setf(ios::left);
	cout << setw(8) << "RAJTSZAM" << "\t"  << setw(40) << "LEADOTT LOVESEK" << "\t" << setw(4) << "PONT" << endl;
	for (int i = 0; i < v; i++) {
		cout << setw(8) << i + 1 << "\t" << setw(40) << tmb[i].clovesek << "\t" << setw(4) << tmb[i].pontszam << endl;
	}
	cout << endl;
}

string Loveszet::konzisztens_lovok() {
	string s;
	for (int i = 0; i < v; i++) {
		if (tmb[i].slovesek.find("++") != string::npos) {
			s += to_string(i + 1) + " ";
		}
	}
	return s;
}

int Loveszet::legtobb_loves() {
	int maxlovo = 0;
	if (v > 1) {
		for (int i = 1; i < v; i++) {
			if (tmb[i].l > tmb[maxlovo].l) maxlovo = i;
		}
	}
	return maxlovo;
}

int Loveszet::loertek(char* sor, int hossz) {
	int aktpont = 20, ertek = 0;
	for (int i = 0; i < hossz; i++) {
		if ((aktpont > 0) && (sor[i] == 45)) aktpont--;
		else ertek += aktpont;
	}
	return ertek;
}

int Loveszet::ossztalalt(int rajtszam) {
	int talalt=0, r = rajtszam - 1;
	for (int i = 0; i < tmb[r].l; i++) {
		if (tmb[r].clovesek[i] == 43) talalt++;
	}
	return talalt;
}

string Loveszet::talalatok_sorszamai(int rajtszam) {
	string s;
	int r = rajtszam - 1;
	for (int i = 0; i < tmb[r].l; i++) {
		if (tmb[r].clovesek[i] == 43) s += to_string(i + 1) + " ";
	}
	return s;
}

int Loveszet::leghosszabb_sorozat(int rajtszam) {
	int talalt = 0, max = 0, r = rajtszam - 1;
	for (int i = 0; i < tmb[r].l; i++) {
		if (tmb[r].clovesek[i] == 43) {
			talalt++;
			if (talalt > max) max = talalt;
		}
		else {
			if (talalt > max) max = talalt; 
			talalt = 0;
		}
	}
	return max;
}

void Loveszet::rangsorol(Versenyzo *a) {
	Versenyzo seged;
	for (int i = 1; i < v; i++) {
		for (int j = v - 1; j >= i; j--) {
			if (a[j - 1].pontszam < a[j].pontszam) {
				seged = a[j - 1];
				a[j - 1] = a[j];
				a[j] = seged;
			}
		}
	}

	int szum;
	rangsor[0].helyezes = 1;
	for (int i = 1; i < v; i++) {
		szum = 0;
		for (int j = 0; j < i; j++) {
			if (rangsor[j].pontszam > rangsor[i].pontszam) szum++;
		}
		rangsor[i].helyezes = ++szum;
	}
}

void Loveszet::adatexport(char *filenev) {
	ofstream ki(filenev);
	cout.setf(ios::left);
	cout << "\n\nHELY.\tRAJTSZ.\tPONT\n";
	for (int i = 0; i < v; i++) {
		cout << rangsor[i].helyezes << (char)9 << rangsor[i].rajtszam << (char)9 << rangsor[i].pontszam << endl;
		ki << rangsor[i].helyezes << (char)9 << rangsor[i].rajtszam << (char)9 << rangsor[i].pontszam << endl;
	}
}