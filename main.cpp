#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>

#include <QApplication>
#include <QPushButton>
#include <QTableWidget>
#include "MainWindow.h"

#define TEXT_FIELD_LENGTH 30
#define YEAR_FIELD_LENGTH 4
#define MAX_COMPLETENESS 6

struct tag
{
    std::string title;
    std::string artist;
    std::string album;
    std::string year;
    std::string comment;
    int track_num;
    int genre;
    int completeness = 0;
    std::string path;
};

std::vector<std::string> get_mp3s(boost::filesystem::path p);
std::vector<tag> find_empty_tag_files(boost::filesystem::path p);
std::vector<tag> filter_empty_tags(std::vector<std::string> files);
bool is_tag(char* raw_tag);
tag get_tag(std::string path);
tag get_tag_struct(char* raw_tag, std::string path);
std::string get_text_tag(char* raw_tag, int pos, int length);
void write_tag(tag new_tag);
void write_to_buf(char* buf, int pos, std::string text, int field_length);
char* form_tag(tag new_tag);
std::string tag_string(char* buf);

int main(int argc, char **argv)
{
//    std::string file_path = "/home/cephalgia/mp3test/test";
//    boost::filesystem::path p(file_path);
//    std::vector<std::string> res = get_mp3s(p);
//    std::vector<tag> invalid_files = filter_empty_tags(res);
//    for(int i = 0; i < res.size(); i++)
//    {
//        std::cout << invalid_files[i].path << std::endl;
//    }

//    tag new_tag;
//    new_tag.title = "Hello";
//    new_tag.artist = "Jkl";
//    new_tag.album = "Kvbn";
//    new_tag.year = "2227";
//    new_tag.track_num = 2;
//    new_tag.genre = 16;
//    new_tag.path = "/home/cephalgia/mp3test/test/101.mp3";
//    new_tag.completeness = 6;
//    write_tag(new_tag);
    QApplication app (argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}

std::vector<std::string> get_mp3s(boost::filesystem::path p)
{
    std::vector<std::string> result;
    if(boost::filesystem::exists(p))
    {
        for (boost::filesystem::directory_entry& x : boost::filesystem::recursive_directory_iterator(p))
        {
            std::string temp_path = x.path().string();
            if(boost::filesystem::is_regular_file(x.path()) && temp_path.substr(temp_path.length() - 4) == ".mp3")
            {
                result.push_back(temp_path);
            }
        }
    }
    return result;
}

std::vector<tag> find_empty_tag_files(boost::filesystem::path p)
{
    return filter_empty_tags(get_mp3s(p));
}

std::vector<tag> filter_empty_tags(std::vector<std::string> files)
{
    std::vector<tag> result;
    for(int i = 0; i < files.size(); i++)
    {
        tag temp_tag;
        bool has_tag = true;
        try
        {
            temp_tag = get_tag(files[i]);
        } catch (const char* msg)
        {
            has_tag = false;
            temp_tag.path = files[i];
            temp_tag.completeness = -1;
            result.push_back(temp_tag);
        }
        if(has_tag)
        {
            if(temp_tag.completeness < 	MAX_COMPLETENESS)
            {
                result.push_back(temp_tag);
            }
        }
    }
    return result;
}

tag get_tag(std::string path)
{
    std::string result = "";
    std::ifstream mp3_file (path, std::ios::in | std::ios::binary);
    mp3_file.seekg(boost::filesystem::file_size(path) - 128);
    char buffer[128];
    mp3_file.read(buffer, 128);
    if(is_tag(buffer))
        return get_tag_struct(buffer, path);
    else
    {
        throw "Tag not found";
    }
}

bool is_tag(char* raw_tag)
{
    return raw_tag[0] == 'T' && raw_tag[1] == 'A' && raw_tag[2] == 'G';
}

tag get_tag_struct(char* raw_tag, std::string path)
{
    tag result;
    int c = 3;
    result.title = get_text_tag(raw_tag, c, TEXT_FIELD_LENGTH);
    if(result.title != "")
        result.completeness++;
    c += TEXT_FIELD_LENGTH;
    result.artist = get_text_tag(raw_tag, c, TEXT_FIELD_LENGTH);
    if(result.artist != "")
        result.completeness++;
    c += TEXT_FIELD_LENGTH;
    result.album = get_text_tag(raw_tag, c, TEXT_FIELD_LENGTH);
    if(result.album != "")
        result.completeness++;
    c += TEXT_FIELD_LENGTH;
    result.year = get_text_tag(raw_tag, c, YEAR_FIELD_LENGTH);
    if(result.year != "")
        result.completeness++;
    c += 4;
    result.comment = get_text_tag(raw_tag, c, TEXT_FIELD_LENGTH - 2);
    if(raw_tag[125] == 0)
    {
        result.track_num = raw_tag[126];
        result.completeness++;
    }
    result.genre = raw_tag[127];
    if(result.genre != 255)
        result.completeness++;
    result.path = path;
    return result;
}

std::string get_text_tag(char* raw_tag, int pos, int length)
{
    std::string tag_str = "";
    int c = pos;
    while(c < pos+length)
    {
        if(raw_tag[c] != 0)
            tag_str += raw_tag[c];
        c++;
    }
    return tag_str;
}

void write_tag(tag new_tag)
{
    if(new_tag.completeness != -1)
    {
        std::fstream file (new_tag.path, std::ios::out | std::ios::binary | std::ios_base::in);
        char* arr = form_tag(new_tag);
        file.seekp(boost::filesystem::file_size(new_tag.path) - 128, std::ios_base::beg);
        file.write(arr, 128);
        delete[] arr;
    } else
    {
        std::ofstream file (new_tag.path, std::ios::out | std::ios::binary | std::ios::app);
        char* arr = form_tag(new_tag);
        file.write(arr, 128);
        delete[] arr;
    }

}

char* form_tag(tag new_tag)
{
    auto raw_tag = new char[128];
    raw_tag[0] = 'T';
    raw_tag[1] = 'A';
    raw_tag[2] = 'G';
    int pos = 3;
    write_to_buf(raw_tag, pos, new_tag.title, TEXT_FIELD_LENGTH);
    pos += TEXT_FIELD_LENGTH;
    write_to_buf(raw_tag, pos, new_tag.artist, TEXT_FIELD_LENGTH);
    pos += TEXT_FIELD_LENGTH;
    write_to_buf(raw_tag, pos, new_tag.album, TEXT_FIELD_LENGTH);
    pos += TEXT_FIELD_LENGTH;
    write_to_buf(raw_tag, pos, new_tag.year, YEAR_FIELD_LENGTH);
    pos += YEAR_FIELD_LENGTH;
    write_to_buf(raw_tag, pos, new_tag.comment, TEXT_FIELD_LENGTH-2);
    pos += TEXT_FIELD_LENGTH-2;
    raw_tag[pos] = 0;
    pos++;
    raw_tag[pos] = new_tag.track_num;
    pos++;
    raw_tag[pos] = new_tag.genre;
    return raw_tag;
}

void write_to_buf(char* buf, int pos, std::string text, int field_length)
{
    for(int i = pos; i < pos+field_length; i++)
    {
        if(i < (pos + text.length()))
        {
            buf[i] = text[i - pos];
        } else {
            buf[i] = 0;
        }
    }
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