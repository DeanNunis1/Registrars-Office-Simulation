#include <iostream>
#include "registrar.h"
#include <cassert>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr << "Error: use " << argv[0] << " filename\n";
        return 0;
    }

    Registar registar (argv[1]);
    registar.simulate();

}
