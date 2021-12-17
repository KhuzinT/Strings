#include <iostream>
#include <vector>
#include <string>

const uint64_t kInfinity = 1000 * 1000 * 1000 * 1LL;

class SuffixTree {
public:
    struct State {
        uint64_t vertex;
        uint64_t position;

        explicit State() {
            vertex = kInfinity;
            position = kInfinity;
        }

        explicit State(const uint64_t& current_vertex, const uint64_t& current_position) {
            vertex = current_vertex;
            position = current_position;
        }
    };

    struct Node {
        uint64_t begin = 0;
        uint64_t end = 0;

        uint64_t parent = 0;
        uint64_t suffix_link = 0;

        uint64_t new_number;
        std::vector<uint64_t> next_vertex;

        explicit Node(const uint64_t& left, const uint64_t& right, const uint64_t& prev) {
            parent = prev;
            suffix_link = kInfinity;

            begin = left;
            end = right;

            new_number = kInfinity;

            next_vertex.resize(27, kInfinity);
        }

        explicit Node() {
            parent = kInfinity;
            suffix_link = kInfinity;

            begin = 0;
            end = 0;

            new_number = kInfinity;

            next_vertex.resize(27, kInfinity);
        }

        uint64_t GetLength() const {
            return end - begin;
        }
    };

    std::vector<Node> tree_;

    std::vector<uint64_t> str_;
    
    State root_;
    
    bool IsVertex(const State& current_state) const {
        return current_state.position == tree_[current_state.vertex].GetLength();
    }
    
    State Go(const State& state, const uint64_t& begin, const uint64_t& end) {
        uint64_t left = begin;
        uint64_t right = end;
        State current_state = state;
        
        while (left < right) {
            if (IsVertex(current_state)) {
                State new_state(tree_[current_state.vertex].next_vertex[str_[left]], 0);
                current_state = new_state;
 
                if (current_state.vertex == kInfinity) {
                    return current_state;
                }
            } else {
                if (str_[tree_[current_state.vertex].begin + current_state.position] != str_[left]) {
                    State new_state(kInfinity, kInfinity);
                    return new_state;
                }
 
                if (right - left < tree_[current_state.vertex].GetLength() - current_state.position) {
                    State new_state(current_state.vertex, current_state.position + right - left);
                    return new_state;
                }
 
                left += (tree_[current_state.vertex].GetLength() - current_state.position);
                current_state.position = tree_[current_state.vertex].GetLength();
            }
        }
        return current_state;
    }

    uint64_t Split(const State& current_state) {
        if (IsVertex(current_state)) {
            return current_state.vertex;
        }

        if (current_state.position == 0) {
            return tree_[current_state.vertex].parent;
        }

        Node new_vertex = tree_[current_state.vertex];

        uint64_t tree_size = tree_.size();
        tree_.emplace_back(new_vertex.begin, new_vertex.begin + current_state.position, new_vertex.parent);

        tree_[new_vertex.parent].next_vertex[str_[new_vertex.begin]] = tree_size;

        tree_[tree_size].next_vertex[str_[new_vertex.begin + current_state.position]] = current_state.vertex;
        tree_[current_state.vertex].parent = tree_size;
        tree_[current_state.vertex].begin += current_state.position;

        return tree_size;
    }

    uint64_t GetSuffixLink(const uint64_t& current_vertex) {
        if (tree_[current_vertex].suffix_link != kInfinity) {
            return tree_[current_vertex].suffix_link;
        }

        if (tree_[current_vertex].parent == kInfinity) {
            return 0;
        }

        uint64_t parent_link = GetSuffixLink(tree_[current_vertex].parent);
        uint64_t current_begin = tree_[current_vertex].begin;

        if (tree_[current_vertex].parent == 0) {
            ++current_begin;
        }

        State tmp_state(parent_link, tree_[parent_link].GetLength());
        uint64_t left = tree_[current_vertex].begin;
        if (tree_[current_vertex].parent == 0) {
            ++left;
        }
        uint64_t right = tree_[current_vertex].end;
        auto new_state = Go(tmp_state, left, right);

        auto result = Split(new_state);

        tree_[current_vertex].suffix_link = result;
        return tree_[current_vertex].suffix_link;
    }

    void AddSymbol(const uint64_t& current_position) {
        while (true) {
            State new_state = Go(root_, current_position, current_position + 1);

            if (new_state.vertex != kInfinity) {
                root_ = new_state;
                return;
            }

            uint64_t mid = Split(root_);
            uint64_t leaf = tree_.size();

            tree_.emplace_back(current_position, str_.size(), mid);
            tree_[mid].next_vertex[str_[current_position]] = leaf;

            root_.vertex = GetSuffixLink(mid);
            root_.position = tree_[root_.vertex].GetLength();

            if (mid == 0) {
                break;
            }
        }
    }

    void Renumber(const uint64_t& current_vertex, uint64_t& current_number) {
        tree_[current_vertex].new_number = current_number;
        ++current_number;

        for (uint64_t iteration = 0; iteration < 27; ++iteration) {
            uint64_t next_vertex = tree_[current_vertex].next_vertex[iteration];
            if (next_vertex != kInfinity) {
                Renumber(next_vertex, current_number);
            }
        }
    }

    void PrintTree() {
        std::cout << tree_.size() << '\n';

        uint64_t current_number = 0;
        Renumber(0, current_number);
        PrintSuffixTree(0);
    }

    void PrintSuffixTree(const uint64_t& current_vertex) {
        for (uint64_t iteration = 0; iteration < 27; ++iteration) {
            uint64_t next_vertex = tree_[current_vertex].next_vertex[iteration];
            if (next_vertex != kInfinity) {
                uint64_t first = tree_[tree_[next_vertex].parent].new_number;
                uint64_t second = 0;
                uint64_t third = tree_[next_vertex].begin;
                uint64_t fourth = tree_[next_vertex].end;
                std::cout << first << ' ' << second << ' ' << third << ' ' << fourth << '\n';

                PrintSuffixTree(next_vertex);
            }
        }
    }

    explicit SuffixTree(std::vector<uint64_t> string) {
        str_ = string;
        root_ = State(0, 0);

        tree_.emplace_back();
        for (uint64_t iteration = 0; iteration < string.size(); ++iteration) {
            AddSymbol(iteration);
        }

    }

};

int main() {
    std::string first_str;
    std::string second_str;

    std::cin >> first_str;
    std::cin >> second_str;

    std::vector<uint64_t> first;
    std::vector<uint64_t> second;
    for (uint64_t iteration = 0; iteration < first_str.length() - 1; ++iteration) {
        first.push_back(first_str[iteration] - 'a' + 1);
    }
    first.push_back(0);
    for (uint64_t iteration = 0; iteration < second_str.length() - 1; ++iteration) {
        second.push_back(second_str[iteration] - 'a' + 1);
    }
    second.push_back(0);

    SuffixTree first_tree(first);
    SuffixTree second_tree(second);

    first_tree.PrintTree();
    second_tree.PrintTree();

    return 0;
}
