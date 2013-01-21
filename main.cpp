#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <fstream>

#include "pugixml/pugixml.hpp"
#include "boost/program_options/options_description.hpp"
#include "boost/program_options/parsers.hpp"
#include "boost/program_options/variables_map.hpp"
#include "song.hpp"
#include "playlist.hpp"

namespace po = boost::program_options;

//  This should either set global settings or return the settings in an
// application-specific data structure, instead of returning the
// variables_map object (all program_options logic should be inside this
// function).
po::variables_map parse_command_line(int argc, char* argv[])
{
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("verbose,v", "print verbose output")
    ("input", po::value<std::string>(), "input file name")
    ("playlist", po::value<std::string>()->default_value("Library"),
     "iTunes playlist to pick songs from")
    ("output,o", po::value<std::string>()->default_value("playlist.m3u"),
     "output file name")
    ("rating-weight", po::value<std::string>()->default_value("linear"),
     "weighting scheme for song rating");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    std::cout << desc;
  }

  return vm;
}

void write_playlist_to_m3u_file(std::vector<Song> playlist, std::string file_name)
{
  std::ofstream fout(file_name, std::ofstream::out | std::ofstream::trunc);

  // print file header
  fout << "#EXTM3U" << std::endl;

  // print each playlist item
  for (std::vector<Song>::size_type i = 0; i < playlist.size(); i++)
  {
    fout << playlist[i].file_name << std::endl;
  }

  fout.close();
}

