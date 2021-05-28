#include "stack.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#ifdef INTERP
#include "neillsdl2.h"
#endif

#define ALPHABETNUMBER 26
#define INITNUMSIZE 100
#define DOUBLE_MAX 3.40282e+038
#define PI 3.14159265358979323846
#define STARTDEGREE 90.0
#define MIDWIDTH 400
#define MIDHEIGHT 300
#define MAXTOKENSIZE 20
#define strsame(A,B) (strcmp(A, B)==0)
#define ON_ERROR(PHRASE) {fprintf(stderr,"Fatal Error %s occured in %s, line %d\n",PHRASE, __FILE__, __LINE__); exit(2); }

#ifdef INTERP
typedef enum direction {rt, lt} direction;
struct position {
    double x;
    double y;
    double degree;
};
typedef struct position position;
struct prog {
    char** wds;
    int capacity;
    int size;
    int cw;
    double* vars;
    position position;
    stack *s;
};
typedef struct prog program;
#endif

#ifdef PARSE
struct prog {
    char** wds;
    int capacity;
    int size;
    int cw;
    double* vars;
    stack *s;
};
typedef struct prog program;
#endif

#ifdef PARSE
program parse_init(int capacity);
void parse_start(program* prog);
void parse_instruction(program* prog);
void parse_loops(program* prog);
#endif

#ifdef INTERP
program interp_init(int capacity);
void interp_start(program* prog, SDL_Renderer *rend);
void interp_instruction(program* prog, SDL_Renderer *rend);
void interp_loops(program* prog, SDL_Renderer *rend);
void change_degree(program* prog, double n, direction dir);
void sdl_line_draw(program* prog, SDL_Renderer *rend, double n);
#endif

double* set_var(program* prog, char varn, double varv);
void turtle_free(program* prog);
void turtle_resize(program* prog, int capacity);
void expr(program* prog, char var_name, double* var_value);
void polish(program* prog);
bool is_digit(char* str);
bool is_var(char* str);
bool is_op(char* str);
bool is_degree(double n);
double count(double op1, double op2, char c);
