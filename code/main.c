#include "turtle.h"

int main(int argc, char** argv)
{
    int i;
    FILE *fp;
    char* fn;
    program prog;
#ifdef INTERP
    SDL_Simplewin sw;
    Neill_SDL_Init(&sw);
    Neill_SDL_SetDrawColour(&sw, 255,251,240);
    prog = interp_init(INITNUMSIZE);
#endif

#ifdef PARSE
    prog = parse_init(INITNUMSIZE);
#endif
    if (argc != 2) {
        ON_ERROR("Wrong arguments, Expected a filename !");
    }
    fn = argv[1];
    fp = nfopen(fn, "r");
    i=0;
    while(fscanf(fp, "%s", prog.wds[i++])==1) {
        prog.size++;
        if (i >= prog.capacity) {
            turtle_resize(&prog, 2*prog.capacity);
        }
    }

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
