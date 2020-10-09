
/********************************************************************
* Description: Implementation of Laser Chess Board Game "Khet"
* Author: David Vulakh
*
* Copyright (c) 2017 David Vulakh.
*
********************************************************************/

#include <windows.h>
#include <gdiplus.h>
#include <stdlib.h>
#include <string.h>
#include <objidl.h>
#include <Unknwn.h>
#include <tchar.h>
#include <chrono>
#include <thread>
#include <stack>

#include "logic.h"

using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

/*** COLOURS ***/
const int CLICKED_FILL = 0x507000FF;
const int LASER_SHIELD = 0xA00050FF;
const int MOUSED_FILL = 0x30FFFFFF;
const int LASER_BLAST = 0xA0FF0000;
const int LASER_BEAM = 0xFF00FF00;
const int MOVE_FILL = 0x5000FF00;
const int TILE_FILL = 0xFF3E3F44;
const int BKGND = 0xFF202020;
const int SHADE = 0xA0202020;

/*** PROPORTIONS ***/
const double TILE_DIM = 0.05;
const double PADY = 0.025;
const double ARROW = 0.2;
const double PADX = 0.1;
const int ARRHW = 2;
int tile_dim;
int arrow;
int padx;
int pady;

/*** BOARD ***/
const string autos = "autosave.aksg";
bool ready = false;
bool turn;
board brd;

/*** WINDOW INFORMATION ***/
static TCHAR szWindowClass[] = _T("Khet");
static TCHAR szTitle[] = _T("Khet");
int width, height;
HINSTANCE hInst;
Graphics* g;
HWND hWnd;

/*** FUNCTIONS ***/
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID paint_laser_flash(tile*, int);
VOID laser(int, int, int, int);
VOID paint_tile(int, int);
VOID paint_tile(tile*);
VOID OnPaint(HDC hdc);
VOID mouse_clicked();
VOID mouse_moved();
VOID paint_board();
VOID background();
BOOL inside(Rect);
VOID key_input();
VOID paint_rot();
Rect ccRect();
Rect cwRect();
Rect cRect();

/*** WINDOW SETUP ***/

//Start window
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{

	WNDCLASSEX wcex;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	///Initialize GDI+
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	///Setup
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	///Notify in case of failure
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	///Store instance handle
	hInst = hInstance;

	///Create board
	brd = board(board::CLASSIC_MODE);
	turn = false;

	///Create the window
	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_BORDER,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 100,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	SetWindowLong(hWnd, GWL_STYLE, 0);

	///Notify in case of failure
	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			szTitle,
			NULL);
		return 1;
	}

	///Maximize window
	ShowWindow(
		hWnd,
		SW_MAXIMIZE);
	UpdateWindow(hWnd);

	///Compute dimensions
	RECT windr;
	g = new Graphics(hWnd);
	GetClientRect(hWnd, &windr);
	width = windr.right - windr.left;
	height = windr.bottom - windr.top;
	tile_dim = width * TILE_DIM;
	arrow = height * ARROW;
	padx = width * PADX;
	pady = width * PADY;

	///Paint board
	ready = true;
	background();
	paint_board();

	///Mainloop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		key_input();
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return (int)msg.wParam;

}

//Process messages
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_ERASEBKGND:
		background();
	case WM_PAINT:
		OnPaint(BeginPaint(hWnd, &ps));
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONUP:
		mouse_clicked();
		break;
	case WM_MOUSEMOVE:
		mouse_moved();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;

}

/*** UI ***/

//String utilities
wstring to_wstring(string s)
{
	wstring ws(s.begin(), s.end());
	return ws;
}
string to_string(wchar_t *w)
{
	wstring ws(w);
	string s(ws.begin(), ws.end());
	return s;
}

//Rectangles
///Get rectangle for a given tile
Rect tileRect(int r, int c) { return Rect(padx + c * tile_dim, pady + r * tile_dim, tile_dim, tile_dim); }
///Get rectangle for turn symbol
Rect cRect()
{
	int y = (pady + board::NUM_ROW * tile_dim + height) / 2;
	int x = padx + board::NUM_COL * tile_dim / 2;
	return Rect(
		x - height * ARROW / 2,
		y - height * ARROW / 2,
		height * ARROW,
		height * ARROW
	);
}
///Get cw arrow rectangle
Rect cwRect() { return Rect(cRect().X - height * ARROW / ARRHW, cRect().Y, height * ARROW / ARRHW, height * ARROW); }
///Get cc arrow rectangle
Rect ccRect() { return Rect(cRect().X + cRect().Width, cRect().Y, height * ARROW / ARRHW, height * ARROW); }

