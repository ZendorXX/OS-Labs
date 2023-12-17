int prime_count(int a, int b) {
    short prime[b + 1];

    for (int i = 0; i < b + 1; ++i) {
        prime[i] = 1;
    }

    prime[0] = prime[1] = 0;
    for (int i = 2; i < b + 1; ++i) {
        if (prime[i] == 1) {
            for (int j = 2 * i; j < b + 1; j += i) {
                prime[j] = 0;
            }
        }
    }

    int result = 0;
    for (int i = a; i <= b; ++i) {
        result += prime[i];
    }

    return result;
}

float pi(int k) {
    float result = 1;

    for (int i = 1; i <= k; ++i) {
        float chis = 4 * i * i;
        float znam = chis - 1;
        float tmp = chis / znam;
        result *= tmp;
    }
    
    return 2 * result;
}

