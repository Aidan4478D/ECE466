int a[10];

int main() {
    int x,*p;
    a[3] = 2;

    x=a[3];

    //hello
    p= &a[5];

    /*printf("hello");*/

    int b;
    b = x + 5;

    printf("b is %d\n", b);
    printf("x is %d\n", x);

    p = 3 + &x;
}
