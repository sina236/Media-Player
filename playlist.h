#ifndef PLAYLIST_H
#define PLAYLIST_H
#include "song.h"
#include <iostream>

using namespace std;

/// @brief A class to store all information for each playlist
///
/// A Playlist object will store the name of the playlist, its unqiue id, and all songs that are in it
/// @authors Edson Huang
/// @authors Colin Brown

class Playlist {
private:
    /// The name of the playlist
    QString name;
    /// Vector of all songs in the Playlist as Song objects
    vector<Song> songList;
    /// Unique id of the playlist being -1 or less
    int id;

public:
    /// @brief Constructor for a Playlist object
    ///
    /// Must specifiy all aspects of the playlist
    /// @param name Specifies the name of the playlist
    /// @param songList Specifies the songs in the playlist as a vector of Song objects
    /// @param id Specifies the unique id number of the playlist in the -1 to -inifinity
    Playlist(QString name, vector<Song> songList, qint64 id);

    /// @brief Constructor for a Playlist object
    ///
    /// songList vector is empty
    /// @param name Specifies the name of the playlist
    /// @param id Specifies the unique id number of the playlist in the -1 to -inifinity
    Playlist(QString name, qint64 id);

    /// @brief Get playlist's name
    ///
    /// Getter method to get the name of the playlist as a QString
    /// @return QString name: The name of the playlist
    QString getName();

    /// @brief Get the playlist ID
    ///
    /// Getter method to get the id of the playlist as a qint64
    /// @return qint64 int: The id of the playlist
    qint64 getID();

    /// @brief Get playlist's songs
    ///
    /// Getter method to get the songs in the playlist as a vector of Song objects
    /// @see Song
    /// @return vector<Song> songList: All songs in the playlist as Song objects in a vector
    vector<Song> getSongs();

    /// @brief Delete a song from the playlist
    ///
    /// Deleted song is specified by the song's unqiue id
    /// @param id The id of the song to be deleted from the playlist
    void deleteSong(int id);

    /// @brief Add a song to a playlist
    ///
    /// Song to be added is given as a Song object
    /// @param song A Song object to be added to the playlist
    void addSong(Song song);

    /// @brief Determine if a given search QString is contained in the playlists name
    ///
    /// Each "word" in the given search string, separated by spaces, is split, and if all
    /// words are in the playlist's name, method returns true
    /// @param search Specifies the critera that will be determined if it is contained in the playlist's name
    /// @returns "true" if all words in the given search string are in the playlist's name, "false" if atleast one word in the given search string is not in the playlist's name
    bool contains(QString search);

    /// @brief Setter to set the id of the playlist
    ///
    /// Used if the playlist's unique id is changed due to another playlist being deleted
    /// @see Library::removePlaylist()
    void setID(qint64 newID);
};

#endif // PLAYLIST_H
