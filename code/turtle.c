#include "turtle.h"

#ifdef PARSE
program parse_init(int capacity)
{
    int i;
    program prog;
    prog.capacity = capacity;
    prog.size = 0;
    prog.cw = 0;
    prog.s = stack_init();
    prog.vars =
        (double*)ncalloc(ALPHABETNUMBER,sizeof(double));
    prog.wds =
        (char**)n2dcalloc(capacity,MAXTOKENSIZE,sizeof(char));
    for (i=0; i<capacity; i++) {
        prog.wds[i][0] = '\0';
    }
    for (i=0; i<ALPHABETNUMBER; i++) {
        prog.vars[i] = DOUBLE_MAX;
    }
    return prog;
}

void parse_start(program* prog)
{
    if(!(strsame(prog->wds[prog->cw], "{"))) {
         ON_ERROR("No right main statement ?");
    }
    prog->cw = prog->cw + 1;
    parse_instruction(prog);
}

void parse_instruction(program* prog)
{
    double n;
    if (prog->cw > prog->size) {
        ON_ERROR("Expected a } ?");
    }
    if (strsame(prog->wds[prog->cw], "}")) {
        return;
    }
    else if(strsame(prog->wds[prog->cw], "FD")) {
        if (is_digit(prog->wds[++(prog->cw)])) {
        }
        else if (is_var(prog->wds[prog->cw])) {
        }
        else {
            ON_ERROR("Expected a digit ?");
        }
    }
    else if(strsame(prog->wds[prog->cw], "LT")) {
        if (is_digit(prog->wds[++(prog->cw)])) {
            n = atof(prog->wds[prog->cw]);
            if (is_degree(n) == false) {
                ON_ERROR("Expected a right degree ?");
            }
        }
        else if (is_var(prog->wds[prog->cw])) {
            n = prog->vars[prog->wds[prog->cw][0] - 'A'];
            if (is_degree(n) == false) {
                ON_ERROR("Expected a right degree ?");
            }
        }
        else {
            ON_ERROR("Expected a digit ?");
        }
    }
    else if(strsame(prog->wds[prog->cw], "RT")) {
        if (is_digit(prog->wds[++(prog->cw)])) {
            n = atof(prog->wds[prog->cw]);
            if (is_degree(n) == false) {
                ON_ERROR("Expected a right degree ?");
            }
        }
        else if (is_var(prog->wds[prog->cw])) {
            n = prog->vars[prog->wds[prog->cw][0] - 'A'];
            if (is_degree(n) == false) {
                ON_ERROR("Expected a right degree ?");
            }
        }
        else {
            ON_ERROR("Expected a digit ?");
        }
    }
    else if(strsame(prog->wds[prog->cw], "DO")) {
            parse_loops(prog);
    }
    else if(strsame(prog->wds[prog->cw], "SET")) {
            polish(prog);
    }
    else {
        ON_ERROR("Expecting a right instruction ?");
    }
    prog->cw = prog->cw + 1;
    parse_instruction(prog);
}

void parse_loops(program* prog)
{
    char var_name;
    double* var_value;
    int  loops, start;
    if (is_var(prog->wds[++(prog->cw)])) {
        var_name = prog->wds[(prog->cw)][0];
        if (strsame(prog->wds[++(prog->cw)], "FROM")) {
            if (is_digit(prog->wds[++(prog->cw)])) {
                var_value = set_var(prog, var_name,
                    atof(prog->wds[prog->cw]));
                if (strsame(prog->wds[++(prog->cw)], "TO")) {
                    if (is_digit(prog->wds[++(prog->cw)])) {
                        loops = atoi(prog->wds[prog->cw]);
                        if (loops - (int)*var_value < 0) {
                            ON_ERROR("No right numbers for loops?");
                        }
                        start = ++(prog->cw);
                        while((int)*var_value <= loops) {
                            prog->cw = start;
                            parse_start(prog);
                            (*var_value) ++;
                        }
                        return;
                    }
                    else {
                        ON_ERROR("Expected a end number for loops ?");
                    }
                }
                else {
                    ON_ERROR("Expected a letter TO for loops ?");
                }
            }
            else {
                ON_ERROR("Expected a start number for loops ?");
            }
        }
        else {
            ON_ERROR("Expected a letter FROM for loops ?");
        }
    }
    else {
        ON_ERROR("Expected a single letter ?");
    }
}
#endif

