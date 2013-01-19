#include <iostream>
#include <vector>

#include "pugixml/pugixml.hpp"
#include "song.hpp"

int main()
{
  bool print = false;

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
  std::vector<Song> song_list;
  pugi::xml_node songs = doc.child("plist").child("dict").child("dict");
  for (pugi::xml_node song_node = songs.child("dict"); song_node; song_node = song_node.next_sibling("dict"))
  {
    i++;

    Song song = create_from_iTunes_node(song_node);
    song_list.push_back(song);

    if (print)
    {
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
  }

  std::cout << "Finished parsing library file.  Found " << i << " songs." << std::endl;

  std::vector<Song> song_dist;
  for (std::vector<Song>::size_type j = 0; j < song_list.size(); j++)
  {
    int song_rating = song_list[j].rating;

    // always include a song once
    song_dist.push_back(song_list[j]);

    // include the song an extra time for each "star"
    for (int k = 0; k < song_rating / 20; k++)
    {
      song_dist.push_back(song_list[j]);
    }
  }

  std::cout << "dist list size: " << song_dist.size() << std::endl;

  return 0;
}
