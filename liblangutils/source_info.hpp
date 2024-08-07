#pragma once
#include<vector>
#include <memory>
#include <string>

using namespace std;

namespace ice::langutils{

struct source_info{
    void parse(string &source){
        __info = make_shared<string>(source);
    }
    bool contain(string reference){
        bool status = false;
        string temp;
        string::iterator it;
        for(it = __info->begin() ; it != __info->end(); it++){
            if(*it == ' ')
                if(temp == reference)
                    status = true;
            else
            temp.push_back(*it);
        }
        return status;
    }
    vector<string> get_string(){
        vector<string> temp;
        string str;
        string::iterator it;
        for(it = __info->begin() ; it != __info->end(); it++){
            if(*it != ' ')
                str.push_back(*it);
            else
                temp.push_back(str);
                str.clear();
        }
        return temp;
    }
    shared_ptr<string> __info;
};

}

struct line_colomn{
    size_t rows;
    size_t colomn;
    line_colomn() = default;
    explicit line_colomn(size_t r, size_t c): rows(r), colomn(c) {}
};