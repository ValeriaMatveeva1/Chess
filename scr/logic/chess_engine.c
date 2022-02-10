#include "vector.c"
#include <stdio.h>


int main()
{
    vector_int_t v;
    v_init(&v);
    v_pop(&v, 1);
    v_insert(&v, 10, 24);
    for (int i = 0; i<5; ++i) v_push(&v,i);
    v_pop(&v, 3);
    v_insert(&v, 1, 24);
    for (int i = 0; i<v.len; ++i) printf("%d\n", v.data[i]);
    
    return 0;
}