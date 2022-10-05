#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Pugiparam.h"

using namespace std;

Pugiparam::Pugiparam(string filename, _mData *_mdata)
{
    Filename = filename;
    pugi::xml_parse_result result = doc.load_file(Filename.c_str());
    if (!result) error = 1;
    std::cout << filename << "  Error -  " << error  << "\n";
    mdata = _mdata;
}

void Pugiparam::getParam(const char *id, const char *name)
{
    for (pugi::xml_node tool: cheinDOC)
    {
        if (!strcmp(tool.attribute("Id").value(), id)) {
                mdata->min_distance = tool.attribute(name).as_int();
        }
    }
}
/*
void Pugiparam::getParam()
{
    pugi::xml_node element = doc.document_element();
    for (pugi::xml_node tool: cheinDOC)
    {
    }
}
*/

void Pugiparam::getParam()
{
    string nname;
    string vvalue;
    stringstream sstr;
    int val;
    Tp ppp;
    pugi::xml_node tools = doc.child("Profile").child("Parameters");
    int i = 0, j = 0;
    for (pugi::xml_node_iterator it = tools.begin(); it != tools.end(); ++it)
    {
        for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
        {
            nname = string(ait->name());
	    vvalue = string(ait->value());
            sstr << vvalue;
	    if (nname == "Id") {
		ppp.id = vvalue;
	    }
	    else {
		ppp.push_p(atoi(sstr.str().c_str()));
	    }
	    sstr.seekg(0); sstr.str("");
        }
	Parameters.push_back(ppp);
	ppp.prp.resize(0);
    }
    for (auto tp : Parameters) {
	for(i = 0; i < tp.sizePrp(); i++) {
	    cout << tp.prp[i] << " ";
	}
	cout << " " << tp.id << endl;
    }
}

void Pugiparam::setParam(const char *id, const char *name, int value)
{
    for (pugi::xml_node tool: cheinDOC)
    {
	cout << id << " " << tool.attribute("Id").value();
        if (!strcmp(tool.attribute("Id").value(), id)) {
		cout << " " << id << " " << value;
                tool.attribute(name).set_value(value);
        }
	cout << endl;
    }
}

void Pugiparam::synchroParam()
{
    for (auto tp : Parameters) {
        for(int i = 0; i < tp.sizePrp(); i++) {
            //cout << tp.prp[i] << " ";
	    //setParam(tp.id, tp.prp[i].Name, tp.prp[i].value);
        }
        //cout << " " << tp.id << endl;
    }
}

