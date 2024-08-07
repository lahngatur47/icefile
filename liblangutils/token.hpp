#pragma once
#include <cstddef>
#include <string>
#include <cassert>
#include <map>
using namespace std;

#define TOKEN_LIST(T, K) \
    /* EOF INDICATOR*/ \
    T(EOS, "EOS", 0)\
     /* punctuator */ \
    T(L_parent, "(", 0)\
    T(R_parent, ")", 0)\
    T(L_bracket, "[",0 )\
    T(R_bracket, "]", 0)\
    T(L_brace, "{", 0)\
    T(R_brace, "}", 0)\
    T(semicolon, ";",0)\
    T(period, ".", 0)\
    T(conditional, "?", 3)\
    T(R_arrow, "->",0 )\
     /* ASSIGNMENT OP */ \
    T(assign, "=", 2)\
     /* another op */ \
    T(assign_bit_or , "|=", 2)\
    T(assign_bit_xor , "^=", 2)\
    T(assign_bit_and , "&=", 2)\
    T(assign_shl , "<<=", 2)\
    T(assign_sar , ">>=", 2)\
    T(assign_add , "=+", 2)\
    T(assign_sub , "-=", 2)\
    T(assign_mul , "*=", 2)\
    T(assign_div , "/=", 2)\
    T(assign_mod , "%=", 2)\
     /* binary op */ \
    T(comma , ",", 1)\
    T(or_ , "||", 4)\
    T(and_ , "&&", 5)\
    T(bit_or , "|", 8)\
    T(bit_xor , "^", 9)\
    T(bit_and , "&", 10)\
    T(shl , "<<", 11)\
    T(sar , ">>", 11)\
    T(add , "+", 12)\
    T(sub , "-", 12)\
    T(mul , "*", 13)\
    T(div , "/", 13)\
    T(mod , "%", 13)\
     /* operator compare */ \
    T(equal , "==", 6)\
    T(not_equal , "!=", 6)\
    T(less_than , "<", 7)\
    T(greater_than , ">", 7)\
    T(less_than_or_equal , "<=", 7)\
    T(greater_than_or_equal , ">=", 7)\
     /* unary operator */ \
    T(not_ , "!", 0)\
    T(bit_not , "~", 0)\
    T(inc , "++", 0)\
    T(dec ,'--', 0)\
    T(delete_ , "delete", 0)\
     /* keyword */ \
    K(as_ , "as", 0)\
    K(break_ , "break", 0)\
    K(catch_ , 'catch', 0)\
    K(const_ , "const", 0)\
    K(continue_ , "continue", 0)\
    K(do_ , "do", 0)\
    K(else_ , "else", 0)\
    K(enum_ , "enum", 0)\
    K(extern_ , 'extern', 0)\
    K(for_ , "for", 0)\
    K(func_ , "func", 0)\
    K(if_ , 'if',0 )\
    K(interface_ , "interface",0 )\
    K(immutable_ , "immutable", 0)\
    K(import_ , "import", 0)\
    K(override_, "override",0)\
    K(new_,"new", 0) \
    K(package, "package", 0)\
    K(public_ , "public", 0)\
    K(pragma_ , "pragma", 0)\
    K(private_ , "private",0 )\
    K(return_ , "return", 0)\
    K(struct_ , "struct",0 )\
    K(throw_ , "throw",0 )\
    K(try_ , "try", 0)\
    K(type_ , "type", 0)\
    K(using , "using", 0)\
    K(virtual_ , "virtual", 0)\
    K(while_ , "while", 0)\
     /* type keyword */ \
    K(short_ , "short",0 )\
    K(uint_ , "uint", 0)\
    K(uint8 , "uint8",0)\
    K(uint16 , "uint16",0 )\
    K(uint32 , "uint32",0 )\
    K(uint64 , "uint64",0 )\
    K(int8_ , "int8", 0)\
    K(int16_ , "int16", 0)\
    K(int32_ , "int32",0 )\
    K(int_ , "int",0 )\
    K(float_ , "float", 0)\
    K(char_ , "char", 0)\
    K(string_ , "string",0 )\
     /* literal */ \
    K(true_ , "true",0 )\
    K(false_ , "false", 0)\
    K(number_ , nullptr, 0)\
    T(string_literal , nullptr, 0)\
    T(unicode_literal , nullptr, 0)\
    T(hex_string_literal , nullptr,0 )\
    T(comment_literal , nullptr, 0)\
    T(identifier_literal , nullptr, 0)\
     /* no keyword */ \
    T(identifier_literal , nullptr,0 )\
     /* keyword */ \
    K( auto_, 'auto', 0)\
    K(case_ , "case", 0)\
    K(copy_of , "copy_of",0 )\
    K(default_ , "default", 0)\
    K(define , "define", 0)\
    K(final_ , "final",0 )\
    K(implement_ , "implement", 0)\
    K(inline_ , "inline", 0)\
    K(mutable_ , "mutable",0 )\
    K(nullptr_ , "nullptr",0 )\
    K(reference_ , "reference", 0)\
    K(size_of , "size_of", 0)\
    K(static_ , "static", 0)\
    K(switch_ , "switch",0 )\
    K(typedef_ , "typedef",0 )\
    K(type_of , "type_of", 0)\
     /* user def0ine */ \
    K(class_ , nullptr,0 )\
    T(error_ , nullptr, 0)\
    K(whitespace , nullptr,0 )


