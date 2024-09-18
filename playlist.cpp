#include "playlist.h"
#include <sstream>

Playlist::Playlist(QString name, std::vector<Song> songList, qint64 id) {
    this->name = name;
    this->songList = songList;
    this->id = id;
}

Playlist::Playlist(QString name, qint64 id) {
    this->name = name;
    this->id = id;
}

QString Playlist::getName(){
    return name;
}

qint64 Playlist::getID(){
    return id;
}

vector<Song> Playlist::getSongs(){
    return songList;
}

void Playlist::setID(qint64 newID){
    id = newID;
}

void Playlist::deleteSong(int id) {
    for (int i = 0; i < songList.size(); i++){
        Song testSong = songList.at(i);
        if (testSong.getID() == id){
            try {
                songList.erase(songList.begin()+i);
            } catch (const std::out_of_range& e) {
                // Handle the exception within the Playlist class
                std::cerr << "Exception caught: " << e.what() << std::endl;
                // Optionally rethrow the exception to propagate it to the calling code
                throw;
            }
            break;
        }
    }
}

void Playlist::addSong(Song song) {
    songList.push_back(song);
}

bool Playlist::contains(QString search) {
    try{
        std::vector<std::string> searchWords; // Vector to hold each word separated by spaces from search input
        std::string searchStr = search.toStdString();
        std::stringstream ss(searchStr);
        std::string separatedWord;

        while (ss >> separatedWord) { // Take each word separated by spaces from input search and put it into vector searchWords
            transform(separatedWord.begin(), separatedWord.end(), separatedWord.begin(), ::tolower);
            searchWords.push_back(separatedWord);
        }

        if (searchWords.empty()) {
            return false;
        }

        std::string nameLower = name.toLower().toStdString();

        for (const std::string& word : searchWords) {
            bool foundInName = nameLower.find(word) != std::string::npos;

            if (!(foundInName)) {
                return false;
            }
        }
        return true;

    } catch (const std::out_of_range& e) {
        std::cerr << "Exception caught - contains method: " << e.what() << std::endl;
    }
    return false;
}
