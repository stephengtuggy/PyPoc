#include <Python.h>
#include <boost/python.hpp>
#include <iostream>

int main() {
    Py_Initialize();

    std::cout << "Hello, World!" << std::endl;

    Py_Finalize();
    return 0;
}
