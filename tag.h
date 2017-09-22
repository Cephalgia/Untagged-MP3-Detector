//
// Created by cephalgia on 07.09.17.
//

#pragma once

#ifndef UNTAGGEDMP3DETECTOR_TAG_H
#define UNTAGGEDMP3DETECTOR_TAG_H

#endif //UNTAGGEDMP3DETECTOR_TAG_H

#include <boost/filesystem.hpp>
#include <QtCore/QStringList>

struct tag
{
    std::string title;
    std::string artist;
    std::string album;
    std::string year;
    std::string comment;
    int track_num = 0;
    int genre = 255;
    int completeness = 0;
    std::string path;
};

#define TEXT_FIELD_LENGTH 30
#define YEAR_FIELD_LENGTH 4
#define MAX_COMPLETENESS 6

const QStringList genres = QStringList() << "Select Genre" << "Blues" << "Classic Rock" << "Country" << "Dance" << "Disco" << "Funk" << "Grunge"
                                   << "Hip-Hop" << "Jazz" << "Metal" << "New Age" << "Oldies" << "Other" << "Pop" << "Rhythm and Blues" << "Rap"
                                   << "Reggae" << "Rock" << "Techno" << "Industrial" << "Alternative" << "Ska" << "Death Metal" << "Pranks";

std::vector<std::string> get_mp3s(boost::filesystem::path p);
std::vector<tag> find_empty_tag_files(boost::filesystem::path p);
std::vector<tag> filter_empty_tags(std::vector<std::string> files);
bool is_tag(char* raw_tag);
tag read_tag(std::string path);
tag get_tag_struct(char* raw_tag, std::string path);
std::string get_text_tag(char* raw_tag, int pos, int length);
void write_tag(tag new_tag);
void write_to_buf(char* buf, int pos, std::string text, int field_length);
char* form_tag(tag new_tag);