#ifndef PRINT_H
#define PRINT_H

void print_atom(_object *obj);
void crlf(void);
void print_ast(_object *ast);
void print_debug_ast(_object *ast);
void print_obj(_object *obj);
void print_list(_object *obj);
void print_hash(_object *obj);
void print_func(_object *obj);

#endif