
#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace Gdiplus;
using namespace std;

#pragma comment (lib,"Gdiplus.lib")

#ifndef LOGIC_H
#define LOGIC_H

class piece;
class board;
class tile;

class board
{
public:

	const static int CLASSIC_MODE = 1;
	const static int IMHOTEP_MODE = 2;
	const static int DYNASTY_MODE = 3;
	const static int EMPTY_BOARD = 0;
	const static int NUM_COL = 10;
	const static int NUM_ROW = 8;

	tile* tiles[NUM_ROW][NUM_COL];
	list<tile*>* repaint;
	bool silver_move;
	list<tile*> move;
	tile* mouse;
	tile* click;

	board(int);
	board();

	tile* piece_in_dir(tile*, int);
	bool load(string);
	bool save(string);
	bool set_up(int);

};

class tile
{
public:

	const static int HORUS = -1;
	const static int NORMAL = 0;
	const static int ANKH = 1;

	int select;
	board* b;
	piece* p;
	int type;
	bool s;
	int r;
	int c;

	tile(int, int, board*);
	tile();

};

class piece
{
public:

	const static char PHARAOH = 'P';
	const static char PYRAMID = 'p';
	const static char SPHINX = 'L';
	const static char ANUBIS = 'A';
	const static char SCARAB = 'S';
	const static char EMPTY = '_';

	const static int NORTH = 0;
	const static int SOUTH = 2;
	const static int EAST = 1;
	const static int WEST = 3;
	const static int DIR = 4;

	static char get_id(piece*);
	static piece* create(char);

	bool silver;
	tile* t;
	int dir;

	piece();

	virtual string image_name() { return ""; }
	virtual void hit(list<tile*>&, int) {}
	virtual void step(tile*);

};

class sphinx : public piece
{
public:

	sphinx(bool);
	sphinx();

	void hit(list<tile*>&, int);
	string image_name();
	list<tile*> fire();

};

class pharaoh : public piece
{
public:

	pharaoh(bool);
	pharaoh();

	void hit(list<tile*>&, int);
	string image_name();

};

class scarab : public piece
{
public:

	scarab(bool);
	scarab();

	void hit(list<tile*>&, int);
	string image_name();
	void step(tile*);

};

class anubis : public piece
{
public:

	anubis(bool);
	anubis();

	void hit(list<tile*>&, int);
	string image_name();

};

class pyramid : public piece
{
public:

	pyramid(bool);
	pyramid();

	void hit(list<tile*>&, int);
	string image_name();

};

bool legal_move(tile*, tile*);
void get_moves(list<tile*>&, tile*);

#endif
