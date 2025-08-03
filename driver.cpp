#include <backend.cpp>

int main()
{
    // for cyan color text
    std::cout << "\x1b[36m";
    
    // for ascii characters
    system("chcp 65001");

    // seed for random
    srand(static_cast<unsigned int>(std::time(0)));
    
    loadDataFromFiles();
    ControlPanel();
    cleanMemory();
    
    // default color
    std::cout << "\x1b[0m";
    
    return 0;
}
