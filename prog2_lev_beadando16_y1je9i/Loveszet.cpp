#include "Loveszet.h"

Loveszet::Loveszet(char *filenev) {
	/*
	A Lövészet osztály konstruktorja

	Megnyitja az argumentumban szereplõ fájlt,
	megszámolja a sorokat, az elsõ sorban megnézi, hogy szám van-e,
	és ez egyezik-e a sorok számával (ami a versenyzõk száma)
	Megvizsgálja, a további sorokban kizárólag érvényes karakterek (+ és-) szerepelnek-e.
	Ha ezek valamelyike nem igaz, a program kilép, mivel a fájl hibás.
	
	Amennyiben a fájl nem hibás, az adatokat egy struktúrába olvassa a program,
	és sorba rendezi õket.

	char pointer: filenev - a megnyitandó fájl neve avagy elérési útvonala
	*/
	ifstream be(filenev);
	if (be.fail()) {
		cerr << "Hiba a fajl megnyitasa soran!\n\nEllenorizze az alabbiakat:\n"
			" - a fajlnev '" << filenev << "',\n - a fajl elerheto a projekt mappajaban,\n"
			"majd inditsa ujra a programot!\nHa a problema tovabbra is fennall, lepjen kapcsolatba a fejlesztovel!\n\n";
		system("pause"); exit(EXIT_FAILURE);
	}

	unsigned int sorok_szama = 0;
	unsigned int fejlec_letszam;
	string sor;

	// megszámoljuk a fájlban szereplõ sorok számát
	while (!be.eof()) {
		getline(be, sor);
		if (sor != "") {
			sorok_szama++;
		}
		if (sor.find(" ") != string::npos) {
			cerr << "Szokoz eszlelve az egyik sorban. A szokozok hibas adatbeolvasast eredmenyeznek.\n"
				"Torolje ki a szokozoket a loveseket szimbolizalo + es - karakterek kozul, "
				"majd inditsa ujra a programot!\nA fajl nem feldolgozhato.\n\n";
			system("pause"); exit(EXIT_FAILURE);
		}
	}
	be.clear();		// kitöröljük az eof flaget
	be.seekg(0);	// visszaugrunk a fájl elejére, hogy beolvashassuk az értékeket

	if (sorok_szama == 0) {
		cerr << "Ures fajl! Nem feldolgozhato." << "\n\n";
		system("pause"); exit(EXIT_FAILURE);
	}

	be >> sor; // beolvassuk az elsõ sort, mely a versenyzõk számát jelöli
	// ha az elsõ sor numerikus, elmentjük számként, ha nem, a program kilép
	if (ervenyes_szoveg(sor, "0123456789")) { 
		fejlec_letszam = stoi(sor); 
	}
	else {
		cerr << "Ervenytelen fajl!\nA fajl elso soraban szamnak (a versenyzok szamanak) kell szerepelnie.\n\n";
		system("pause"); exit(EXIT_FAILURE);
	}

	// v - a versenyzõk száma (sorok száma, mínusz 1, mert az elsõ sorban maga a létszám szerepel)
	Loveszet::v = sorok_szama - 1;

	// ha a fejlécben szereplõ szám nem egyezik a sorok számával, a fájl hibás
	if (fejlec_letszam != v) {
		cerr << "Hibas fajl!\nA fajl elso soraban levo letszam nem egyezik a fajlban tarolt adatok szamaval. "
			"A fajl nem feldolgozhato." << endl << endl;
		system("pause"); exit(EXIT_FAILURE);
	}

	// ha a versenyzõk száma nem 2 és 100 közötti, a program kilép
	if (v < 2 || v > 100) {
		cerr << "Legalabb 2 es legfeljebb 100 versenyzo lehet a fajlban.\nA fajl nem feldolgozhato." << endl << endl;
		system("pause"); exit(EXIT_FAILURE);
	}

	// a versenyzõk számával megegyezõ méretû struktúra tömb létrehozása
	Loveszet::tmb = new Versenyzo[v];
	int nemerv_index = 0;
	int i = 0;

	// beolvasunk minden nem üres sort, és megnézzük, hogy csak érvényes karakterek szerepelnek-e benne
	// ha érvényes a sor, egy struktúra tömbben elmentõdnek az adatok
	while (!be.eof() && i < v && be >> tmb[i].slovesek) {
		if (tmb[i].slovesek.length() > 40) {
			tmb[i].slovesek = tmb[i].slovesek.substr(0, 40);
		}
		if (tmb[i].slovesek.length() < 4) {
			tmb[i].slovesek.replace(0,tmb[i].slovesek.length(),tmb[i].slovesek.length(),'-');
		}
		tmb[i].l = tmb[i].slovesek.length();
		for (int j = 0; j < tmb[i].l; j++) {
			tmb[i].clovesek[j] = tmb[i].slovesek[j];
		}
		
		tmb[i].rajtszam = i + 1;
		if (!Loveszet::ervenyes_szoveg(tmb[i].clovesek,tmb[i].l)) {
			// a felhasználót az összes érvénytelen sorról értesítjük
			cout << setw(2) << i+1 << ". versenyzo rekordja ervenytelen: " << tmb[i].slovesek << endl; nemerv_index++;
		}
		//
		tmb[i].pontszam = Loveszet::loertek(tmb[i].clovesek,tmb[i].l);
		i++; 
	}
	// ha volt érvénytelen karaktert tartalmazó sor, a program már kigyûjtötte õket, itt kilép
	if (nemerv_index) {
		cerr << endl << nemerv_index << " db ervenytelen rekord a fajlban, a fajl nem feldolgozhato!\n"
			"A rekordok csak '+' es '-' karaktereket tartalmazhatnak.\n\n";
		system("pause"); exit(EXIT_FAILURE);
	}

	// fájl bezárása, mivel a rekordokat elmenettük a struktúrába
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
	/*
	Lövészet osztály destruktorja
	Törli a tmb és rangsor struktúra tömböket
	*/
	delete[]tmb;
	delete[]rangsor;
}

