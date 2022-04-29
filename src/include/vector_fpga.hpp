#pragma once

#include <vector>
#include <iostream>
#include <CL/sycl.hpp>
#include <sycl/ext/intel/fpga_device_selector.hpp>
namespace sycl = cl::sycl;

namespace fpga {

template <typename T, int N>
double vector_operation(sycl::queue queue, std::vector<T>& A, std::vector<T>& B, std::vector<T>& C) {

    /********************************************************
     *  Data initilzation
     ********************************************************/
    // A
    T* device_A = sycl::malloc_device<T>(N, queue);
    queue.memcpy(device_A, A.data(), N*sizeof(T));

    // B
    T* device_B = sycl::malloc_device<T>(N, queue);
    queue.memcpy(device_B, B.data(), N*sizeof(T));

    // C
    T* device_C = sycl::malloc_device<T>(N, queue);
    queue.memset(device_C, 0, N*sizeof(T));


    /********************************************************
     *  Launching kernel
     ********************************************************/
    auto event = queue.submit([&] (sycl::handler& cgh) {
        cgh.single_task( [=] () {
            #pragma unroll 4
            for (int i=0; i<N; i++) {
                for (int j=0; j<100; j++)
                    device_C[i] += (device_A[i] + device_B[i])%j;
            }
        });
    });


    /********************************************************
     *  Memcpy result
     ********************************************************/
    queue.memcpy(C.data(), device_C, N*sizeof(T));
    queue.wait();

    auto time = 1e-9 * (event.template get_profiling_info<sycl::info::event_profiling::command_end>() -
                        event.template get_profiling_info<sycl::info::event_profiling::command_start>());
    
    return time;
}


}
