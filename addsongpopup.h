#ifndef ADDSONGPOPUP_H
#define ADDSONGPOPUP_H

#include <QDialog>
#include "library.h"
#include "song.h"
#include "playlist.h"
#include "qlistwidget.h"
#include <QListWidgetItem>


namespace Ui {
class AddSongPopup;
}
///
/// @brief This class represents the add song popup
///
/// This class represents the popup that appears when a user chooses adds a song to a playlist.
/// It handles the current playlist clicked by the user and reads + writes to the library
/// @author Edson Huang
///
class AddSongPopup : public QDialog
{
    Q_OBJECT

public:
    ///
    /// \brief Constructor for the AddSongPopup class
    /// \param library The current library object of the application
    /// \param activePlaylistName The name of the currently selected playlist
    /// \param clickedPlaylist The object of the currently selected playlist
    /// \param parent The parent widget
    ///
    explicit AddSongPopup(Library &library, Playlist &clickedPlaylist, QWidget *parent = nullptr);
    /// \brief Destructor for the AddSongPopup class
    ~AddSongPopup();

private slots:
    ///
    /// @brief Adds user's selected song to the currently active playlist
    ///
    void on_addSongPopupButton_clicked();

    ///
    /// @brief Refines the song view based on the user's search
    /// \param arg1 Text in the search bar
    ///
    void on_addSongSearchBar_textChanged(const QString &arg1);

private:
    /// User interface object
    Ui::AddSongPopup *ui;
    /// Library object of the application
    Library &library;
    /// Track songs in search
    std::vector<int> searchListOrder;
    /// Object of selected playlist
    Playlist &clickedPlaylist;
};

#endif // ADDSONGPOPUP_H
