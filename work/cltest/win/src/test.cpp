#define CL_HPP_TARGET_OPENCL_VERSION 200
#include <CL/cl.h>
#include <CL/cl.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <vector>
#include <fstream>
#include <iostream>

void CL_CALLBACK kernelComplete(cl_event e, cl_int status, void* data)
{
    printf("ev: %f\n", ((float*)data));
}

int main()
{
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
    source.open("fun.cl", std::ios::ate);
    std::streampos srcsize = source.tellg();
    char* strmem = new char[srcsize];
    source.seekg(0, std::ios::beg);
    source.read(strmem, srcsize);
    source.close();
    cl::Program prog(c, strmem, true);
    delete [] strmem;
    //printf("prog: %s\n", prog.getBuildInfo<CL_PROGRAM_BUILD_LOG>(ctxDevices)[0].second.c_str());
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
    e.setCallback(CL_COMPLETE, &kernelComplete, (void*)(&vec_ret[0][0]));
    printf("%f\n", vec_ret[0][0]);

//----------------------------------------------
    return 0;
}