#include "../impl/nordshrift/complete/class_bindings.h"
#include <cassert>
int main(){const auto* b=nordshrift::complete::findClass("runtime","Stream");assert(b);assert(nordshrift::complete::classCount()>0);assert(nordshrift::complete::moduleCount()>0);return 0;}
