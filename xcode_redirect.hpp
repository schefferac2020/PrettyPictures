// Identifier: A8A3A33EF075ACEF9B08F5B9845569ECCB423725
//  xcode_redirect.hpp
//
//  Created by Marcus Darden on 1/13/19.
//  Copyright © 2019 Marcus Darden. All rights reserved.
//
// Usage: add the following line to main().
//    xcode_redirect(argc, argv);

#ifndef xcode_redirect_hpp
#define xcode_redirect_hpp

#include <iostream>
#include <cstdio>

// REQUIRES: argc & argv from main(), and optind from getopt.h
// MODIFIES: optind is incremented until it equals argc
// EFFECTS:
//     Looks for extra arguments after getopt() finishes reading options.
//     It replaces simple shell functionality by using freopen() to change
//     stdin (from < infilename) and stdout (from > outfilename).
inline void xcode_redirect(int argc, char **argv, int optind = 1) {
    while (optind < argc) {
        switch (*argv[optind]) {
            case '<':  // redirect input
                if (!freopen(argv[++optind], "r", stdin)) {
                    std::cerr << "Unable to open input file: " << argv[optind] << std::endl;
                    exit(1);
                }
                break;
            case '>':  // redirect output
                if (!freopen(argv[++optind], "w", stdout)) {
                    std::cerr << "Unable to open output file: " << argv[optind] << std::endl;
                    exit(1);
                }
                break;
        }  // switch
        ++optind;
    }  // while
}  // xcode_redirect()

#endif /* xcode_redirect_hpp */
