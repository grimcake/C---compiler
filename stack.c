#include "stack.h"
#include <string.h>

static void stackGrow(Stack *s){
    s->allocLength *= 2;
    s->element = realloc(s->element, s->allocLength*s->elemSize);
}

void stackCreate(Stack* s, int elemSize){
    s->elemSize = elemSize;
    s->elemNum = 0;
    s->allocLength = 4;
    s->element = malloc(s->allocLength*s->elemSize);
    if(s->element == NULL){
        printf("malloc error\n");
    }
}

void stackDestroy(Stack* s){
    free(s->element);
}

void stackPush(Stack* s, void* element){
    if(s->elemNum == s->allocLength){
        stackGrow(s);
        if(s->elemNum == s->allocLength){
            printf("steckGrow error\n");
        }
    }
    void* target = (char*)(s->element)+s->elemNum*s->elemSize;
    memcpy(target, element, s->elemSize);
    s->elemNum++;
}

void stackPop(Stack* s, void* element){
    if(s->elemNum == 0){
        printf("stack is empty\n");
        return;
    }
    void* target = (char*)(s->element)+(s->elemNum-1)*s->elemSize;
    memcpy(element, target, s->elemSize);
    s->elemNum--;
}



/*int main(){
    Stack* s;
    stackCreate(s, sizeof(int));
    int k = 12;
    stackPush(s, &k);
    int num;
    stackPop(s, &num);
    stackPop(s, &num);
    stackPop(s, &num);
    printf("%d\n", num);

}*/
