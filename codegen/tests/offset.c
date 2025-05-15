int a;
int b;

int f(int c, int d) {
    int e;
    int f;

    a = 1;
    f = 3;
    e = 2;
    d = 1;
    c = 0;
    b = 1;

    
    return a + b + c + d + e + f;
}

int main() {
    
    int z;
    z=f(1, 2);
    printf("a+b=%d\n", a + b);  
   
    return z;
}
