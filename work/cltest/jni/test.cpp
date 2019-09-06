#define CL_HPP_TARGET_OPENCL_VERSION 200
#include <CL/cl.h>
#include <CL/cl2.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <vector>
#include <fstream>
#include <iostream>

void CL_CALLBACK kernelComplete(cl_event e, cl_int status, void* data)
{
    printf("ev: %f\n", ((float*)data)[0]);
}

int main()
{
//-------------- C HEADER ----------------
    /*cl_platform_id* platforms;
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
	//printf("function result: %s\n", program_log);
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

    float vec[32] = {0.0f};
    float vec_ret[32] = {0.0f};
    cl_mem input_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(vec), vec, &err);
    cl_mem output_buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(vec), NULL, &err);

    clSetKernelArg(addone, 0, sizeof(cl_mem), &input_buff);
    clSetKernelArg(addone, 1, sizeof(cl_mem), &output_buff);

    size_t work_size = 32;
    clEnqueueNDRangeKernel(queue, addone, 1, NULL, &work_size, NULL, 0, NULL, NULL);

    cl_event ev;
    err = clEnqueueReadBuffer(queue, output_buff, CL_FALSE, 0, sizeof(vec_ret), vec_ret, 0, NULL, &ev);

    printf("%f\n", vec_ret[0]);

    //cl_buffer_region region;
    //region.size = 10 * sizeof(float);
    //region.origin = 10 * sizeof(float);
    //cl_mem sub_buffer = clCreateSubBuffer(input_buff, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, CL_BUFFER_CREATE_TYPE_REGION, &region, &err);

    //float matrix[80];
    //float rst[80];
    //cl_mem matrix_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(matrix), matrix, &err);
    //const size_t buffer_origin[3] = {5*sizeof(float), 3, 0};
    //const size_t host_origin[3] = {1*sizeof(float), 1, 0};
    //const size_t rst_region[3] = {4*sizeof(float), 4, 1};
    //err = clSetKernelArg(addone, 0, sizeof(cl_mem), &matrix_buffer);
    //err = clEnqueueTask(queue, addone, 0, NULL, NULL);
    //err = clEnqueueWriteBuffer(queue, matrix_buffer, CL_TRUE, 0, sizeof(matrix), matrix, 0, NULL, NULL);
    //err = clEnqueueReadBufferRect(queue, matrix_buffer, CL_TRUE, buffer_origin, host_origin, rst_region, 10*sizeof(float), 0, 10*sizeof(float), 0, rst, 0, NULL, NULL);

    clSetEventCallback(ev, CL_COMPLETE, &kernelComplete, (void*)vec_ret);

    clReleaseEvent(ev);
    clReleaseCommandQueue(queue);*/

//----------------- CPP HEADER ------------------

    float vec[32][4] = {0.0f};
    float vec_ret[32][4] = {0.0f};
    size_t work_size = 32;
    cl_int err;
    std::string info;
    std::vector<cl::Platform> p;
    std::vector<cl::Device> d, ctxDevices;
    cl::Platform::get(&p);
    p[0].getInfo(CL_PLATFORM_NAME, &info);
    printf("%s\n", info.c_str());
    p[0].getDevices(CL_DEVICE_TYPE_ALL, &d);
    cl::Context c(d);
    ctxDevices = c.getInfo<CL_CONTEXT_DEVICES>();
    for (auto &&d : ctxDevices)
    {
        printf("dev: %s\n", d.getInfo<CL_DEVICE_NAME>().c_str());
    }
    std::ifstream source;
    source.open("/data/fun.cl", std::ios::ate);
    std::streampos srcsize = source.tellg();
    char* strmem = new char[srcsize];
    source.seekg(0, std::ios::beg);
    source.read(strmem, srcsize);
    source.close();
    cl::Program prog(c, strmem, true);
    delete [] strmem;
    printf("prog: %s\n", prog.getBuildInfo<CL_PROGRAM_BUILD_LOG>()[0].second.c_str());
    printf("src: %s\n", prog.getInfo<CL_PROGRAM_SOURCE>().c_str());
    cl::Kernel simpleAddKernel(prog, "addone");
    cl::Buffer inbuff(c, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(vec), vec, &err);
    cl::Buffer outbuff(c, CL_MEM_WRITE_ONLY, sizeof(vec), NULL, &err);
    simpleAddKernel.setArg(0, inbuff);
    simpleAddKernel.setArg(1, outbuff);
    cl::Event e;
    cl::CommandQueue q(c, ctxDevices[0]);
    cl::NDRange offset(0, 0);
    cl::NDRange global_size(32, 4);
    cl::NDRange local_size(4, 2);
    q.enqueueNDRangeKernel(simpleAddKernel, offset, global_size, local_size);
    q.enqueueReadBuffer(outbuff, false, 0, sizeof(vec), vec_ret, NULL, &e);
    e.setCallback(CL_COMPLETE, &kernelComplete, (void*)vec_ret);
    printf("%f\n", vec_ret[0][0]);

//----------------------------------------------
    return 0;
}