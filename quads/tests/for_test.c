int f() {
    int i, sum;
    for(i = 0; i < 10; i++) {
        sum = sum + i;
        if (sum == 3) continue;
        if (sum > 7) break;
        sym--;
    }
    return sum;
}
