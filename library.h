#ifndef LIBRARY_H
#define LIBRARY_H
#include "playlist.h"
#include "json.h"

/// @brief A class to store all songs and playlists in the library
///
/// On creation of a Library object, all songs and playlist in storage are loaded in the Library object
/// @author Colin Brown
class Library
{
private:
    vector<Song> songs;
    vector<Playlist> playlists;
    JSON json = JSON();

public:
    /// @brief Constructor for a Library object
    ///
    /// Automatically fills the songs and playlists vectors with Song
    /// and Playlist objects from resources folder
    Library();

    /// @brief Get all songs
    ///
    /// Getter method to get all songs in a vector
    /// @see Song
    /// @returns vector<Song> songs: A vector of all songs as Song objects
    vector<Song> getSongs();

    /// @brief Get all playlists
    ///
    /// Getter method to get all playlists in a vector
    /// @see Playlist
    /// @returns vector<Playlist> playlists: A vector of all playlists as Playlist objects
    vector<Playlist> &getPlaylists();

    /// @brief Saves all Playlist objects into the "playlists.json" file
    ///
    /// Runs the savePlaylists method in JSON
    /// @see JSON::savePlaylists()
    void savePlaylists();

    /// @brief Remove a playlist from the library
    ///
    /// Removes a playlist from the vector of all Playlist objects. Updates all playlist id's after
    /// deleted playlist to keep sequential order of ids -1,-2,-3,...
    /// @param id The unique id of the Playlist object to be deleted in the range -1 to -infinity
    void removePlaylist(int id);
};

#endif // LIBRARY_H
