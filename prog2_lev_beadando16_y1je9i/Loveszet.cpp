#include "Loveszet.h"

Loveszet::Loveszet(char *filenev) {
	/*
	A L�v�szet oszt�ly konstruktorja

	Megnyitja az argumentumban szerepl� f�jlt,
	megsz�molja a sorokat, az els� sorban megn�zi, hogy sz�m van-e,
	�s ez egyezik-e a sorok sz�m�val (ami a versenyz�k sz�ma)
	Megvizsg�lja, a tov�bbi sorokban kiz�r�lag �rv�nyes karakterek (+ �s-) szerepelnek-e.
	Ha ezek valamelyike nem igaz, a program kil�p, mivel a f�jl hib�s.
	
	Amennyiben a f�jl nem hib�s, az adatokat egy strukt�r�ba olvassa a program,
	�s sorba rendezi �ket.

	char pointer: filenev - a megnyitand� f�jl neve avagy el�r�si �tvonala
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

	// megsz�moljuk a f�jlban szerepl� sorok sz�m�t
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
	be.clear();		// kit�r�lj�k az eof flaget
	be.seekg(0);	// visszaugrunk a f�jl elej�re, hogy beolvashassuk az �rt�keket

	if (sorok_szama == 0) {
		cerr << "Ures fajl! Nem feldolgozhato." << "\n\n";
		system("pause"); exit(EXIT_FAILURE);
	}

	be >> sor; // beolvassuk az els� sort, mely a versenyz�k sz�m�t jel�li
	// ha az els� sor numerikus, elmentj�k sz�mk�nt, ha nem, a program kil�p
	if (ervenyes_szoveg(sor, "0123456789")) { 
		fejlec_letszam = stoi(sor); 
	}
	else {
		cerr << "Ervenytelen fajl!\nA fajl elso soraban szamnak (a versenyzok szamanak) kell szerepelnie.\n\n";
		system("pause"); exit(EXIT_FAILURE);
	}

	// v - a versenyz�k sz�ma (sorok sz�ma, m�nusz 1, mert az els� sorban maga a l�tsz�m szerepel)
	Loveszet::v = sorok_szama - 1;

	// ha a fejl�cben szerepl� sz�m nem egyezik a sorok sz�m�val, a f�jl hib�s
	if (fejlec_letszam != v) {
		cerr << "Hibas fajl!\nA fajl elso soraban levo letszam nem egyezik a fajlban tarolt adatok szamaval. "
			"A fajl nem feldolgozhato." << endl << endl;
		system("pause"); exit(EXIT_FAILURE);
	}

	// ha a versenyz�k sz�ma nem 2 �s 100 k�z�tti, a program kil�p
	if (v < 2 || v > 100) {
		cerr << "Legalabb 2 es legfeljebb 100 versenyzo lehet a fajlban.\nA fajl nem feldolgozhato." << endl << endl;
		system("pause"); exit(EXIT_FAILURE);
	}

	// a versenyz�k sz�m�val megegyez� m�ret� strukt�ra t�mb l�trehoz�sa
	Loveszet::tmb = new Versenyzo[v];
	int nemerv_index = 0;
	int i = 0;

	// beolvasunk minden nem �res sort, �s megn�zz�k, hogy csak �rv�nyes karakterek szerepelnek-e benne
	// ha �rv�nyes a sor, egy strukt�ra t�mbben elment�dnek az adatok
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
			// a felhaszn�l�t az �sszes �rv�nytelen sorr�l �rtes�tj�k
			cout << setw(2) << i+1 << ". versenyzo rekordja ervenytelen: " << tmb[i].slovesek << endl; nemerv_index++;
		}
		//
		tmb[i].pontszam = Loveszet::loertek(tmb[i].clovesek,tmb[i].l);
		i++; 
	}
	// ha volt �rv�nytelen karaktert tartalmaz� sor, a program m�r kigy�jt�tte �ket, itt kil�p
	if (nemerv_index) {
		cerr << endl << nemerv_index << " db ervenytelen rekord a fajlban, a fajl nem feldolgozhato!\n"
			"A rekordok csak '+' es '-' karaktereket tartalmazhatnak.\n\n";
		system("pause"); exit(EXIT_FAILURE);
	}

	// f�jl bez�r�sa, mivel a rekordokat elmenett�k a strukt�r�ba
	be.close();

	// kiiratjuk a versenyzok rekordjait: sorsz�m, l�v�sek �s el�rt pontsz�m
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
	L�v�szet oszt�ly destruktorja
	T�rli a tmb �s rangsor strukt�ra t�mb�ket
	*/
	delete[]tmb;
	delete[]rangsor;
}

