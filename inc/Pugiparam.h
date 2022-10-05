#ifndef PUGIPARAM__H
#define PUGIPARAM__H
#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include "pugixml.hpp"
#include "def.h"

#define cheinDOC doc.child("Profile").child("Parameters").children("Parameter")

using namespace std;


typedef struct {
	string Name;
	int value;
} _prp;

typedef struct {
	string id;
	//_prp prp[3];
	vector<int> prp;
} _Parameter;

class Tp {
  public:
//    Tp (double d) : _d { d } {}
    Tp () {}
    void push_p(int p) { prp.push_back(p); }
    string id;
    vector<int> prp;
    int sizePrp() { return prp.size(); }
  private:
    double _d;
};

class Pugiparam  {
private:
    _mData *mdata;
    //_Parameter Parameters[3];
    vector<Tp> Parameters;
    string Filename;
    int error = 0;
    pugi::xml_document doc;

public:
    Pugiparam(string filename, _mData *_mdata);
    void getParam(const char *id, const char *name);
    void getParam();
    void setParam(const char *id, const char *name, int value);
    void synchroParam();
    string viewParam(int index);
    ~Pugiparam() { doc.save_file(Filename.c_str()); };
};

#endif
