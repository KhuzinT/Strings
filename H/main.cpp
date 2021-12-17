#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

const uint64_t kInfinity = 1000 * 1000 * 1000 * 1LL;

std::vector<uint64_t> ZFunction(const std::vector<uint64_t>& str) {
    std::vector<uint64_t> z_block(str.size());
    z_block[0] = str.size();
    uint64_t left = 0;
    uint64_t right = 0;
    for (uint64_t iteration = 1; iteration < str.size(); ++iteration) {
        if (iteration < right) {
            z_block[iteration] = z_block[iteration - left];
            if (right - iteration < z_block[iteration]) {
                z_block[iteration] = right - iteration;
            }
        }
        while (iteration + z_block[iteration] < str.size() &&
               str[z_block[iteration]] == str[iteration + z_block[iteration]]) {
            ++z_block[iteration];
        }
        if (z_block[iteration] + iteration > right) {
            left = iteration;
            right = iteration + z_block[iteration];
        }
    }

    return z_block;
}

std::vector<uint64_t> GetMirror(const std::vector<uint64_t>& cubes) {
    auto reverse_cubes = cubes;
    std::reverse(reverse_cubes.begin(), reverse_cubes.end());

    std::vector<uint64_t> new_cubes(cubes.size() + reverse_cubes.size() + 1);
    for (uint64_t iteration = 0; iteration < new_cubes.size(); ++iteration) {
        if (iteration < cubes.size()) {
            new_cubes[iteration] = cubes[iteration];
        } else if (iteration == cubes.size()) {
            new_cubes[iteration] = kInfinity;
        } else {
            new_cubes[iteration] = reverse_cubes[iteration - reverse_cubes.size() - 1];
        }
    }
    auto z_block = ZFunction(new_cubes);

    std::vector<uint64_t> z_block_for_reverse_cubes;
    for (uint64_t iteration = cubes.size() + 1; iteration < z_block.size(); ++iteration) {
        z_block_for_reverse_cubes.push_back(z_block[iteration]);
    }

    std::vector<uint64_t> mirror;
    for (uint64_t iteration = 0; iteration < z_block_for_reverse_cubes.size(); ++iteration) {
        if (z_block_for_reverse_cubes[iteration] % 2 == 0) {
            if (z_block_for_reverse_cubes[iteration] == z_block_for_reverse_cubes.size() - iteration) {
                uint64_t count = cubes.size() - z_block_for_reverse_cubes[iteration] / 2;
                mirror.push_back(count);
            }
        }
    }
    mirror.push_back(cubes.size());

    return mirror;
}

int main() {
    int64_t count_cubes = 0;
    int64_t alphabet_size = 0;
    std::cin >> count_cubes >> alphabet_size;

    std::vector<uint64_t> cubes(count_cubes);
    for (int64_t iteration = 0; iteration < count_cubes; ++iteration) {
        std::cin >> cubes[iteration];
    }

    auto mirror = GetMirror(cubes);
    for (auto& elem : mirror) {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;
    return 0;
}
