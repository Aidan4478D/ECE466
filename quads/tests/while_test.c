int f() {
    int a, b, c;
    a = 1;
    b = 5;
    while(a > b) {
        a++;
        if(a > b) break;
        c = a % 3;
        if(a == c) continue;
        b = a + 1;
    }
    return a;
}
