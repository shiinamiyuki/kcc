//
// Created by xiaoc on 2018/9/11.
//
int printf(char *s, int a);

int main() {
   /* printf("Hello world!\n", 0);
    int x;
    x = 2 * (3 + 4);
    int y;
    y = 2 + x;
    printf("%d\n", y);
    return 0;*/
   int x = 10;
   int y = 20;
   if(y > 20){
       x = 2;
   }else if(y > 10){
       if(x > 1){
           x = 3;
       }else{
           x = 4;
       }
       x = x + 1;
   }else{
       x = 0;
   }
    return x;
}