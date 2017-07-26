#ifndef PRINT_H
#define PRINT_H

void print_atom(_object *obj);
void display_atom(_object *obj);
void print_string(char*s);
void crlf(void);
void print_ast(_object *ast);
void print_debug_ast(_object *ast);
void print_obj(_object *obj);
void clean_print_obj(_object *obj);
void display_obj(_object *obj);
void print_list(_object *obj);
void print_hash(_object *obj);
void display_list(_object *obj);
void display_hash(_object *obj);
void print_func(_object *obj);
object_t *type_proc(object_t *obj);
object_t *prn_proc(object_t *obj);

#endif