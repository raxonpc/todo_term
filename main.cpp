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

    auto list = tasks.get_tasks();

    for(auto& it : list) {
        std::cout   << it.get_title() << ' ' << DateToString(it.get_deadline().value()) 
                    << ' ' << std::boolalpha << it.get_done() << '\n';
    }
    
    return 0;
}