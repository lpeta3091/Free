#include <iostream>
#include "pugixml.hpp"
using namespace std;

int main()
{

	pugi::xml_document doc;
	doc.load_file("StudentsInfo.xml");

	pugi::xpath_query student_query("/data/student");

	pugi::xpath_query name_query("Name/text()");

	pugi::xpath_query roll_query("Roll/text()");

	pugi::xpath_node_set xpath_students = doc.select_nodes(student_query);

	for (pugi::xpath_node xpath_student : xpath_students)
	{
	    // Since Xpath results can be nodes or attributes, you must explicitly get
	    // the node out with .node()

	    pugi::xml_node student = xpath_student.node();

	    pugi::xml_node name = student.select_node(name_query).node();

	    pugi::xml_node roll = student.select_node(roll_query).node();

	    cout << "Student name: " << name.value() << endl;

	    cout << "        roll: " << roll.value() << endl;
	}

	return 0;
}
