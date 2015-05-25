#ifndef INTERFACE_H
#define INTERFACE_H

#include <ncurses.h>
#include <stdlib.h>
#include <cdk/cdk.h>
#include "interface.h"

void init_window();
void refresh_screen();
int check_keys();

extern char **connects_list;
extern int connections_num;

#endif // INTERFACE_H
