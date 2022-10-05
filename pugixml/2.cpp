#include "pugixml.hpp"

#include <iostream>
using namespace std;

int main()
{
    pugi::xml_document doc;

    // get a test document
    doc.load_file("ProjectInfo.xml");

    pugi::xml_node project = doc.child("project");

    cout << "Project name: " << project.child("name").text().get() << std::endl;
    cout << "Project version: " << project.child("version").text().as_double() << std::endl;
    cout << "Project visibility: " << (project.child("public").text().as_bool(/* def= */ true) ? "public" : "private") << std::endl;


    cout << endl;


    // change project version
    project.child("version").text() = 1.5;

    // add description element and set the contents
    // note that we do not have to explicitly add the node_pcdata child
    project.append_child("description").text().set("a test project");


    doc.save(cout);
    doc.save_file("ProjectInfo.xml");
}
