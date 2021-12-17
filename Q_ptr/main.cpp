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
        std::vector<std::shared_ptr<Node>> next_vertex;
        std::vector<std::weak_ptr<Node>> auto_transition;

        std::weak_ptr<Node> parent;

        std::weak_ptr<Node> suffix_link;
        std::weak_ptr<Node> end_suffix_link;

        uint32_t count_string_that_end_at_this_vertex = 0;

        char symbol = 0;

        uint32_t pattern_number = kInfinity;

        explicit Node(const uint32_t& alphabet_size, const char& current_symbol, std::shared_ptr<Node> prev) {
            next_vertex.resize(alphabet_size);
            auto_transition.resize(alphabet_size);

            parent = prev;

            count_string_that_end_at_this_vertex = 0;
            symbol = current_symbol;

            pattern_number = kInfinity;
        }
    };

    std::shared_ptr<Node> root_;

    PatternT pattern_;

    uint32_t alphabet_size_ = 0;

    char symbol_of_beginning_of_alphabet_ = 0;

public:
    explicit Bohr(const uint32_t& alphabet_size, const char& symbol_of_beginning_of_alphabet) {
        alphabet_size_ = alphabet_size;
        root_ = std::make_shared<Node>(alphabet_size, '#', nullptr);
        symbol_of_beginning_of_alphabet_ = symbol_of_beginning_of_alphabet;
    }

    void AddNewString(const std::string& str) {
        std::shared_ptr<Node> current_vertex = root_;

        for (uint32_t iteration = 0; iteration < str.length(); ++iteration) {
            char symbol = str[iteration];
            uint32_t position = symbol - symbol_of_beginning_of_alphabet_;

            if (current_vertex->next_vertex[position] == nullptr) {
                current_vertex->next_vertex[position] = std::make_shared<Node>(alphabet_size_, symbol, current_vertex);
            }
            current_vertex = current_vertex->next_vertex[position];
        }

        current_vertex->count_string_that_end_at_this_vertex += 1;

        pattern_.push_back(str);
        current_vertex->pattern_number = pattern_.size() - 1;
    }

    bool IsThisStringInBohr(const std::string& str) const {
        std::shared_ptr<Node> current_vertex = root_;

        for (uint32_t iteration = 0; iteration < str.length(); ++iteration) {
            char symbol = str[iteration];
            uint32_t position = symbol - symbol_of_beginning_of_alphabet_;
            current_vertex = current_vertex->next_vertex[position];
            if (current_vertex == nullptr) {
                return false;
            }
        }

        return current_vertex->count_string_that_end_at_this_vertex;
    }

    std::shared_ptr<Node> GetTransitions(std::shared_ptr<Node> current_vertex, const char& symbol) {
        uint32_t position = symbol - symbol_of_beginning_of_alphabet_;
        if (current_vertex->auto_transition[position].lock() != nullptr) {
            return current_vertex->auto_transition[position].lock();
        }

        if (current_vertex->next_vertex[position] != nullptr) {
            current_vertex->auto_transition[position] = current_vertex->next_vertex[position];
        } else if (current_vertex == root_) {
            current_vertex->auto_transition[position] = root_;
        } else {
            auto link = GetSuffixLink(current_vertex);
            current_vertex->auto_transition[position] = GetTransitions(link, symbol);
        }

        return current_vertex->auto_transition[position].lock();
    }

    std::shared_ptr<Node> GetSuffixLink(std::shared_ptr<Node> current_vertex) {
        if (current_vertex->suffix_link.lock() != nullptr) {
            return current_vertex->suffix_link.lock();
        }

        if (current_vertex == root_ || current_vertex->parent.lock() == root_) {
            current_vertex->suffix_link = root_;
            return root_;
        }

        auto parent_link = GetSuffixLink(current_vertex->parent.lock());
        current_vertex->suffix_link = GetTransitions(parent_link, current_vertex->symbol);
        return current_vertex->suffix_link.lock();
    }

    std::shared_ptr<Node> GetEndSuffixLink(std::shared_ptr<Node> current_vertex) {
        if (current_vertex->end_suffix_link.lock() != nullptr) {
            return current_vertex->end_suffix_link.lock();
        }
        auto link = GetSuffixLink(current_vertex);
        if (link == root_) {
            current_vertex->end_suffix_link = root_;
            return root_;
        }

        if (link->count_string_that_end_at_this_vertex > 0) {
            current_vertex->end_suffix_link = link;
            return link;
        }

        current_vertex->end_suffix_link = GetEndSuffixLink(link);
        return current_vertex->end_suffix_link.lock();
    }

    void CheckEndSuffixLink(std::shared_ptr<Node> current_vertex, const uint32_t& iteration, DictionaryT& dictionary) {
        for (auto new_vertex = current_vertex; new_vertex != root_; new_vertex = GetEndSuffixLink(new_vertex)) {
            if (new_vertex->count_string_that_end_at_this_vertex > 0) {
                auto current_string = pattern_[new_vertex->pattern_number];
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

        auto current_vertex = root_;
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