namespace ice::langutils
{

    enum class token: unsigned int{
    #define T(name, str, precedence) name,
        TOKEN_LIST(T, T)
        token_size
        #undef T
    };

    constexpr size_t count(){
        return static_cast<size_t>(token::token_size);
    }
    bool is_keyword(token tok){
        return (token::as_ <= tok && tok <= token::false_) || (token::auto_ <= tok && tok <= token::class_);
    }
    bool is_data_type(token tok){
        return token::short_ <= tok && tok <= token::string_;
    }
    bool is_assignment_op(token tok){
        return token::assign <= tok && tok <= token::assign_mod;
    }
    bool is_binary_op(token tok){
        return token::comma <= tok && tok <= token::mod;
    }

    bool is_commutative_op(token tok){
        return tok == token::bit_or || tok == token::bit_xor || tok == token::bit_and || tok == token:: add || tok == token::mul || tok == token::equal || tok == token::not_equal;
    }
    bool is_compare_op(token tok){
        return token::equal <= tok && tok <= token::greater_than_or_equal;
    }
    bool is_bit_op(token tok){
        return (token::bit_or <= tok && tok <= token::bit_and) || tok == token::bit_not;
    }
    bool is_bool_op(token tok){
        return (token::or_ <= tok && tok <= token::and_) || tok == token::not_;
    }
    bool is_unary_op(token tok){
        return (token::not_ <= tok && tok <= token::delete_) || tok == token::sub;
    }
    bool is_count_op(token tok){
        return tok == token::inc || tok == token::dec;
    }
    bool is_shift_op(token tok){
        return tok == token::sar || tok == token::shl;
    }
    bool is_visibility_var_op(token tok){
        return tok == token::private_ || tok == token::public_;
    }

    const char* to_string(token tok){
        #define T(name, str, prec) case token::name : return str;
        switch (tok)
        {
            TOKEN_LIST(T, T)
        default:
            return "";
        }
    }

    constexpr int precedence(token tok){
        uint8_t constexpr prec[count()] = {
            #define T(name, str, pre) pre,
            TOKEN_LIST(T,T)
            #undef T
        };
        return prec[static_cast<size_t>(tok)];
    }
    const char* name(token tok){
        #define T(name, str, pre) #name,
        static const char* const names[count()] = { TOKEN_LIST(T,T)};
        #undef T
        assert(static_cast<size_t>(tok) < count());
        return names[static_cast<size_t>(tok)];
    }
    string friendly_name(token tok){
        const char *name__ = to_string(tok);
        if(name__){
            return string(name__);
        }
        name__ = name(tok);
        assert(name__ != nullptr);
        return string(name__);
    }
    static token keyword_by_name(string &name){
        #define K(name, str, pre) {str, token::name},
        #define T(name, str, pre)
        map<string, token> key({TOKEN_LIST(T,K)});
        #undef K
        #undef T
        auto it = key.find(name);
        return it == key.end() ? token::identifier_literal : it->second;
    }
} // namespace ice::t0oken
