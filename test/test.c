#include "../code/turtle.h"


int main(void)
{
    int i;
    FILE *fp;
    program prog;
    char str[10];
#ifdef INTERP
    SDL_Simplewin sw;
    Neill_SDL_Init(&sw);
    Neill_SDL_SetDrawColour(&sw, 255,251,240);
    prog = interp_init(INITNUMSIZE);
#endif

#ifdef PARSE
    prog = parse_init(INITNUMSIZE);
#endif
    assert(prog.capacity == INITNUMSIZE);
    assert(prog.size == 0);
    assert(prog.cw == 0);
    /*fp = nfopen("data/loop.txt", "r");*/
     /*fp = nfopen("data/variables.txt", "r");*/
    /*fp = nfopen("data/nestedloops.txt", "r");*/
    fp = nfopen("data/basic.txt", "r");
    /*fp = nfopen("data/basic_wrong_1.txt", "r");*/
    /*fp = nfopen("data/basic_wrong_2.txt", "r");*/
    /*fp = nfopen("data/basic_wrong_3.txt", "r");*/
    /*fp = nfopen("data/basic_wrong_4.txt", "r");*/
    /*fp = nfopen("data/basic_wrong_5.txt", "r");*/
    /*fp = nfopen("data/loop_wrong_1.txt", "r");*/
    /*fp = nfopen("data/loop_wrong_2.txt", "r");*/
    /*fp = nfopen("data/variables_wrong_1.txt", "r");*/
    /*fp = nfopen("data/variables_wrong_2.txt", "r");*/
    /*fp = nfopen("data/variables_wrong_3.txt", "r");*/
    i=0;
    while(fscanf(fp, "%s", prog.wds[i++])==1) {
        prog.size++;
        if (i >= prog.capacity - 1) {
            turtle_resize(&prog, 2*prog.capacity);
            assert(prog.capacity > INITNUMSIZE);
        }
    }
    assert(i<prog.capacity);
    /* test inside functions*/
    assert(is_degree(-1) == false);
    assert(is_degree(1) == true);
    assert(fabs(count(1.0, 3.0, '+') - 4.0) < 0.000001);
    assert(fabs(count(21.0, 11.0, '-') - 10.0) < 0.000001);
    assert(fabs(count(9.0, 2.0, '*') - 18.0) < 0.000001);
    assert(fabs(count(9.0, 3.0, '/') - 3.0) < 0.000001);
    str[0] = '*';
    str[1] = '\0';
    assert(is_op(str) == true);
    str[0] = '-';
    str[1] = '\0';
    assert(is_op(str) == true);
    str[0] = '+';
    str[1] = '\0';
    assert(is_op(str) == true);
    str[0] = '/';
    str[1] = '\0';
    assert(is_op(str) == true);
    str[0] = '8';
    str[1] = '\0';
    assert(is_op(str) == false);
    str[0] = 'z';
    str[1] = '\0';
    assert(is_op(str) == false);
    str[0] = '*';
    str[1] = '/';
    str[2] = '\0';
    assert(is_op(str) == false);

    str[0] = '8';
    str[1] = '\0';
    assert(is_digit(str) == true);
    str[0] = '8';
    str[1] = '.';
    str[2] = '1';
    str[3] = '\0';
    assert(is_digit(str) == true);
    str[0] = 'z';
    str[1] = '\0';
    assert(is_digit(str) == false);
    str[0] = '*';
    str[1] = '\0';
    assert(is_digit(str) == false);
    str[0] = '.';
    str[1] = '1';
    str[2] = '1';
    str[3] = '\0';
    assert(is_digit(str) == false);
    str[0] = '1';
    str[1] = '1';
    str[2] = '.';
    str[3] = '\0';
    assert(is_digit(str) == false);

    str[0] = 'A';
    str[1] = '\0';
    assert(is_var(str) == true);
    str[0] = 'Z';
    str[1] = '\0';
    assert(is_var(str) == true);
    str[0] = 'a';
    str[1] = '\0';
    assert(is_var(str) == false);
    str[0] = 'A';
    str[1] = 'B';
    str[2] = '\0';
    assert(is_var(str) == false);

#ifdef PARSE
    parse_start(&prog);
    if (++(prog.cw) != prog.size) {
        ON_ERROR("Expected a right instruction in the end ?");
    }
    printf("Parsed OK\n");
#endif

#ifdef INTERP
    interp_start(&prog, sw.renderer);
    if (++(prog.cw) != prog.size) {
        ON_ERROR("Expected a right instruction in the end ?");
    }
#endif

    fclose(fp);
    turtle_free(&prog);

#ifdef INTERP
    Neill_SDL_UpdateScreen(&sw);
    do{
        Neill_SDL_Events(&sw);
    }while(!sw.finished);
    SDL_Quit();
    atexit(SDL_Quit);
#endif

    return 0;
}
