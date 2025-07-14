#include <backend.cpp>

int main()
{
    // blue color
    std::cout << "\x1b[32m";
    
    // for ascii characters
    system("chcp 65001");
    
    // seed for random
    srand(static_cast<unsigned int>(std::time(0)));
    
    loadDataFromFiles();
    ControlPanel();
    // saveData();
    cleanMemory();
    
    // default color
    std::cout << "\x1b[0m";
    return 0;
}
