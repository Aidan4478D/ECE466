int f() {

    int a;
    char x;
    double l;

    a = 3;

    a = sizeof(a);
    x = sizeof(x);
    l = sizeof(l);

    a = sizeof(int *);

    return a + x + l;
}
