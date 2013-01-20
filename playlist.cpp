#include <string.h>

#include "playlist.hpp"

playlist::playlist()
  : id(-1)
{
}

playlist create_playlist_from_iTunes_node(const pugi::xml_node playlist_node,
                                          std::vector<Song> songs)
{
  playlist playlist;

  // todo: create playlist

  return playlist;
}
