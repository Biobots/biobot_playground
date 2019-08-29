#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main()
{
    cl_platform_id* platforms;
    cl_uint num_platforms;
    cl_int err;
    err = clGetPlatformIDs(4, NULL, &num_platforms);
    platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id) * num_platforms);
    err = clGetPlatformIDs(num_platforms, platforms, NULL);
    for (int i = 0; i < num_platforms; i++)
    {
        char pform_name[40];
        char pform_vendor[40];
        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(pform_name), &pform_name, NULL);
        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(pform_vendor), &pform_vendor, NULL);
        printf("name: %s \nvendor: %s\n", pform_name, pform_vendor);
    }
    cl_uint num_devices;
    cl_device_id* devices;
    devices = (cl_device_id*)malloc(sizeof(cl_device_id) * num_devices);
    err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 1, devices, NULL);
    char dev_name[40];
    err = clGetDeviceInfo(devices[0], CL_DEVICE_NAME, sizeof(dev_name), &dev_name, NULL);
    printf("dev name: %s \n", dev_name);
    cl_device_id dev = devices[0];
    free(devices);
    free(platforms);
    cl_context context = clCreateContext(NULL, 1, &dev, NULL, NULL, &err);

    FILE* program_handle = NULL;
	char* program_buffer = NULL;
    char* program_log = NULL;
	size_t program_size;
	size_t log_size;
    program_handle = fopen("/data/fun.cl", "r");
    if (program_handle == NULL)
    {
        printf("error:%d %s \n", errno, strerror(errno));
        return 0;
    }
    fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle);
    rewind(program_handle);
    program_buffer = (char*)malloc(program_size + 1);
    program_buffer[program_size] = '\0';
    fread(program_buffer, sizeof(char), program_size, program_handle);
    fclose(program_handle);
    cl_program program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer, (const size_t*)&program_size, &err);
    clBuildProgram(program, 1, &dev, NULL, NULL, NULL);
    clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
    program_log = (char*) malloc(log_size + 1);
    program_log[log_size] = '\0';
	clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG, log_size + 1, program_log, NULL);
	printf("build result: %s\n", program_log);
	free(program_log);

	program_log = NULL;
    clGetProgramInfo(program, CL_PROGRAM_SOURCE, 0, NULL, &log_size);
    program_log = (char*) malloc(log_size + 1);
    program_log[log_size] = '\0';
	clGetProgramInfo(program, CL_PROGRAM_SOURCE, log_size + 1, program_log, NULL);
	printf("function result: %s\n", program_log);
	free(program_log);
	program_log = NULL;

    //cl_uint num_kernels;
    //cl_kernel* kernels;
    //clCreateKernelsInProgram(program, 0, NULL, &num_kernels);
    //printf("num of kernels: %d\n", (int)num_kernels);
    //clCreateKernelsInProgram(program, num_kernels, kernels, NULL);

    cl_kernel testfunc = clCreateKernel(program, "tempfunc", &err);
    if (err != CL_SUCCESS)
    {
        printf("error: %d\n", (int)err);
    }
    char* kernel_log = NULL;
    err = clGetKernelInfo(testfunc, CL_KERNEL_FUNCTION_NAME, 0, NULL, &log_size);
    kernel_log = (char*) malloc(log_size + 1);
    kernel_log[log_size] = '\0';
    err = clGetKernelInfo(testfunc, CL_KERNEL_FUNCTION_NAME, log_size, kernel_log, NULL);
    printf("function name: %s size:%d\n", kernel_log, (int)log_size);

    cl_kernel addone = clCreateKernel(program, "addone", &err);

    cl_command_queue queue = clCreateCommandQueue(context, dev, 0, &err);
    err = clEnqueueTask(queue, addone, 0, NULL, NULL);

    float vec[32];
    cl_mem input_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(vec), vec, &err);
    cl_mem output_buff = clCreateBUffer(context, CL_MEM_WRITE_ONLY, sizeof(vec), NULL, &err);

    clSetKernelArg(addone, 0, sizeof(cl_mem), &input_buff);
    clSetKernelArg(addone, 1, sizeof(cl_mem), &output_buff);

    cl_buffer_region region;
    region.size = 10 * sizeof(float);
    region.origin = 10 * sizeof(float);
    cl_mem sub_buffer = clCreateSubBuffer(input_buff, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, CL_BUFFER_CREATE_TYPE_REGION, &region, &err);

    

    clReleaseCommandQueue(queue);

    return 0;
}