int a;

int f(int b) {
    int c;

    a = 3;
    b = a;
    b = 2 + a;

    c = a + b;
    c = a * b;
    c = a - b;
    c = a / b;
    c = a % b;

    return c;
}
