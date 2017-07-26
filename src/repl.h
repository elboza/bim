/*
 * author: Fernando Iazeolla
 * license: GPLv2
 */

#ifndef REPL_H
#define REPL_H

#define MAX_CMD 255
void reset_stdin(void);
int quit_shell;
void shell(void);
void execute(char *s);
void run_exec(char *buff);
void run_script(char *file);

#endif
