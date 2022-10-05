#include "pugixml.hpp"
#include<iostream>
using namespace std;

#define Pnode pugi::xml_node

inline int Param(Pnode *ppval, Pnode pp) {
	Pnode ppv = pp.first_child();
	*ppval = ppv;
	return pp.text().as_int();
}

int main()
{
	int Min_dist, Max_dist, Min_angle, Max_angle;
	Pnode min_dist_val, max_dist_val, min_angle_val, max_angle_val;
	pugi::xml_document doc;
	doc.load_file("init.xml");           //Load xml file

        Pnode parameters = doc.first_child();                      //get the first child of xml document i.e catalog
        Pnode param = parameters.first_child();                    //get the first child of catalog i.e book
        Pnode min_dist = param.first_child();                      //get the first child of book i.e author
	Min_dist = Param(&min_dist_val, min_dist);

        Pnode max_dist = min_dist.next_sibling();

	Max_dist = Param(&max_dist_val, max_dist);
	cout << Min_dist << " " << Max_dist << endl;


	pugi::xpath_query param_query("/parameters");
	pugi::xpath_query name_query("id/text()");
	pugi::xpath_node_set xpath_parameters = doc.select_nodes(param_query);
	//cout << "--" << xpath_parameters << endl;
	int i = 0;
	for (pugi::xpath_node xpath_param : xpath_parameters)
        {
	    cout << "--- " << i++ << endl;
            pugi::xml_node pp = xpath_param.node();
            pugi::xml_node id = pp.select_node(name_query).node();
            //pugi::xml_node roll = student.select_node(roll_query).node();
            cout << "Id: " << id.value() << endl;
            //cout << "        roll: " << roll.value() << endl;
        }


/*
	pugi::xpath_query param_query("angle");
	pugi::xpath_query id_query("id/text()");
	//pugi::xml_node parameters1 = xpath_student.node();

	//Pnode parameters1 = max_dist.next_sibling();
	cout << parameters1 << endl;
	Pnode param1 = parameters1.first_child();
	Pnode min_angle = param1.first_child();                      //get the first child of book i.e author
        Min_angle = Param(&min_angle_val, min_angle);

	Pnode max_angle = min_angle.next_sibling();

        Max_angle = Param(&max_angle_val, max_angle);
        cout << Min_angle << " " << Max_angle << endl;
*/

	min_dist_val.set_value("44");
        doc.save_file("init.xml");

	return 0;

}
