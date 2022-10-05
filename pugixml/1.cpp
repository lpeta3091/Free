#include "pugixml.hpp"
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

int main()
{
    int min_dist, max_dist, min_angle, max_angle;
    string str;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("setup.xml");
    if (!result)
        return -1;

    for (pugi::xml_node tool: doc.child("Profile").child("Parameters").children("Parameter"))
    {
	if (!strcmp(tool.attribute("Id").value(), "distance")) {
		min_dist = tool.attribute("Min").as_int();
		std::cout << "Min distance " << min_dist << "\n";
		str = to_string(min_dist + 1);
		tool.attribute("Min").set_value(str.c_str());
	}
        std::cout << "Id " << tool.attribute("Id").value() <<  "   Min " << tool.attribute("Min").value() <<"\n";
    }

   std::cout << "\n";

   for (pugi::xml_node tool: doc.child("Profile").child("Parameters").children("Parameter"))
    {
        if (!strcmp(tool.attribute("Id").value(), "angle")) {
                max_angle = tool.attribute("Max").as_int();
                std::cout << "Max angle " << max_angle << "\n";
                //str = to_string(min_dist + 1);
                //tool.attribute("Min").set_value(str.c_str());
        }
        std::cout << "Id " << tool.attribute("Id").value() <<  "   Max " << tool.attribute("Max").value() <<"\n";
    }




//    doc.save(std::cout);
    std::cout << "Saving result: " << doc.save_file("setup.xml") << std::endl;
}
