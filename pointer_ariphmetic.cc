#include <iostream>



int main()
{
    int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int* parr = arr;
    
    for (unsigned int i = 0; i < 4; ++i) {
        std::cout << *parr++ << std::endl;
    }

    return 0;
}
