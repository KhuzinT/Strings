#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>

class SuffixArray {
private:
    std::string str_;

    std::vector<int64_t> array_;
    std::vector<int64_t> color_;

    int64_t max_symbol_ = 0;

    struct Prefix {
        int64_t begin = 0;
        int64_t end = 0;

        int64_t number = 0;

        explicit Prefix() {
            begin = 0;
            end = 0;
            number = 0;
        }

        explicit Prefix(const int64_t& start, const int64_t& finish, const int64_t& count) {
            begin = start;
            end = finish;
            number = count;
        }

        bool operator==(const Prefix& other) const {
            return begin == other.begin && end == other.end;
        }

        bool operator!=(const Prefix& other) const {
            return !(*this == other);
        }
    };

    void InitializationSuffixArray() {
        bool first_time = true;
        for (uint64_t position = 0; position < str_.size(); position *= 2) {
            if (!first_time && position == 0) {
                position = 1;
            }
            first_time = false;
            std::vector<Prefix> prefixes;

            for (uint64_t iteration = 0; iteration < str_.size(); ++iteration) {
                uint64_t number = array_[iteration] - position + str_.size();
                number %= str_.size();
                prefixes.emplace_back(color_[number], color_[array_[iteration]], number);
            }

            if (position) {
                prefixes = PrefixSort(prefixes, prefixes.size());
            } else {
                prefixes = PrefixSort(prefixes, max_symbol_ + 1);
            }

            array_[0] = prefixes[0].number;
            color_[array_[0]] = 0;

            for (uint64_t iteration = 1; iteration < str_.size(); ++iteration) {
                array_[iteration] = prefixes[iteration].number;
                color_[array_[iteration]] = color_[array_[iteration - 1]];
                if (prefixes[iteration] != prefixes[iteration - 1]) {
                    ++color_[array_[iteration]];
                }
            }
            int64_t last = array_[array_.size() - 1];
            if (color_[last] == static_cast<int64_t>(str_.length()) - 1) {
                break;
            }
        }
    }

    std::vector<Prefix> PrefixSort(const std::vector<Prefix>& prefixes, const uint64_t& max_size) {
        std::vector<Prefix> sort_prefixes(prefixes.size());
        std::vector<int64_t> tmp(max_size, 0);

        for (uint64_t iteration = 0; iteration < prefixes.size(); ++iteration) {
            tmp[prefixes[iteration].end] += 1;
        }
        for (uint64_t iteration = 1; iteration < max_size; ++iteration) {
            tmp[iteration] += tmp[iteration - 1];
        }

        for (uint64_t iteration = prefixes.size() - 1; iteration > 0; --iteration) {
            int64_t position = tmp[prefixes[iteration].end] - 1;
            sort_prefixes[position] = prefixes[iteration];
        }
        int64_t position = tmp[prefixes[0].end] - 1;
        sort_prefixes[position] = prefixes[0];

        return sort_prefixes;
    }

public:
    explicit SuffixArray(const std::string& str) {
        str_ = str;
        array_.resize(str.size());
        color_.resize(str.size());

        for (uint64_t iteration = 0; iteration < str_.size(); ++iteration) {
            array_[iteration] = static_cast<int64_t>(iteration);
            color_[iteration] = str_[iteration];

            if (color_[iteration] > max_symbol_) {
                max_symbol_ = color_[iteration];
            }
        }

        InitializationSuffixArray();
    }

    std::vector<uint64_t> GetRefrain() {
        InitializationLcp();

        std::stack<LcpInfo> stack;
        stack.push(LcpInfo(-1, 0, 0));

        std::vector<uint64_t> answer;

        uint64_t len = str_.size() - 1;
        uint64_t entry_count = 1;
        int64_t position = 0;

        for (uint64_t iteration = 0; iteration < str_.size() - 1; ++iteration) {
            while (stack.top().length > lcp_array_[iteration]) {
                auto& back = stack.top();
                if (len * entry_count < back.length * (iteration - back.left)) {
                    len = back.length;
                    position = array_[iteration];
                    entry_count = iteration - back.left;
                }
                stack.pop();
            }
            stack.emplace(lcp_array_[iteration], iteration, stack.top().position);
        }

        answer.push_back(len * entry_count);
        for (uint64_t iteration = position; iteration < position + len; ++iteration) {
            answer.push_back(str_[iteration] + 1 - 'a');
        }

        return answer;
    }

private:
    struct LcpInfo {
        int64_t length = 0;
        int64_t position = 0;
        int64_t left = 0;

        explicit LcpInfo() {
            length = 0;
            position = 0;
            left = 0;
        }

        explicit LcpInfo(const int64_t& len, const int64_t& pos, const int64_t& l) {
            length = len;
            position = pos;
            left = l;
        }
    };

    std::vector<int64_t> lcp_array_;
    std::vector<int64_t> positions_;

    void InitializationLcp() {
        lcp_array_.resize(str_.length());
        positions_.resize(str_.length());

        for (int64_t iteration = 0; iteration < static_cast<int64_t>(str_.size()); ++iteration) {
            positions_[array_[iteration]] = iteration;
        }

        int64_t current_lcp = 0;
        for (int64_t iteration = 0; iteration < static_cast<int64_t>(str_.size()); ++iteration) {
            current_lcp = (current_lcp ? current_lcp - 1 : current_lcp);

            if (positions_[iteration] == static_cast<int64_t>(str_.size()) - 1) {
                lcp_array_[positions_[iteration]] = -1;
                current_lcp = 0;
            } else {
                int64_t next_position = array_[positions_[iteration] + 1];
                int64_t max_size = std::max(iteration, next_position);
                max_size += current_lcp;
                while (max_size < static_cast<int64_t>(str_.size())) {
                    if (str_[iteration + current_lcp] != str_[next_position + current_lcp]) {
                        break;
                    }
                    max_size = std::max(iteration, next_position);
                    max_size += current_lcp;

                    ++current_lcp;
                }
                lcp_array_[positions_[iteration]] = current_lcp;
            }
        }
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int64_t sequence_size = 0;
    int64_t range_of_numbers = 0;

    std::cin >> sequence_size >> range_of_numbers;

    std::vector<int64_t> sequence(sequence_size);
    for (int64_t iteration = 0; iteration < sequence_size; ++iteration) {
        std::cin >> sequence[iteration];
    }

    std::string result_str;
    for (int64_t iteration = 0; iteration < sequence_size; ++iteration) {
        result_str += static_cast<char>(sequence[iteration] + 'a' - 1);
    }

    SuffixArray suffix_array(result_str + "$");

    auto refrain = suffix_array.GetRefrain();
    std::cout << refrain[0] << std::endl;
    std::cout << refrain.size() - 1 << std::endl;
    for (uint64_t iteration = 1; iteration < refrain.size(); ++iteration) {
        std::cout << refrain[iteration] << ' ';
    }
    std::cout << std::endl;

    return 0;
}
