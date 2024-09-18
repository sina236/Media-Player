#include "mainwindow.h"
#include "library.h"
#include "ui_mainwindow.h"
#include "addsongpopup.h"
#include "playlistpopup.h"
#include <QPixmap>
#include <iostream>
#include <QDir>
#include <QMediaPlayer>
#include <QUrl>
#include <QDebug>
#include <QAudioOutput>
#include <QSlider>
#include <random>

QMediaPlayer* player = new QMediaPlayer; // QMediaPlayer to play music
QAudioOutput* audioOutput = new QAudioOutput; // The audio output device
Library library = Library(); // Song and playlist library
QTime currTime = QTime().currentTime(); // The current time

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    audioOutput->setVolume(0.5);
    player->setAudioOutput(audioOutput);
    ui->setupUi(this);
    setWindowTitle("Spotishy");

    for (Song song : library.getSongs()){
        queue.push_back(song.getID());
    }
    updateQueueView();
    updateNP();
    refresh3Playlists();

    // connecting all volume sliders
    connect(ui->volumeSliderLib, &QSlider::valueChanged, this, &MainWindow::updateVolumeSliders);
    connect(ui->volumeSliderQP, &QSlider::valueChanged, this, &MainWindow::updateVolumeSliders);
    connect(ui->volumeSliderNP, &QSlider::valueChanged, this, &MainWindow::updateVolumeSliders);
    // connecting play-pause buttons
    connect(ui->playPauseButtonLib, &QPushButton::clicked, this, &MainWindow::handlePlayPauseButton);
    connect(ui->playPauseButtonQP, &QPushButton::clicked, this, &MainWindow::handlePlayPauseButton);
    connect(ui->playPauseButtonNP, &QPushButton::clicked, this, &MainWindow::handlePlayPauseButton);
    // connect all sliders to allow user to change song position
    connect(ui->songPositionSliderLib, &QSlider::valueChanged, this, &MainWindow::handleChangeSongPos);
    connect(ui->songPositionSliderQP, &QSlider::valueChanged, this, &MainWindow::handleChangeSongPos);
    connect(ui->songPositionSliderNP, &QSlider::valueChanged, this, &MainWindow::handleChangeSongPos);
    // connect sliders so that if user presses the slider the song pauses
    connect(ui->songPositionSliderLib, &QSlider::sliderPressed, this, [=]() {player->pause();});
    connect(ui->songPositionSliderQP, &QSlider::sliderPressed, this, [=]() {player->pause();});
    connect(ui->songPositionSliderNP, &QSlider::sliderPressed, this, [=]() {player->pause();});
    // connect sliders so that when user releases the slider, the song resumes if it was playing
    connect(ui->songPositionSliderLib, &QSlider::sliderReleased, this, [=]() {if (playing) player->play();});
    connect(ui->songPositionSliderQP, &QSlider::sliderReleased, this, [=]() {if (playing) player->play();});
    connect(ui->songPositionSliderNP, &QSlider::sliderReleased, this, [=]() {if (playing) player->play();});
    // Run updateWithSongPlaying() while the song is playing
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::updateWithSongPlaying);

    // Fill librarySearchListLib with all songs and playlists
    on_librarySearchBarLib_textChanged("");

    lastClickedIsPlaylist = false;
}


MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::on_changeLibToQPButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(0);
    refresh3Playlists();
    updateTime();
}


void MainWindow::on_changeLibToNPButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(2);
    updateTime();
}


void MainWindow::on_changeNPToLibButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(1);
    updateTime();
}


void MainWindow::on_changeQPtoLibButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(1);
    updateTime();
}


void MainWindow::on_changeQPToNPButtonQP_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(2);
    refresh3Playlists();
    updateTime();
}


void MainWindow::on_changeNPToQpButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(0);
    refresh3Playlists();
    updateTime();
}


void MainWindow::on_lastTrackButtonQP_clicked()
{
    lastSong();
    playSong();
    playing = 1;
}


void MainWindow::on_skipTrackButtonQP_clicked()
{
    nextSong();
    playSong();
    playing = 1;

}


void MainWindow::on_skipTrackButtonLib_clicked()
{
    nextSong();
    playSong();
    playing = 1;
}


void MainWindow::on_lastTrackButtonLib_clicked()
{
    lastSong();
    playSong();
    playing = 1;
}


void MainWindow::on_skipTrackButtonNP_clicked()
{
    nextSong();
    playSong();
    playing = 1;
}


