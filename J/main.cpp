#include <iostream>
#include <string>
#include <vector>
#include <set>

std::string GetMinStringFromPrefix(const std::vector<uint32_t>& prefix) {
    std::string str;
    for (uint32_t iteration = 0; iteration < prefix.size(); ++iteration) {
        str += "a";
    }
    str[0] = 'a';
    for (uint32_t iteration = 1; iteration < prefix.size(); ++iteration) {
        if (prefix[iteration] != 0) {
            str[iteration] = str[prefix[iteration] - 1];
            continue;
        }
        std::set<char> symbols_that_cannot_be_used;
        uint32_t current_prefix = prefix[iteration - 1];
        while (current_prefix > 0) {
            symbols_that_cannot_be_used.insert(str[current_prefix]);
            current_prefix = prefix[current_prefix - 1];
        }
        for (char current_symbol = 'b'; current_symbol <= 'z'; ++current_symbol) {
            if (symbols_that_cannot_be_used.find(current_symbol) == symbols_that_cannot_be_used.end()) {
                str[iteration] = current_symbol;
                break;
            }
        }
    }
    return str;
}

std::vector<uint32_t> GetPrefixFromZBlock(const std::vector<uint32_t>& z_block) {
    std::vector<uint32_t> prefix(z_block.size(), 0);
    for (uint32_t iteration = 1; iteration < z_block.size(); ++iteration) {
        if (z_block[iteration] != 0) {
            for (uint32_t delta = z_block[iteration] - 1; delta > 0 && prefix[iteration + delta] == 0; --delta) {
                prefix[iteration + delta] = delta + 1;
            }
            if (prefix[iteration] == 0) {
                prefix[iteration] = 1;
            }
        }
    }

    return prefix;
}

std::string GetMinStringFromZBlock(const std::vector<uint32_t>& z_block) {
    auto prefix = GetPrefixFromZBlock(z_block);
    return GetMinStringFromPrefix(prefix);
}

int main() {
    uint32_t string_size = 0;
    std::cin >> string_size;

    std::vector<uint32_t> z_block(string_size, 0);
    for (uint32_t iteration = 0; iteration < string_size; ++iteration) {
        std::cin >> z_block[iteration];
    }

    std::cout << GetMinStringFromZBlock(z_block) << std::endl;
    return 0;
}
