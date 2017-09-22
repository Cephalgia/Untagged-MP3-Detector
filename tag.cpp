//
// Created by cephalgia on 07.09.17.
//

#include <iostream>
#include <vector>


#include "tag.h"

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
            temp_tag = read_tag(files[i]);
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

tag read_tag(std::string path)
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
    bool has_tag = true;
    try
    {
        read_tag(new_tag.path);
    } catch (const char* msg)
    {
        has_tag = false;
        std::ofstream file (new_tag.path, std::ios::out | std::ios::binary | std::ios::app);
        char* arr = form_tag(new_tag);
        file.write(arr, 128);
        delete[] arr;
        file.close();
    }

    if(has_tag)
    {
        std::fstream file (new_tag.path, std::ios::out | std::ios::binary | std::ios_base::in);
        char* arr = form_tag(new_tag);
        file.seekp(boost::filesystem::file_size(new_tag.path) - 128, std::ios_base::beg);
        file.write(arr, 128);
        delete[] arr;
        file.close();
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
