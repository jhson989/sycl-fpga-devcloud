#include <vector>
#include <iostream>
#include <algorithm>
#include <CL/sycl.hpp>
#include <sycl/ext/intel/fpga_device_selector.hpp>
namespace sycl = cl::sycl;

#include "include/vector_fpga.hpp"

void print_properties(sycl::queue& queue);
#define DTYPE int
const size_t N=4*1e+7;



int main(void) {


    std::cout << "=================================================\n";
    std::cout << "SYCL for FPGA : Single_task example\n";
    std::cout << "-- "<<N<<"D vectors point-wise arithmetic operation example\n";
    std::cout << "-- total size of 3 "<<N<<"D vectors: (C=A+B) "<<1e-9*sizeof(DTYPE)*(3*N)<<" GB\n";
    std::cout << "=================================================\n\n";

    std::cout << std::fixed;
    std::cout.precision(3);
    sycl::property_list properties{sycl::property::queue::enable_profiling()};

    /**********************************************************
     * Accelerator Setup
     **********************************************************/

#ifdef FPGA_EMU
    // FPGA emulator
    sycl::ext::intel::fpga_emulator_selector fpga;
    sycl::queue fpga_q(fpga, properties);
    print_properties(fpga_q);
#else
    sycl::ext::intel::fpga_selector fpga;
    sycl::queue fpga_q(fpga, properties);
    print_properties(fpga_q);
#endif

    /**********************************************************
     * Data preparation
     **********************************************************/

    std::vector<DTYPE> A(N);
    std::generate(A.begin(), A.end(), [](){return std::rand()%11-5;});
    std::vector<DTYPE> B(N);
    std::generate(B.begin(), B.end(), [](){return std::rand()%11-5;});
    std::vector<DTYPE> C(N, 0);

    /**********************************************************
     * Launch kernels
     **********************************************************/

    std::cout << "=================================================\n";
    std::cout << "Performance report\n";
    
    // FPGA 
    double time_fpga = fpga::vector_operation<DTYPE, N>(fpga_q, A, B, C);
    std::cout << "-- FPGA : "<<time_fpga<<" s\n";
    return 0;
}









/****************************************************
 * Utility functions
 *****************************************************/

void print_properties(sycl::queue& queue) {

    sycl::device dev = queue.get_device();

    std::cout << "=============== Device Properties ===============" << std::endl;
    std::cout << "Name: " << dev.get_info<sycl::info::device::name>() << std::endl;
    std::cout << "Vendor: " << dev.get_info<sycl::info::device::vendor>() << std::endl;
    std::cout << "Memory size: " << dev.get_info<sycl::info::device::global_mem_size>()/1024.0f/1024.0f/1024.0f << " GB"  << std::endl;
    std::cout << "=================================================" << std::endl << std::endl;
}
