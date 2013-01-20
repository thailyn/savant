#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <string>
#include <vector>

#include "song.hpp"
#include "pugixml/pugixml.hpp"

class playlist
{
public:
  playlist();

  int id;
  std::string name;
  std::string persistent_id;

  std::vector<Song> songs;
};

playlist create_playlist_from_iTunes_node(const pugi::xml_node playlist_node,
                                          std::vector<Song> songs);

#endif
