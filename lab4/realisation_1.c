int prime_count(int a, int b) {
    int result = 0;
    
    for (int i = a; i <= b; ++i) {
        int div_cnt = 0;

        for (int j = 1; j <= i; ++j) {
            if (i % j == 0) {
                div_cnt += 1;
            }
        }

        if (div_cnt == 2) {
            result += 1;
        }
    }

    return result;   
}

float pi(int k) {
    float result = 1;
    int accumulator = 3;

    for (int i = 1; i <= k; ++i, accumulator += 2) {
        result = result + (float)(i % 2 == 1 ? -1 : 1) / (float) accumulator;
    }

    return 4 * result;
}