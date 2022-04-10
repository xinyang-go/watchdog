#include <cstdlib>
#include <iostream>
#include <thread>

void help(const char *app) {
    std::cout << "Usage: " << app << " [flag]" << std::endl;
    std::cout << std::endl;
    std::cout << "    flag:" << std::endl;
    std::cout << "        --zero-exit" << std::endl;
    std::cout << "        --nonzero-exit" << std::endl;
    std::cout << "        --segmentation-fault" << std::endl;
    exit(-1);
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (argv[i] == std::string("--zero-exit")) {
            std::thread t1([]() {
                std::cout << "example stdout in thread." << std::endl;
            });
            std::thread t2([]() {
                std::cerr << "example stderr in thread." << std::endl;
            });
            t1.join();
            t2.join();
            std::cout << "example stdout in main." << std::endl;
            std::cout << "example stderr in main." << std::endl;
            return 0;
        } else if (argv[i] == std::string("--nonzero-exit")) {
            std::thread t1([]() {
                std::cout << "example stdout in thread." << std::endl;
            });
            std::thread t2([]() {
                std::cout << "example stderr in thread." << std::endl;
            });
            t1.join();
            t2.join();
            std::cout << "example stdout in main." << std::endl;
            std::cout << "example stderr in main." << std::endl;
            return -1;
        } else if (argv[i] == std::string("--segmentation-fault")) {
            std::thread t1([]() {
                std::cout << "example stdout in thread." << std::endl;
            });
            std::thread t2([]() {
                std::cout << "example stderr in thread." << std::endl;
            });
            t1.join();
            t2.join();
            std::cout << "example stdout in main." << std::endl;
            std::cout << "example stderr in main." << std::endl;
            (*(volatile int *) nullptr) = 0;
        } else {
            help(argv[0]);
        }
    }
    help(argv[0]);
}