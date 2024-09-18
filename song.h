#ifndef SONG_H
#define SONG_H
#include <iostream>
#include <QDir>

/// @brief A class to store all information for each song
///
/// A Song object will store the address of the song mp3 and its album cover jpg
/// as well as the song's name, artist, album, and a unqiue id number
/// @author Colin Brown

class Song {
private:
    /// The name of the song
    QString songName;
    /// The artist of the song
    QString songArtist;
    /// The album name of the song
    QString songAlbum;
    /// The absolute address of the song's mp3
    QString address;
    /// The absolute address of the song's album cover jpg
    QString albumCover;
    /// The unique id of the song
    int id;

public:
    /// @brief Constructor for a Song object
    ///
    /// All aspects of the Song must be specified
    /// @param name Specifies the name of the song
    /// @param album Specifies the name of the album of the song
    /// @param artist Specifies the name of the artist of the song
    /// @param address Specifes the address of the mp3 of the song
    /// @param albumCover Specifies the address of the jpg for the album of cover of the song
    /// @param id Specifies the unique id number of the song in the range of 0-infinity
    Song(QString name, QString album, QString artist, QString address, QString albumCover, int id);

    /// @brief Get the song's name
    ///
    /// Getter method to get the name of the song as a QString
    /// @return QString name: The name of the song
    QString getSongName();

    /// @brief Get the song's album name
    ///
    /// Getter method to get the album name of the song as a QString
    /// @returns QString album: The album name of the song
    QString getSongAlbum();

    /// @brief Get the song's artist name
    ///
    /// Getter method to get the artist name of the song as a QString
    /// @returns QString name: The artist name of the song
    QString getSongArtist();

    /// @brief Get the address of the song's mp3
    ///
    /// Getter method to get the absolute address of the song's mp3 as a QString
    /// @returns QString address: The absolue address of the song's mp3
    QString getAddress();

    /// @brief Get the song's id
    ///
    /// Getter method to get the unique id of a song as an int
    /// @returns int id: The id of the song
    int getID();

    /// @brief Get the address of the song's album cover
    ///
    /// Getter method to get the absolute address of jpg the song's album cover as a QString
    /// @returns QString albumCover: The absolue address of the song's album cover jpg
    QString getSongCover();

    /// @brief Determine if a given search QString is contained in the song's name, artist, or album name
    ///
    /// Each "word" in the given search string, separated by spaces, is split, and if all
    /// words are in atleast one of the name, artist, or album, contains returns true
    /// @param search Specifies the critera that will be determined if it is contained in the songs name, artist, or album name
    /// @returns "true" if all words in the given search string are in the song's metadata, "false" if atleast one word in the given search string is not in the song's matadata
    bool contains(QString search);
};

#endif // SONG_H
