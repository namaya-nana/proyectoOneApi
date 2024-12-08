// Arquitectura2/src/gpu_practice.cpp
#include <sycl/sycl.hpp>

using namespace sycl;

int main() {
//# Create a device queue with device selector
    try {
        queue q(gpu_selector_v);
        std::cout << "Device: " << q.get_device().get_info<info::device::name>() << "\n";
    } catch (const sycl::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}
