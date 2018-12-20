/****************************************************************
Aufgabe 4.1
Autor Georg Westerkamp
Datum 12.12.2018
Kurzbeschreibung: Diese Funktionen dienen zum Einlesen einer BMP Datei und deren Darstellung.
*****************************************************************/

#include "main.h"

// ============================================================================================================
void erzeugenTestbild(TBild *pBild)
{
	// Das Bild wird zeilenweise gespeichert. 
	// Da die Zeilenlänge nicht konstant ist, wird für jede Zeile dynamisch Speicher angefordert 
	// und zwar genau so viel, wie für dieses Bild die Zeile lang ist.
	// Jedes Pixel belegt dabei sizeof(COLORREF) Bytes.

	// Da auch die Anzahl der Zeilen nicht konstant ist, werden die Zeilen als verkettete Liste geführt.
	// pBild->pBitmap zeigt auf das erste Listen(=Zeilen)-Element bestimmt also den Anfang der Liste.

	// Wir haben es hier also mit *zwei* dynamischen Strukturen zu tun, einmal der verketteten Liste der Zeilen
	// vom Typ TBmpZeilenElement und dann für jede Zeile einen Zeiger auf die Pixel dieser Zeile
	// TBmpZeilenElement.pPixelZeile.

	// Achten Sie darauf, dass Sie ein bisheriges Bild ggf erst wieder freigeben, bevor Sie ein neues Bild erzeugen.

	TBmpZeilenElement *pLast = NULL;

	loescheBild(pBild);										// eventuell vorhandes Bild löschen.

	pBild->breite = 400;
	pBild->hoehe = 400;

	for (int zeile = 0; zeile < pBild->hoehe; ++zeile)
	{
		// zerst eine neues Zeielen-Element anlegen
		TBmpZeilenElement *pZeile = (TBmpZeilenElement*)malloc(sizeof(TBmpZeilenElement));
		if (pZeile == NULL)
		{
			MessageBox(NULL, "kein Speicher mehr", "Fehler", MB_OK | MB_ICONERROR);
			return;
		}
		// verlinkung mit Nachfolger
		pZeile->next = pLast;
		pLast = pZeile;

		// jetzt für die Pixel dieser Zeile Speicher anfordern.
		pZeile->pPixelZeile = (COLORREF*)malloc(sizeof(COLORREF)*pBild->breite);
		if (pZeile->pPixelZeile == NULL)
		{
			MessageBox(NULL, "kein Speicher mehr", "Fehler", MB_OK | MB_ICONERROR);
			return;
		}
		// Die Pixel dieser Zeile mit einem bestimmten willkürlichen Muster füllen.
		for (int x = 0; x < pBild->breite; ++x)
		{
#define QUANTISIERER(x) ((((x)*32)/32) % 256)
			const int rotAnteil = QUANTISIERER(x + zeile);
			const int gruenAnteil = QUANTISIERER(zeile);
			const int blauAnteil = QUANTISIERER(x);

			const COLORREF pixel = RGB(rotAnteil, gruenAnteil, blauAnteil);
			pZeile->pPixelZeile[x] = pixel;
		}
	}

	pBild->pBitmap = pLast;
	strcpy_s(&pBild->dateiName[0], sizeof(pBild->dateiName), "<Testbild>");
}

// ============================================================================================================
void zeichnenBild(const HDC hdc, const TBild *pBild)
{
	int zeile = 0;
	TBmpZeilenElement *pZeile = pBild->pBitmap;

	while (pZeile != NULL)
	{
		const COLORREF *pPixel = pZeile->pPixelZeile;

		for (int x = 0; x < pBild->breite; ++x)
		{
			SetPixel(hdc, x, zeile, *pPixel);
			++pPixel;
		}

		++zeile;
		pZeile = pZeile->next;
		
	}
}

// ============================================================================================================
void loescheBild(TBild *pBild)
{
	TBmpZeilenElement *pAktZeile = pBild->pBitmap;

	while (pAktZeile != NULL)
	{
		// zuerst den Pixel Speicher für diese Zeile freigeben
		if (pAktZeile->pPixelZeile)
		{
			free(pAktZeile->pPixelZeile);
			pAktZeile->pPixelZeile = NULL;
		}

		// jetzt das Listenelement löschen
		{
			TBmpZeilenElement *pHilf = pAktZeile;
			pAktZeile = pAktZeile->next;
			free(pHilf);
		}
	}

	// zum Schluss das Bild "ausnullen".
	pBild->breite = 0;
	pBild->hoehe = 0;
	pBild->dateiName[0] = 0;
	pBild->pBitmap = NULL;
}