bool Loveszet::ervenyes_szoveg(const string &szoveg, string erv_karakterek) {
	/*
	Megvizsg�lja, hogy egy adott string form�tum� sz�veg csak �rv�nyes karaktereket tartalmaz-e
	
	const string &szoveg: referencia a vizsg�land� sz�vegre
	erv_karakterek (opcion�lis): az elfogadott karakterekb�l �ll� string, alap�rtelmezetten "+-"

	return bool: true, ha csak �rv�nyes karaktert tartalmaz a sz�veg, false, ha legal�bb 1 �vr�nytelent
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
	Megvizsg�lja, hogy egy adott char form�tum� sz�veg csak �rv�nyes karaktereket tartalmaz-e
	
	char *szoveg: a karaktersorozat 0. elem�nek mem�riac�m�re mutat
	int h: a karaktersorozat hossza

	return bool: true, ha csak �rv�nyes karaktert tartalmaz a sz�veg, false, ha legal�bb 1 �vr�nytelent
	*/
	int i = 0;
	while (i < h && ((szoveg[i] == PLUSZ) || (szoveg[i] == MINUSZ))) i++;
	return i == h;
}

void Loveszet::kiir() {
	/*
	Ki�rja a k�perny�re a beolvasott f�jlb�l kinyert adatokat:
	- rajtsz�m
	- leadott l�v�sek +- k�dol�ssal
	- el�rt pontsz�m
	*/
	cout.setf(ios::left);	// balra z�r�s
	cout << setw(8) << "RAJTSZAM" << "\t"  << setw(40) << "LEADOTT LOVESEK" << "\t" << setw(4) << "PONT" << endl;
	for (int i = 0; i < v; i++) {
		cout << setw(8) << i + 1 << "\t" << setw(40) << tmb[i].clovesek << "\t" << setw(4) << tmb[i].pontszam << endl;
	}
	cout << endl;
}