void MainWindow::on_lastTrackButtonNP_clicked()
{
    lastSong();
    playSong();
    playing = 1;
}


void MainWindow::handlePlayPauseButton()
{
    if (!playing){
        playSong();
    }else{
        pauseSong();
    }
}

void MainWindow::updateVolumeSliders(int value)
{
    float floatValue = static_cast<float>(value) / 100.0;
    audioOutput->setVolume(floatValue);

    ui->volumeSliderLib->setValue(value);
    ui->volumeSliderQP->setValue(value);
    ui->volumeSliderNP->setValue(value);
}

void MainWindow::updateNP(){
    QPixmap albumCoverPic;
    try{albumCoverPic = QPixmap(library.getSongs().at(queue.at(currSongPos)).getSongCover());
    }catch (const std::out_of_range& e) {
        std::cerr << "Exception caught - contains method: " << e.what() << std::endl;
    }
    player->setSource(QUrl::fromLocalFile(library.getSongs().at(queue.at(currSongPos)).getAddress()));

    // Turn image of first song into pixmap
    ui->albumCoverLabelLib->setPixmap(albumCoverPic.scaled(ui->albumCoverLabelLib->width(),ui->albumCoverLabelLib->height())); // Set image of small album cover
    ui->songNameLabelLib->setText(library.getSongs().at(queue.at(currSongPos)).getSongName()); // Set library page labels
    ui->artistNameLabelLib->setText(library.getSongs().at(queue.at(currSongPos)).getSongArtist());
    ui->songTimeLenLabelLib->setText(millisecondsToString(player->duration()));

    ui->albumCoverLabelNP->setPixmap(albumCoverPic.scaled(ui->albumCoverLabelNP->width(),ui->albumCoverLabelNP->height())); // Set image of large album cover
    ui->songNameLabelNP->setText(library.getSongs().at(queue.at(currSongPos)).getSongName()); // Set now playing page labels
    ui->artistnameLabelNP->setText(library.getSongs().at(queue.at(currSongPos)).getSongArtist());
    ui->albumNameLabelNP->setText(library.getSongs().at(queue.at(currSongPos)).getSongAlbum());
    ui->songTimeLenLabelNP->setText(millisecondsToString(player->duration()));

    ui->albumCoverLabelQP->setPixmap(albumCoverPic.scaled(ui->albumCoverLabelLib->width(),ui->albumCoverLabelLib->height())); // Set image of small album cover
    ui->songNameLabelQP->setText(library.getSongs().at(queue.at(currSongPos)).getSongName()); // Set quick play page labels
    ui->artistNameLabelQP->setText(library.getSongs().at(queue.at(currSongPos)).getSongArtist());
    ui->songTimeLenLabelQP->setText(millisecondsToString(player->duration()));

    updateTime();
}


void MainWindow::updateTime(){
    QString stringTime = QTime().currentTime().toString("h:mm ap");
    stringTime.chop(3);
    ui->currentTimeLib->setText(stringTime);
    ui->currentTimeQP->setText(stringTime);
}


void MainWindow::nextSong(){
    if (currSongPos+1 < queue.size()){
        currSongPos++;
    } else {
        currSongPos = 0;
    }
    updateQueueView();
    updateNP();
}


void MainWindow::lastSong(){
    cerr << player->position() << endl;
    if (player->position() > 3000){
        player->setSource(QUrl::fromLocalFile(library.getSongs().at(queue.at(currSongPos)).getAddress()));
        updateTime();
    }
    else {
        if (currSongPos == 0){
            currSongPos = queue.size()-1;
        } else {
            currSongPos--;
        }
    }
    updateQueueView();
    updateNP();
}

void MainWindow::playSong(){

    player->play();
        playing = 1;
        ui->playPauseButtonLib->setText("⏸");
        ui->playPauseButtonNP->setText("⏸");
        ui->playPauseButtonQP->setText("⏸");

}

void MainWindow::pauseSong(){

    player->pause();
        playing = 0;
        ui->playPauseButtonLib->setText("⏵");
        ui->playPauseButtonNP->setText("⏵");
        ui->playPauseButtonQP->setText("⏵");
}

void MainWindow::on_addPlaylistButton_clicked()
{
    // Creates playlist popup
    PlaylistPopup* popup = new PlaylistPopup(library);
    // Detects when popup is closed
    connect(popup, &QDialog::finished, this, [=]() {
        // Updates library with new playlists and refreshes 3 playlists
        on_librarySearchBarLib_textChanged(ui->librarySearchBarLib->displayText());
        refresh3Playlists();
    });

    // Popup appears on screen and takes focus
    popup->setModal(true);
    popup->exec();
    updateTime();
}



