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
	FILE *fp;
	loescheBild(pBild);

	fp = fopen(dateiName, "rb");
	if (fp == NULL)
	{
		printf("Das hat nicht geklappt!");
	}
	else
	{

	}
	return FALSE;
}

// ============================================================================================================
void spiegelnBild(TBild *pBild)
{
	// Aufgabe 4.2. Hier eintragen
	return;
}