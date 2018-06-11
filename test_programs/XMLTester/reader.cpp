#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_iterators.hpp"
#include <cstring>

using namespace rapidxml;
using namespace std;

int main(void)
{

	xml_document<> doc;    // character type defaults to char
	xml_node<> * root_node;
	ifstream theFile("TEST.xml");


	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc.parse<0>(&buffer[0]);
	// Find our root node
	//root_node = doc.first_node("DEVICE"); // initialize 

	root_node = doc.first_node("DEVICE");

	//DEBUG!
	//cout << "Printing whole XML document ..." << endl;
	//cout << doc << endl;

	xml_node<>* inner_node;

	cout << "Extracting relevant info ..." << endl;
	for (xml_node<> * actionmapping_node = root_node->first_node("actionmapping"); actionmapping_node; actionmapping_node = actionmapping_node->next_sibling())
	{
		cout << "  This is actionMapping " << actionmapping_node->first_attribute("name")->value() << endl;
		cout << "  It contains certain Constraints like following:\n" << endl;

		inner_node = actionmapping_node->first_node();
		
		// whille inner_node is not a nullptr
		while (inner_node)
		{
			if (strncmp(inner_node->name(),"constraint",inner_node->name_size()) == 0)
			{
				cout << "    This is constraint " << inner_node->first_attribute("name")->value() << endl;
				cout << "    It contains certain Vars like following:\n" << endl;

				for (xml_node<> * var_node = inner_node->first_node("var"); var_node; var_node = var_node->next_sibling())
				{
					cout << "      This is var " << var_node->first_attribute("name")->value() << "\n"
						<< "      equal:  " << var_node->first_attribute("equal")->value() << "\n"
						<< "      higher: " << var_node->first_attribute("higher")->value() << "\n"
						<< "      lower:  " << var_node->first_attribute("lower")->value() << "\n"
						<< endl;
				}
			}
			else if (strncmp(inner_node->name(), "action_pin", inner_node->name_size()) == 0)
			{
				cout << "\n      " << inner_node->value();
			}
			else
			{
				//cout << ":(" << endl;
				//continue;
				break;
			}

			inner_node = inner_node->next_sibling();
		}
	}

	system("PAUSE");
	return 0;
}