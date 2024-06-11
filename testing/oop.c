#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


typedef struct {
    size_t totalSize;
    char* text;
} Object; 

typedef struct {
    Object super;
    char* text2;
} Derived;



int main() {

    Derived derived;
    derived.text2 = "derived text";
    derived.super.text = "super text";

    void* ptr = &derived;

    Object* super = (Object*)ptr;
    Derived* sub = (Derived*)ptr;

    printf("Super text: %s\n", super->text);
    printf("Derived text: %s\n", sub->text2);
    



    return 0;
}