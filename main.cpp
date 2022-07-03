#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <boost/python.hpp>
#include <iostream>

int main() {
    Py_Initialize();

    boost::python::import("__main__");

    std::cout << "Hello, World!" << std::endl;

    Py_Finalize();
    return 0;
}
