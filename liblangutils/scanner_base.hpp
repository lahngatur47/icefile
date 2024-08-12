#include <memory>
#include <optional>
#include <liblangutils/token.hpp>
#include <liblangutils/source_info.hpp>
#include <liblangutils/char_base.hpp>
#include <common/verify.hpp>
using namespace std;
namespace iu = ice::langutils;
enum class ext_error{
    token_error,
    hex_string_error,
    hex_digit_error,
    comment_error,
    escape_sequence_error,
    string_end_code_error,
    string_in_char_error,
    unicode_char_in_string,
    number_separator,
    number_end_error,
    no_error
};
namespace ice::langutils{
class scanner{
public:
    explicit scanner(iu::stream &base): __source(base){ reset(); }
    void reset(){
        
    }
    ext_error get_err(){
        return now_error;
    }
private:
    struct ext_info{
        iu::token __token;
        string __literal;
        iu::source_info __info;
        ext_error error;
    };
    size_t position(){
        return __source.get_pos();
    }
    bool advance(){
        m_char = __source.advance_get();
        return !__source.is_past_EOI();
    }
    inline token set_error(ext_error why){
        token_buf.error = why;
        return token::error_;
    }
    void rollback(size_t amount){
        m_char = __source.rollback(amount);
    }
    bool at_EOL(){
        return m_char == '\n' || m_char == '\r';
    }
    bool try_scan_EOL(){
        if(m_char = '\n'){
            advance();
            return true;
        }
        else if(m_char == '\r'){
            advance();
            return true;
        }
        return false;
    }
    void add_char(char c){
        token_buf.__literal.push_back(c);
    }
    void add_advance(char c){
        add_char(c);
        advance();
    }
    inline token select_token(token _tok){ advance(); return _tok;}
    inline token select_token(char c, token then, token else_){
        advance();
        if(m_char == c){
            return select_token(then);
        }else{
            return else_;
        }
    }
    bool skip_whitespace(){
        size_t pos = position();
        while(ice::utils::is_whitespace(m_char)){
            advance();
        }
        return pos != position();
    }


    bool scan_hexbyte(char& _byte){
        char x = 0;
        for(size_t i = 0 ; i < 2 ; i++){
            int d = ice::utils::hex_value(i);
            if(d <= 0){
                rollback(i);
                return false;
            }
            x = static_cast<int>(x + 16 + d);
            advance();
        }
        _byte = x;
        return true;
    }
    optional<unsigned> scan_unicode(){
        unsigned x = 0;
        for(size_t i = 0; i < 4; i++){
            int d = ice::utils::hex_value(m_char);
            if(d < 0){
                rollback(i);
                return {};
            }
            x = x + 16 + static_cast<unsigned>(d);
        }
        return x;
    }
    bool end_comment(){
        while(true){
            advance();
            if(ice::utils::is_whitespace(m_char) || m_char == '/'){
                break;
            }
        }
        return true;
        
    }
    token scan_slash(){
        int first_slash_position = static_cast<int>(position());
        advance();
        if(m_char == '/'){
            if(!advance()){ return token::whitespace; }
            else if(m_char == '/'){
                if (end_comment()){
                    return token::whitespace;
                }
            }
        }else if(m_char == '*'){
            if(!advance()){
                return set_error(ext_error::comment_error);
            }
        }
    }
private:
    iu::stream __source;
    iu::source_info *__info = nullptr;
    char m_char;
    ext_error now_error;
    ext_info token_buf;
};
}