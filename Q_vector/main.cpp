#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>

using DictionaryT = std::unordered_map<std::string, std::vector<uint32_t>>;
using PatternT = std::vector<std::string>;

const uint32_t kInfinity = 1000 * 1000 * 1000 * 1LL;

const char kAlphabetSize = 26;
const char kBeginOfAlphabet = 'a';

class Bohr {
private:
    struct Node {
        std::vector<uint32_t> next_vertex;
        std::vector<uint32_t> auto_transition;

        uint32_t parent = kInfinity;

        uint32_t suffix_link = kInfinity;
        uint32_t end_suffix_link = kInfinity;

        uint32_t count_string_that_end_at_this_vertex = 0;

        char symbol = 0;

        uint32_t pattern_number = kInfinity;

        explicit Node(const uint32_t& alphabet_size, const char& current_symbol, const uint32_t& prev) {
            next_vertex.resize(alphabet_size, kInfinity);
            auto_transition.resize(alphabet_size, kInfinity);

            parent = prev;

            suffix_link = kInfinity;
            end_suffix_link = kInfinity;

            count_string_that_end_at_this_vertex = 0;
            symbol = current_symbol;

            pattern_number = kInfinity;
        }
    };

    std::vector<Node> bohr_;

    PatternT pattern_;

    uint32_t alphabet_size_ = 0;

    char symbol_of_beginning_of_alphabet_ = 0;

public:
    explicit Bohr(const uint32_t& alphabet_size, const char& symbol_of_beginning_of_alphabet) {
        alphabet_size_ = alphabet_size;
        bohr_.emplace_back(alphabet_size_, '#', kInfinity);
        symbol_of_beginning_of_alphabet_ = symbol_of_beginning_of_alphabet;
    }

    void AddNewString(const std::string& str) {
        uint32_t current_vertex = 0;

        for (uint32_t iteration = 0; iteration < str.length(); ++iteration) {
            char symbol = str[iteration];
            uint32_t position = symbol - symbol_of_beginning_of_alphabet_;

            if (bohr_[current_vertex].next_vertex[position] == kInfinity) {
                bohr_.emplace_back(alphabet_size_, symbol, current_vertex);
                bohr_[current_vertex].next_vertex[position] = bohr_.size() - 1;
            }

            current_vertex = bohr_[current_vertex].next_vertex[position];
        }

        bohr_[current_vertex].count_string_that_end_at_this_vertex += 1;

        pattern_.push_back(str);
        bohr_[current_vertex].pattern_number = pattern_.size() - 1;
    }

    bool IsThisStringInBohr(const std::string& str) const {
        uint32_t current_vertex = 0;
        for (uint32_t iteration = 0; iteration < str.length(); ++iteration) {
            char symbol = str[iteration];
            uint32_t position = symbol - symbol_of_beginning_of_alphabet_;
            if (bohr_[current_vertex].next_vertex[position] == kInfinity) {
                return false;
            }
            current_vertex = bohr_[current_vertex].next_vertex[position];
        }

        return true;
    }

    uint32_t GetTransitions(const uint32_t& current_vertex, const char& symbol) {
        uint32_t position = symbol - symbol_of_beginning_of_alphabet_;
        if (bohr_[current_vertex].auto_transition[position] != kInfinity) {
            return bohr_[current_vertex].auto_transition[position];
        }

        if (bohr_[current_vertex].next_vertex[position] != kInfinity) {
            bohr_[current_vertex].auto_transition[position] = bohr_[current_vertex].next_vertex[position];
        } else if (current_vertex == 0) {
            bohr_[current_vertex].auto_transition[position] = 0;
        } else {
            auto link = GetSuffixLink(current_vertex);
            bohr_[current_vertex].auto_transition[position] = GetTransitions(link, symbol);
        }

        return bohr_[current_vertex].auto_transition[position];
    }

    uint32_t GetSuffixLink(const uint32_t& current_vertex) {
        if (bohr_[current_vertex].suffix_link != kInfinity) {
            return bohr_[current_vertex].suffix_link;
        }

        if (current_vertex == 0 || bohr_[current_vertex].parent == 0) {
            bohr_[current_vertex].suffix_link = 0;
            return 0;
        }

        auto parent_link = GetSuffixLink(bohr_[current_vertex].parent);
        bohr_[current_vertex].suffix_link = GetTransitions(parent_link, bohr_[current_vertex].symbol);
        return bohr_[current_vertex].suffix_link;
    }

    uint32_t GetEndSuffixLink(const uint32_t& current_vertex) {
        if (bohr_[current_vertex].end_suffix_link != kInfinity) {
            return bohr_[current_vertex].end_suffix_link;
        }
        auto link = GetSuffixLink(current_vertex);
        if (link == 0) {
            bohr_[current_vertex].end_suffix_link = 0;
            return 0;
        }

        if (bohr_[link].count_string_that_end_at_this_vertex > 0) {
            bohr_[current_vertex].end_suffix_link = link;
            return link;
        }

        bohr_[current_vertex].end_suffix_link = GetEndSuffixLink(link);
        return bohr_[current_vertex].end_suffix_link;
    }

    void CheckEndSuffixLink(const uint32_t& current_vertex, const uint32_t& iteration, DictionaryT& dictionary) {
        for (auto new_vertex = current_vertex; new_vertex != 0; new_vertex = GetEndSuffixLink(new_vertex)) {
            if (bohr_[new_vertex].count_string_that_end_at_this_vertex > 0) {
                auto current_string = pattern_[bohr_[new_vertex].pattern_number];
                dictionary[current_string].push_back(iteration - current_string.length() + 1);
            }
        }
    }

    DictionaryT FindAllOccurrencesOfPatternInText(const std::string& text, const PatternT& pattern) {
        for (uint32_t iteration = 0; iteration < pattern.size(); ++iteration) {
            AddNewString(pattern[iteration]);
        }

        DictionaryT dictionary;
        for (uint32_t iteration = 0; iteration < pattern.size(); ++iteration) {
            dictionary[pattern[iteration]] = {};
        }

        uint32_t current_vertex = 0;
        for (uint32_t iteration = 0; iteration < text.length(); ++iteration) {
            current_vertex = GetTransitions(current_vertex, text[iteration]);
            CheckEndSuffixLink(current_vertex, iteration + 1, dictionary);
        }

        return dictionary;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::string text;
    std::cin >> text;

    Bohr bohr(kAlphabetSize, kBeginOfAlphabet);

    uint32_t count_of_request = 0;
    std::cin >> count_of_request;

    PatternT pattern(count_of_request);
    for (uint32_t iteration = 0; iteration < count_of_request; ++iteration) {
        std::cin >> pattern[iteration];
    }

    auto dictionary = bohr.FindAllOccurrencesOfPatternInText(text, pattern);
    for (uint32_t iteration = 0; iteration < pattern.size(); ++iteration) {
        auto output_vector = dictionary[pattern[iteration]];
        std::cout << output_vector.size() << ' ';
        for (auto& elem : output_vector) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }

    return 0;
}
