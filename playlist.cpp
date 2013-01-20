#include <string.h>
#include <stdlib.h>

#include "playlist.hpp"

playlist::playlist()
  : id(-1)
{
}

playlist create_playlist_from_iTunes_node(const pugi::xml_node playlist_node,
                                          std::vector<Song> songs)
{
  playlist playlist;

  for (pugi::xml_node key = playlist_node.child("key"); key;
       key = key.next_sibling("key"))
  {
    const char* key_text = key.child_value();
    if (strcmp(key_text, "Name") == 0)
    {
      pugi::xml_node value = key.next_sibling("string");
      playlist.name = value.child_value();
    }
    else if (strcmp(key_text, "Playlist ID") == 0)
    {
      pugi::xml_node value = key.next_sibling("integer");
      playlist.id = atoi(value.child_value());
    }
    else if (strcmp(key_text, "Playlist Persistent ID") == 0)
    {
      pugi::xml_node value = key.next_sibling("string");
      playlist.persistent_id = value.child_value();
    }
  }

  // iterate through each playlist item, and add the Song object
  pugi::xml_node songs_array = playlist_node.child("array");
  for (pugi::xml_node song = songs_array.child("dict"); song;
       song = song.next_sibling("dict"))
  {
    // iterate through each playlist item node
    for (pugi::xml_node key = song.child("key"); key;
         key = key.next_sibling("key"))
    {
      const char* key_text = key.child_value();
      if (strcmp(key_text, "Track ID") == 0)
      {
        pugi::xml_node value = key.next_sibling("integer");
        int track_id = atoi(value.child_value());

        // find the song which has the same id, and add it to the playlist
        for (std::vector<Song>::size_type i = 0; i < songs.size(); i++)
        {
          if (songs[i].id == track_id)
          {
            playlist.songs.push_back(songs[i]);
            break;
          }
        }
      }
    }
  }

  return playlist;
}
