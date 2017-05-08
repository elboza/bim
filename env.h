#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

object_t *make_symbol(char *value);
int is_pair(object_t *obj);
void set_car(object_t *obj, object_t *value);
void set_cdr(object_t *obj, object_t *value);

object_t *the_empty_list;
object_t *false;
object_t *true;
object_t *bottom;
object_t *symbol_table;
object_t *the_empty_environment;
object_t *the_global_environment;

object_t *error_proc(object_t *args);


object_t *make_primitive_proc(object_t *(*fn)(struct _object *arguments));
char are_number_args(object_t *arguments);
char are_float_args(object_t *arguments);

object_t *enclosing_environment(object_t *env);
object_t *first_frame(object_t *env);
object_t *make_frame(object_t *variables, object_t *values);
object_t *frame_variables(object_t *frame);
object_t *frame_values(object_t *frame);
void add_binding_to_frame(object_t *var, object_t *val, object_t *frame);
object_t *extend_environment(object_t *vars, object_t *vals,object_t *base_env);
object_t *lookup_variable_value(object_t *var, object_t *env);
void set_variable_value(object_t *var, object_t *val, object_t *env);
void define_variable(object_t *var, object_t *val, object_t *env);
object_t *setup_environment(void);
void populate_environment(object_t *env);
object_t *make_environment(void);
void init_env(void);

#endif
