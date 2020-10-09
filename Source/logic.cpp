
#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "logic.h"

using namespace Gdiplus;
using namespace std;

#pragma comment (lib,"Gdiplus.lib")

/*** DIRECTIONS ***/
const int MOVE[piece::DIR][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

/*** BOARD ***/

//Constructors
///Given board setup status
board::board(int mode) { set_up(mode); }
///Default
board::board() { set_up(CLASSIC_MODE); }
///Set up the board
bool board::set_up(int mode)
{

	repaint = new list<tile*>();
	move = list<tile*>();
	silver_move = TRUE;
	click = NULL;
	mouse = NULL;

	for (int r = 0; r < NUM_ROW; r++) {
		for (int c = 0; c < NUM_COL; c++)
			tiles[r][c] = new tile(r, c, this);
		tiles[r][NUM_COL - 1]->type = tile::ANKH;
		tiles[r][0]->type = tile::HORUS;
	}
	tiles[NUM_ROW - 1][NUM_COL - 2]->type = tile::HORUS;
	tiles[0][NUM_COL - 2]->type = tile::HORUS;
	tiles[NUM_ROW - 1][1]->type = tile::ANKH;
	tiles[0][1]->type = tile::ANKH;

	switch (mode) {

		case CLASSIC_MODE: {
			(new sphinx(true))->step(tiles[7][9]);
			(new sphinx(false))->step(tiles[0][0]);
			tiles[0][0]->p->dir = piece::SOUTH;
			(new scarab(false))->step(tiles[3][4]);
			(new scarab(false))->step(tiles[3][5]);
			tiles[3][5]->p->dir = piece::EAST;
			(new scarab(true))->step(tiles[4][4]);
			tiles[4][4]->p->dir = piece::WEST;
			(new scarab(true))->step(tiles[4][5]);
			tiles[4][5]->p->dir = piece::SOUTH;
			(new pharaoh(false))->step(tiles[0][5]);
			tiles[0][5]->p->dir = piece::SOUTH;
			(new pharaoh(true))->step(tiles[7][4]);
			(new anubis(false))->step(tiles[0][4]);
			tiles[0][4]->p->dir = piece::SOUTH;
			(new anubis(false))->step(tiles[0][6]);
			tiles[0][6]->p->dir = piece::SOUTH;
			(new anubis(true))->step(tiles[7][3]);
			(new anubis(true))->step(tiles[7][5]);
			(new pyramid(false))->step(tiles[0][7]);
			tiles[0][7]->p->dir = piece::EAST;
			(new pyramid(false))->step(tiles[1][2]);
			tiles[1][2]->p->dir = piece::SOUTH;
			(new pyramid(false))->step(tiles[3][0]);
			(new pyramid(false))->step(tiles[4][0]);
			tiles[4][0]->p->dir = piece::EAST;
			(new pyramid(false))->step(tiles[3][7]);
			tiles[3][7]->p->dir = piece::EAST;
			(new pyramid(false))->step(tiles[4][7]);
			(new pyramid(false))->step(tiles[5][6]);
			tiles[5][6]->p->dir = piece::EAST;
			(new pyramid(true))->step(tiles[2][3]);
			tiles[2][3]->p->dir = piece::WEST;
			(new pyramid(true))->step(tiles[3][2]);
			tiles[3][2]->p->dir = piece::SOUTH;
			(new pyramid(true))->step(tiles[4][2]);
			tiles[4][2]->p->dir = piece::WEST;
			(new pyramid(true))->step(tiles[7][2]);
			tiles[7][2]->p->dir = piece::WEST;
			(new pyramid(true))->step(tiles[3][9]);
			tiles[3][9]->p->dir = piece::WEST;
			(new pyramid(true))->step(tiles[4][9]);
			tiles[4][9]->p->dir = piece::SOUTH;
			(new pyramid(true))->step(tiles[6][7]);
			return TRUE;
		}

		case DYNASTY_MODE: {
			(new sphinx(true))->step(tiles[7][9]);
			(new sphinx(false))->step(tiles[0][0]);
			tiles[0][0]->p->dir = piece::SOUTH;
			(new pyramid(false))->step(tiles[0][4]);
			tiles[0][4]->p->dir = piece::SOUTH;
			(new anubis(false))->step(tiles[0][5]);
			tiles[0][5]->p->dir = piece::SOUTH;
			(new pyramid(false))->step(tiles[0][6]);
			tiles[0][6]->p->dir = piece::EAST;
			(new pharaoh(false))->step(tiles[1][5]);
			tiles[1][5]->p->dir = piece::SOUTH;
			(new pyramid(false))->step(tiles[2][0]);
			(new pyramid(false))->step(tiles[2][4]);
			tiles[2][4]->p->dir = piece::SOUTH;
			(new anubis(false))->step(tiles[2][5]);
			tiles[2][5]->p->dir = piece::SOUTH;
			(new scarab(false))->step(tiles[2][6]);
			tiles[2][6]->p->dir = piece::WEST;
			(new pyramid(false))->step(tiles[3][0]);
			tiles[3][0]->p->dir = piece::EAST;
			(new scarab(false))->step(tiles[3][2]);
			tiles[3][2]->p->dir = piece::SOUTH;
			(new pyramid(true))->step(tiles[3][4]);
			tiles[3][4]->p->dir = piece::WEST;
			(new pyramid(true))->step(tiles[3][6]);
			tiles[3][6]->p->dir = piece::EAST;
			(new pyramid(false))->step(tiles[4][3]);
			tiles[4][3]->p->dir = piece::WEST;
			(new pyramid(false))->step(tiles[4][5]);
			tiles[4][5]->p->dir = piece::EAST;
			(new scarab(true))->step(tiles[4][7]);
			(new pyramid(true))->step(tiles[4][9]);
			tiles[4][9]->p->dir = piece::WEST;
			(new scarab(true))->step(tiles[5][3]);
			tiles[5][3]->p->dir = piece::EAST;
			(new anubis(true))->step(tiles[5][4]);
			(new pyramid(true))->step(tiles[5][5]);
			(new pyramid(true))->step(tiles[5][9]);
			tiles[5][9]->p->dir = piece::SOUTH;
			(new pharaoh(true))->step(tiles[6][4]);
			(new pyramid(true))->step(tiles[7][3]);
			tiles[7][3]->p->dir = piece::WEST;
			(new anubis(true))->step(tiles[7][4]);
			(new pyramid(true))->step(tiles[7][5]);
			return TRUE;
		}

		case IMHOTEP_MODE: {
			(new sphinx(true))->step(tiles[7][9]);
			(new sphinx(false))->step(tiles[0][0]);
			tiles[0][0]->p->dir = piece::SOUTH;
			(new pharaoh(false))->step(tiles[0][5]);
			tiles[0][5]->p->dir = piece::SOUTH;
			(new pharaoh(true))->step(tiles[7][4]);
			(new anubis(false))->step(tiles[0][4]);
			tiles[0][4]->p->dir = piece::SOUTH;
			(new anubis(false))->step(tiles[0][6]);
			tiles[0][6]->p->dir = piece::SOUTH;
			(new anubis(true))->step(tiles[7][3]);
			(new anubis(true))->step(tiles[7][5]);
			(new scarab(false))->step(tiles[0][7]);
			tiles[0][7]->p->dir = piece::EAST;
			(new pyramid(true))->step(tiles[2][3]);
			tiles[2][3]->p->dir = piece::WEST;
			(new pyramid(false))->step(tiles[2][6]);
			(new pyramid(false))->step(tiles[3][0]);
			(new pyramid(false))->step(tiles[4][0]);
			tiles[4][0]->p->dir = piece::EAST;
			(new pyramid(true))->step(tiles[3][1]);
			tiles[3][1]->p->dir = piece::SOUTH;
			(new pyramid(true))->step(tiles[4][1]);
			tiles[4][1]->p->dir = piece::WEST;
			(new pyramid(true))->step(tiles[3][4]);
			tiles[3][4]->p->dir = piece::EAST;
			(new scarab(false))->step(tiles[3][5]);
			tiles[3][5]->p->dir = piece::EAST;
			(new scarab(true))->step(tiles[4][4]);
			tiles[4][4]->p->dir = piece::WEST;
			(new pyramid(false))->step(tiles[4][5]);
			tiles[4][5]->p->dir = piece::WEST;
			(new pyramid(false))->step(tiles[3][8]);
			tiles[3][8]->p->dir = piece::EAST;
			(new pyramid(false))->step(tiles[4][8]);
			(new pyramid(true))->step(tiles[3][9]);
			tiles[3][9]->p->dir = piece::WEST;
			(new pyramid(true))->step(tiles[4][9]);
			tiles[4][9]->p->dir = piece::SOUTH;
			(new pyramid(true))->step(tiles[5][3]);
			tiles[5][3]->p->dir = piece::SOUTH;
			(new pyramid(false))->step(tiles[5][6]);
			tiles[5][6]->p->dir = piece::EAST;
			(new scarab(true))->step(tiles[7][2]);
			tiles[7][2]->p->dir = piece::WEST;
		}

		default: return FALSE;

	}

}

//Piece in direction
tile* board::piece_in_dir(tile* t, int dir)
{
	repaint->push_back(t);
	bool f = true;
	while (
		t->r + MOVE[dir][0] >= 0 &&
		t->c + MOVE[dir][1] >= 0 &&
		t->r + MOVE[dir][0] < NUM_ROW &&
		t->c + MOVE[dir][1] < NUM_COL &&
		(tiles[t->r][t->c]->p == NULL || f)
		) {
		t = tiles[t->r + MOVE[dir][0]][t->c + MOVE[dir][1]];
		repaint->push_back(t);
		f = false;
	}
	return t;
}

//Loading and saving
///Load game at file location
bool board::load(string f)
{
	try
	{
		///Clear
		set_up(EMPTY_BOARD);
		///File and dumps
		ifstream fin(f);
		string s;
		int dump;
		///Turn
		fin >> dump;
		silver_move = dump;
		///Populate
		for (int r = 0; r < NUM_ROW; r++) {
			fin >> s;
			for (int c = 0; c < NUM_COL; c++) {
				tiles[r][c]->p = piece::create(s[c]);
				if (tiles[r][c]->p != NULL)
					tiles[r][c]->p->step(tiles[r][c]);
			}
		}
		///Rotate
		for (int r = 0; r < NUM_ROW; r++)
			for (int c = 0; c < NUM_COL; c++)
				fin >> (tiles[r][c]->p != NULL ? tiles[r][c]->p->dir : dump);
		///Team
		for (int r = 0; r < NUM_ROW; r++)
			for (int c = 0; c < NUM_COL; c++) {
				fin >> dump;
				if (tiles[r][c]->p != NULL)
					tiles[r][c]->p->silver = dump;
			}
		return TRUE;
	}
	catch (...) { return FALSE; }
}
///Save game at file location
bool board::save(string f)
{
	try
	{
		///File
		ofstream fout(f);
		///Turn
		fout << (int)(silver_move) << endl << endl;
		///Piece types
		for (int r = 0; r < NUM_ROW; r++) {
			for (int c = 0; c < NUM_COL; c++)
				fout << piece::get_id(tiles[r][c]->p);
			fout << endl;
		}
		fout << endl;
		///Piece directions
		for (int r = 0; r < NUM_ROW; r++) {
			for (int c = 0; c < NUM_COL; c++)
				fout << (tiles[r][c]->p != NULL ? tiles[r][c]->p->dir : 0) << " ";
			fout << endl;
		}
		fout << endl;
		///Piece affiliations
		for (int r = 0; r < NUM_ROW; r++) {
			for (int c = 0; c < NUM_COL; c++)
				fout << (tiles[r][c]->p != NULL ? (int)tiles[r][c]->p->silver : 0) << " ";
			fout << endl;
		}
		return TRUE;
	}
	catch (...) { return FALSE; }
}

/*** TILE ***/

//Constructors
///Given location
tile::tile(int r, int c, board* b)
{
	this->type = NORMAL;
	this->select = 0;
	this->s = FALSE;
	this->p = NULL;
	this->b = b;
	this->r = r;
	this->c = c;
}
///Default
tile::tile()
{
	this->type = NORMAL;
	this->select = 0;
	this->s = FALSE;
	this->p = NULL;
	this->b = NULL;
	this->r = 0;
	this->c = 0;
}

/*** PIECE ***/

//Constructor
///Default
piece::piece()
{
	this->silver = TRUE;
	this->dir = NORTH;
	this->t = NULL;
}

//Notation of pieces
///Piece to character
char piece::get_id(piece* p)
{
	if (p == NULL)
		return EMPTY;
	return
		typeid(*p) != typeid(pharaoh) ?
		typeid(*p) != typeid(pyramid) ?
		typeid(*p) != typeid(sphinx) ?
		typeid(*p) != typeid(anubis) ?
		SCARAB : ANUBIS : SPHINX : PYRAMID : PHARAOH;
}
///Piece from character
piece* piece::create(char c)
{
	return
		c != PHARAOH ?
		c != PYRAMID ?
		c != SPHINX ?
		c != ANUBIS ?
		c != SCARAB ?
		NULL : (piece*)(new scarab()) : new anubis() : new sphinx() : new pyramid() : new pharaoh();
}

//Step into tile: precondition: move is legal
void piece::step(tile* n)
{
	if(t != NULL)
		t->p = NULL;
	n->p = this;
	t = n;
}

/*** PIECES ***/

//Sphinx
///Constructor given team
sphinx::sphinx(bool silver)
{
	this->silver = silver;
	this->dir = NORTH;
	this->t = NULL;
}
///Default constructor
sphinx::sphinx()
{
	this->silver = TRUE;
	this->dir = NORTH;
	this->t = NULL;
}
///Image
string sphinx::image_name() { return (silver ? "silver_sphinx" : "red_sphinx") + to_string(dir) + ".PNG"; }
///Fire laser
list<tile*> sphinx::fire()
{
	list<tile*> l = list<tile*>();
	l.push_back(t->b->piece_in_dir(t, dir));
	if (l.back()->p != NULL)
		l.back()->p->hit(l, (dir + 2) % DIR);
	return l;
}
///Get hit: nothing happens
void sphinx::hit(list<tile*>& l, int dir) { l.push_back(NULL); }

//Pharaoh
///Constructor given team
pharaoh::pharaoh(bool silver)
{
	this->silver = silver;
	this->dir = NORTH;
	this->t = NULL;
}
///Default constructor
pharaoh::pharaoh()
{
	this->silver = TRUE;
	this->dir = NORTH;
	this->t = NULL;
}
///Image
string pharaoh::image_name() { return (silver ? "silver_pharaoh" : "red_pharaoh") + to_string(dir) + ".PNG"; }
///Get hit: removed from board
void pharaoh::hit(list<tile*>& l, int dir) { l.push_back(t); }

//Scarab
///Constructor given team
scarab::scarab(bool silver)
{
	this->silver = silver;
	this->dir = NORTH;
	this->t = NULL;
}
///Default constructor
scarab::scarab()
{
	this->silver = TRUE;
	this->dir = NORTH;
	this->t = NULL;
}
string scarab::image_name() { return (silver ? "silver_scarab" : "red_scarab") + to_string(dir) + ".PNG"; }
///Get hit: bounce laser (facing up = \)
void scarab::hit(list<tile*>& l, int dir)
{
	int nd = dir != this->dir ? dir != (this->dir + 1) % DIR ? dir != (this->dir + 2) % DIR ? (this->dir + 2) % DIR : (this->dir + 3) % DIR : this->dir : (this->dir + 1) % DIR;
	l.push_back(t->b->piece_in_dir(t, nd));
	if (l.back() == t)
		l.pop_back();
	else if(l.back()->p != NULL)
		l.back()->p->hit(l, (nd + 2) % DIR);

}
///Step in: swap
void scarab::step(tile* n)
{
	if (n->p != NULL)
		n->p->t = t;
	if (t != NULL)
		t->p = n->p;
	n->p = this;
	t = n;
}

//Anubis
///Constructor given team
anubis::anubis(bool silver)
{
	this->silver = silver;
	this->dir = NORTH;
	this->t = NULL;
}
///Default constructor
anubis::anubis()
{
	this->silver = TRUE;
	this->dir = NORTH;
	this->t = NULL;
}
string anubis::image_name() { return (silver ? "silver_anubis" : "red_anubis") + to_string(dir) + ".PNG"; }
///Get hit: shield or die
void anubis::hit(list<tile*>& l, int dir)
{
	if (dir == this->dir)
		l.push_back(NULL);
	else
		l.push_back(t);
}

//Pyramid
///Constructor given team
pyramid::pyramid(bool silver)
{
	this->silver = silver;
	this->dir = NORTH;
	this->t = NULL;
}
///Default constructor
pyramid::pyramid()
{
	this->silver = TRUE;
	this->dir = NORTH;
	this->t = NULL;
}
string pyramid::image_name() { return (silver ? "silver_pyramid" : "red_pyramid") + to_string(dir) + ".PNG"; }
///Get hit: reflect or die (up is \ with reflective surface ^)
void pyramid::hit(list<tile*>& l, int dir)
{
	if (dir == this->dir) {
		l.push_back(t->b->piece_in_dir(t, (dir + 1) % DIR));
		if (l.back() == t)
			l.pop_back();
		else if(l.back()->p != NULL)
			l.back()->p->hit(l, (dir + 3) % DIR);
	}
	else if (dir == (this->dir + 1) % DIR) {
		l.push_back(t->b->piece_in_dir(t, (dir + 3) % DIR));
		if (l.back() == t)
			l.pop_back();
		else if (l.back()->p != NULL)
			l.back()->p->hit(l, (dir + 1) % DIR);
	}
	else
		l.push_back(t);
}

/*** MOVE LOGIC ***/

//Test if move is legal
bool legal_move(tile* t1, tile* t2)
{
	if (t1->p == NULL)
		return FALSE;
	if (t2->type == tile::ANKH && !t1->p->silver)
		return FALSE;
	if (t2->type == tile::HORUS && t1->p->silver)
		return FALSE;
	if (typeid(*(t1->p)) == typeid(sphinx))
		return FALSE;
	if (abs(t2->r - t1->r) > 1 || abs(t2->c - t1->c) > 1)
		return FALSE;
	if (t2->p != NULL && (typeid(*(t2->p)) == typeid(pharaoh) || typeid(*(t2->p)) == typeid(scarab) || typeid(*(t2->p)) == typeid(sphinx)))
		return FALSE;
	if (typeid(*(t1->p)) != typeid(scarab) && t2->p != NULL)
		return FALSE;
	return TRUE;
}

//Examine all possible moves, return list of legal ones
void get_moves(list<tile*>& l, tile* t)
{
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			if (
				!(i == 0 && j == 0) &&
				t->r + i >= 0 &&
				t->c + j >= 0 &&
				t->r + i < board::NUM_ROW &&
				t->c + j < board::NUM_COL &&
				legal_move(t, t->b->tiles[t->r + i][t->c + j])
				)
				l.push_back(t->b->tiles[t->r + i][t->c + j]);
}