#ifdef INTERP
program interp_init(int capacity)
{
    int i;
    program prog;
    prog.capacity = capacity;
    prog.size = 0;
    prog.cw = 0;
    prog.position.x = MIDWIDTH;
    prog.position.y = MIDHEIGHT;
    prog.position.degree = STARTDEGREE;
    prog.s = stack_init();
    prog.vars =
        (double*)ncalloc(ALPHABETNUMBER,sizeof(double));
    prog.wds =
        (char**)n2dcalloc(capacity,MAXTOKENSIZE,sizeof(char));
    for (i=0; i<capacity; i++) {
        prog.wds[i][0] = '\0';
    }
    for (i=0; i<ALPHABETNUMBER; i++) {
        prog.vars[i] = DOUBLE_MAX;
    }
    return prog;
}

void interp_start(program* prog, SDL_Renderer *rend)
{
    if(!(strsame(prog->wds[prog->cw], "{"))) {
         ON_ERROR("No right main statement ?");
    }
    prog->cw = prog->cw + 1;
    interp_instruction(prog, rend);
}

void interp_instruction(program* prog, SDL_Renderer *rend)
{
    double n;
    if (prog->cw > prog->size) {
        ON_ERROR("Expected a } ?");
    }
    if (strsame(prog->wds[prog->cw], "}")) {
        return;
    }
    else if(strsame(prog->wds[prog->cw], "FD")) {
        if (is_digit(prog->wds[++(prog->cw)])) {
            n = atof(prog->wds[prog->cw]);
            sdl_line_draw(prog, rend, n);
        }
        else if (is_var(prog->wds[prog->cw])) {
            n = prog->vars[prog->wds[prog->cw][0] - 'A'];
            sdl_line_draw(prog, rend, n);
        }
        else {
            ON_ERROR("Expected a digit ?");
        }
    }
    else if(strsame(prog->wds[prog->cw], "LT")) {
        if (is_digit(prog->wds[++(prog->cw)])) {
            n = atof(prog->wds[prog->cw]);
            if (is_degree(n) == false) {
                ON_ERROR("Expected a right degree ?");
            }
            change_degree(prog, n, lt);
        }
        else if (is_var(prog->wds[prog->cw])) {
            n = prog->vars[prog->wds[prog->cw][0] - 'A'];
            if (is_degree(n) == false) {
                ON_ERROR("Expected a right degree ?");
            }
            change_degree(prog, n, lt);
        }
        else {
            ON_ERROR("Expected a digit ?");
        }
    }
    else if(strsame(prog->wds[prog->cw], "RT")) {
        if (is_digit(prog->wds[++(prog->cw)])) {
            n = atof(prog->wds[prog->cw]);
            if (is_degree(n) == false) {
                ON_ERROR("Expected a right degree ?");
            }
            change_degree(prog, n, rt);
        }
        else if (is_var(prog->wds[prog->cw])) {
            n = prog->vars[prog->wds[prog->cw][0] - 'A'];
            if (is_degree(n) == false) {
                ON_ERROR("Expected a right degree ?");
            }
            change_degree(prog, n, rt);
        }
        else {
            ON_ERROR("Expected a digit ?");
        }
    }
    else if(strsame(prog->wds[prog->cw], "DO")) {
            interp_loops(prog, rend);
    }
    else if(strsame(prog->wds[prog->cw], "SET")) {
            polish(prog);
    }
    else {
        ON_ERROR("Expecting a right instruction ?");
    }
    prog->cw = prog->cw + 1;
    interp_instruction(prog, rend);
}


void interp_loops(program* prog, SDL_Renderer *rend)
{
    char var_name;
    double* var_value;
    int  loops, start;
    if (is_var(prog->wds[++(prog->cw)])) {
        var_name = prog->wds[(prog->cw)][0];
        if (strsame(prog->wds[++(prog->cw)], "FROM")) {
            if (is_digit(prog->wds[++(prog->cw)])) {
                var_value = set_var(prog, var_name,
                    atoi(prog->wds[prog->cw]));
                if (strsame(prog->wds[++(prog->cw)], "TO")) {
                    if (is_digit(prog->wds[++(prog->cw)])) {
                        loops = atoi(prog->wds[prog->cw]);
                        if (loops - (int)*var_value < 0) {
                            ON_ERROR("No right numbers for loops?");
                        }
                        start = ++(prog->cw);
                        while((int)*var_value <= loops) {
                            prog->cw = start;
                            interp_start(prog, rend);
                            (*var_value) ++;
                        }
                        return;
                    }
                    else {
                        ON_ERROR("Expected a end number for loops ?");
                    }
                }
                else {
                    ON_ERROR("Expected a letter TO for loops ?");
                }
            }
            else {
                ON_ERROR("Expected a start number for loops ?");
            }
        }
        else {
            ON_ERROR("Expected a letter FROM for loops ?");
        }
    }
    else {
        ON_ERROR("Expected a single letter ?");
    }
}

