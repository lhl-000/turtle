#include "stack.h"
#define FORMATSTR "%f"
#define ELEMSIZE 20
#define STACKTYPE "Realloc"
#define FIXEDSIZE 16
#define SCALEFACTOR 2

struct stack {
   /* Underlying array */
   stacktype* a;
   int size;
   int capacity;
};

stack* stack_init(void)
{
   stack *s = (stack*) ncalloc(sizeof(stack), 1);
   /* Some implementations would allow you to pass
      a hint about the initial size of the stack */
   s->a = (stacktype*) ncalloc(sizeof(stacktype), FIXEDSIZE);
   s->size = 0;
   s->capacity= FIXEDSIZE;
   return s;
}

void stack_push(stack* s, stacktype d)
{
   if(s==NULL){
       return;
   }
   s->a[s->size] = d;
   s->size = s->size + 1;
   if(s->size >= s->capacity){
      s->a = (stacktype*) nremalloc(s->a, sizeof(stacktype)*s->capacity*SCALEFACTOR);
      s->capacity = s->capacity*SCALEFACTOR;
   }
}

bool stack_pop(stack* s, stacktype* d)
{
   if((s == NULL) || (s->size < 1)){
      return false;
   }
   s->size = s->size - 1;
   *d = s->a[s->size];
   return true;
}

bool stack_isempty(stack* s)
{
    return s->size == 0;
}

bool stack_peek(stack* s, stacktype* d)
{
   if((s==NULL) || (s->size <= 0)){
      /* Stack is Empty */
      return false;
   }
   *d = s->a[s->size-1];
   return true;
}

bool stack_free(stack* s)
{
   if(s==NULL){
      return true;
   }
   free(s->a);
   free(s);
   return true;
}