bool Loveszet::ervenyes_szoveg(const string &szoveg, string erv_karakterek) {
	/*
	Megvizsgálja, hogy egy adott string formátumú szöveg csak érvényes karaktereket tartalmaz-e
	
	const string &szoveg: referencia a vizsgálandó szövegre
	erv_karakterek (opcionális): az elfogadott karakterekbõl álló string, alapértelmezetten "+-"

	return bool: true, ha csak érvényes karaktert tartalmaz a szöveg, false, ha legalább 1 évrénytelent
	*/
	bool rosszchar = false;
	int talalat;

	for (char const &c: szoveg) {
		talalat = erv_karakterek.find(c);
		if (talalat == string::npos) {
			rosszchar = true; break;
		}
	}
	return not rosszchar;
}

bool Loveszet::ervenyes_szoveg(char *szoveg, int h) {
	/*
	Megvizsgálja, hogy egy adott char formátumú szöveg csak érvényes karaktereket tartalmaz-e
	
	char *szoveg: a karaktersorozat 0. elemének memóriacímére mutat
	int h: a karaktersorozat hossza

	return bool: true, ha csak érvényes karaktert tartalmaz a szöveg, false, ha legalább 1 évrénytelent
	*/
	int i = 0;
	while (i < h && ((szoveg[i] == PLUSZ) || (szoveg[i] == MINUSZ))) i++;
	return i == h;
}

void Loveszet::kiir() {
	/*
	Kiírja a képernyõre a beolvasott fájlból kinyert adatokat:
	- rajtszám
	- leadott lövések +- kódolással
	- elért pontszám
	*/
	cout.setf(ios::left);	// balra zárás
	cout << setw(8) << "RAJTSZAM" << "\t"  << setw(40) << "LEADOTT LOVESEK" << "\t" << setw(4) << "PONT" << endl;
	for (int i = 0; i < v; i++) {
		cout << setw(8) << i + 1 << "\t" << setw(40) << tmb[i].clovesek << "\t" << setw(4) << tmb[i].pontszam << endl;
	}
	cout << endl;
}

string Loveszet::min_ket_talalat_kivalogat() {
	/*
	Eldöntés tételével azonosítja azon versenyzõket, akiknak van legalább kettõ egymás követõ találata
	Végigmegy a versenyzõk struktúra tömbjének char lövésrekordjain (0-tól n-1 -ig), 
	és megvizsgálja, hogy adott elem és az utána következõ elem egyaránt plusz jel-e.
	Ha igen, van legalább két egymást követõ talált lövés

	return string: a versenyzõk rajtszámai egybefûzött stringként, szóközzel elválasztva
	*/
	string s;
	int j;
	char *c;
	c = new char [v];
	int db=0;

	for (int i = 0; i < v; i++) {
		j = 0; // 0-tól (n-1)-ig menve megnézzük, az aktuális és a köv. elem is +
		while (j < (tmb[i].l - 1) && (tmb[i].clovesek[j] != PLUSZ || tmb[i].clovesek[j + 1] != PLUSZ)) {
			j++;
		}
		if (j < (tmb[i].l - 1)) {
			//s += to_string(i + 1) + " ";
			c[db] = i + 1;
			db++;
		}
	}

	for (int i = 0; i < db; i++) {
		s += to_string(c[i]) + " ";
	}

	delete[]c;
	return s;
}

string Loveszet::min_ket_talalat_stringfinddal() {
	/*
	str.find funkcióval azonosítja azon versenyzõket, akiknak van legalább kettõ egymás követõ találata
	Végigmegy a versenyzõk struktúra tömbjének string lövésrekordjain,
	és megvizsgálja, a rekordjukban, szerepel-e a "++" string

	return string: a versenyzõk rajtszám egybefûzött stringként, szóközzel elválasztva
	*/
	string s;
	for (int i = 0; i < v; i++) {
		if (tmb[i].slovesek.find("++") != string::npos) {
			s += to_string(i + 1) + " ";
		}
	}
	return s;
}

