#include "Loveszet.h"

Loveszet::Loveszet(char* filenev) {
	/*
	A L�v�szet oszt�ly konstruktorja

	Megnyitja az argumentumban szerepl� f�jlt,
	megsz�molja a sorokat, az els� sorban megn�zi, hogy sz�m van-e,
	�s ez egyezik-e a sorok sz�m�val (ami a versenyz�k sz�ma)
	Megvizsg�lja, a tov�bbi sorokban kiz�r�lag �rv�nyes karakterek (+ �s-) szerepelnek-e.
	Ha ezek valamelyike nem igaz, a program kil�p, mivel a f�jl hib�s.
	
	Amennyiben a f�jl nem hib�s, az adatokat egy strukt�r�ba olvassa a program,
	�s sorba rendezi �ket.

	@param char pointer: filenev - a megnyitand� f�jl neve avagy el�r�si �tvonala
	*/
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

	// megsz�moljuk a f�jlban szerepl� sorok sz�m�t
	while (!be.eof()) {
		getline(be, sor);
		if (sor != "") {
			sorok_szama++;
		}
	}
	be.clear();		// kit�r�lj�k az eof flaget
	be.seekg(0);	// visszaugrunk a f�jl elej�re, hogy beolvashassuk az �rt�keket

	be >> sor; // beolvassuk az els� sort, mely a versenyz�k sz�m�t jel�li
	// ha az els� sor numerikus, elmentj�k sz�mk�nt, ha nem, a program kil�p
	if (ervenyes_szoveg(sor, "0123456789")) { 
		fajlban_letszam = stoi(sor); 
	}
	else {
		cout << "Ervenytelen fajl!\nAfajl elso soraban szamnak (a versenyzok szamanak) kell szerepelnie.\n\n";
		system("pause"); exit(EXIT_FAILURE);
	}

	// ha a f�jlban szerepl� l�tsz�m nem egyezik a sorok sz�m�val (m�nusz 1, mert az els� sorban maga a l�tsz�m szerepel), a f�jl hib�s
	if (fajlban_letszam != sorok_szama - 1) {
		cerr << "Hibas fajl!\nA fajl elso soraban levo letszam nem egyezik a fajlban tarolt adatok szamaval."
			"A fajl nem feldolgozhato." << endl << endl;
		system("pause"); exit(EXIT_FAILURE);
	}

	//v - a versenyz�k sz�ma (sorok sz�ma, m�nusz 1, mert az els� sorban maga a l�tsz�m szerepel)
	Loveszet::v = sorok_szama - 1;
	Loveszet::tmb = new Versenyzo[v];
	int nemerv_index=0;

	// beolvasunk minden nem �res sort, �s megn�zz�k, hogy csak �rv�nyes karakterek szerepelnek-e benne
	// ha �rv�nyes a sor, egy strukt�ra t�mbben elment�dnek az adatok
	while (!be.eof() && i < v && be >> tmb[i].clovesek) {
		tmb[i].slovesek = tmb[i].clovesek;
		tmb[i].l = tmb[i].slovesek.length();
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
		cerr << endl << nemerv_index << " ervenytelen rekord a fajlban, a fajl nem feldolgozhato!\n"
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
	@param szoveg: szoveg - konstans referencia string - a sz�veg, amelyet vizsg�lunk
	@param erv_karakterek: erv_karakterek - konstans string (opcion�lis) - az elfogadott karakterekb�l �ll� string, alap�rtelmezetten "+-"

	@return bool - true, ha csak �rv�nyes karaktert tartalmaz a sz�veg, false, ha legal�bb 1 �vr�nytelent
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
	@param szoveg: szoveg - char mutat� - a karaktersorozat 0. elem�re mutat
	@param erv_karaktere: erv_karakterek - integer - a karaktersorozat hossz�t tartalmazza

	@return bool - true, ha csak �rv�nyes karaktert tartalmaz a sz�veg, false, ha legal�bb 1 �vr�nytelent
	*/
	int i = 0;
	while (((szoveg[i] == PLUSZ) || (szoveg[i] == MINUSZ)) && i < h) i++;
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

string Loveszet::min_ket_talalat_eldontessel() {
	/*
	Eld�nt�s t�tel�vel azonos�tja azon versenyz�ket, akiknak van legal�bb kett� egym�s k�vet� tal�lata
	V�gigmegy a versenyz�k strukt�ra t�mbj�nek char l�v�srekordjain (0-t�l n-1 -ig), 
	�s megvizsg�lja, hogy adott elem �s az ut�na k�vetkez� elem egyar�nt plusz jel-e.
	Ha igen, van legal�bb k�t egym�st k�vet� tal�lt l�v�s

	@return a versenyz�k rajtsz�m egybef�z�tt stringk�nt, sz�k�zzel elv�lasztva
	*/
	string s;
	int j;
	for (int i = 0; i < v; i++) {
		j = 0; // 0-t�l (n-1)-ig menve megn�zz�k, van-e olyan elem, ahol az akut�lis �s a k�v. elem is +
		while ((tmb[i].clovesek[j] != PLUSZ || tmb[i].clovesek[j + 1] != PLUSZ) && j < (tmb[i].l - 1)) {
			j++;
		}
		if (j < (tmb[i].l - 1)) {
			s += to_string(i + 1) + " ";
		}
	}
	return s;
}

string Loveszet::min_ket_talalat_stringfinddal() {
	/*
	str.find funkci�val azonos�tja azon versenyz�ket, akiknak van legal�bb kett� egym�s k�vet� tal�lata
	V�gigmegy a versenyz�k strukt�ra t�mbj�nek string l�v�srekordjain,
	�s megvizsg�lja, a rekordjukban, szerepel-e a "++" string

	@return a versenyz�k rajtsz�m egybef�z�tt stringk�nt, sz�k�zzel elv�lasztva
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

	@return int - a legt�bb l�v�st lead� versenyz� rajtsz�ma
	*/
	int maxlovo = 0;
	if (v > 1) {
		for (int i = 1; i < v; i++) {
			if (tmb[i].l > tmb[maxlovo].l) maxlovo = i;
		}
	}
	return maxlovo;
}

int Loveszet::loertek(char *sor, int hossz) {
	/*
	Kisz�m�tja a versenyz�k strukt�ra t�mbje egy adott elem�nek a pontsz�m�rt�k�t 
	a leadott l�v�sek szimb�lumai alapj�n.
	A f�ggv�ny egyszerre egy versenyz� l�v�seinek rekordj�t kapja meg.
	Szab�ly: 
	- 20 pontot �r a legels� c�lpont eltal�l�sa. 
	- Minden c�lt t�vesztett l�v�s eggyel cs�kkenti a k�vetkez� tal�lattal megszerezhet� pontok sz�m�t

	@param char *sor: a f�jl adott sor�b�l k�pzett karaktert�mb 0. elem�nek mem�riac�me
	@param int hossz: a karaktert�mb elemsz�ma (a kezd�c�mt�l val� elt�r�s)
	@return int: a versenyz� pontsz�ma
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
	�sszegz�s t�tel�nek m�dszer�vel a f�ggv�ny kisz�molja, �ssz. h�ny tal�lat ("+") volt

	@param int rajtszam: a rendezetlen t�mb indexe, ami a rajtsz�m
	@return int: az �sszes tal�lat sz�ma
	*/
	int talalt=0, r = rajtszam - 1;
	for (int i = 0; i < tmb[r].l; i++) {
		if (tmb[r].clovesek[i] == PLUSZ) talalt++;
	}
	return talalt;
}

string Loveszet::talalatok_sorszamai(int rajtszam) {
	string s;
	int r = rajtszam - 1;
	for (int i = 0; i < tmb[r].l; i++) {
		if (tmb[r].clovesek[i] == PLUSZ) s += to_string(i + 1) + " ";
	}
	return s;
}

int Loveszet::leghosszabb_sorozat(int rajtszam) {
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