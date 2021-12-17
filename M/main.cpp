#include <algorithm>
#include <iostream>
#include <climits>
#include <memory>
#include <string>
#include <vector>
#include <map>

class Trie {
public:
    explicit Trie(const char first, const size_t size_of_abc) {
        root_ = std::make_shared<TrieNode>('#', size_of_abc);
        k_code_of_begining_ = first;
        k_size_of_abc_ = size_of_abc;
    }

    bool FindStr(const std::string& str) const {
        std::shared_ptr<TrieNode> current_node = root_;

        for (auto symbol : str) {
            size_t symbol_code = symbol - k_code_of_begining_;

            current_node = current_node->next_symbols_[symbol];

            if (current_node == nullptr) {
                return false;
            }
        }

        return current_node->is_term_;
    }

    void InsertStr(const std::string& str) {
        std::shared_ptr<TrieNode> current_node = root_;

        for (auto symbol : str) {
            size_t symbol_code = symbol - k_code_of_begining_;

            if (current_node->next_symbols_[symbol] == nullptr) {
                current_node->next_symbols_[symbol] = std::make_shared<TrieNode>(symbol, k_size_of_abc_);
            }

            current_node = current_node->next_symbols_[symbol];
            current_node->elements_in_++;
        }

        current_node->is_term_ = true;
    }

    size_t CountSubstr(const std::string& str) const {
        std::shared_ptr<TrieNode> current_node = root_;

        for (auto symbol : str) {
            size_t symbol_code = symbol - k_code_of_begining_;

            current_node = current_node->next_symbols_[symbol];

            if (current_node == nullptr) {
                return 0;
            }
        }

        return current_node->elements_in_;
    }

private:
    struct TrieNode {
        std::map<char, std::shared_ptr<TrieNode>> next_symbols_;
        char value_;
        size_t elements_in_;
        bool is_term_;

        explicit TrieNode(size_t size_of_abc, char value = '#') {
            value_ = value;
            elements_in_ = 0;
            is_term_ = false;
        }
    };

    std::shared_ptr<TrieNode> root_;
    char k_code_of_begining_ = 0;
    size_t k_size_of_abc_ = 0;
};

size_t CountMinCost(const std::vector<size_t>& costs, const Trie& trie, const size_t len_num, const size_t len_abc,
                    const char first, const std::string& str, std::string& min_cost_str) {
    size_t min_cost = INT_MAX;
    size_t current_len = str.length();
    char current_letter = first;
    // char best_letter = 0;
    std::string best_word;

    for (size_t i = 0; i < len_abc; ++i) {
        std::string str_to_check = str + current_letter;
        size_t counter = trie.CountSubstr(str_to_check);

        if (counter == 0) {
            min_cost_str = str_to_check;
            if (min_cost_str.length() > len_num) {
                min_cost_str = min_cost_str.substr(0, len_num);
            }

            for (size_t j = current_len + 1; j < len_num; ++j) {
                min_cost_str += current_letter;
            }

            // std::cout << "update best_word for " << str_to_check << " to " << best_word << std::endl;

            // best_letter = current_letter;
            min_cost = 0;
            return 0;
        }

        size_t delta = 0;
        if (current_len > 0) {
            delta = counter * costs[current_len - 1];
        }

        size_t sons_min_cost = CountMinCost(costs, trie, len_num, len_abc, first, str_to_check, best_word);
        if (counter * costs[current_len] - delta + sons_min_cost < min_cost) {
            min_cost = counter * costs[current_len] - delta + sons_min_cost;
            // best_letter = current_letter;
            min_cost_str = best_word;
            // std::cout << "Update min_cost for " << str_to_check << " to " << min_cost << std::endl;
        }

        ++current_letter;
    }

    return min_cost;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    size_t members = 0;
    size_t len_num = 0;
    size_t len_abc = 0;

    std::cin >> members >> len_num >> len_abc;

    std::vector<size_t> costs;

    for (size_t i = 0; i < len_num; ++i) {
        size_t cost = 0;
        std::cin >> cost;

        costs.push_back(cost);
    }

    Trie trie('0', len_abc);

    for (size_t i = 0; i < members; ++i) {
        std::string ticket;
        std::cin >> ticket;

        trie.InsertStr(ticket);
    }

    std::string min_cost_str;
    std::string empty_str;
    size_t min_cost = CountMinCost(costs, trie, len_num, len_abc, '0', empty_str, min_cost_str);
    std::cout << min_cost_str << std::endl;
    std::cout << min_cost << std::endl;

    return 0;
}