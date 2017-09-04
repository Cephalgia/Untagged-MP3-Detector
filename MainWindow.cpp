//
// Created by cephalgia on 04.09.17.
//

#include "MainWindow.h"
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    setFixedSize(1200, 900);
    m_button = new QPushButton("Open", this);
    m_button->setGeometry(5, 5, 80, 30);

    setup_list_table();

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

    file_list->show();
}

MainWindow::~MainWindow()
{
    delete(file_list);
    delete(m_button);
}