void MainWindow::updateActivePlaylist()
{
    ui->activePlaylistLabelLib->setText(activePlaylist.getName());

    ui->activePlaylistList->clear();
    activePlaylistOrder.clear();

    // Updates the songs for the active playlist in library view
    for (int i = 0; i < activePlaylist.getSongs().size(); ++i) {
        Song currSong = activePlaylist.getSongs().at(i);
        activePlaylistOrder.push_back(currSong.getID());
        QListWidgetItem *songItem = new QListWidgetItem(QIcon(currSong.getSongCover()), currSong.getSongName() + " - " + currSong.getSongArtist());
        ui->activePlaylistList->addItem(songItem);
    }
    updateTime();
}

void MainWindow::on_addSongButton_clicked()
{
    if (lastClickedIsPlaylist) {
        // Creates song popup
        AddSongPopup* popup = new AddSongPopup(library, library.getPlaylists().at((searchListOrder.at(ui->librarySearchListLib->currentRow())+1)*-1));
        // Detects when popup is closed
        connect(popup, &QDialog::finished, this, [=]() {
            // Updates playlists + queue
            activePlaylist = library.getPlaylists().at((searchListOrder.at(ui->librarySearchListLib->currentRow())+1)*-1);
            updateActivePlaylist();
            if (playlistSongPlaying){
                queue.clear();
                for (Song song : activePlaylist.getSongs()) {
                    queue.push_back(song.getID());
                }
                updateQueueView();
            }
        });

        // Popup appears on screen and takes focus
        popup->setModal(true);
        popup->exec();
        updateTime();
    }
}

void MainWindow::refresh3Playlists() {
    // Clears all playlists
    ui->quickPlaylist1->clear();
    ui->quickPlaylist2->clear();
    ui->quickPlaylist3->clear();
    ui->quickPlaylist1Label->setText("");
    ui->quickPlaylist2Label->setText("");
    ui->quickPlaylist3Label->setText("");

    if (library.getPlaylists().size()>0) {
        // Generates random available index from playlist 1
        std::random_device rd;
        std::mt19937 gen(rd());
        std::size_t vectorSize = library.getPlaylists().size();
        std::uniform_int_distribution<std::size_t> distribution(0, vectorSize - 1);
        std::size_t randomIndex = distribution(gen);
        qpPlaylist1 = &library.getPlaylists()[randomIndex];
        // Sets name of playlist in GUI
        ui->quickPlaylist1Label->setText(qpPlaylist1->getName());
        // Populates playlist 1 view with selected playlist
        for (int i = 0; i < qpPlaylist1->getSongs().size(); ++i) {
            Song currSong = qpPlaylist1->getSongs().at(i);
            qpPlaylist1Order.push_back(currSong.getID());
            QListWidgetItem *songItem = new QListWidgetItem(QIcon(currSong.getSongCover()), currSong.getSongName() + " - " + currSong.getSongArtist());
            ui->quickPlaylist1->addItem(songItem);
        }

        // Generates random available index from playlist 2
        std::random_device rd2;
        std::mt19937 gen2(rd());
        std::size_t vectorSize2 = library.getPlaylists().size();
        std::uniform_int_distribution<std::size_t> distribution2(0, vectorSize2 - 1);
        std::size_t randomIndex2 = distribution(gen);
        qpPlaylist2 = &library.getPlaylists()[randomIndex2];
        // Sets name of playlist in GUI
        ui->quickPlaylist2Label->setText(qpPlaylist2->getName());
        // Populates playlist 2 view with selected playlist
        for (int i = 0; i < qpPlaylist2->getSongs().size(); ++i) {
            Song currSong = qpPlaylist2->getSongs().at(i);
            qpPlaylist2Order.push_back(currSong.getID());
            QListWidgetItem *songItem = new QListWidgetItem(QIcon(currSong.getSongCover()), currSong.getSongName() + " - " + currSong.getSongArtist());
            ui->quickPlaylist2->addItem(songItem);
        }

        // Generates random available index from playlist 3
        std::random_device rd3;
        std::mt19937 gen3(rd());
        std::size_t vectorSize3 = library.getPlaylists().size();
        std::uniform_int_distribution<std::size_t> distribution3(0, vectorSize3 - 1);
        std::size_t randomIndex3 = distribution(gen);
        qpPlaylist3 = &library.getPlaylists()[randomIndex3];
        // Sets name of playlist in GUI
        ui->quickPlaylist3Label->setText(qpPlaylist3->getName());
        // Populates playlist 3 view with selected playlist
        for (int i = 0; i < qpPlaylist3->getSongs().size(); ++i) {
            Song currSong = qpPlaylist3->getSongs().at(i);
            qpPlaylist3Order.push_back(currSong.getID());
            QListWidgetItem *songItem = new QListWidgetItem(QIcon(currSong.getSongCover()), currSong.getSongName() + " - " + currSong.getSongArtist());
            ui->quickPlaylist3->addItem(songItem);
        }
    }
}

