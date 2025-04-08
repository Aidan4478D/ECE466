int f(int a, int b) {
    for(a = 1; a < b; a++) {
        b -= a;
    }
    return b;
}

void g(int f) {

h:
    switch(f) {
        case 1: printf("Hello"); goto l; break;
        default: printf("No"); break;
    }
    f--;

    double d;
    d = -10.1;

    if(f > d) goto h;
}
