int words(char *str);

void int_to_char(int val, char *str);

void push(char *segment, char *value);

void pop(char *segment, char *value);

void branching(char *command, char *label);

void math(char *operator, char *str);

void call(char *str1, char *str2, char *str3);

void function(char *str1, char *str2, char *str3);

void return_value(char *str);

void parser(char *str1, char *str2, int count, int argc);

void parse_elem(char *str1, char *str2, char *str3);

void parse_last_elem(char *str1, char *str2);

void parse_name(char *str1, char *str2);

void push_static_mult(char *str1, char *str2, char *str3);

void pop_static_mult(char *str1, char *str2, char *str3);

void copy(char *str1, char *str2);

int bool_vm(char *str);

