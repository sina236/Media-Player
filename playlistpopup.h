#ifndef PLAYLISTPOPUP_H
#define PLAYLISTPOPUP_H

#include <QDialog>
#include "library.h"

namespace Ui {
class PlaylistPopup;
}

/// @brief Popup for creating a new playlist
///
/// Can create a new playing and input a name for it
/// @authors Edson Huang
class PlaylistPopup : public QDialog
{
    Q_OBJECT

public:
    /// @brief Create a popup to create a playlist
    /// @param library The library from mainwindow of all songs and playlists
    /// @param parant The parant QWidget
    explicit PlaylistPopup(Library &library, QWidget *parent = nullptr);
    /// Deconstructor of playlist creator popup
    ~PlaylistPopup();

private slots:
    /// @brief Process inputted name for new playlist
    /// Process inputted name for new playlist when the user clicks the enter key
    void on_playlistNameInput_returnPressed();

    /// @brief Process inputted name for new playlist
    /// Process inputted name for new playlist when the user clicks the confirm button
    void on_confirmButton_clicked();

private:
    /// Pointer to the UI
    Ui::PlaylistPopup *ui;
    /// Address of the library
    Library &library;
};

#endif // PLAYLISTPOPUP_H
