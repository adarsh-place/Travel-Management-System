#include <backend.cpp>

int main()
{
    // for light blue color text
    std::cout << "\x1b[32m";
    
    // for ascii characters
    system("chcp 65001");

    // for emoji
    std::setlocale(LC_ALL, "");
    
    // seed for random
    srand(static_cast<unsigned int>(std::time(0)));
    
    loadDataFromFiles();
    ControlPanel();
    cleanMemory();
    
    // default color
    std::cout << "\x1b[0m";
    
    return 0;
}
