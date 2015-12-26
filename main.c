#include <stdlib.h>
#include <CL/cl.h>
#include <stdio.h>
#define MAX_SOURCE_SIZE (0x100000)
int main()
{
    cl_platform_id platform_id = NULL;
    cl_uint ret_num_platforms;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_context context = NULL;
    cl_command_queue command_queue = NULL;
    cl_mem memobjOut = NULL;
    cl_program program = NULL;
    cl_kernel kernel = NULL;
    size_t kernel_code_size;
    char *kernel_src_str;
    float *result;
    cl_int ret;
    FILE *fp;
    float tall = 0;
    float weight = 0;
    printf("How tall are you? (cm)\n");
    scanf("%f",&tall);
    printf("How weight are you? (kg)\n");
    scanf("%f",&weight);
    kernel_src_str = (char *)malloc(MAX_SOURCE_SIZE);
    result = (float *)malloc(sizeof(float));
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
    context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
    fp = fopen("bmi_cl.cl", "r");
    kernel_code_size = fread(kernel_src_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);
    program = clCreateProgramWithSource(context, 1, (const char **)&kernel_src_str, (const size_t *)&kernel_code_size, &ret);
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "bmi_cl", &ret);
    memobjOut = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float), NULL, &ret);
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobjOut);
    ret = clSetKernelArg(kernel, 1, sizeof(float),    (void *)&tall);
    ret = clSetKernelArg(kernel, 2, sizeof(float),    (void *)&weight);
    ret = clEnqueueTask(command_queue, kernel, 0, NULL, NULL);
    ret = clEnqueueReadBuffer(command_queue, memobjOut, CL_TRUE, 0, sizeof(float), result, 0, NULL, NULL);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(memobjOut);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    printf( "\nYou're BMI is %f\n",*result);
    free(result);
    free(kernel_src_str);
    return 0;
}
