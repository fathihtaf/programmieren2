#ifndef MAIN_H
#define MAIN_H

#define _CRT_SECURE_NO_WARNINGS
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             // Selten verwendete Komponenten aus Windows-Headern ausschlie�en
#include <windows.h>					// Windows Headerdatein, um die graphische Oberl�che nutzen zu k�nnen.

// C RunTime-Headerdateien
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <commdlg.h>
#include "resource.h"
#include <stdio.h>

// selbst definiert Datentypen.
typedef struct _BmpZeilenElement
{
	COLORREF					*pPixelZeile;
	struct _BmpZeilenElement	*next;
} TBmpZeilenElement;
typedef struct
{
	char				dateiName[MAX_PATH];		// Der Name der Datei, die angezeigt werden soll.
	int					breite;
	int					hoehe;
	TBmpZeilenElement	*pBitmap;
} TBild;

#pragma pack(push,1)	// Dieses Pragma sorgt daf�r, dass der Compiler das struct m�glichst dicht packt und
						// kein F�llbyte dazwischenlegt.
typedef struct _BitmapFileHeader
{
	unsigned short	kKennung;
	unsigned int	dateiGroesseInBytes;	// wert gilt als unzuverl�ssig
	unsigned int	reserved1zero;			// reserviert = 0
	unsigned int	offsetInBytes;			// so viele Bytes nach Dateianfang beginnen die Bilddaten
} TBitmapFileHeader;						// sizeof() == 14

typedef struct _BitmapInfoHeader
{
	unsigned int	biSize;					// sizeof(TBitmapInfoHeader) in Bytes
	unsigned int	biWidth;				// Breite des Bildes in Pixeln
	unsigned int	biHeight;				// H�he des Bildes in Pixeln
											// heightInPix > 0 --> erste Zeile ist unten
											// heightInPix < 0 --> erste Zeile ist oben
	unsigned short	biPlanes;				// immer == 1
	unsigned short	biBitCount;				// 1,4,8,16,24 oder 36.
	unsigned int	biCompression;			// 0 = uncom 1 = RLL(8bit) 2 = RLL(4bit), 3 = palette
	unsigned int	biSizeImage;			// Anzahl der Bilddaten in Bytes oder 0
	unsigned int	biXPelsPerMeter;		// Pixel pro Meter in x-Richtung oder 0
	unsigned int	biYPelsPerMeter;		// ... in y-Richtung oder 0
	unsigned int	biClrUsed;				// Anzahl der verwendeten Farben oder 0
	unsigned int	biClrImportant;			//
} TBitmapInfoHeader;
#pragma pack(pop)

// erfragt den Dateinamen, der Datei, die angezeigt werden soll.
// Das Ergebnis steht dann in pufferDateiName.
// liefert TRUE, wenn ein Name in pufferDateiName eingetragen wurde, sonst FALSE
extern BOOL erfrageDateiname(const HWND hWnd, char *pufferDateiName, const int pufferGroesse);

// Erzeugt ein Test-Bild mit willk�rlichem - aber konstatem - Muster.
// Damit kann die Anzeigefunktion �berpr�ft werden.
extern void erzeugenTestbild(TBild *pBild);

// Bereitet das Windows-Fester f�r ein neues Bild vor.
// DIESE Funktion nichtver�ndern.
extern void vorbereitenUndBildZeichnen(const HWND hWnd, const TBild *pBild);

// Hier wird aus den Daten in pBild das Bild in das Windows-Fenster gezeichnet.
extern void zeichnenBild(const HDC hdc, const TBild *pBild);

// l�scht das Bild und gibt allen tempor�ren Speicher wieder frei.
extern void loescheBild(TBild *pBild);

// L�dt die Datei in pBild.
// Liefert TRUE, wenn das geklappt hat sonst FALSE
extern BOOL ladeDatei(const char *dateiName, TBild *pBild);

// Spiegelt das Bild, so dass es danach "auf dem Kopf" steht.
extern void spiegelnBild(TBild *pBild);

#endif // MAIN_H