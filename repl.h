/*
 * author: Fernando Iazeolla
 * license: GPLv2
 */

#ifndef REPL_H
#define REPL_H

#define MAX_CMD 255
int quit_shell;
void shell(void);
void execute(char *s);

#endif
