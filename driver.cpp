#include <backend.cpp>

int main() {
    std::cout<<"\x1b[32m";

    // loadDataFromFiles();
    ControlPanel();
    // saveData();
    
    std::cout<<"\x1b[0m";
    return 0;
}
