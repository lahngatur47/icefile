#include <memory>
#include <liblangutils/token.hpp>
#include <liblangutils/source_info.hpp>
#include <liblangutils/char_base.hpp>
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
    bool advace(){
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
            advace();
            return true;
        }
        else if(m_char == '\r'){
            advace();
            return true;
        }
        return false;
    }
    void add_char(char c){
        token_buf.__literal.push_back(c);
    }
    void add_advance(char c){
        add_char(c);
        advace();
    }
    inline token select_token(token _tok){ advace(); return _tok;}
    inline token select_token(char c, token then, token else_){
        advace();
        if(m_char == c){
            return select_token(then);
        }else{
            return else_;
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