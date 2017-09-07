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

class MainWindow : public QMainWindow
{

Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void btnOpen_clicked();

private:
    QPushButton *m_button;
    QTableWidget *file_list;
    QFileDialog *dialog;
    QString directoryName;

    void setup_list_table();
    void setup_path_button();
    void setup_directory_dialog();

};
