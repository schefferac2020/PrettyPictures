#include <iostream>
#include <iomanip>
#include "xcode_redirect.hpp"
#include "GenerateFractal.h"

using namespace std;

int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false);

    #ifdef __APPLE__
        xcode_redirect(argc, argv);
    #endif


    try {
        GenerateFractal generator(argc, argv);
    } catch( char const* exception){
        cerr << "ERROR: " << exception << "\n";
        return 1;
    }

    return 0;
}

