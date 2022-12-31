#include <chrono>
#include <iostream>
#include "db.hpp"

int main(int argc, char* argv[]) {
    if(argc < 2) return 1;

    TaskDB tasks{ argv[1] };

    Task task {
        "Clean the dishes",
        DateType{ 
            std::chrono::year{ 2023 },
            std::chrono::month{ 1 },
            std::chrono::day{ 3 }
         }
    };

    tasks.add_task(task);
    
    return 0;
}