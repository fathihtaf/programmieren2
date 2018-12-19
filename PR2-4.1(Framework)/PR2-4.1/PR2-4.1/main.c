/****************************************************************
Aufgabe 4.1
Autor Georg Westerkamp
Datum 12.12.2018
Kurzbeschreibung: Das Programm stellt den Rahmen für einen Bildbetrachter unter Windows bereit.
In *dieser* Datei keine Änderungen vornehmen.

Die hier verwendeten Methoden nutzen das sogenannte Win32 API, welches 
z.B. hier  https://docs.microsoft.com/en-us/windows/desktop/learnwin32/painting-the-window
erklärt wird.
Die genenauen Umstände sind für das Verständnis und die Lösung dieser Aufgabe nicht
notwendig.
*****************************************************************/

#include "main.h"

// Globale Variablen:
HINSTANCE hInst;									// Aktuelle Instanz
WCHAR szWindowClass[] = L"BMP-Betrachter";			// Klassenname des Hauptfensters				
TBild Bild = { 0 };									// Das Bild. Hierauf nicht direkt zugreifen.!

// Vorwärtsdeklarationen der in diesem Codemodul enthaltenen Funktionen:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Das ist der Titel des Hauptfensters
const char bmpFensterTitel[] = "BMP-Betrachter";

// ============================================================================================================
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MyRegisterClass(hInstance);

	erzeugenTestbild(&Bild);

	// Anwendungsinitialisierung ausführen:
	if (!InitInstance(hInstance, nCmdShow)) return FALSE;

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PR241));

	MSG msg;

	// Hauptnachrichtenschleife:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

// ============================================================================================================
//
//  FUNKTION: MyRegisterClass()
//
//  ZWECK: Registriert die Fensterklasse.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PR241));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PR241);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

