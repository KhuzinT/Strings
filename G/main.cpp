#include <iostream>
#include <string>
#include <vector>

std::vector<uint32_t> ZFunction(const std::string& str) {
    std::vector<uint32_t> z_block(str.length());
    z_block[0] = str.length();
    uint32_t left = 0;
    uint32_t right = 0;
    for (uint32_t iteration = 1; iteration < str.size(); ++iteration) {
        if (iteration <= right) {
            z_block[iteration] = z_block[iteration - left];
            if (right - iteration + 1 < z_block[iteration]) {
                z_block[iteration] = right - iteration + 1;
            }
        }
        while (iteration + z_block[iteration] < str.length() &&
               str[z_block[iteration]] == str[iteration + z_block[iteration]]) {
            ++z_block[iteration];
        }
        if (z_block[iteration] + iteration - 1 > right) {
            left = iteration;
            right = iteration + z_block[iteration] - 1;
        }
    }

    return z_block;
}

int main() {
    std::string str;
    std::cin >> str;

    auto z_block = ZFunction(str);
    for (auto& elem : z_block) {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;
}
