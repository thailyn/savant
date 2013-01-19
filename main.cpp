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
    std::cout << i << ": Song: ";
    std::string title = "";
    std::string artist = "";
    for (pugi::xml_node key = song.child("key"); key; key = key.next_sibling("key"))
    {
      //std::cout << "Song: ";
      std::string keyText = key.child_value();
      if (strcmp(key.child_value(), "Name") == 0)
      {
        pugi::xml_node value = key.next_sibling("string");
        title = value.child_value();
        //std::cout << value.text() << " - ";
        std::cout << value.child_value() << " - ";
      }
      else if (strcmp(key.child_value(), "Artist") == 0)
      {
        pugi::xml_node value = key.next_sibling("string");
        artist = value.child_value();
        //std::cout << value.text();
        std::cout << value.child_value();
      }
    }
    std::cout << std::endl;
    //std::cout << "*Song: " << title << " - " << artist << std::endl;
  }

  return 0;
}
