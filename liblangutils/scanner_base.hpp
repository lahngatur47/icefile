#include <memory>
#include <liblangutils/char_base.hpp>
#include <liblangutils/token.hpp>
#include <common/verify.hpp>
#include <cassert>
namespace __iu = ice::utils;
namespace iu = ice::langutils;
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
	octal_no_allowed,
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

class scanner{
public:
	iu::token current(){
		return next_and_collect();
	}
private:
	iu::token select_token(iu::token __tok){
		advance();
		return __tok;
	}
	iu::token select_token(char next, iu::token then, iu::token else_){
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
					break;

				case '+':{
					advance();
					if(char_buffer == '='){
						now_token = select_token(iu::token::assign_add);
					}else if(char_buffer =='+'){
						now_token = select_token(iu::token::inc);
					}else{
						now_token = select_token(iu::token::add);
					}
					break;

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
					break;

				}
				case '*':{
					now_token = select_token('=', iu::token::assign_mul, iu::token::mul);
					break;

				}
				case '/':{
					now_token = scan_slash();
					break;

				}
				case '%':{
					now_token = select_token('=', iu::token::assign_mod, iu::token::mod);
					break;

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
					break;

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
					break;


				}
				case '&':{
                    advance();
                    if(char_buffer == '='){
                        now_token = select_token(iu::token::assign_bit_and);
                    }else if(char_buffer == '&'){
                        now_token = select_token(iu::token::and_);
                    }else{
                        now_token = select_token(iu::token::bit_and);
                    }
					break;
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
					break; 
				}
				case '!':{
                    now_token = select_token('=', iu::token::not_equal, iu::token::not_);
					break;
				}
				case '~':{
                    now_token = select_token(iu::token::bit_not);
					break;
				}
				case '^':{
                    now_token = select_token(iu::token::bit_xor);
					break;
				}
				case '(':{
                    now_token = select_token(iu::token::L_parent);
					break;
				}
				case ')':{
				    now_token = select_token(iu::token::R_parent);
					break;
				}
				case '[':{
                    now_token = select_token(iu::token::L_bracket);
					break;
				}
				case ']':{
                    now_token = select_token(iu::token::R_bracket);
					break;
				}
				case '{':{
                    now_token = select_token(iu::token::L_brace);
					break;
				}
				case '}':{
                    now_token = select_token(iu::token::R_brace);
					break;
				}
				case '.':{
                    now_token = select_token(iu::token::period);
					break;
				}
				case ';':{
                    now_token = select_token(iu::token::semicolon);
					break;
				}
				case ':':{
                    now_token = select_token(iu::token::colon);
					break;
				}
				case '?':{
                    now_token = select_token(iu::token::conditional);
					break;
				}
				default:{
					if(__iu::is_identifier_start(char_buffer)){
						scan_keyword_or_identifier();
					}else if(__iu::is_decimal(char_buffer)){
						now_token = scan_number();
					}else if(skip_whitespace()){
						now_token = iu::token::whitespace;
					}else if(is_source_past_EOI()){
						now_token = iu::token::EOS;
					}
					break;
				}
			}
		}
	}
	void scan_keyword_or_identifier(){
		assert(__iu::is_identifier_start(char_buffer) && "no matching identifier start");
		while(__iu::is_identifier_part(char_buffer) || char_buffer == '.'){
			add_literal_and_advance();
		}
	}
	iu::token  scan_string(bool tar){
		std::size_t start_pos = source_pos();
		char const quote = char_buffer;
		while(char_buffer != quote && is_source_past_EOI()){
			add_literal_and_advance();
		}
		advance();
		return iu::token::string_literal;
	}
	void scan_decimal_digit(){
		if(!__iu::is_decimal(char_buffer)){
			return;
		}
		do{
			add_literal_and_advance();
		}while(!__source.is_past_EOI() && __iu::is_decimal(char_buffer) || char_buffer == '_');
	}
	iu::token scan_number(char char_seen){
		enum{DECIMAL, HEX ,BINARY} kind = DECIMAL;
		if(char_seen == '.'){
			add_literal('.');
			if(char_buffer == '_'){
				return set_error(error::token_error);
			}
			scan_decimal_digit();
		}else{
			assert(char_seen == 0);
			if(char_buffer == '0'){
				add_literal_and_advance();
				if(char_buffer == 'x'){
					kind = HEX;
					add_literal_and_advance();
					if(!__iu::is_hexdigit(char_buffer)){
						return set_error(error::hexdigit_error);
					}
					while(__iu::is_hexdigit(char_buffer)){
						add_literal_and_advance();
					}
				}else if(__iu::is_decimal(char_buffer)){
					return set_error(error::octal_no_allowed);
				}
			}
		}
		if(kind == DECIMAL){
			scan_decimal_digit();
		}
		if(__iu::is_decimal(char_buffer) || __iu::is_identifier_start(char_buffer)){
			return set_error(error::token_error);
		}
		return iu::token::number_;
	}
	iu::token scan_slash(){
		int first_slash_pos = static_cast<int>(source_pos());
		advance();
		if (char_buffer == '/')
		{
			if(!advance()){
				return iu::token::whitespace;
			}
			return skip_comment(1);
		}
		else if(char_buffer == '*'){
			return skip_comment(2);
		}else if(char_buffer == '='){
			return select_token(iu::token::assign_div);
		}else{
			return iu::token::div;	
		}
		
	}
	iu::token skip_comment(short status__){

	}
	iu::token next_and_collect(){
		scan_keyword_or_token();
		return __token_now;
	}
	bool advance(){
		char_buffer = __source.advance_get();
		return !__source.is_past_EOI();
	}
	void add_literal(char &__ref){
		__buffer_source.push_back(__ref);
	}
	void add_literal_and_advance(){
		add_literal(char_buffer);
		advance();
	}
	bool is_source_past_EOI(){
		return __source.is_past_EOI();
	}
	size_t source_pos(){
		return __source.get_pos();
	}
	bool skip_whitespace(){
		size_t now_pos = source_pos();
		while(ice::utils::is_whitespace(char_buffer))	advance();
		return source_pos() != now_pos;
	}
	iu::token scan_keyword_or_token(){

	}
	
private:
	iu::stream __source;
	std::string __buffer_source;
	iu::token __token_now;
	error __why = error::no_error;
	char char_buffer;
};

}
