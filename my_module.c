//
// Template for a module in the project
//
// includes
#define _XOPEN_SOURCE 700

#include "my_module.h"
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "l_flag.h"

/*
 * count number of files
 */
int dir_countfile(const char* fpath)
{
    DIR* dp;
    struct dirent* entry;
    int num_files = 0;

    if ((dp = opendir(fpath)) == NULL)
    {
        printf("Err: Cannot open directory - %s\n", fpath);
    }
    else
    {
        while ((entry = readdir(dp)) != NULL)
        {
            num_files++;
        }
    }
    closedir(dp);
    return num_files;
}

/*
 * fill array of filenames
 */
int dir_filenames(const char* fpath, char** arr)
{
    DIR* dp;
    struct dirent* entry;
    int ret_val = 0;

    if ((dp = opendir(fpath)) == NULL)
    {
        printf("Err: Cannot open directory - %s\n", fpath);
        ret_val = 1;
    }
    else
    {
        int loc = 0;
        while ((entry = readdir(dp)) != NULL)
        {
            if (entry->d_name == NULL)
            {
                printf("Warning: Filename NULL\n");
                continue;
            }
            arr[loc] = strdup(entry->d_name);
            loc++;
        }
    }
    closedir(dp);
    return ret_val;
}

/*
 * type handler and looping for: tolower()
 */
void send_tolower(char* src)
{
    // loop through src and convert every char to lower.
    int counter = strlen(src);
    for (int i = 0; i < counter; i++)
    {
        src[i] = tolower(src[i]);
    }
}

/*
 * handle casting, capitals, and strcmp
 */
int sort_wrapper(const void* a, const void* b)
{
    int ret_val = 0;
    char* cpy_a = strdup(*(const char**)a);
    char* cpy_b = strdup(*(const char**)b);
    send_tolower(cpy_a);
    send_tolower(cpy_b);
    ret_val = strcmp(cpy_a, cpy_b);

    // free memory
    free(cpy_a);
    free(cpy_b);
    cpy_a = NULL;
    cpy_b = NULL;
    return ret_val;
}

// main listing function
int function(char* fpath, const struct stat* stat_struct, const u_int8_t flags)
{
    int num_files = 0;
    int ret_val = 0;
    char** file_names = NULL;

    if (S_ISREG(stat_struct->st_mode))
    {
        char* fname = basename(fpath);
        l_flag_print(fname, stat_struct);
        printf("Filename: %s\n", fname);
    }

    else if (flags & DFLAG)
    {
        char* dirname = basename(fpath);
        char* out_string = print_file(fpath);
        printf("Dir name: %s\n", dirname);
    }

    else if (S_ISDIR(stat_struct->st_mode))
    {
        num_files = dir_countfile(fpath);
        size_t size_mem = sizeof(char*);
        file_names = calloc(num_files, size_mem);

        if (file_names == NULL)
        {
            printf("Err: Mem alloc error\n");
            ret_val = 1;
            goto end;
        }
        int filename_result = dir_filenames(fpath, file_names);
        if (filename_result != 0)
        {
            printf("Err: There was error\n");
            ret_val = 1;
            goto end;
        }

        // pointer to sort_wrapper
        int (*comp_fun)(const void*, const void*) = &sort_wrapper;

        // sort alphabetically
        qsort(file_names, num_files, size_mem, comp_fun);

        for (int i = 0; i < num_files; i++)
        {
            if ((!(flags & AFLAG)) && (file_names[i][0] == '.'))
            {
                continue;
            }
            if (flags & LFLAG)
            {
                // workout length of path
                int max_len = (strlen(fpath) + 1) + (strlen(file_names[i]) + 1);
                char* dst = calloc(max_len, sizeof(char));  // alocate space

                // stat struct
                struct stat temp_struct;
                snprintf(dst, max_len, "%s/%s", fpath, file_names[i]);  // concat path
                int stat_status = stat(dst, &temp_struct);
                if (stat_status != 0)
                {
                    printf("%s\n", strerror(errno));
                    continue;
                }

                l_flag_print(file_names[i], &temp_struct);

                // free dst
                free(dst);
                dst = NULL;
            }
            else
            {
                printf("%s\n", file_names[i]);
            }
        }
    }
end:
    // free memory
    for (int i = 0; i < num_files; i++)
    {
        free(file_names[i]);
        file_names[i] = NULL;
    }
    free(file_names);
    file_names = NULL;
    return ret_val;
}
