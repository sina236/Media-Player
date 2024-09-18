#include "song.h"
#include <sstream>

Song::Song(QString name, QString album, QString artist, QString address, QString albumCover, int id) {
    QDir dir;
    songName = name;
    songAlbum = album;
    songArtist = artist;
    this->address = dir.absolutePath() + address;
    this->albumCover = dir.absolutePath() + albumCover;
    this->id = id;
}

QString Song::getSongName(){
    return songName;
}

QString Song::getSongAlbum() {
    return songAlbum;
}

QString Song::getSongArtist() {
    return songArtist;
}

QString Song::getAddress(){
    return address;
}

int Song::getID(){
    return id;
}

QString Song::getSongCover() {
    return albumCover;
}

bool Song::contains(QString search) {
    try{
        std::vector<std::string> searchWords; // Vector to hold each word separated by spaces from search input
        std::string searchStr = search.toStdString();
        std::stringstream ss(searchStr);
        std::string separatedWord;
        std::string nameLower = songName.toLower().toStdString();
        std::string artistLower = songArtist.toLower().toStdString();
        std::string albumLower = songAlbum.toLower().toStdString();

        while (ss >> separatedWord) { // Take each word separated by spaces from input search and put it into vector searchWords
            transform(separatedWord.begin(), separatedWord.end(), separatedWord.begin(), ::tolower);
            searchWords.push_back(separatedWord);
        }

        if (searchWords.empty()) {
            return false;
        }

        for (const std::string& word : searchWords) { // Check if a word from search sting is in either the song name, album, or artist
            bool foundInName = nameLower.find(word) != std::string::npos;
            bool foundInAlbum = albumLower.find(word) != std::string::npos;
            bool foundInArtist = artistLower.find(word) != std::string::npos;

            if (!(foundInName || foundInAlbum || foundInArtist)) {
                return false;
            }
        }

        return true;
    } catch (const std::out_of_range& e) {
    std::cerr << "Exception caught - contains method: " << e.what() << std::endl;
    }
}
