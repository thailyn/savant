#include <iostream>
#include <string.h>
#include <stdlib.h>
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
    int id = 0;
    std::string title = "";
    std::string artist = "";
    std::string album = "";
    std::string file_name = "";

    int rating = -1;
    int album_rating = -1;
    bool album_rating_computed = false;

    for (pugi::xml_node key = song.child("key"); key; key = key.next_sibling("key"))
    {
      std::string keyText = key.child_value();
      if (strcmp(key.child_value(), "Track ID") == 0)
      {
        pugi::xml_node value = key.next_sibling("integer");
        id = atoi(value.child_value());
      }
      else if (strcmp(key.child_value(), "Name") == 0)
      {
        pugi::xml_node value = key.next_sibling("string");
        title = value.child_value();
      }
      else if (strcmp(key.child_value(), "Artist") == 0)
      {
        pugi::xml_node value = key.next_sibling("string");
        artist = value.child_value();
      }
      else if (strcmp(key.child_value(), "Album") == 0)
      {
        pugi::xml_node value = key.next_sibling("string");
        album = value.child_value();
      }
      else if (strcmp(key.child_value(), "Rating") == 0)
      {
        pugi::xml_node value = key.next_sibling("integer");
        rating = atoi(value.child_value());
      }
      else if (strcmp(key.child_value(), "Album Rating") == 0)
      {
        pugi::xml_node value = key.next_sibling("integer");
        album_rating = atoi(value.child_value());
      }
      else if (strcmp(key.child_value(), "Album Rating Computed") == 0)
      {
        pugi::xml_node value = key.next_sibling("true");
        album_rating_computed = value;
      }
      else if (strcmp(key.child_value(), "Location") == 0)
      {
        pugi::xml_node value = key.next_sibling("string");
        file_name = value.child_value();
      }
    }
    std::cout << i << ": (" << id << ") " << title << " - " << artist << " - " << album << std::endl;
    std::cout << "rating: " << rating << ", album rating: " << album_rating << ", computed: " << album_rating_computed << std::endl;
    std::cout << "file: " << file_name << std::endl;
  }

  return 0;
}
