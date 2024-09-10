#include <memory>
#include <liblangutils/char_base.hpp>
#include <liblangutils/token.hpp>
namespace iu = ice::utils;
#ifdef UNICODE_SUPPORT
	#if ICE_VERSION == 1008L
		static bool unicode_support = true;
	#else
		static bool unicode_support = false;
	#endif
#endif
enum class error{
	no_error,
	token_error,
	hexstring_error,
	hexdigit_error,
	comment_error,
	escape_error,
	quote_string_error,
	number_separator_error,
	define_user_operator_error
};
struct separate_desc_code{
	std::string code;
	iu::token is_token;
	bool next_EOL;
	error why = error::no_error;
};
namespace ice::utils{
class g_branch{
public:
	explicit g_branch(std::queue &other): look(other){}
	separate_desc_code restore_before(){
		return static_cast<separate_desc_code>(before);
	}
private:
	std::queue<separate_desc_code> look;
	void* current = nullptr;
	void* next = nullptr;
	void* before = nullptr;
	std::size_t MAX_limit = 10;
	static bool auto_restore = true;
};
class scanner_main_control{
protected:
	//c = comment controll
	bool c_s_line = false;
	bool c_m_line = false;
};
class scanner{
public:
	std::tuple<string, iu::token, error> bind(){
		return make_tuple<string, iu::token, error>(__buffer_source, __token_now, __why);
	}
	token current(){
		return next_and_collect();
	}
private:
	iu::token select_token(token __tok){
		advance();
		return __tok;
	}
	iu::token select_token(char next, token then, token else_){
		advance();
		if(char_buffer == next){
			return select_token(then);
		}else{
			return else_;
		}
	}
	void scan_token(){
		iu::token now_token;
		while(advance()){
			switch(char_buffer){
				case '"':
				case '\'':{
					now_token = scan_string(false);
				}
				case '+':{
					advance();
					if(char_buffer == '='){
						now_token = select_token(iu::token::assign_add);
					}else if(char_buffer =='+'){
						now_token = select_token(iu::token::inc);
					}else{
						now_token = select_token(iu::token::add);
					}
				}
				case '-':{
					advance();
					if(char_buffer == '='){
						now_token = select_token(iu::token::assign_sub);
					}else if(char_buffer == '-'){
						now_token = select_token(iu::token::dec);
					}else if(char_buffer == '>'){
						now_token = select_token(iu::token::R_arrow);
					}else{
						now_token = select_token(iu::token::sub);
					}
				}
				case '*':{
					now_token = select_token('=', iu::token::assign_mul, iu::token::mul);
				}
				case '/':{
					now_token = scan_slash();
				}
				case '%':{
					now_token = select_token('=', iu::token::assign_mod, iu::token::mod);
				}
				case '<':{
					advance();
					if(char_buffer == '<'){
						now_token = select_token('=', iu::token::assign_shl, iu::token::shl);
					}else if(char_buffer == '='){
						now_token = select_token(iu::token::less_than_or_equal);
					}else{
						now_token = select_token(iu::token::less_than);
					}
				}
				case '>':{
					advance();
					if(char_buffer == '>'){
                        now_token = select_token('=', iu::token::assign_sar, iu::token::sar);
					}else if(char_buffer == '='){
                        now_token = select_token(iu::token::greater_than_or_equal);
					}else{
                        now_token = select_token(iu::token::greater_than);
					}

				}
				case '&':{
                    advance();
                    if(char_buffer == '='){
                        now_token = select_token(iu::token::assign_bit_and);
                    }else if(char_buffer == '&'){
                        now_token = select_token(iu::token::and_);
                    }else{
                        now_token = select_token(bit_and);
                    }
				}
				case '|':{
                    advance();
                    if(char_buffer == '='){
                        now_token = select_token(iu::token::assign_bit_or);
                    }else if(char_buffer = '|'){
                        now_token = select_token(iu::token::or_);
                    }else{
                        now_token = select_token(iu::token::bit_or);
                    }
				}
				case '!':{
                    now_token = select_token('=', iu::token::not_equal, iu::token::not_);
				}
				case '~':{
                    now_token = select_token(iu::token::bit_not);
				}
				case '^':{
                    now_token = select_token(iu::token::bit_xor);
				}
				case '(':{
                    now_token = select_token(iu::token::L_parent);
				}
				case ')':{
				    now_token = select_token(iu::token::R_parent);
				}
				case '[':{
                    now_token = select_token(iu::token::L_bracket);
				}
				case ']':{
                    now_token = select_token(iu::token::R_bracket);
				}
				case '{':{
                    now_token = select_token(iu::token::L_brace);
				}
				case '}':{
                    now_token = select_token(iu::token::R_brace);
				}
				case '.':{
                    now_token = select_token(iu::token::period);
				}
				case ';':{
                    now_token = select_token(iu::token::semicolon);
				}
				case ':':{
                    now_token = select_token(iu::token::colon);
				}
				case '?':{
                    now_token = select_token(iu::token::conditional);
				}
				default:{
					if(iu::is_identifier_star(char_buffer)){
						iu::keyword_by_name()
					}
				}
			}
		}
	}
	void scan_string(){}

	token next_and_collect(){
		scan_keyword_or_token();
		return __token_now;
	}
	bool advance(){
		char_buffer = __source.advance();
		return !__source.is_past_EOI();
	}
	void add_literal(char &_ref){
		__buffer_source.push_back(ref);
	}
	size_t source_pos(){
		return __source.get_pos();
	}
	bool skip_whitespace(){
		size_t now_pos = source_pos();
		while(iu::is_whitespace(char_buffer))	advance();
		return source_pos() != now_pos;
	}
private:
	iu::char_base __source;
	std::string __buffer_source;
	iu::token __token_now;
	error __why = error::no_error;
	char char_buffer;
};

}
