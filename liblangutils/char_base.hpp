#include <memory>
#include <string>
#include <cstddef>
#include <cassert>
using namespace std;
namespace ice::langutils{
class stream{
public:
    explicit stream(string &source): full_source(source){}
    bool is_past_EOI(size_t __forward = 0){
        return (pos + __forward) >= full_source.size();
    }
    char get(size_t __amount = 0){
        return full_source[pos + __amount];
    }
    void reset(){
        pos = 0;
    }
    char rollback(size_t __amout){
        assert(pos >= full_source.size());
        pos = __amout;
        return get();
    }
    size_t get_pos(){
        return this->pos;
    }
    char set_position(size_t __pos){
        assert(__pos <= full_source.size());
        pos = __pos;
        return get();
    }
    char advance_get(size_t __amount = 1){
        if(is_past_EOI()){
            return 0;
        }
        pos += __amount;
        if(is_past_EOI()){
            return 0;
        }
        return full_source[pos];
    }
private:
    string full_source;
    size_t pos;
};
}