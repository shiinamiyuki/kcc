void putchar(char c);

void *malloc(int size);

int getchar();

int printf(char *s, ...);

int main() {
    int size = 10000;
    int *cell = malloc(10000 * 4);
    for (int i = 0; i < size; i++) {
        cell[i] = 0;
    }
    char *program = malloc(10000);
    for (int i = 0;; i++) {
        char c = getchar();
        //printf("program=%x, c=%c, i=%d\n", program, c, i);
        program[i] = c;
        if (c == '!') {
            program[i] = 0;
            break;
        }
    }
    int pc = 0;
    int ptr = 0;
    while (1) {
        char c = program[pc];
        //printf("pc=%d, %c\n", pc, c);
        if (!c) {
            printf("end\n");
            break; 
        }
        if (c == '+') {
            program[ptr]++;
            pc++;
        } else if (c == '-') {
            program[ptr]--;
            pc++;
        } else if (c == '<') {
            ptr--;
            if (ptr < 0) {
                ptr += size;
            }
            pc++;
        } else if (c == '>') {
            ptr++;
            if (ptr >= size) {
                ptr -= size;
            }
            pc++;
        } else if (c == ',') {
            program[ptr] = getchar();
            pc++;
        } else if (c == '.') {
            putchar(program[ptr]);
            pc++;
        } else if (c == '[') {
            if (program[ptr]) {
                pc++;
            } else {
                int cnt = 0;
                while (1) {
                    c = program[pc];
                    if (c == '[') {
                        cnt++;
                    }
                    if (c == ']') {
                        cnt--;
                        if (cnt == 0)
                            break;
                    }
                    pc++;
                }
                pc++;
            }
        } else if (c == ']') {
            if (!program[ptr]) {
                pc++;
            } else {
                int cnt = 0;
                while (1) {
                    c = program[pc];
                   
                    if (c == ']') {
                        cnt++;                       
                    }
                     if (c == '[') {
                        cnt--;
                         if (cnt == 0)
                            break;
                    }
                    pc--;
                }
                pc++;
            }
        } else {
            pc++;
        }
    }
}