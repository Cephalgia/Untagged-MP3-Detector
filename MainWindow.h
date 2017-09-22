//
// Created by cephalgia on 04.09.17.
//

#ifndef UNTAGGEDMP3DETECTOR_MAINWINDOW_H
#define UNTAGGEDMP3DETECTOR_MAINWINDOW_H

#endif //UNTAGGEDMP3DETECTOR_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QFileDialog>
#include <QFormLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include "tag.h"

class MainWindow : public QMainWindow
{

Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void btnOpen_clicked();
    void scan_directory();
    void fill_table();
    void load_tag_to_form(int, int);
    void save_file();

signals:
    void directory_selected();
    void directory_scanned();

private:
    QPushButton *m_button;
    QTableWidget *file_list;
    QFileDialog *dialog;

    QString directoryName;

    QWidget *edit_tag_area;
    QFormLayout *layout;
    QLabel *path;
    QLineEdit *line_title;
    QLineEdit *line_artist;
    QLineEdit *line_album;
    QLineEdit *line_year;
    QLineEdit *line_comment;
    QLineEdit *line_track_num;
    QComboBox *select_genre;
    QPushButton *save_button;

    std::vector<tag> target_files;

    void setup_list_table();
    void setup_path_button();
    void setup_directory_dialog();
    void setup_tag_edit_area();

};
