int a[10];

int f() {
    int x,*p;
    x=a[3];
    p= &a[5];

    int b;
    b = x + 5;

    p = 3 + &x;
}