void MainWindow::on_librarySearchBarLib_textChanged(const QString &arg1)
{
    ui->librarySearchListLib->clear();
    searchListOrder.clear();

    if (arg1.isEmpty()){
        // Fill search list with all songs and playlists
        for (Song song : library.getSongs()){
            searchListOrder.push_back(song.getID());
            ui->librarySearchListLib->addItem(new QListWidgetItem(QIcon(song.getSongCover()), song.getSongName() + " - " + song.getSongArtist())); // Add song to search list
        }
        for (Playlist playlist : library.getPlaylists()){
            searchListOrder.push_back(playlist.getID());
            ui->librarySearchListLib->addItem(new QListWidgetItem(playlist.getName())); // Add playlist to search list
        }
    }
    else {
        for (Song song : library.getSongs()){ // Search songs for search string
            if (song.contains(arg1)){
                searchListOrder.push_back(song.getID()); // Keep track of the item ids in the search list
                ui->librarySearchListLib->addItem(new QListWidgetItem(QIcon(song.getSongCover()), song.getSongName() + " - " + song.getSongArtist())); // Add song to search list
            }
        }
        for (Playlist playlist : library.getPlaylists()){ // Search playlists for search string
            if (playlist.contains(arg1)){
                searchListOrder.push_back(playlist.getID()); // Keep track of the item ids in the search list
                ui->librarySearchListLib->addItem(new QListWidgetItem(playlist.getName())); // Add playlist to search list
            }
        }
    }
}


void MainWindow::on_librarySearchListLib_itemClicked()
{
    if (searchListOrder.at(ui->librarySearchListLib->currentRow()) >= 0) { // If song selected, play song
        lastClickedIsPlaylist = false;
        if (playlistSongPlaying){
            queue.clear();
            for (Song song : library.getSongs()){ // Fill the queue with the library songs if previously playing playlist
                queue.push_back(song.getID());
            }
        }
        currSongPos = searchListOrder.at(ui->librarySearchListLib->currentRow()); // Update currSong to selected song
        updateNP(); // Update now playing with new currSong
        updateQueueView();
        playSong(); // Play the new song
        playing = 1;
        activePlaylist = Playlist("", 0);
        updateActivePlaylist(); // The playlist being referenced
        playlistSongPlaying = false;
    } else { // If playlist selected, play playlist
        activePlaylist = library.getPlaylists().at((searchListOrder.at(ui->librarySearchListLib->currentRow())+1)*-1);
        updateActivePlaylist(); // The playlist being referenced
        lastClickedIsPlaylist = true;
    }
}

void MainWindow::handleChangeSongPos(int value){
    if(changingPositionSlider<=0){ // If song position slider changed due to human interaction
        // Update all position sliders
        ui->songPositionSliderLib->setSliderPosition(value);
        ui->songPositionSliderQP->setSliderPosition(value);
        ui->songPositionSliderNP->setSliderPosition(value);

        int songPos = (float)player->duration()*(float)value/100; // Get the song position from the slider
        player->setPosition(songPos); // Update the media player to slider position
    } else changingPositionSlider--; // If song position slider changed not due to human interaction
}

void MainWindow::updateWithSongPlaying(){
    // Update the position slider as the song plays
    changingPositionSlider = 3;
    int pos = ((float)player->position()/(float)player->duration()*100);
    ui->songPositionSliderLib->setSliderPosition(pos);
    ui->songPositionSliderQP->setSliderPosition(pos);
    ui->songPositionSliderNP->setSliderPosition(pos);
    changingPositionSlider = 0;

    // Update the current position time label
    QString currentSongTime = millisecondsToString(player->position());
    ui->songTimePosLabelLib->setText(currentSongTime);
    ui->songTimePosLabelQP->setText(currentSongTime);
    ui->songTimePosLabelNP->setText(currentSongTime);

    updateTime();

    if (player->position() >= player->duration()-100){
        nextSong();
        playSong();
        playing = 1;
    }
}

