#include "pugixml.hpp"

#include <string.h>
#include <iostream>
using namespace std;

/*
bool small_timeout(pugi::xml_node node)
{
    return node.attribute("Timeout").as_int() < 20;
}
*/
struct allow_remote_predicate
{
    bool operator()(pugi::xml_attribute attr) const
    {
        return strcmp(attr.name(), "AllowRemote") == 0;
    }

    bool operator()(pugi::xml_node node) const
    {
        return node.attribute("AllowRemote").as_bool();
    }
};

int main()
{
    pugi::xml_document doc;
    if (!doc.load_file("ToolsInformation.xml")) return -1;

    pugi::xml_node tools = doc.child("Profile").child("Tools");


    // Find child via predicate (looks for direct children only)
    cout << tools.find_child(allow_remote_predicate()).attribute("Filename").value() << endl;

    // Find node via predicate (looks for all descendants in depth-first order)
    cout << doc.find_node(allow_remote_predicate()).attribute("Filename").value() << endl;

    // Find attribute via predicate
    cout << tools.last_child().find_attribute(allow_remote_predicate()).value() << endl;

    // We can use simple functions instead of function objects
    // cout << tools.find_child(small_timeout).attribute("Filename").value() << endl;
}
