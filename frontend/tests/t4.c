int a;
int f(float c) {
    double b;
    while(a > b) {
        a--;
        while(b > c) { 
            b /= c;
        }
    }
    if(a > c) 
        return a;
    else 
        return c;
}
