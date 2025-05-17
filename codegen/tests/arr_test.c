int a[10];

int main() {
    int x,*p;
    a[3] = 2;
    a[8] = 8;

    x=a[3];
    printf("x is %d\n", x);

    //hello
    p= &a[5];

    /*printf("hello");*/

    int b;
    b = x + 5;

    if (x < 5) {
        x = b + 5;
    }
    else {
        x = 100;
    }

    printf("b is %d\n", b);
    printf("p[2] is %d\n", *p[2]); // should be 8
    printf("x is now %d\n", x);

    p = 3 + &x;
}