void sdl_line_draw(program* prog, SDL_Renderer *rend, double n)
{
    SDL_RenderDrawLine(rend, prog->position.x, prog->position.y,
        prog->position.x + n*cos(PI *prog->position.degree/180),
        prog->position.y - n*sin(PI*prog->position.degree/180));
    prog->position.x =
        prog->position.x + n*cos(PI*prog->position.degree/180);
    prog->position.y =
        prog->position.y - n*sin(PI*prog->position.degree/180);
    return;
}

void change_degree(program* prog, double n, direction dir)
{
    if (dir ==  rt) {
        prog->position.degree =
        fmod(n * 360 + prog->position.degree - n, 360);
    }
    else if (dir == lt) {
        prog->position.degree =
        fmod (fabs(prog->position.degree + n), 360);
    }
    else {
        ON_ERROR("NO right direction order ?");
    }
    return;
}
#endif

void turtle_free(program* prog)
{
    n2dfree((void**)prog->wds, prog->capacity);
    free(prog->vars);
    stack_free(prog->s);
}

void turtle_resize(program* prog, int capacity)
{
    int i;
    prog->wds = (char**)n2drecalloc((void**)prog->wds,
        prog->capacity, capacity, MAXTOKENSIZE,
        MAXTOKENSIZE, sizeof(char));
    prog->capacity = capacity;
    for(i=prog->capacity; i<capacity; i++) {
        prog->wds[i][0] = '\0';
    }
}
double* set_var(program* prog, char varn, double varv)
{
    int n;
    n = varn - 'A';
    prog->vars[n] = varv;
    return &(prog->vars[n]);
}

void polish(program* prog)
{
    char var_name;
    double var_value;
    if (is_var(prog->wds[++(prog->cw)])) {
        var_name = prog->wds[(prog->cw)][0];
        if (strsame(prog->wds[++(prog->cw)], ":=")) {
            prog->cw = prog->cw + 1;
            expr(prog, var_name, &var_value);
            set_var(prog, var_name, var_value);
            return;
        }
        else {
            ON_ERROR("Expected := ?");
        }
    }
    else {
        ON_ERROR("Expected a single letter ?");
    }
}

void expr(program* prog, char var_name, double* var_value)
{
    double op1 , op2, n;
    if (prog->cw >= prog->size) {
        ON_ERROR("Expected a ; ?");
    }
    if (strsame(prog->wds[(prog->cw)], ";")) {
        if(stack_pop(prog->s, var_value)){
            if (stack_isempty(prog->s)) {
               return;
            }
            else {
                ON_ERROR("Stack still have numbers ?");
            }
        }
        else {
            ON_ERROR("No right polish expression ?");
        }
    }
    else if (is_var(prog->wds[(prog->cw)])) {
        n = prog->vars[prog->wds[(prog->cw)][0]-'A'];
        stack_push(prog->s, n);
    }
    else if (is_digit(prog->wds[(prog->cw)])) {
        n = atof(prog->wds[(prog->cw)]);
        stack_push(prog->s, n);
    }
    else if (is_op(prog->wds[(prog->cw)])) {
        if ((stack_pop(prog->s, &op2))
            && (stack_pop(prog->s, &op1))) {
            n = count(op1, op2, prog->wds[prog->cw][0]);
            stack_push(prog->s,n);
        }
        else {
            ON_ERROR("No right polish expression ?");
        }
    }
    else {
        ON_ERROR("No right polish expression ?");
    }
    prog->cw = prog->cw + 1;
    expr(prog, var_name, var_value);
}

bool is_var(char* str)
{
    if((isupper(str[0])) && (strlen(str) == 1)) {
        return true;
    }
    return false;
}

bool is_digit(char* str)
{
    int i, length;
    length = strlen(str);
    for (i=0; i<length; i++) {
        if ((i == 0 || i == length-1)
            && (str[i] == '.')) {
                return false;
            }
        if ((str[i] < '0' || str[i] > '9')
            && (str[i] != '.')) {
            return false;
        }
    }
    return true;
}

bool is_op(char* str)
{
    if(((str[0] == '+') || (str[0] == '-') ||
        (str[0] == '*') || (str[0] == '/'))
            && ((strlen(str) == 1))) {
        return true;
    }
    return false;
}

bool is_degree(double n) {
    if (n >= 0) {
        return true;
    }
    else {
        return false;
    }
}

double count(double op1, double op2, char c)
{
    switch (c) {
        case '+':
            return op1 + op2;
        case '-':
            return op1 - op2;
        case '*':
            return op1 * op2;
        case '/':
            return op1 / op2;
        default:
            ON_ERROR("NO right oprand ?");
    }
}
