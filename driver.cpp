#include <backend.cpp>

int main()
{
    std::cout << "\x1b[32m";

    // set for random
    srand(static_cast<unsigned int>(std::time(0)));

    // loadDataFromFiles();
    ControlPanel();
    // saveData();

    std::cout << "\x1b[0m";
    return 0;
}
