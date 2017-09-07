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

    connect(m_button, SIGNAL (clicked()), this, SLOT(btnOpen_clicked()));

}

void MainWindow::setup_list_table()
{
    file_list = new QTableWidget(this);

    file_list->setColumnCount(1);
    file_list->setGeometry(0, 40, 300, 900);
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

void MainWindow::btnOpen_clicked()
{
    directoryName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                      "/home/cephalgia",
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
    std::cout << directoryName.toStdString();
}

MainWindow::~MainWindow()
{
    delete(file_list);
    delete(m_button);
    delete(dialog);
}