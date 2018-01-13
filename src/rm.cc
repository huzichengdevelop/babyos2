/*
 * guzhoudiaoke@126.com
 * 2018-01-13
 */

#include "userlib.h"

int main(int argc, char** argv)
{
    if (argc < 2) {
        userlib_t::printf("Usage: rm files...\n");
        userlib_t::exit(0);
    }

    for (int i = 1; i < argc; i++) {
        if (userlib_t::unlink(argv[i]) < 0) {
            userlib_t::printf("rm %s failed\n", argv[i]);
            break;
        }
    }

    userlib_t::exit(0);
}