//Mouse input
///Get tile under mouse
tile* get_tile()
{
	POINT ms;
	GetCursorPos(&ms);
	int c = (ms.x - padx) / tile_dim;
	int r = (ms.y - pady) / tile_dim;
	if (ms.x >= padx && ms.y >= pady && r >= 0 && r < board::NUM_ROW && c >= 0 && c < board::NUM_COL)
		return brd.tiles[r][c];
	return NULL;
}
///Return whether mouse over a rectangle
BOOL inside(Rect r)
{
	POINT ms;
	GetCursorPos(&ms);
	return
		ms.x > r.X &&
		ms.x - r.X < r.Width &&
		ms.y > r.Y &&
		ms.y - r.Y < r.Height;
}
///Mouse moved response: highlight tile
VOID mouse_moved()
{
	if (brd.mouse == get_tile())
		return;
	if (brd.mouse != NULL)
		paint_tile(brd.mouse);
	brd.mouse = get_tile();
	if (brd.mouse != NULL) {
		SolidBrush b(MOUSED_FILL);
		g->FillRectangle(&b, tileRect(brd.mouse->r, brd.mouse->c));
	}
}
///Mouse clicked
VOID mouse_clicked() try
{

	///First click -- select a piece
	if (brd.click == NULL) {
		if (get_tile() != NULL && get_tile()->p != NULL && brd.silver_move == get_tile()->p->silver) {

			///Show as selected
			brd.click = get_tile();
			brd.click->select = CLICKED_FILL;
			paint_tile(brd.click);

			///Display legal moves
			get_moves(brd.move, brd.click);
			for (tile* t : brd.move) {
				t->select = MOVE_FILL;
				paint_tile(t);
			}

		}
	}

	///Second click
	else {

		///Unselect
		if (get_tile() != NULL && brd.click == get_tile()) {

			///Show as unselected
			brd.click->select = 0;
			paint_tile(brd.click);
			brd.click = NULL;

			///Unselect moves
			for (tile* t : brd.move) {
				t->select = 0;
				paint_tile(t);
			}
			brd.move.clear();

		}

		///Take legal move
		else {

			///Legal move
			bool moved = false;

			///Normal move
			if (get_tile() != NULL && legal_move(brd.click, get_tile())) {
				brd.click->p->step(get_tile());
				paint_tile(get_tile());
				moved = true;
			}

			///Rotations
			else if (inside(cwRect())) {
				brd.click->p->dir = (brd.click->p->dir + 1) % piece::DIR;
				moved = true;
			}
			else if (inside(ccRect())) {
				brd.click->p->dir = (piece::DIR + brd.click->p->dir - 1) % piece::DIR;
				moved = true;
			}

			///Cleanup
			if (moved) {

				///Unselect moves
				brd.click->select = 0;
				paint_tile(brd.click);
				brd.click = NULL;
				for (tile* t : brd.move) {
					t->select = 0;
					paint_tile(t);
				}
				brd.move.clear();

				///Fire laser
				int lx, ly;
				list<tile*> l;
				brd.repaint->clear();
				if (brd.silver_move) {
					l = ((sphinx*)(brd.tiles[7][9]->p))->fire();
					lx = tileRect(7, 9).X + tileRect(7, 9).Width / 2;
					ly = tileRect(7, 9).Y + tileRect(7, 9).Height / 2;
				}
				else {
					l = ((sphinx*)(brd.tiles[0][0]->p))->fire();
					lx = tileRect(0, 0).X + tileRect(0, 0).Width / 2;
					ly = tileRect(0, 0).Y + tileRect(0, 0).Height / 2;
				}
				brd.silver_move = !brd.silver_move;
				tile* last = NULL;
				for (tile* t : l) {
					if (t != NULL) {
						///Draw laser
						int x, y;
						x = tileRect(t->r, t->c).X + tileRect(t->r, t->c).Width / 2;
						y = tileRect(t->r, t->c).Y + tileRect(t->r, t->c).Height / 2;
						laser(lx, ly, x, y);
						lx = x;
						ly = y;
						if (t->p != NULL && (typeid(*(t->p)) != typeid(pyramid)))
							paint_tile(t);
						if (last != NULL && last->p != NULL && (typeid(*(last->p)) != typeid(pyramid)))
							paint_tile(last);
						///Sleep
						this_thread::sleep_for(chrono::milliseconds(250));
						///Kill
						if (t == last) {
							if (t->p != NULL)
								paint_laser_flash(t, LASER_BLAST);
							///Pharaoh died
							if (typeid(*(t->p)) == typeid(pharaoh))
								if (t->p->silver)
									MessageBox(NULL,
										_T("Game Over: Red Wins"),
										szTitle,
										NULL);
								else
									MessageBox(NULL,
										_T("Game Over: Silver Wins"),
										szTitle,
										NULL);
							t->p = NULL;
							paint_tile(t);
						}
						last = t;
					}
					///Shield
					else
						if (last != NULL && last->p != NULL)
							paint_laser_flash(last, LASER_SHIELD);
				}
				///Cleanup laser
				for (tile* t : *brd.repaint) {
					if (t != NULL)
						paint_tile(t);
				}

				///Save
				brd.save(autos);

			}

		}

	}

	///Repaint turn symbol
	paint_rot();

}
catch (...)
{
	MessageBox(NULL, _T("An error has occurred -- restoring with autosave."), szTitle, NULL);
	if (!brd.load(autos))
		MessageBox(NULL, _T("Error while loading autosave file."), szTitle, NULL);
	paint_board();
}

