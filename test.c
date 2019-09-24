//
// Created by xiaoc on 2018/9/11.
//
//int foo(int x,int y, int a,int b,int c,int d,int e,int f,int g,int i);

int printf(char *s, ...);

void puts(char *s);

void *malloc(int size);

int foo(int x, int y, int a, int b, int c, int d, int e, int f, int g, int i) {
    return i;
}


int f(int x) {
    return x + x;
}

int g() {
    return 123;
}

int abs(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

int fact(int x) {
    if (x == 0) {
        return 1;
    }
    return x * fact(x - 1);
}

int globalValue;
enum {
    TOK_INT = 4, TOK_CHAR
};
struct A {
    int a, b, c;
};
struct Node;
struct Node {
    int value;
    struct Node *next;
};
struct Node;

struct Node *createNode(int value) {
    struct Node *p = (struct Node *) malloc(sizeof(struct Node));
    p->value = value;
    return p;
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
    printf("hello world! argc is %d\n", argc);
    printf("constant folding: (3*2)<<3=%d\n", (3 * 2) << 3);
    printf("foo = %d\n", foo(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
    int i = 0;
    while (i < 10) {
        printf("%d\n", i);
        i = i + 1;
    }
    printf("%d\n", g());
    printf("%d\n", f(i));
    printf("abs(x)=%d\n", abs(x));
    int *p = &x;
    *p = 234;
    printf("*p=%d\n", *p);
    printf("x*3=%d\n", x * 3);
    int *p2 = (int *) malloc(8);
    printf("p2 = %x\n", (int) p2);
    printf("p2 = %x\n", (int) (p2 + 1));
    p2[1] = 123;
    printf("p2[1] = %d\n", p2[1]);
    printf("fact(10) = %d\n", fact(10));
    globalValue = 3333;
    printf("globalValue = %d\n", globalValue);
    char *s = (char *) malloc(123);
    s[0] = 'a';
    s[1] = 'b';
    s[2] = 0;
    printf("TOK_CHAR=%d\n", TOK_CHAR);
    x = 3;
    if (x > 0 && x < 5) {
        printf("yes\n");
    }
    printf("ternary: x > 2 ? 1 : 0 = %d\n", x > 2 ? 1 : 0);
    printf("x++=%d\n", x++);
    printf("++x=%d\n", ++x);
    for (int i = 0;; i++) {
        if (i > 10)
            break;
        printf("%d\n", i);
    }
    int k = 10;
    do {
        k++;
    } while (k < 10);
    printf("%d\n", k);
    struct A a;
    a.a = 33333;
    a.b = 1333;
    printf("a.a=%d, a.b=%d\n", a.a, a.b);
    struct A *pa;
    pa = &a;
    printf("pa->a=%d, pa->b=%d\n", pa->a, pa->b);
    struct Node *node = createNode(123);
    printf("node->value=%d\n", node->value);
}


