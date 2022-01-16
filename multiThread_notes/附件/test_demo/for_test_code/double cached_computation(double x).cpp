double cached_computation(double x)
{
    // 不好：这两个静态变量导致多线程的使用情况中的数据竞争
    static double cached_x = 0.0;
    static double cached_result = COMPUTATION_OF_ZERO;
    double result;
​
    if (cached_x == x)
        return cached_result;
    result = computation(x);
    cached_x = x;
    cached_result = result;
    return result;
}
//但在多线程环境中，其两个 static 变量将导致数据竞争进而发生未定义的行为。