int Loveszet::legtobb_loves() {
	/*
	Maximum kiválasztás elvével megkeresi a legtöbb lövést leadó versenyzõt
	Egyenlõség esetén a kisebb rajtszámú versenyzõ számít maximumnak

	return int: a legtöbb lövést leadó versenyzõ rajtszáma
	*/
	int maxlovo = 0;
	if (v > 1) {
		for (int i = 1; i < v; i++) {
			if (tmb[i].l > tmb[maxlovo].l) maxlovo = i;
		}
	}
	return maxlovo + 1;
}

int Loveszet::loertek(char *sor, int hossz) {
	/*
	Kiszámítja a versenyzõk struktúra tömbje egy adott elemének a pontszámértékét 
	a leadott lövések szimbólumai alapján.
	A függvény egyszerre egy versenyzõ lövéseinek rekordját kapja meg.
	Szabály: 
	- 20 pontot ér a legelsõ célpont eltalálása. 
	- Minden célt tévesztett lövés eggyel csökkenti a következõ találattal megszerezhetõ pontok számát

	char *sor: a fájl adott sorából képzett karaktertömb 0. elemének memóriacíme
	int hossz: a karaktertömb elemszáma (a kezdõcímtõl való eltérés)
	
	return int: a versenyzõ pontszáma
	*/
	int aktpont = 20, ertek = 0;
	for (int i = 0; i < hossz; i++) {							// végigmegyünk a rekord karakterein
		if ((aktpont > 0) && (sor[i] == MINUSZ)) aktpont--;		// ha nem talált, és a megszerezhetõ pontszám még nem 0, csökkentjük a megszerezhetõ pontszámot 1-el
		else ertek += aktpont;									// ha talált, az eddig megszerzett pontszámokhoz hozzáadjuk az aktuálisan megszerezhetõ pontszámot
	}
	return ertek;
}

int Loveszet::ossztalalt(int rajtszam) {
	/*
	Megszámlálás tételének módszerével a függvény kiszámolja, össz. hány találat ("+") volt

	int rajtszam: a rendezetlen tömb indexe, ami a rajtszám
	
	return int: az összes találat száma
	*/
	int talalt=0, r = rajtszam - 1;
	for (int i = 0; i < tmb[r].l; i++) {
		if (tmb[r].clovesek[i] == PLUSZ) talalt++;
	}
	return talalt;
}

string Loveszet::talalatok_sorszamai(int rajtszam) {
	/*
	Adott sorozatra/rekordra visszaadja, hányadik lövések találtak célt.

	int rajtszam: a rendezetlen tömb indexe, ami a rajtszám
	
	return string: a célt talált lövések sorszámai, szóközökkel elválasztott stringgé összefûzve
	*/
	string s;
	int r = rajtszam - 1;
	char *c;
	c = new char[tmb[r].l];
	int db = 0;

	for (int i = 0; i < tmb[r].l; i++) {
		if (tmb[r].clovesek[i] == PLUSZ) {
			c[db] = i + 1;
			db++;
		}
	}

	for (int i = 0; i < db; i++) {
		s += to_string(c[i]) + " ";
	}

	delete[]c;
	return s;
}

int Loveszet::leghosszabb_sorozat(int rajtszam) {
	/*
	Adott sorozatra/rekordra visszaadja, hány lövésbõl állt a leghosszabb pozitív sorozat

	int rajtszam: a rendezetlen tömb indexe, ami a rajtszám
	
	return int: a leghosszabb pozitív sorozat nagysága (hány találatból állt a leghosszabb sorozat)
	*/
	int talalt = 0, max = 0, r = rajtszam - 1;
	for (int i = 0; i < tmb[r].l; i++) {
		if (talalt > max) max = talalt;
		if (tmb[r].clovesek[i] == PLUSZ) {
			talalt++;
		}
		else {
			talalt = 0;
		}
	}
	return max;
}

void Loveszet::rangsorol(Versenyzo *a) {
	/*
	Buborékos elvvel sorba rendezi a versenyzõ struktúra tömböt, majd helyezést rendel a versenyzõkhöz.

	Versenyzo pointer a: versenyzõ tömb 0. elemének memóriacíme
	*/
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
	/*
	A megadott fájlba exportálja a sorbarendezett rangsor nevû Versenyzõ struktúra tömböt,
	adott formátumban (helyezés, rajtszm, pontszám), pontszm szerint csökkenõ sorrendben

	char pointer filenev: az exportálandó fájl neve (vagy teljes elérési útvonala)
	*/
	ofstream ki(filenev);
	cout.setf(ios::left);
	cout << "\n\nHELY.\tRAJTSZ.\tPONT\n";
	for (int i = 0; i < v; i++) {
		cout << rangsor[i].helyezes << (char)9 << rangsor[i].rajtszam << (char)9 << rangsor[i].pontszam << endl;
		ki << rangsor[i].helyezes << (char)9 << rangsor[i].rajtszam << (char)9 << rangsor[i].pontszam << endl;
	}
}