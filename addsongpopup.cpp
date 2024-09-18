#include "addsongpopup.h"
#include "ui_addsongpopup.h"

AddSongPopup::AddSongPopup(Library &library, Playlist &clickedPlaylist, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSongPopup),
    library(library),
    clickedPlaylist(clickedPlaylist)
{
    ui->setupUi(this);
    setWindowTitle("Spotishy");
    // Populates song view in popup
    for (Song song : library.getSongs()){
        searchListOrder.push_back(song.getID());
        ui->addSongsList->addItem(new QListWidgetItem(QIcon(song.getSongCover()), song.getSongName() + " - " + song.getSongArtist()));
    }
}

AddSongPopup::~AddSongPopup()
{
    delete ui;
}

void AddSongPopup::on_addSongPopupButton_clicked()
{
    // Gets selected song
    Song addSong = library.getSongs().at((searchListOrder.at(ui->addSongsList->currentRow())));
    bool songAlreadyIn = false;
    // Checks if song already exists in playlist
    for (int i = 0; i < clickedPlaylist.getSongs().size(); i++){
        if (clickedPlaylist.getSongs().at(i).getID() == addSong.getID()) {
            ui->errorLabel->setText("That song is already in this playlist");
            songAlreadyIn = true;
            break;
        }
    }
    // Add song to playlist
    if (!songAlreadyIn){
        clickedPlaylist.addSong(addSong);
        library.savePlaylists();

        this->close();
    }
}


void AddSongPopup::on_addSongSearchBar_textChanged(const QString &arg1)
{
    ui->addSongsList->clear();
    searchListOrder.clear();

    // Updates the song view based on user's search
    for (Song song : library.getSongs()) {
        if (song.contains(arg1)){
            searchListOrder.push_back(song.getID());
            ui->addSongsList->addItem(new QListWidgetItem(QIcon(song.getSongCover()), song.getSongName() + " - " + song.getSongArtist()));
        }
    }
}
