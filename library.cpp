#include "library.h"

Library::Library()
{
    songs = json.getSongs();
    playlists = json.getPlaylists(&songs);
}

vector<Song> Library::getSongs(){
    return songs;
}

vector<Playlist>& Library::getPlaylists(){
    return playlists;
}

void Library::savePlaylists(){
    json.savePlaylists(&playlists);
}

void Library::removePlaylist(int id){
    for (int i = 0; i < playlists.size(); i++){
        Playlist testPlaylist = playlists.at(i);
        if (testPlaylist.getID() == id){
            try {
                playlists.erase(playlists.begin()+i);

                for (int j = i, k = 0; j < playlists.size(); j++, k++){
                    playlists.at(j).setID(id-k);
                }
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
