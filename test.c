//
// Created by xiaoc on 2018/9/11.
//
//int foo(int x,int y, int a,int b,int c,int d,int e,int f,int g,int i);
int printf(char *s, int x);
void * malloc(int size);
int foo(int x, int y, int a, int b, int c, int d, int e, int f, int g, int i);

int foo(int x, int y, int a, int b, int c, int d, int e, int f, int g, int i) {
    return i;
}

int f(int x);
int f(int x){
    return x + x;
}
int g();
int g(){
    return 123;
}

int abs(int x){
    if(x < 0){
        return -x;
    }
    return x;
}
int main(int argc, char **argv) {
    /* printf("Hello world!\n", 0);
     int x;
     x = 2 * (3 + 4);
     int y;
     y = 2 + x;
     printf("%d\n", y);
     return 0;*/
//    int *p;
//    p = 10;
//    char c;
//    c = 3;
//    c = c * 3;
//    float x;
//    x = 3.0 * 4;
//    int y = 4;
//    int x = foo(1,2,3,y,5,6,7,8,9, 10);
    int x = -3;
    printf("hello world! argc is %d\n",argc);
    // printf("hello world! argc is %d\n", printf("%d", 3123));
    printf("foo = %d\n", foo(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
//   printf("expr = %d\n", 2* (3+4));
    int i = 0;
    while (i < 10) {
        printf("%d\n", i);
        i = i + 1;
    }
    printf("%d\n", g());
    printf("%d\n",f(i));
    printf("abs(x)=%d\n",abs(x));
    int * p = &x;
    *p = 234;
    printf("*p=%d\n",*p);
    printf("x*3=%d\n", x*3);
    int * p2 = (void*)malloc(8);
    *(p2 + 4) = 123;
    printf("p2[1] = %d\n",*(p2+4));
    
}