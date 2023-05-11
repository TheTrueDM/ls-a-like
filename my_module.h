//
// Header file for a module in the project
//

#ifndef BASE_PROJECT_MY_MODULE_H
#define BASE_PROJECT_MY_MODULE_H

#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Declare globals
extern uint8_t LFLAG;
extern uint8_t AFLAG;
extern uint8_t DFLAG;
extern uint8_t SFLAG_T;
extern uint8_t SFLAG_Z;
extern uint8_t SFLAG_N;
extern uint8_t RFLAG;

// Declare a function to prove the template#
int function(char*, const struct stat*, const u_int8_t);
// int a_function(const char*);

#endif  // BASE_PROJECT_MY_MODULE_H
