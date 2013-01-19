#include <iostream>
#include "pugixml/pugixml.hpp"

int main()
{
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file("library.xml");

  if (result)
  {
    std::cout << "Result is good!" << std::endl;
  }
  else
  {
    std::cout << "Result is bad." << std::endl;
    return 0;
  }

  return 0;
}
