int f() {

    int a;
    a = 3;

    int b;
    b = sizeof(a);

    a = sizeof(*a);

    return b;
}
