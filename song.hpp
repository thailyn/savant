#ifndef SONG_H
#define SONG_H

#include <string>
#include "pugixml/pugixml.hpp"

class Song
{
public:
  Song();

  int id;
  std::string title;
  std::string artist;
  std::string album;
  int track_number;

  int rating;
  int album_rating;
  bool album_rating_computed;
  int play_count;

  std::string file_name;
};

Song create_from_iTunes_node(const pugi::xml_node song_node);

#endif