//Key input
VOID key_input()
{
	///Load game
	if (GetAsyncKeyState('O') && GetAsyncKeyState(VK_CONTROL)) {
		OPENFILENAME ofn = { 0 };
		wchar_t fn[MAX_PATH] = L"amonkhet_save";
		ofn.lStructSize = sizeof(ofn);
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST;
		ofn.hInstance = GetModuleHandle(0);
		ofn.nMaxFile = MAX_PATH;
		ofn.nFilterIndex = 1;
		ofn.lpstrFile = fn;
		ofn.lpstrDefExt = _T("aksg");
		GetSaveFileName(&ofn);
		brd.load(to_string(fn));
		paint_board();
	}
	///Save game
	if (GetAsyncKeyState('S') && GetAsyncKeyState(VK_CONTROL)) {
		OPENFILENAME ofn = { 0 };
		wchar_t fn[MAX_PATH] = L"amonkhet_save";
		ofn.lStructSize = sizeof(ofn);
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST;
		ofn.hInstance = GetModuleHandle(0);
		ofn.nMaxFile = MAX_PATH;
		ofn.nFilterIndex = 1;
		ofn.lpstrFile = fn;
		ofn.lpstrDefExt = _T("aksg");
		GetSaveFileName(&ofn);
		brd.save(to_string(fn));
	}
}

/*** PAINTING ***/

//Paint rotation buttons
VOID paint_rot()
{
	///Erase turn symbol
	if (turn != brd.silver_move) {
		SolidBrush b(BKGND);
		g->FillRectangle(&b, cRect());
	}
	///Paint arrows
	Image cc(L"cc_arrow.PNG");
	Image cw(L"cw_arrow.PNG");
	g->DrawImage(&cc, ccRect());
	g->DrawImage(&cw, cwRect());
	///Grey out arrows
	if (brd.click == NULL) {
		SolidBrush b(SHADE);
		g->FillRectangle(&b, ccRect());
		g->FillRectangle(&b, cwRect());
	}
	///Paint turn symbol
	if (turn != brd.silver_move) {
		Image i(brd.silver_move ? L"ankh.PNG" : L"horus.PNG");
		g->DrawImage(&i, cRect());
	}
	turn = brd.silver_move;
}

//Paint bkgnd
VOID background()
{
	SolidBrush b(BKGND);
	if(g != NULL)
		g->FillRectangle(&b, Rect(0, 0, width, height));
}

//Paint a tile
VOID paint_tile(tile* t)
{
	SolidBrush b(TILE_FILL);
	g->FillRectangle(&b, tileRect(t->r, t->c));
	if (t->type == tile::ANKH) {
		Image i(L"ankh.png");
		g->DrawImage(&i, tileRect(t->r, t->c));
	}
	if (t->type == tile::HORUS) {
		Image i(L"horus.png");
		g->DrawImage(&i, tileRect(t->r, t->c));
	}
	if (t->p != NULL) {
		Image i(to_wstring(t->p->image_name()).c_str());
		g->DrawImage(&i, tileRect(t->r, t->c));
	}
	if (t->select) {
		SolidBrush b(t->select);
		g->FillRectangle(&b, tileRect(t->r, t->c));
	}
	Pen p(0xFF000000);
	g->DrawRectangle(&p, tileRect(t->r, t->c));
}
VOID paint_tile(int r, int c) { paint_tile(brd.tiles[r][c]); }

//Paint a laser flash on a tile
VOID paint_laser_flash(tile* t, int col)
{
	SolidBrush b(col);
	g->FillRectangle(&b, tileRect(t->r, t->c));
	this_thread::sleep_for(chrono::milliseconds(1000));
}

//Paint a laser beam between points
VOID laser(int x1, int y1, int x2, int y2)
{
	SolidBrush b(LASER_BEAM);
	Pen p(&b, tile_dim / 10);
	g->DrawLine(&p, Point(x1, y1), Point(x2, y2));
}

//Paint the board
VOID paint_board()
{
	turn = !brd.silver_move;
	paint_rot();
	for (int i = 0; i < board::NUM_ROW; i++)
		for (int j = 0; j < board::NUM_COL; j++)
			paint_tile(i, j);
}

VOID OnPaint(HDC hdc)
{
	if (ready)
		paint_board();
}
