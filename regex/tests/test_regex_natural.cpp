#include "../include/sleela_regex_natural.hpp"
#include <cassert>
int main(){using namespace sleela::regex::natural;Diagnostic d{};assert(validate("begin digit+ end",&d)==Status::ok);assert(canonical_symbol("letter")=="[[:alpha:]]");assert(validate("mystery",&d)==Status::unknown_word);assert(validate("(digit+",&d)==Status::unbalanced);}
