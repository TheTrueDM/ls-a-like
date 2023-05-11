//
// Template main for project
//

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "l_flag.h"
#include "my_module.h"

// Setting globals
uint8_t LFLAG = 0x1;
uint8_t AFLAG = 0x2;
uint8_t DFLAG = 0x4;
uint8_t SFLAG_T = 0x8;
uint8_t SFLAG_Z = 0x10;
uint8_t SFLAG_N = 0x20;
uint8_t RFLAG = 0x40;

int main(int argc, char* argv[])
{
    int ret = 0;
    int opt = 0;

    char* fpath = NULL;

    uint8_t flag_arr = 0x0;

    while ((opt = getopt(argc, argv, "lads:r")) != -1)
    {
        switch (opt)
        {
            case 'l':
                flag_arr |= LFLAG;
                break;
            case 'a':
                flag_arr |= AFLAG;
                break;
            case 'd':
                // if flag_arr & DFLAG;
                flag_arr |= DFLAG;
                break;
            // sort
            case 's':
                if (strlen(optarg) > 1)
                {
                    printf("Err: Incorrect use of 's' flag\n");
                    break;
                }
                switch (optarg[0])
                {
                    case 't':
                        flag_arr |= SFLAG_T;
                        break;
                    case 'z':
                        flag_arr |= SFLAG_Z;
                        break;
                    default:
                    case 'n':
                        flag_arr |= SFLAG_N;
                        break;
                }
                break;
            case ':':
                flag_arr |= SFLAG_N;
                break;
            case 'r':
                flag_arr |= RFLAG;
                break;
            case '?':
                printf("unknown option: %c\n", optopt);  // optopt == unknown flags
                break;
        }
    }

    // assumes only handling one path -- FIX
    // handle extra args
    int itrs = 0;
    for (; optind < argc; optind++)
    {
        itrs++;
        // do something better here
        if (itrs > 1)
        {
            printf("Err: Too many file paths, using first provided\n");
        }
        fpath = argv[optind];
    }

    struct stat my_struct;
    int stat_status = stat(fpath, &my_struct);

    if (stat_status == 0)
    {
        ret = function(fpath, &my_struct, flag_arr);
    }
    else
    {
        ret = 1;
    }

    // printf("Err: Incorrect Args\n");
    // ret = 1;

    return ret;
}
