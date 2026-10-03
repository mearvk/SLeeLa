#include "../include/sleela_regex.hpp"
#include <cassert>
int main(){
    // Anchored pattern: whole-string match and capture count.
    auto p=sleela::regex::Pattern::ere("^(hello|world)[0-9]+$");
    assert(p.valid());
    auto m=p.match("hello42");
    assert(m.matched&&m.whole.start==0&&m.whole.end==7&&m.captures.size()==1);
    assert(p.replace_first("hello42","X")=="X");

    // Unanchored pattern: a mid-string search finds the substring `world7`
    // (indices 3..9). The anchored pattern above cannot match a substring.
    auto u=sleela::regex::Pattern::ere("(hello|world)[0-9]+");
    auto s=u.search("xx world7 yy");
    assert(s.matched&&s.whole.start==3&&s.whole.end==9);

    auto parts=sleela::regex::Pattern::ere(",").split("a,b,c");
    assert(parts.size()==3&&parts[1]=="b");
    assert(sleela::regex::Pattern::escape("a+b.c")=="a\\+b\\.c");
    auto bad=sleela::regex::Pattern::ere("(");
    assert(!bad.valid());
}
