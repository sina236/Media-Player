#include "playlistpopup.h"
#include "ui_playlistpopup.h"
#include "playlist.h"

PlaylistPopup::PlaylistPopup(Library &library, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaylistPopup),
    library(library)
{
    ui->setupUi(this);
    setWindowTitle("Spotishy");
}

PlaylistPopup::~PlaylistPopup()
{
    delete ui;
}

void PlaylistPopup::on_playlistNameInput_returnPressed()
{
    QString playlistName = ui->playlistNameInput->text();
    if (playlistName.isEmpty()) {
        playlistName = "Untitled";
    }

    QString originalName = playlistName;

    for (auto i = library.getPlaylists().begin(); i != library.getPlaylists().end(); ++i) {
        if (i->getName() == playlistName) {
            int suffix = 1;

            for (auto j = library.getPlaylists().begin(); j != library.getPlaylists().end(); ++j) {
                if (j != i && j->getName() == playlistName + QString::number(suffix)) {
                    ++suffix;
                    j = library.getPlaylists().begin();
                }
            }

            playlistName = originalName + QString::number(suffix);
            break;
        }
    }

    Playlist playlist(playlistName, (library.getPlaylists().size()*-1)-1);
    library.getPlaylists().push_back(playlist);
    library.savePlaylists();


    this->close();
}


void PlaylistPopup::on_confirmButton_clicked()
{
    QString playlistName = ui->playlistNameInput->text();
    if (playlistName.isEmpty()) {
        playlistName = "Untitled";
    }

    QString originalName = playlistName;

    for (auto it = library.getPlaylists().begin(); it != library.getPlaylists().end(); ++it) {
        if (it->getName() == playlistName) {
            int suffix = 1;

            for (auto innerIt = library.getPlaylists().begin(); innerIt != library.getPlaylists().end(); ++innerIt) {
                if (innerIt != it && innerIt->getName() == playlistName + QString::number(suffix)) {
                    ++suffix;
                    innerIt = library.getPlaylists().begin();
                }
            }

            playlistName = originalName + QString::number(suffix);
            break;
        }
    }

    Playlist playlist(playlistName, (library.getPlaylists().size()*-1)-1);
    library.getPlaylists().push_back(playlist);
    library.savePlaylists();

    this->close();
}

