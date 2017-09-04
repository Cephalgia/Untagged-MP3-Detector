//
// Created by cephalgia on 04.09.17.
//

#ifndef UNTAGGEDMP3DETECTOR_MAINWINDOW_H
#define UNTAGGEDMP3DETECTOR_MAINWINDOW_H

#endif //UNTAGGEDMP3DETECTOR_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QPushButton *m_button;
    QTableWidget *file_list;
    QFrame *list_frame;

    void setup_list_table();
};