// ============================================================================================================
//
//   FUNKTION: InitInstance(HINSTANCE, int)
//
//   ZWECK: Speichert das Instanzenhandle und erstellt das Hauptfenster.
//
//   KOMMENTARE:
//
//        In dieser Funktion wird das Instanzenhandle in einer globalen Variablen gespeichert, und das
//        Hauptprogrammfenster wird erstellt und angezeigt.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Instanzenhandle in der globalen Variablen speichern

	HWND hWnd = CreateWindowW(szWindowClass, (LPWSTR) bmpFensterTitel, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd) return FALSE;			// Fenster konnte nicht angelegt werden

	{
		HMENU hMenuCurrentBar = GetMenu(hWnd);
		HMENU hDatei = GetSubMenu(hMenuCurrentBar, 0);

		{ // Menu-Eintrag "Testbild laden" hinzufügen
			MENUITEMINFO mif = { 0 };
			mif.cbSize = sizeof(mif);
			mif.fMask = MIIM_STRING | MIIM_ID;
			mif.fType = 0;
			mif.dwTypeData = (LPSTR)"Testbild auswählen";
			mif.cch = strlen(mif.dwTypeData);
			mif.fState = MFS_ENABLED;
			mif.wID = IDM_ACTIVATETEST;
			InsertMenuItem(hDatei, 0, TRUE, &mif);
		}

		{ // Menu-Eintrag "Bild laden" hinzufügen
			MENUITEMINFO mif = { 0 };
			mif.cbSize = sizeof(mif);
			mif.fMask = MIIM_STRING | MIIM_ID;
			mif.fType = 0;
			mif.dwTypeData = (LPSTR)"Bild laden";
			mif.cch = strlen(mif.dwTypeData);
			mif.fState = MFS_ENABLED;
			mif.wID = IDM_OPENFILE;
			InsertMenuItem(hDatei, 1, TRUE, &mif);
		}


		{ // Trennstrich hinzufügen.
			MENUITEMINFO mif = { 0 };
			mif.cbSize = sizeof(mif);
			InsertMenuItem(hDatei, 2, TRUE, &mif);
		}
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

// ============================================================================================================
//
//  FUNKTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ZWECK:  Verarbeitet Meldungen vom Hauptfenster.
//
//  WM_COMMAND  - Verarbeiten des Anwendungsmenüs
//  WM_PAINT    - Darstellen des Hauptfensters
//  WM_DESTROY  - Ausgeben einer Beendenmeldung und zurückkehren
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Menüauswahl bearbeiten:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			MessageBox(NULL, "... und tschüss", "Programm verlassen", MB_OK);

			DestroyWindow(hWnd);
			break;
		case IDM_OPENFILE:
		{
			char dateiname[MAX_PATH] = { 0 };
			BOOL b = erfrageDateiname(hWnd, &dateiname[0], sizeof(dateiname));
			if (b)
			{
				//MessageBox(NULL, dateiname, "Sie haben ausgewählt", MB_OK);

				// Datei laden
				ladeDatei(dateiname, &Bild);

				// windows mitteilen, dass wir den Fensterinhalt geändert haben.
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		case IDM_ACTIVATETEST:
		{
			erzeugenTestbild(&Bild);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		char titelPuffer[200] = { 0 };
		vorbereitenUndBildZeichnen(hWnd, &Bild);

		// Titelzeile vorbereiten und setzen
		if (&Bild.dateiName[0] != 0)
			sprintf_s(&titelPuffer[0], sizeof(titelPuffer), "%s - %s", bmpFensterTitel, &Bild.dateiName[0]);
		else
			sprintf_s(&titelPuffer[0], sizeof(titelPuffer), "%s", bmpFensterTitel);

		// ... setzen
		SetWindowTextW(hWnd, (LPWSTR)titelPuffer);
	}
	break;
	case WM_DESTROY:
		loescheBild(&Bild);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ============================================================================================================
// Meldungshandler für Infofeld.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// ============================================================================================================
BOOL erfrageDateiname(const HWND hWnd, char *pufferDateiName, const int pufferGroesse)
{
	BOOL rueckgabe = FALSE;

	OPENFILENAME ofn;									// dieses Struktur braucht GetOpenFileNam
	memset((void*)&ofn, 0, sizeof(ofn));				// die ganze Struktur auf 0 setzen
	ofn.lStructSize = sizeof(ofn);						// muss so stehen gemäß Anleitung
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = pufferDateiName;					// hier hinein wird der Dateiname gelegt.
	ofn.nMaxFile = pufferGroesse;						// und der Puffer kann so viele Zeichen aufnehmen.
	ofn.lpstrFilter = "Alle\0*.*\0nur BMP\0*.BMP\0";	// ein Filter, welche Dateien angezeigt werden sollen.
	ofn.nFilterIndex = 2;								// wählt hier den zweiten Filter als default
	ofn.lpstrTitle = "Wählen Sie die Datei, die angezeigt werden soll";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (pufferGroesse < 10) return rueckgabe;			// puffer ist zu klein
	pufferDateiName[0] = 0;								// damit gibt es keine Vorgabe, wo die Suche beginnen soll

	rueckgabe = GetOpenFileName(&ofn);					// jetzt User nach Dateinamen fragen.

	return rueckgabe;
}

// ============================================================================================================
void vorbereitenUndBildZeichnen(const HWND hWnd, const TBild *pBild)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	// Den Bildschirm mit einem Muster füllen
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	HRGN bgRgn = CreateRectRgnIndirect(&clientRect);

	// Hintergrund füllen
	HBRUSH hBrush = CreateSolidBrush(RGB(100, 100, 100));
	FillRgn(hdc, bgRgn, hBrush);

	// Muster-Fläche
	HPEN hPen = CreatePen(PS_DOT, 1, RGB(0, 255, 0));
	SelectObject(hdc, hPen);
	SetBkColor(hdc, RGB(0, 0, 0));
	Rectangle(hdc, 0, 0, pBild->breite, pBild->hoehe);

	// Text caption
	////SetBkColor(hdc, RGB(255, 255, 255));
	////RECT textRect = { 10, 210, 200, 200 };
	////DrawText(hdc, TEXT("Georg Test"), -1, &textRect, DT_CENTER | DT_NOCLIP);

	zeichnenBild(hdc, pBild);

	// Clean up
	DeleteObject(bgRgn);
	DeleteObject(hBrush);
	DeleteObject(hPen);

	GetStockObject(WHITE_BRUSH);
	GetStockObject(DC_PEN);

	EndPaint(hWnd, &ps);
}


