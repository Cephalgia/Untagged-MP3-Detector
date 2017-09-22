

#include <QApplication>
#include "MainWindow.h"


std::string tag_string(char* buf);

int main(int argc, char **argv)
{
    QApplication app (argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}


std::string tag_string(char* buf)
{
    std::string str = "";
    for(int i = 0; i < 128; i++)
    {
        if(buf[i] != 0)
            str += buf[i];
        else
            str += " ";
    }
    return str;
}