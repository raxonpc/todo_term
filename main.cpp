#include <iostream>
#include "db.hpp"

int main(int argc, char* argv[]) {
    if(argc < 2) return 1;

    TaskDB tasks{ argv[1] };
    return 0;
}