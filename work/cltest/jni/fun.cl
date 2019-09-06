__kernel void tempfunc(__global char16* dst)
{
    *dst = (char16)('a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a');
}

__kernel void addone(__global float* input, __global float* output)
{
    int i = get_global_id(0);
    int j = get_global_id(1);
    output[i][j] = input[i][j] + 1;
}

__kernel void reduction_scalar(__global float* data, __local float* partial_sums, __global float* output)
{
    int lid = get_local_id(0);
    int group_size = get_local_size(0);
    partial_sums[lid] = data[get_global_id(0)];
    barrier(CLK_LOCAL_MEM_FENCE);
    for (int i = group_size / 2; i > 0; i >>= 1)
    {
        if (lid < i)
        {
            partial_sums[lid] += partial_sums[lid + 1];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    if (lid == 0)
    {
        output[get_group_id(0)] = partial_sums[0];
    }
}

__kernel void reduction_vector(__global float4* data, __local float4* partial_sums, __global float4* output)
{
    int lid = get_local_id(0);
    int group_size = get_local_size(0);
    partial_sums[lid] = data[get_global_id(0)];
    barrier(CLK_LOCAL_MEM_FENCE);
    for (int i = group_size / 2; i > 0; i >>= 1)
    {
        if (lid < i)
        {
            partial_sums[lid] += partial_sums[lid + 1];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    if (lid == 0)
    {
        output[get_group_id(0)] = partial_sums[0];
    }
}

__kernel void reduction_complete(__global float4* data, __local float4* partial_sums, __global float* output)
{
    int lid = get_local_id(0);
    int group_size = get_local_size(0);
    partial_sums[lid] = data[get_global_id(0)];
    barrier(CLK_LOCAL_MEM_FENCE);
    for (int i = group_size / 2; i > 0; i >>= 1)
    {
        if (lid < i)
        {
            partial_sums[lid] += partial_sums[lid + 1];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    if (lid == 0)
    {
        *output = partial_sums[0].s0 + partial_sums[0].s1 + partial_sums[0].s2 + partial_sums[0].s3;
    }
}