QString MainWindow::millisecondsToString(int ms){
    if (ms/1000%60 < 10){
         return QString::number(ms/1000/60) + ":0" + QString::number(ms/1000%60);
    } else {
        return QString::number(ms/1000/60) + ":" + QString::number(ms/1000%60);
    }
}

void MainWindow::on_removeSongButton_clicked()
{
    if (ui->activePlaylistList->currentRow() >= 0){
        library.getPlaylists().at(activePlaylist.getID()*-1-1).deleteSong(activePlaylistOrder.at(ui->activePlaylistList->currentRow()));
        activePlaylist.deleteSong(activePlaylistOrder.at(ui->activePlaylistList->currentRow()));
        updateActivePlaylist();
        library.savePlaylists();
        if (playlistSongPlaying){
            queue.clear();
            for (Song song : activePlaylist.getSongs()){ // Fill the queue with the playlist songs
                queue.push_back(song.getID());
            }
            updateQueueView();
        }
    }
}


void MainWindow::on_activePlaylistList_itemClicked(QListWidgetItem *item)
{
    queue.clear();
    for (Song song : activePlaylist.getSongs()){ // Fill the queue with the playlist songs
        queue.push_back(song.getID());
    }
    currSongPos = ui->activePlaylistList->currentRow(); // Update currSong to selected song
    updateNP(); // Update now playing with new currSong
    updateQueueView();
    playSong(); // Play the new song
    playlistSongPlaying = true;
    playing = 1;
}

void MainWindow::on_removePlaylistButton_clicked()
{
    if (lastClickedIsPlaylist){
        if (ui->librarySearchListLib->currentRow() >= 0){
            library.removePlaylist(searchListOrder.at(ui->librarySearchListLib->currentRow()));
            library.savePlaylists();
            if (activePlaylist.getID() == searchListOrder.at(ui->librarySearchListLib->currentRow())){
                activePlaylist = Playlist("", 0);
                updateActivePlaylist();
                lastClickedIsPlaylist = false;
            }
            on_librarySearchBarLib_textChanged(ui->librarySearchBarLib->text());
        }
    }
}

void MainWindow::updateQueueView(){
    ui->queueListLib->clear();
    for (int i = 1; i < queue.size(); i++){
        ui->queueListLib->addItem(new QListWidgetItem(QIcon(library.getSongs().at(queue.at((currSongPos+i)%queue.size())).getSongCover()), library.getSongs().at(queue.at((currSongPos+i)%queue.size())).getSongName() + " - " + library.getSongs().at(queue.at((currSongPos+i)%queue.size())).getSongArtist()));
    }
}


void MainWindow::on_quickPlaylist1_itemClicked(QListWidgetItem *item)
{
    queue.clear();

    // Adds all songs in playlist to queue
    for (Song song : qpPlaylist1->getSongs()) {
        queue.push_back(song.getID());
    }
    // Plays the selected song in playlist
    currSongPos = ui->quickPlaylist1->currentRow();
    // Updates now playing and queue GUI elements
    updateNP();
    updateQueueView();
    playSong();
    playlistSongPlaying = true;
    playing = 1;
}


void MainWindow::on_quickPlaylist2_itemClicked(QListWidgetItem *item)
{
    queue.clear();

    // Adds all songs in playlist to queue
    for (Song song : qpPlaylist2->getSongs()) {
        queue.push_back(song.getID());
    }
    // Plays the selected song in playlist
    currSongPos = ui->quickPlaylist2->currentRow();
    // Updates now playing and queue GUI elements
    updateNP();
    updateQueueView();
    playSong();
    playlistSongPlaying = true;
    playing = 1;
}


void MainWindow::on_quickPlaylist3_itemClicked(QListWidgetItem *item)
{
    queue.clear();

    // Adds all songs in playlist to queue
    for (Song song : qpPlaylist3->getSongs()) {
        queue.push_back(song.getID());
    }
    // Plays the selected song in playlist
    currSongPos = ui->quickPlaylist3->currentRow();
    // Updates now playing and queue GUI elements
    updateNP();
    updateQueueView();
    playSong();
    playlistSongPlaying = true;
    playing = 1;
}