string Loveszet::min_ket_talalat_kivalogat() {
	/*
	Eld�nt�s t�tel�vel azonos�tja azon versenyz�ket, akiknak van legal�bb kett� egym�s k�vet� tal�lata
	V�gigmegy a versenyz�k strukt�ra t�mbj�nek char l�v�srekordjain (0-t�l n-1 -ig), 
	�s megvizsg�lja, hogy adott elem �s az ut�na k�vetkez� elem egyar�nt plusz jel-e.
	Ha igen, van legal�bb k�t egym�st k�vet� tal�lt l�v�s

	return string: a versenyz�k rajtsz�mai egybef�z�tt stringk�nt, sz�k�zzel elv�lasztva
	*/
	string s;
	int j;
	char *c;
	c = new char [v];
	int db=0;

	for (int i = 0; i < v; i++) {
		j = 0; // 0-t�l (n-1)-ig menve megn�zz�k, az aktu�lis �s a k�v. elem is +
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
	str.find funkci�val azonos�tja azon versenyz�ket, akiknak van legal�bb kett� egym�s k�vet� tal�lata
	V�gigmegy a versenyz�k strukt�ra t�mbj�nek string l�v�srekordjain,
	�s megvizsg�lja, a rekordjukban, szerepel-e a "++" string

	return string: a versenyz�k rajtsz�m egybef�z�tt stringk�nt, sz�k�zzel elv�lasztva
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
	Maximum kiv�laszt�s elv�vel megkeresi a legt�bb l�v�st lead� versenyz�t
	Egyenl�s�g eset�n a kisebb rajtsz�m� versenyz� sz�m�t maximumnak

	return int: a legt�bb l�v�st lead� versenyz� rajtsz�ma
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
	Kisz�m�tja a versenyz�k strukt�ra t�mbje egy adott elem�nek a pontsz�m�rt�k�t 
	a leadott l�v�sek szimb�lumai alapj�n.
	A f�ggv�ny egyszerre egy versenyz� l�v�seinek rekordj�t kapja meg.
	Szab�ly: 
	- 20 pontot �r a legels� c�lpont eltal�l�sa. 
	- Minden c�lt t�vesztett l�v�s eggyel cs�kkenti a k�vetkez� tal�lattal megszerezhet� pontok sz�m�t

	char *sor: a f�jl adott sor�b�l k�pzett karaktert�mb 0. elem�nek mem�riac�me
	int hossz: a karaktert�mb elemsz�ma (a kezd�c�mt�l val� elt�r�s)
	
	return int: a versenyz� pontsz�ma
	*/
	int aktpont = 20, ertek = 0;
	for (int i = 0; i < hossz; i++) {							// v�gigmegy�nk a rekord karakterein
		if ((aktpont > 0) && (sor[i] == MINUSZ)) aktpont--;		// ha nem tal�lt, �s a megszerezhet� pontsz�m m�g nem 0, cs�kkentj�k a megszerezhet� pontsz�mot 1-el
		else ertek += aktpont;									// ha tal�lt, az eddig megszerzett pontsz�mokhoz hozz�adjuk az aktu�lisan megszerezhet� pontsz�mot
	}
	return ertek;
}

int Loveszet::ossztalalt(int rajtszam) {
	/*
	Megsz�ml�l�s t�tel�nek m�dszer�vel a f�ggv�ny kisz�molja, �ssz. h�ny tal�lat ("+") volt

	int rajtszam: a rendezetlen t�mb indexe, ami a rajtsz�m
	
	return int: az �sszes tal�lat sz�ma
	*/
	int talalt=0, r = rajtszam - 1;
	for (int i = 0; i < tmb[r].l; i++) {
		if (tmb[r].clovesek[i] == PLUSZ) talalt++;
	}
	return talalt;
}

string Loveszet::talalatok_sorszamai(int rajtszam) {
	/*
	Adott sorozatra/rekordra visszaadja, h�nyadik l�v�sek tal�ltak c�lt.

	int rajtszam: a rendezetlen t�mb indexe, ami a rajtsz�m
	
	return string: a c�lt tal�lt l�v�sek sorsz�mai, sz�k�z�kkel elv�lasztott stringg� �sszef�zve
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
	Adott sorozatra/rekordra visszaadja, h�ny l�v�sb�l �llt a leghosszabb pozit�v sorozat

	int rajtszam: a rendezetlen t�mb indexe, ami a rajtsz�m
	
	return int: a leghosszabb pozit�v sorozat nagys�ga (h�ny tal�latb�l �llt a leghosszabb sorozat)
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
	Bubor�kos elvvel sorba rendezi a versenyz� strukt�ra t�mb�t, majd helyez�st rendel a versenyz�kh�z.

	Versenyzo pointer a: versenyz� t�mb 0. elem�nek mem�riac�me
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
	A megadott f�jlba export�lja a sorbarendezett rangsor nev� Versenyz� strukt�ra t�mb�t,
	adott form�tumban (helyez�s, rajtszm, pontsz�m), pontszm szerint cs�kken� sorrendben

	char pointer filenev: az export�land� f�jl neve (vagy teljes el�r�si �tvonala)
	*/
	ofstream ki(filenev);
	cout.setf(ios::left);
	cout << "\n\nHELY.\tRAJTSZ.\tPONT\n";
	for (int i = 0; i < v; i++) {
		cout << rangsor[i].helyezes << (char)9 << rangsor[i].rajtszam << (char)9 << rangsor[i].pontszam << endl;
		ki << rangsor[i].helyezes << (char)9 << rangsor[i].rajtszam << (char)9 << rangsor[i].pontszam << endl;
	}
}