int main(int argc, char* argv[])
{
  bool print = false;
  std::string input_file_name = "";
  std::string iTunes_playlist_name = "";
  std::string output_file_name = "";
  std::string rating_weight_scheme = "";

  po::variables_map vm;
  vm = parse_command_line(argc, argv);

  if (vm.count("help"))
  {
    // already printed the help information in parse_command_line
    return 0;
  }

  if (vm.count("verbose"))
  {
    std::cout << "Using verbose output." << std::endl;
    print = true;
  }
  if (vm.count("playlist"))
  {
    std::cout << "iTunes playlist: " << vm["playlist"].as<std::string>() << std::endl;
    iTunes_playlist_name = vm["playlist"].as<std::string>();
  }
  if (vm.count("output"))
  {
    std::cout << "Output playlist file name: " << vm["output"].as<std::string>() << std::endl;
    output_file_name = vm["output"].as<std::string>();
  }
  if (vm.count("input"))
  {
    input_file_name = vm["input"].as<std::string>();
    std::cout << "Using input file " << input_file_name << std::endl;
  }
  else
  {
    std::cout << "Error: intput file name must be provided.  Quitting." << std::endl;
    return 3;
  }
  if (vm.count("rating-weight"))
  {
    rating_weight_scheme = vm["rating-weight"].as<std::string>();
    if (!(rating_weight_scheme == "linear" || rating_weight_scheme == "iTunes"))
    {
      std::cout << "Error: invalid rating weighting scheme '" << rating_weight_scheme
                << "'.  Allowed values are 'linear' and 'iTunes'.  Quitting." << std::endl;
      return 4;
    }
  }
  std::cout << "Using rating weighting scheme '" << rating_weight_scheme << "'." << std::endl;

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(input_file_name.c_str());

  if (result)
  {
    std::cout << "Successfully loaded file." << std::endl;
  }
  else
  {
    std::cout << "Failed to load file.  Quitting." << std::endl;
    return 1;
  }

  // parse the library input file and create the song list
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

  i = 0;
  std::vector<playlist> playlist_list;
  pugi::xml_node playlists = doc.child("plist").child("dict").child("array");
  for (pugi::xml_node playlist_node = playlists.child("dict"); playlist_node;
       playlist_node = playlist_node.next_sibling("dict"))
  {
    i++;

    playlist playlist = create_playlist_from_iTunes_node(playlist_node, song_list);
    playlist_list.push_back(playlist);

    if (print)
    {
      std::cout << i << ": (" << playlist.id << ") " << playlist.name << " - " << playlist.persistent_id << std::endl;
      for (std::vector<Song>::size_type j = 0; j < playlist.songs.size(); j++)
      {
        std::cout << j << ": (" << playlist.songs[j].id << ") (" << playlist.songs[j].rating << ") "
                  << playlist.songs[j].title << " - " << playlist.songs[j].artist << " - "
                  << playlist.songs[j].album << std::endl;
      }
      std::cout << std::endl;
    }
  }

  std::cout << "Finished parsing library file.  Found " << song_list.size()
            << " songs and " << playlist_list.size() << " playlists." << std::endl;

  playlist selected_playlist;
  for (std::vector<playlist>::size_type j = 0; j < playlist_list.size(); j++)
  {
    if (playlist_list[j].name == iTunes_playlist_name)
    {
      selected_playlist = playlist_list[j];
      break;
    }
  }
  if (selected_playlist.name != iTunes_playlist_name)
  {
    std::cout << "Could not find iTunes playlist " << iTunes_playlist_name << " to create playlist.  Quitting." << std::endl;
    return 2;
  }

  std::cout << "Using iTunes playlist " << iTunes_playlist_name << " to create playlist.  Contains "
            << selected_playlist.songs.size() << " songs." << std::endl;

  // create the song distribution list
  std::vector<Song> song_dist;
  for (std::vector<Song>::size_type j = 0; j < selected_playlist.songs.size(); j++)
  {
    int song_rating = selected_playlist.songs[j].rating;

    if (rating_weight_scheme == "linear")
    {
      // always include a song once
      song_dist.push_back(selected_playlist.songs[j]);

      // include the song an extra time for each "star"
      for (int k = 0; k < song_rating / 20; k++)
      {
        song_dist.push_back(selected_playlist.songs[j]);
      }
    }
    else if (rating_weight_scheme == "iTunes")
    {
      int include_count = 0;
      int num_stars = song_rating / 20;

      switch (num_stars)
      {
      case 0:
        include_count = 4;
        break;
      case 1:
        include_count = 12;
        break;
      case 2:
        include_count = 15;
        break;
      case 3:
        include_count = 19;
        break;
      case 4:
        include_count = 23;
        break;
      case 5:
        include_count = 27;
        break;
      }

      for (int k = 0; k < include_count; k++)
      {
        song_dist.push_back(selected_playlist.songs[j]);
      }
    }
  }

  std::cout << "Distributed list size: " << song_dist.size() << std::endl;

  // print out the distribution list
  if (print)
  {
    for (std::vector<Song>::size_type j = 0; j < song_dist.size(); j++)
    {
      std::string stars = "";
      for (int k = 0; k < 5; k++)
      {
        if (k < song_dist[j].rating / 20)
        {
          stars = stars + "*";
        }
        else
        {
          stars = stars + " ";
        }
      }

      std::cout << j << ": (" << song_dist[j].id << ") (" << stars << ") "
                << song_dist[j].title << " - " << song_dist[j].artist << " - "
                << song_dist[j].album << std::endl;
      std::cout << "rating: " << song_dist[j].rating << ", album rating: "
                << song_dist[j].album_rating << ", computed: "
                << song_dist[j].album_rating_computed << std::endl;
    }
  }

  unsigned playlist_size = 25;
  std::vector<Song> playlist;
  std::default_random_engine e(time(0));
  std::uniform_int_distribution<unsigned> u(0, song_dist.size());
  for (std::vector<Song>::size_type j = 0; j < playlist_size; j++)
  {
    unsigned value = u(e);
    playlist.push_back(song_dist[value]);
  }

  for (std::vector<Song>::size_type j = 0; j < playlist.size(); j++)
  {
      std::string stars = "";
      for (int k = 0; k < 5; k++)
      {
        if (k < playlist[j].rating / 20)
        {
          stars = stars + "*";
        }
        else
        {
          stars = stars + " ";
        }
      }

      std::cout << j << ": (" << playlist[j].id << ") (" << stars << ") "
                << playlist[j].title << " - " << playlist[j].artist << " - "
                << playlist[j].album << std::endl;
  }

  write_playlist_to_m3u_file(playlist, output_file_name);

  return 0;
}
