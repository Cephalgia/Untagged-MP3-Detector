//
// Created by cephalgia on 04.09.17.
//

#include "MainWindow.h"
#include <QHeaderView>
#include <iostream>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    setFixedSize(1200, 900);

    setup_path_button();
    setup_list_table();
    setup_directory_dialog();
    setup_tag_edit_area();

    connect(m_button, SIGNAL (clicked()), this, SLOT(btnOpen_clicked()));
    connect(this, SIGNAL (directory_selected()), this, SLOT(scan_directory()));
    connect(this, SIGNAL (directory_scanned()), this, SLOT(fill_table()));
    connect(file_list, SIGNAL (cellClicked(int, int)), this, SLOT(load_tag_to_form(int, int)));


}

void MainWindow::setup_list_table()
{
    file_list = new QTableWidget(this);

    file_list->setColumnCount(1);
    file_list->setGeometry(5, 40, 300, 890);
    file_list->setColumnWidth(0, 300);

    QPalette p = file_list->palette();
    p.setColor(QPalette::Base, Qt::white);

    file_list->setShowGrid(false);
    file_list->verticalHeader()->setVisible(false);
    file_list->horizontalHeader()->setVisible(false);

}

void MainWindow::setup_path_button()
{
    m_button = new QPushButton("Open", this);
    m_button->setGeometry(5, 5, 80, 30);
}

void MainWindow::setup_directory_dialog()
{
    dialog = new QFileDialog(this);
    dialog->setFileMode(QFileDialog::Directory);
    dialog->setDirectory("/home/cephalgia");
}

void MainWindow::setup_tag_edit_area()
{
    edit_tag_area = new QWidget(this);
    edit_tag_area->setGeometry(305, 40, 895, 895);

    layout = new QFormLayout();

    path = new QLabel(tr(""));
    line_title = new QLineEdit;
    line_artist = new QLineEdit;
    line_album = new QLineEdit;
    line_year = new QLineEdit;
    line_comment = new QLineEdit;
    line_track_num = new QLineEdit;
    select_genre = new QComboBox();
    save_button = new QPushButton("Save", edit_tag_area);

    select_genre->insertItems(0, genres);
    save_button->setEnabled(false);

    layout->addRow(new QLabel(tr("Path to selected file: ")), path);
    layout->addRow(new QLabel(tr("Title")), line_title);
    layout->addRow(new QLabel(tr("Artist")), line_artist);
    layout->addRow(new QLabel(tr("Album")), line_album);
    layout->addRow(new QLabel(tr("Year")), line_year);
    layout->addRow(new QLabel(tr("Comment")), line_comment);
    layout->addRow(new QLabel(tr("Track number")), line_track_num);
    layout->addRow(new QLabel(tr("Genre")), select_genre);
    layout->addRow(save_button);

    edit_tag_area->setLayout(layout);
    edit_tag_area->show();
}

void MainWindow::load_tag_to_form(int row, int col)
{
    path->setText(QString::fromStdString(target_files[row].path));
    line_title->setText(QString::fromStdString(target_files[row].title));
    line_artist->setText(QString::fromStdString(target_files[row].artist));
    line_album->setText(QString::fromStdString(target_files[row].album));
    line_year->setText(QString::fromStdString(target_files[row].year));
    line_comment->setText(QString::fromStdString(target_files[row].comment));
    line_track_num->setText(QString::number(target_files[row].track_num));
    int current_genre = target_files[row].genre;
    if(current_genre < 255)
    {
        select_genre->setCurrentIndex(current_genre+1);
    } else
    {
        select_genre->setCurrentIndex(0);
    }

    save_button->setEnabled(true);

    connect(save_button, SIGNAL(clicked()), this, SLOT(save_file()));
}

void MainWindow::btnOpen_clicked()
{
    directoryName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                      "/home/cephalgia",
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
    if(!directoryName.isEmpty())
    {
        directory_selected();
    }
}

void MainWindow::scan_directory()
{
    file_list->setRowCount(0);

    target_files = find_empty_tag_files(boost::filesystem::path(directoryName.toStdString()));
    if(!target_files.empty())
    {
        directory_scanned();
    }
}

void MainWindow::fill_table()
{
    file_list->setRowCount(target_files.size());
    for(int i = 0; i < target_files.size(); i++)
    {
        std::string item_name = target_files[i].path.substr(target_files[i].path.find_last_of('/')+1);
        file_list->setItem(i,0, new QTableWidgetItem(QString::fromStdString(item_name.c_str())));
    }
}

void MainWindow::save_file()
{
    tag new_tag;
    new_tag.title = line_title->text().toStdString();
    new_tag.artist = line_artist->text().toStdString();
    new_tag.album = line_album->text().toStdString();
    new_tag.year = line_year->text().toStdString();
    new_tag.comment = line_comment->text().toStdString();
    new_tag.track_num = line_track_num->text().toInt();

    int selected_genre = select_genre->currentIndex();
    if(selected_genre == 0)
    {
        new_tag.genre = 255;
    } else
    {
        new_tag.genre = select_genre->currentIndex() - 1;
    }

    new_tag.path = path->text().toStdString();
    write_tag(new_tag);
}

MainWindow::~MainWindow()
{
    delete(file_list);
    delete(m_button);
    delete(dialog);
}