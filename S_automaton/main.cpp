#include <iostream>
#include <string>
#include <vector>

const uint64_t kInfinity = 1000 * 1000 * 1000 * 1LL;

class SuffixTree {
private:
    struct State {
        uint64_t length = 0;
        uint64_t link = 0;
        std::vector<uint64_t> next_vertex;
    };

    uint64_t max_length_ = 0;

    std::vector<State> tree_;

    uint64_t current_length_ = 0;
    uint64_t last_ = 0;

public:
    explicit SuffixTree(const uint64_t& max_length) {
        max_length_ = max_length;
        tree_.resize(2 * max_length_ + 1);

        tree_[0].length = 0;
        tree_[0].link = kInfinity;
        tree_[0].next_vertex.resize(26, kInfinity);

        current_length_ = 1;

        last_ = 0;
    }

    void AddSymbol(const char& symbol) {
        uint64_t position = symbol - 'a';

        current_length_ += 1;
        auto length = current_length_;

        tree_[length].length = tree_[last_].length + 1;
        tree_[length].next_vertex.resize(26, kInfinity);
        auto current_vertex = last_;

        while (current_vertex != kInfinity && tree_[current_vertex].next_vertex[position] == kInfinity) {
            tree_[current_vertex].next_vertex[position] = length;
            current_vertex = tree_[current_vertex].link;
        }

        if (current_vertex == kInfinity) {
            tree_[length].link = 0;
            last_ = length;
            return;
        }

        auto new_vertex = tree_[current_vertex].next_vertex[position];
        if (tree_[current_vertex].length + 1 == tree_[new_vertex].length) {
            tree_[length].link = new_vertex;
            last_ = length;
            return;
        }

        ++current_length_;
        auto copy = current_length_;

        tree_[copy].length = tree_[current_vertex].length + 1;
        tree_[copy].next_vertex = tree_[new_vertex].next_vertex;
        tree_[copy].link = tree_[new_vertex].link;

        while (current_vertex != kInfinity && tree_[current_vertex].next_vertex[position] == new_vertex) {
            tree_[current_vertex].next_vertex[position] = copy;
            current_vertex = tree_[current_vertex].link;
        }

        tree_[new_vertex].link = copy;
        tree_[length].link = copy;

        last_ = length;
    }

    void FindNumberOfDifferentSubstringsForEachPrefixOfString(const std::string& str) {
        uint64_t current_number = 0;
        for (uint64_t iteration = 0; iteration < str.length(); ++iteration) {
            AddSymbol(str[iteration]);
            auto suffix_link = tree_[tree_[last_].link];
            current_number += tree_[last_].length - suffix_link.length;
            printf("%lu\n", current_number);
        }
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::string str;
    std::cin >> str;

    SuffixTree tree(str.length());

    tree.FindNumberOfDifferentSubstringsForEachPrefixOfString(str);

    return 0;
}
