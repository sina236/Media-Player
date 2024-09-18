#ifndef JSON_H
#define JSON_H
#include "playlist.h"
#include <vector>


/// @brief A class to manage all aspects of reading from and writing to .json files
///
/// Can read in all songs and playlists as well as save new playlists
/// @authors Colin Brown
/// @authors Ronak Toprani
class JSON
{
public:
    /// @brief Constructor for a JSON object
    ///
    /// Does not have any function
    JSON();

    /// @brief Reads the "songlist.json" file and returns it as a vector of Song objects
    ///
    /// Reads "songlist.json" and for each song and its information, turns that
    /// song into a Song object and puts it into the back of a vector of Song objects. The
    /// Song objects in the vector have the same order as the json and their position in
    /// the vector corresponds to their unique id
    /// @see Song
    /// @returns A vector of Song objects that were in the "songlist.json" file
    vector<Song> getSongs();

    /// @brief Reads the "songlist.json" file and returns it as a vector of Playlist objects
    ///
    /// Reads "playlists.json" and for each playlist and its information, turns that
    /// playlist into a Playlist object and puts it into the back of a vector of Playlist objects. The
    /// Playlist objects in the vector have the same order as the json and their position in
    /// the vector can be gotten by taking their unique id, multiplying it by -1 and then subtracting 1
    /// @see Playlist
    /// @returns A vector of Playlist objects that were in the "playlists.json" file
    vector<Playlist> getPlaylists(vector<Song>* allSongs);

    /// @brief Saves all Playlist objects into the "playlists.json" file
    ///
    /// Takes a vector of Playlist objects and turns each into json text and writes it to the
    /// "playlists.json" file in proper json format. Json file can then be read again on next startup
    /// having saved the changes made previously
    /// @see Playlist
    /// @param allPlaylists A pointer to the vector of Playlist objects to be saved in "playlists.json"
    void savePlaylists(vector<Playlist>* allPlaylists);
};

#endif // JSON_H
