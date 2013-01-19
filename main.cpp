#include <iostream>
#include <string.h>
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
    return 1;
  }

  int i = 0;
  pugi::xml_node songs = doc.child("plist").child("dict").child("dict");
  for (pugi::xml_node song = songs.child("dict"); song; song = song.next_sibling("dict"))
  {
    i++;
    std::cout << i << ": Song: " << song.child_value("key") << std::endl;
  }

  return 0;
}