// ============================================================================================================
BOOL ladeDatei(const char *dateiName, TBild *pBild)
{
	// Aufgabe 4.1. Hier eintragen.
	/*TBild *pbild = (TBild*)malloc(sizeof(TBild));*/
	
	//a) Öffnen Sie die Datei „dateiName“ als Binärdatei zum Lesen.
	TBild *anfang = NULL;
	TBild *headerInfo = NULL;
	FILE *fp;
	int i = 0;
	int read;
	fp = fopen(dateiName, "rb");
	if (fp == NULL)
	{
		printf("Das hat nicht geklappt!\n");
		MessageBox(NULL, "kein Speicher mehr", "Fehler", MB_OK | MB_ICONERROR);

		return FALSE;
	}
	else
	{
		while(!feof(fp))
		{
			i++;
			headerInfo = (TBild*)realloc(headerInfo, (i + 1) * sizeof(TBild));

			if (headerInfo == NULL)
			{
				printf("Fehler bei Erweiterung des dynamischen Arrays!");
				return TRUE;
			}
			else
			{
				read = (fread((headerInfo + i), sizeof(TBild), 1, fp));
				if (read == 1)
				{
					headerInfo->pBitmap->next = anfang;
					anfang = headerInfo;
				}
			}
		}
		MessageBox(NULL, "top", "Cool", MB_OK | MB_ICONERROR);
		fclose(fp);
		return anfang;
	}
	//b) Löschen Sie das aktuelle Bild mit der entsprechenden Funktion.
	loescheBild(pBild);
	//c) Lesen Sie den Dateiheader aus und prüfen Sie, ob es sich um eine im 
	//	Sinne dieser Aufgabenstellung gültige BMP - Datei handelt.

	//d) Positionieren Sie den File - Pointer an die Stelle, an der die Bilddaten beginnen.
	//e) Berechnen Sie, wie viele Bytes eine Bildzeile in der BMP Datei lang ist.Dabei beachten : 
	//• Jeder Pixel benötigt genau drei Bytes.
	//• Das Ende einer Zeile ist so lange mit 0 - Bytes aufgefüllt, bis dass die Anzahl der Bytes 
	//	durch 4 teilbar ist, d.h.die nächste Zeile fängt immer an einer durch 4 teilbaren Adresse an.
	//f) Berechnen Sie, wie viele Zeilen das Bild enthält, beachten Sie die Angaben zu biHeight bezüglich des Vorzeichens.
	//g) Erzeugen Sie dynamisch einen Zwischenpuffer, der eine ganze Zeile aus der Datei aufnehmen kann.
	//h) Berechnen Sie, wie viele Bytes eine Zeile im TBild benötigt.Dieser Wert ist anders 
	//	als der vorige des Zwischenpuffers!Die Pixel im TBild sind alle vom Typ COLORREF mit sizeof(COLORREF) == 4. 
	//i) Erzeugen Sie eine verkettete Liste von Zeilen(TBmpZeilenElement) und erzeugen Sie für jede Zeile mit malloc() 
	//	dynamisch einen Speicherbereich, der eine solches Zeilenelement aufnehmen kann.
	//j) Lesen Sie zeilenweise die Bilddaten aus der Datei und legen Sie sie zunächst im Zwischenpuffer ab.
	//k) Übertragen Sie die Pixel aus dem Zwischenpuffer in die TBild - Zeile.Dabei beachten : 
	//	• Die Pixel werden durch Tupel von drei Bytes dargestellt.Ein Byte liefert den rot - Anteil, 
	//		eines den grün - Anteil und eines den blau - Anteil.Hohe Werte bedeuten hohe Helligkeit, 
	//		d.h.das Tupel(0, 0, 0) bedeutet schwarz und das Tupel(255, 255, 255) bedeutet weiß.
	//	• In der BMP Datei sind die Werte blau / grün / rot sortiert, d.h.erst kommt immer der blauAnteil,
	//		dann der grüne, dann der rote.
	//	• Im Windows - System verwenden Sie für die Umwandlung das Makro „RGB(…), 
	//		wobei hier die Reihenfolge rot / grün / blau ist.Mit anderen Worten :
	//	Die Pixel - Zeilen für Windows bestehen aus COLORREF - Werten, die Sie wie folgt aus den RGB Werten erzeugen können :
	//l) Am Ende sorgen Sie dafür, dass alle Felder des TBild mit den richtigen Werten ausgefüllt sind und return mit TRUE;
	//	Wenn Sie alles richtig gemacht haben, wir das Bild dann angezeigt.
	//m) Wenn Sie während des Einlesens einen Fehler entdecken, dann return FALSE.
	//	… int rot = …; // aus dem Zwischenpuffer int gruen = …; int blau = …; 

	//	COLORREF cr = RGB(rot, gruen, blau);   …
	//		Georg Westerkamp Programmieren II WS 2018 / 2019
	//		5 / 5
	//n) Sorgen Sie im Fehlerfall dafür, dass alle nicht mehr benötigten dynamischen Speicherbereiche wieder freigegeben werden und alle Dateien auf jeden Fall geschlossen werden !



	
	return FALSE;
}

//TBild *oeffneBild(TBild *filmarray, FILE *pDatei)
//{
//	int i = 0;
//
//	while (fread((filmarray + i), sizeof(TFilm), 1, pDatei))
//	{
//		i++;
//		filmarray = (TFilm*)realloc(filmarray, (i + 1) * sizeof(TFilm));
//
//		if (filmarray == NULL)
//		{
//			printf("Fehler bei Erweiterung des dynamischen Arrays!");
//			return 0;
//		}
//	}
//	anzahlFilme = i - 1;
//
//	return filmarray;
//}

// ============================================================================================================
void spiegelnBild(TBild *pBild)
{
	// Aufgabe 4.2. Hier eintragen
	return;
}