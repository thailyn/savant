#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "pugixml/pugixml.hpp"
#include "song.hpp"

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
  for (pugi::xml_node song_node = songs.child("dict"); song_node; song_node = song_node.next_sibling("dict"))
  {
    i++;

    Song song = create_from_iTunes_node(song_node);

    std::string stars = "";
    for (int j = 0; j < 5; j++)
    {
      if (j < song.rating / 20)
      {
        stars = stars + "*";
      }
      else
      {
        stars = stars + " ";
      }
    }

    std::cout << i << ": (" << song.id << ") (" << stars << ") " << song.title << " - " << song.artist << " - " << song.album << std::endl;
    std::cout << "rating: " << song.rating << ", album rating: " << song.album_rating << ", computed: " << song.album_rating_computed << std::endl;
    std::cout << "file: " << song.file_name << std::endl;
    std::cout << std::endl;
  }

  return 0;
}
