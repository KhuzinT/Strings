#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <map>

const uint64_t kAlphabetSize = 10;
const char kBeginOfAlphabet = '0';

using NameT = std::string;
using CodeT = std::string;

class Bohr {
private:
    struct Node {
        std::vector<std::shared_ptr<Node>> next_vertex;

        uint64_t count_string_that_end_at_this_vertex = 0;

        char symbol = 0;
        std::string string_that_end_at_this_vertex;

        explicit Node(const uint64_t& alphabet_size, const char& current_symbol) {
            next_vertex.resize(alphabet_size, nullptr);
            count_string_that_end_at_this_vertex = 0;
            symbol = current_symbol;
        }
    };

    std::shared_ptr<Node> root_;

    uint64_t alphabet_size_ = 0;

    char symbol_of_beginning_of_alphabet_ = 0;

public:
    explicit Bohr(const uint64_t& alphabet_size, const char& symbol_of_beginning_of_alphabet) {
        alphabet_size_ = alphabet_size;
        root_ = std::make_shared<Node>(alphabet_size, '#');
        symbol_of_beginning_of_alphabet_ = symbol_of_beginning_of_alphabet;
    }

    void AddNewString(const std::string& str) {
        std::shared_ptr<Node> current_vertex = root_;

        for (uint64_t iteration = 0; iteration < str.length(); ++iteration) {
            char symbol = str[iteration];
            uint64_t position = symbol - symbol_of_beginning_of_alphabet_;

            if (current_vertex->next_vertex[position] == nullptr) {
                current_vertex->next_vertex[position] = std::make_shared<Node>(alphabet_size_, symbol);
            }

            current_vertex = current_vertex->next_vertex[position];
        }

        current_vertex->string_that_end_at_this_vertex = str;
        current_vertex->count_string_that_end_at_this_vertex += 1;
    }

    bool IsThisStringInBohr(const std::string& str) const {
        std::shared_ptr<Node> current_vertex = root_;

        for (uint64_t iteration = 0; iteration < str.length(); ++iteration) {
            char symbol = str[iteration];
            uint64_t position = symbol - symbol_of_beginning_of_alphabet_;
            current_vertex = current_vertex->next_vertex[position];
            if (current_vertex == nullptr) {
                return false;
            }
        }

        return current_vertex->count_string_that_end_at_this_vertex;
    }

    uint64_t GetCountOfEmployedNumber(const std::shared_ptr<Node>& current_vertex,
                                      std::map<CodeT, uint64_t>& codes_to_count, const int64_t& difference) {
        auto count_of_free_number = static_cast<uint64_t>(std::pow(10, difference));
        if (current_vertex == nullptr) {
            return 0;
        }
        bool is_terminate_vertex = true;
        for (auto& next_vertex : current_vertex->next_vertex) {
            if (next_vertex != nullptr) {
                is_terminate_vertex = false;
            }
        }
        if (is_terminate_vertex) {
            codes_to_count[current_vertex->string_that_end_at_this_vertex] = count_of_free_number;
            return count_of_free_number;
        }

        uint64_t count_of_employed_number = 0;
        for (uint64_t iteration = 0; iteration < current_vertex->next_vertex.size(); ++iteration) {
            auto next_vertex = current_vertex->next_vertex[iteration];
            count_of_employed_number += GetCountOfEmployedNumber(next_vertex, codes_to_count, difference - 1);
        }
        if (current_vertex->count_string_that_end_at_this_vertex == 0) {
            return count_of_employed_number;
        }

        codes_to_count[current_vertex->string_that_end_at_this_vertex] =
            count_of_free_number - count_of_employed_number;
        return count_of_free_number;
    }

    std::map<NameT, uint64_t> GetNumberOfFreeCodes(const std::map<NameT, CodeT>& cities, const uint64_t& max_length) {
        std::map<NameT, uint64_t> answer;

        std::map<CodeT, uint64_t> codes_to_count;
        GetCountOfEmployedNumber(root_, codes_to_count, static_cast<int64_t>(max_length));

        for (auto& city : cities) {
            auto name = city.first;
            auto code = city.second;

            answer[name] = codes_to_count[code];
        }

        return answer;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    uint64_t string_count = 0;
    std::cin >> string_count;

    uint64_t string_length = 0;
    std::cin >> string_length;

    Bohr dictionary(kAlphabetSize, kBeginOfAlphabet);

    std::map<NameT, CodeT> cities;
    for (uint64_t iteration = 0; iteration < string_count; ++iteration) {
        CodeT code;
        std::cin >> code;

        NameT name;
        std::cin >> name;

        cities[name] = code;
        dictionary.AddNewString(code);
    }

    auto codes = dictionary.GetNumberOfFreeCodes(cities, string_length);
    for (auto& info : codes) {
        std::cout << info.first << ' ' << info.second << '\n';
    }

    return 0;
}
