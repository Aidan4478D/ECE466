int f(char* a, int b) {
    printf("value is %d, %s\n", b, a);
    return b + 1;
}

int main() {
    char *g, *i;
    g = "hello";
    i = "hello, 2";
    return f(i, 3);

}
