#ifndef EVAL_H
#define EVAL_H

char is_self_evaluating(object_t *exp);
char is_variable(object_t *expression);
char is_tagged_list(object_t *expression, object_t *tag);

object_t *eval_list(object_t *exp,object_t *env);
object_t *eval_hash(object_t *exp,object_t *env);
char is_assignment(object_t *exp);
object_t *assignment_variable(object_t *exp);
object_t *assignment_value(object_t *exp);
char is_definition(object_t *exp);
int is_global(object_t *exp);
object_t *definition_variable(object_t *exp);
object_t *definition_value(object_t *exp);
object_t *make_if(object_t *predicate, object_t *consequent,object_t *alternative);
char is_if(object_t *expression);
object_t *if_predicate(object_t *exp);
object_t *if_consequent(object_t *exp);
object_t *if_alternative(object_t *exp);
object_t *make_lambda(object_t *parameters, object_t *body);
char is_lambda(object_t *exp);
object_t *lambda_parameters(object_t *exp);
object_t *lambda_body(object_t *exp);
object_t *make_begin(object_t *seq);
char is_begin(object_t *exp);
object_t *begin_actions(object_t *exp);
char is_last_exp(object_t *seq);
object_t *first_exp(object_t *seq);
object_t *rest_exps(object_t *seq);
char is_cond(object_t *exp);
object_t *cond_clauses(object_t *exp);
object_t *cond_predicate(object_t *clause);
object_t *cond_actions(object_t *clause);
char is_cond_else_clause(object_t *clause);
object_t *sequence_to_exp(object_t *seq);
object_t *expand_clauses(object_t *clauses);
object_t *cond_to_if(object_t *exp);
object_t *make_application(object_t *operator, object_t *operands);
char is_application(object_t *exp);
object_t *operator(object_t *exp);
object_t *operands(object_t *exp);
char is_no_operands(object_t *ops);
object_t *first_operand(object_t *ops);
object_t *rest_operands(object_t *ops);

object_t *binding_parameter(object_t *binding);
object_t *binding_argument(object_t *binding);
object_t *bindings_parameters(object_t *bindings);
object_t *bindings_arguments(object_t *bindings);

//object_t *make_undef_vars(object_t *lp);
object_t *make_init_andbody(object_t *lp,object_t *lb,object_t *body);

char is_and(object_t *exp);
object_t *and_tests(object_t *exp);
char is_or(object_t *exp);
object_t *or_tests(object_t *exp);
object_t *apply_operator(object_t *arguments);
object_t *prepare_apply_operands(object_t *arguments);
object_t *apply_operands(object_t *arguments);
object_t *eval_expression(object_t *arguments);
object_t *eval_environment(object_t *arguments);
object_t *list_of_values(object_t *exps, object_t *env);
object_t *eval_assignment(object_t *exp, object_t *env);
object_t *eval_definition(object_t *exp, object_t *env);

object_t *eval(object_t *exp, object_t *env);
object_t *run(object_t *ast,object_t *env);

#endif
