#include <iostream>
#include "pugixml/pugixml.hpp"

int main()
{
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file("library.xml");

  if (result)
  {
    std::cout << "Successfully loaded file." << std::endl;
  }
  else
  {
    std::cout << "Failed to load file.  Quitting." << std::endl;
    return 0;
  }

  return 0;
}
