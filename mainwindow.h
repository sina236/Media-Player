#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qlistwidget.h"
#include "playlist.h"
#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class represents the main application window.
 *
 * This class manages the user interface, playback controls, playlists, and other features
 * for "Spotishy" music player application.
 *
 * @authors Colin Brown
 * @authors Ronak Toprani
 * @authors Edson Huang
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for the MainWindow class.
     *
     * Initializes the main window and associated components.
     *
     * @param parent The parent widget.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor for the MainWindow class.
     *
     * Cleans up resources and memory allocated for the MainWindow.
     */
    ~MainWindow();

    Ui::MainWindow *ui; /**< User interface object. */

private slots:
    // Playback and navigation slots

    /**
     * @brief Switches the main window to the Quick Play mode.
     */
    void on_changeLibToQPButton_clicked();

    /**
     * @brief Switches the main window to the Now Playing mode.
     */
    void on_changeLibToNPButton_clicked();

    /**
     * @brief Switches the main window to the Library mode.
     */
    void on_changeNPToLibButton_clicked();

    /**
     * @brief Switches the main window to the Library mode from Quick Play.
     */
    void on_changeQPtoLibButton_clicked();

    /**
     * @brief Switches the main window to the Now Playing mode from Quick Play.
     */
    void on_changeQPToNPButtonQP_clicked();

    /**
     * @brief Switches the main window to the Quick Play mode from Now Playing.
     */
    void on_changeNPToQpButton_clicked();

    /**
     * @brief Skips to the next track in the current playlist or queue.
     */
    void on_skipTrackButtonLib_clicked();

    /**
     * @brief Skips to the next track in the Now Playing mode.
     */
    void on_skipTrackButtonNP_clicked();

    /**
     * @brief Goes back to the last track in the Now Playing mode.
     */
    void on_lastTrackButtonNP_clicked();

    /**
     * @brief Goes back to the last track in the Library mode.
     */
    void on_lastTrackButtonLib_clicked();

    /**
     * @brief Goes back to the last track in the Quick Play mode.
     */
    void on_lastTrackButtonQP_clicked();

    /**
     * @brief Skips to the next track in the Quick Play mode.
     */
    void on_skipTrackButtonQP_clicked();

    // Search and playlist management slots

    /**
     * @brief Updates the search list in the Library mode based on user input.
     *
     * @param arg1 The user input in the search bar.
     */
    void on_librarySearchBarLib_textChanged(const QString &arg1);

    /**
     * @brief Handles item click event in the search list in the Library mode.
     */
    void on_librarySearchListLib_itemClicked();

    /**
     * @brief Opens a dialog to add a new playlist.
     */
    void on_addPlaylistButton_clicked();

    /**
     * @brief Opens a dialog to add a new song to the active playlist.
     */
    void on_addSongButton_clicked();

    /**
     * @brief Removes the selected song from the active playlist.
     */
    void on_removeSongButton_clicked();

    /**
     * @brief Handles item click event in the active playlist.
     *
     * Plays the selected song from the active playlist.
     *
     * @param item The clicked item in the active playlist.
     */
    void on_activePlaylistList_itemClicked(QListWidgetItem *item);

    /**
     * @brief Removes the active playlist.
     */
    void on_removePlaylistButton_clicked();

    /**
     * @brief Handles item click event in Quick Playlist 1.
     *
     * Plays the selected song from Quick Playlist 1.
     *
     * @param item The clicked item in Quick Playlist 1.
     */
    void on_quickPlaylist1_itemClicked(QListWidgetItem *item);

    /**
     * @brief Handles item click event in Quick Playlist 2.
     *
     * Plays the selected song from Quick Playlist 2.
     *
     * @param item The clicked item in Quick Playlist 2.
     */
    void on_quickPlaylist2_itemClicked(QListWidgetItem *item);

    /**
     * @brief Handles item click event in Quick Playlist 3.
     *
     * Plays the selected song from Quick Playlist 3.
     *
     * @param item The clicked item in Quick Playlist 3.
     */
    void on_quickPlaylist3_itemClicked(QListWidgetItem *item);

private:
    // Playback and UI update functions

    /**
     * @brief Updates the Now Playing information.
     */
    void updateNP();

    /**
     * @brief Updates the current time display.
     */
    void updateTime();

    /**
     * @brief Plays the next song in the queue.
     */
    void nextSong();

    /**
     * @brief Plays the last song in the queue or rewinds the current song.
     */
    void lastSong();

    /**
     * @brief Plays the current song.
     */
    void playSong();

    /**
     * @brief Pauses the current song.
     */
    void pauseSong();

    /**
     * @brief Handles the play/pause button click event.
     */
    void handlePlayPauseButton();

    /**
     * @brief Handles the change in song position.
     *
     * @param value The new position value.
     */
    void handleChangeSongPos(int value);

    /**
     * @brief Updates the volume sliders.
     *
     * @param value The new volume value.
     */
    void updateVolumeSliders(int value);

    /**
     * @brief Updates the UI when a song is playing.
     */
    void updateWithSongPlaying();

    /**
     * @brief Handles the audio output change event.
     */
    void audioOutputChanged();

    /**
     * @brief Converts milliseconds to a formatted string.
     *
     * @param ms The time in milliseconds.
     * @return The formatted time string.
     */
    QString millisecondsToString(int ms);

    /**
     * @brief Refreshes the three Quick Playlists.
     */
    void refresh3Playlists();

    /**
     * @brief Updates the active playlist.
     */
    void updateActivePlaylist();

    /**
     * @brief Updates the queue view.
     */
    void updateQueueView();

    QString activePlaylistName; /**< Name of the active playlist. */
    std::vector<int> searchListOrder; /**< Track what songs are being shown in the search list. */
    bool playing = false; /**< Keep track of whether a song is playing. */
    float floatVolumeSlider = 0.0; /**< Volume slider value. */
    std::vector<int> queue; /**< Vector of song IDs in the queue. */
    int currSongPos = 0; /**< Position of the current playing song in the queue. */
    int changingPositionSlider = 0; /**< Value to track if the user is changing the song position. */
    std::vector<int> activePlaylistOrder; /**< Order of songs in the active playlist. */
    Playlist activePlaylist = Playlist("holder", 0); /**< Active playlist object. */
    bool lastClickedIsPlaylist; /**< Keep track of the last clicked item (song or playlist). */
    bool playlistSongPlaying = false; /**< Keep track of whether a playlist song is playing. */
    Playlist *qpPlaylist1; /**< Pointer to QuickPlay Playlist 1. */
    Playlist *qpPlaylist2; /**< Pointer to QuickPlay Playlist 2. */
    Playlist *qpPlaylist3; /**< Pointer to QuickPlay Playlist 3. */
    std::vector<int> qpPlaylist1Order; /**< Order of songs in QuickPlay Playlist 1. */
    std::vector<int> qpPlaylist2Order; /**< Order of songs in QuickPlay Playlist 2. */
    std::vector<int> qpPlaylist3Order; /**< Order of songs in QuickPlay Playlist 3. */
};

#endif // MAINWINDOW_H
