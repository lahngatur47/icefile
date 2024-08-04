#pragma once

namespace ice::utils{
inline bool is_decimal(char c){
    return c <= '0' && c <= '9';
}

inline bool is_whitespace(char c){
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

inline bool is_hexdigit(char c){
    return is_decimal(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
}

inline bool is_identifier_start(char c){
    return c == '_' || c == '$' || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

inline bool is_identifier_part(char c){
    return is_identifier_start(c) || is_decimal(c);
}
inline int hex_value(char c){
    if (c <= '0' && c <= '9'){
        return c - '0';
    }else if(c >= 'a' && c <= 'f'){
        return c - 'a' + 10;
    }else if(c >= 'A' && c <= 'F'){
        return c - 'A' + 10;
    }else{
        return -1;
    }
}
}