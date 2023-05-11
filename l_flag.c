#define _XOPEN_SOURCE 700

#include "l_flag.h"
#include <ctype.h>
#include <libgen.h>
#include <pwd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "my_module.h"

char* get_user(uid_t uid)
{
    struct passwd* pws;
    pws = getpwuid(uid);
    return pws->pw_name;
}

int l_flag_print(const char* filename, const struct stat* stat_struct)
{
    // timespec format handler
    char time[20];  // set to epoch
    struct timespec time_in = stat_struct->st_mtim;
    struct tm* time_tm = localtime(&(time_in.tv_sec));

    strftime(time, 20, "%Y-%m-%d_%X", time_tm);

    // get user name of user id
    char* uname = get_user(stat_struct->st_uid);

    // declare variables for creating permissions string
    char ftype = ' ';
    char* rwx = "rwxrwxrwx";
    int mask_2 = 0x100;  // bitmask to loop through perms bits
    char out_perms[10];
    out_perms[9] = '\0';  // making last char of perms null byte
    int perms = 0x1FF;    // perms bitmask

    // get 7 least sig bits relating to permisions
    perms &= (stat_struct->st_mode);

    // check filetype bit
    switch (stat_struct->st_mode & S_IFMT)
    {
        case S_IFSOCK:
            ftype = '=';
            break;
        case S_IFLNK:
            ftype = '@';
            break;
        case S_IFREG:
            ftype = '-';
            break;
        case S_IFBLK:
            ftype = 'b';
            break;
        case S_IFDIR:
            ftype = 'd';
            break;
        case S_IFCHR:
            ftype = 'c';
            break;
        case S_IFIFO:
            ftype = '|';
            break;
        default:
            printf("Err: unexpected filetype\n");
            break;
    }

    // create string of perms
    for (int i = 0; i < 9; i++)
    {
        if (mask_2 & perms)
        {
            out_perms[i] = rwx[i];
        }
        else
        {
            out_perms[i] = '-';
        }
        mask_2 = mask_2 >> 1;
    }

    // printf("%x\n", perms);
    printf("%c%s %s %ld %s %s\n", ftype, out_perms, uname, stat_struct->st_size, time,
           filename);

    return 0;
}

// int print_file(char* src, const u_int8_t flags)
// {
//     return 0;
// }