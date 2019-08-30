__kernel void tempfunc(__global char16* dst)
{
    *dst = (char16)('a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a');
}

__kernel void addone(__global float* input, __global float* output)
{
    int i = get_global_id(0);
    output[i] = input[i] + 1;
}