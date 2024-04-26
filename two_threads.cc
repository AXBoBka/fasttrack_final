#include <iostream>
#include <thread>
#include <mutex>

std::mutex mutex;
unsigned int value = 0;

void write(void) {
    for (unsigned int i = 0; i < 100; ++i) {
        mutex.lock();
        value = i;
        std::cout << "Записано число - " << value << std::endl;
        mutex.unlock();
    }
}

void read(void) {
    for (unsigned int i = 0; i < 100; ++i) {
        mutex.lock();
        std::cout << "Прочитано число - " << value << std::endl;
        mutex.unlock();
    }
}

int main() {
    std::thread thread_write(write);
    std::thread thread_read(read);

    thread_write.join();
    thread_read.join();

    return 0;
}
