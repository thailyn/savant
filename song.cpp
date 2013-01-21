#include <string.h>
#include <stdlib.h>

#include "song.hpp"

Song::Song()
  : id(-1), rating(0), album_rating(0), album_rating_computed(false), play_count(0)
{
}

Song create_from_iTunes_node(const pugi::xml_node song_node)
{
  Song song;

  for (pugi::xml_node key = song_node.child("key"); key;
       key = key.next_sibling("key"))
  {
    std::string keyText = key.child_value();
    if (strcmp(key.child_value(), "Track ID") == 0)
    {
      pugi::xml_node value = key.next_sibling("integer");
      song.id = atoi(value.child_value());
    }
    else if (strcmp(key.child_value(), "Name") == 0)
    {
      pugi::xml_node value = key.next_sibling("string");
      song.title = value.child_value();
    }
    else if (strcmp(key.child_value(), "Artist") == 0)
    {
      pugi::xml_node value = key.next_sibling("string");
      song.artist = value.child_value();
    }
    else if (strcmp(key.child_value(), "Album") == 0)
    {
      pugi::xml_node value = key.next_sibling("string");
      song.album = value.child_value();
    }
    else if (strcmp(key.child_value(), "Rating") == 0)
    {
      pugi::xml_node value = key.next_sibling("integer");
      song.rating = atoi(value.child_value());
    }
    else if (strcmp(key.child_value(), "Album Rating") == 0)
    {
      pugi::xml_node value = key.next_sibling("integer");
      song.album_rating = atoi(value.child_value());
    }
    else if (strcmp(key.child_value(), "Album Rating Computed") == 0)
    {
      pugi::xml_node value = key.next_sibling("true");
      song.album_rating_computed = value;
    }
    else if (strcmp(key.child_value(), "Location") == 0)
    {
      pugi::xml_node value = key.next_sibling("string");
      song.file_name = value.child_value();
    }
  }

  return song;
}
