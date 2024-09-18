#include "json.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>



JSON::JSON()
{

}

vector<Song> JSON::getSongs(){
    vector<Song> allSongs; // Vector to hold all songs
    try{
    QString jsonStr;
    QDir dir;
    QFile myFile(dir.absolutePath() + "/resources/songlist.json"); // File of singlist JSON


    if (myFile.exists()){
        if (myFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            jsonStr = myFile.readAll(); // Read all file into jsonStr
            myFile.close();

            QJsonDocument myDoc = QJsonDocument::fromJson(jsonStr.toUtf8());
            QJsonArray myArr = myDoc.array();

            if (myArr.empty()){
                std::cerr << "JSON error, myArr empty" << std::endl;
            } else {
                for (int i = 0; i < myArr.size(); i++){
                    QJsonObject val = myArr.at(i).toObject(); // Turn array position into json object
                    QString name = val.value("songName").toString(); // Get value with songName key
                    QString album = val.value("songAlbum").toString(); // Get value with songAlbum key
                    QString artist = val.value("songArtist").toString(); // Get value with songArtist key
                    QString address = val.value("address").toString(); // Get value with address key
                    QString albumCover = val.value("albumCover").toString(); // Get value with albumCover key
                    qint64 id = val.value("id").toInteger(); // Get value with id key
                    allSongs.push_back(Song(name, album, artist, address, albumCover, (int)id)); // Turn all JSON info into a Song object and add to vector
                }
            }
        }
    }else{
    printf("hello");
    printf("%d\n", allSongs.size());
    }
    }catch (const std::out_of_range& e) {
    std::cerr << "Exception caught - json get songs method: " << e.what() << std::endl;
    }
    return allSongs; // Return vector with all Song objects
}

vector<Playlist> JSON::getPlaylists(vector<Song>* allSongs){
    vector<Playlist> allPlaylists; // Vector to hold all songs
    QString jsonStr;
    QDir dir;
    QFile myFile(dir.absolutePath() + "/resources/playlists.json"); // File of playlists JSON

    if (myFile.exists()){
        if (myFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            jsonStr = myFile.readAll(); // Read all file into jsonStr
            myFile.close();

            QJsonDocument myDoc = QJsonDocument::fromJson(jsonStr.toUtf8());
            QJsonArray myArr = myDoc.array();

            if (myArr.empty()){
                std::cerr << "JSON error, myArr empty" << std::endl;
            } else {
                for (int i = 0; i < myArr.size(); i++){
                    vector<Song> playlistSongs;
                    QJsonObject val = myArr.at(i).toObject(); // Turn array position into json object
                    QString name = val.value("name").toString(); // Get value with name key
                    qint64 id = val.value("id").toInteger(); // Get value with id key
                    QJsonArray songsArr = val.value("songs").toArray(); // Get array of song ids
                    for (int i = 0; i < songsArr.size(); i++){
                        playlistSongs.push_back(allSongs->at(songsArr[i].toInt())); // Get song id
                    }
                    allPlaylists.push_back(Playlist(name, playlistSongs, id));
                }
            }
        }
    }
    return allPlaylists; // Return vector with all Song objects
}

void JSON::savePlaylists(vector<Playlist>* allPlaylists){
    QDir dir;
    QFile myFile(dir.absolutePath() + "/resources/playlists.json"); // File of playlists JSON

    if (myFile.exists()){
        if (myFile.open(QIODevice::ReadWrite | QIODevice::Truncate)){ // Erase all text in playlists json
            myFile.close();
        }
        if (myFile.open(QIODevice::ReadWrite | QIODevice::Text)){ // Write all playlists to JSON
            myFile.write("[\n");
            for (int playlistNum = 0; playlistNum < allPlaylists->size(); playlistNum++){ // For each playlist
                qint64 playlistID = allPlaylists->at(playlistNum).getID(); // Get playlist ID
                QString playlistName = allPlaylists->at(playlistNum).getName(); // Get playlist name
                myFile.write("    {\n        \"name\": \"" + playlistName.toUtf8() + "\",\n        \"id\": " + QByteArray::number(playlistID) + ",\n        \"songs\": ["); // Write playlist name and ID
                for (int playlistSong = 0; playlistSong < allPlaylists->at(playlistNum).getSongs().size(); playlistSong++){ // For each song in the playlist
                    if (playlistSong != 0) myFile.write(","); // If first song, don't write ","
                    qint64 songID = allPlaylists->at(playlistNum).getSongs().at(playlistSong).getID();
                    myFile.write(QByteArray::number(songID)); // Write song ID in JSON array
                }
                if (playlistNum < allPlaylists->size()-1) myFile.write("]\n    },\n"); // If there is another playlist to write, include ","
                else myFile.write("]\n    }\n"); // If last playlist was just written
            }
            myFile.write("]");
            myFile.close();
        }
    }
}
