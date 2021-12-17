#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>

using DictionaryT = std::unordered_map<std::string, std::vector<uint32_t>>;
using PatternT = std::vector<std::string>;

const uint32_t kInfinity = 1000 * 1000 * 1000 * 1LL;

class Bohr {
private:
    struct Node {
        std::unordered_map<char, uint32_t> next_vertex;
        std::unordered_map<char, uint32_t> auto_transition;

        uint32_t parent = kInfinity;

        uint32_t suffix_link = kInfinity;
        uint32_t end_suffix_link = kInfinity;

        uint32_t count_string_that_end_at_this_vertex = 0;

        char symbol = 0;

        uint32_t pattern_number = kInfinity;

        explicit Node(const char& current_symbol, const uint32_t& prev) {

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

public:
    explicit Bohr() {
        bohr_.emplace_back('#', kInfinity);
    }

    void AddNewString(const std::string& str) {
        uint32_t current_vertex = 0;

        for (uint32_t iteration = 0; iteration < str.length(); ++iteration) {
            char symbol = str[iteration];

            auto it = bohr_[current_vertex].next_vertex.find(symbol);
            if (it == bohr_[current_vertex].next_vertex.end()) {
                bohr_.emplace_back(symbol, current_vertex);
                bohr_[current_vertex].next_vertex[symbol] = bohr_.size() - 1;
            }

            current_vertex = bohr_[current_vertex].next_vertex[symbol];
        }

        bohr_[current_vertex].count_string_that_end_at_this_vertex += 1;

        pattern_.push_back(str);
        bohr_[current_vertex].pattern_number = pattern_.size() - 1;
    }

    uint32_t GetTransitions(const uint32_t& current_vertex, const char& symbol) {
        // uint32_t position = symbol - symbol_of_beginning_of_alphabet_;
        auto it = bohr_[current_vertex].auto_transition.find(symbol);
        if (it != bohr_[current_vertex].auto_transition.end()) {
            return bohr_[current_vertex].auto_transition[symbol];
        }

        it = bohr_[current_vertex].next_vertex.find(symbol);
        if (it != bohr_[current_vertex].next_vertex.end()) {
            bohr_[current_vertex].auto_transition[symbol] = bohr_[current_vertex].next_vertex[symbol];
        } else if (current_vertex == 0) {
            bohr_[current_vertex].auto_transition[symbol] = 0;
        } else {
            auto link = GetSuffixLink(current_vertex);
            bohr_[current_vertex].auto_transition[symbol] = GetTransitions(link, symbol);
        }

        return bohr_[current_vertex].auto_transition[symbol];
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

struct Segment {
    uint32_t begin = 0;
    uint32_t end = 0;

    explicit Segment(const uint32_t& current_begin, const uint32_t& current_end) {
        begin = current_begin;
        end = current_end;
    }

    bool IsThisNumberInSegment(const uint32_t& number) {
        return begin <= number && number <= end;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::string text;
    std::cin >> text;

    Bohr bohr;

    uint32_t count_of_request = 0;
    std::cin >> count_of_request;

    std::vector<Segment> segments;
    PatternT pattern(count_of_request);
    for (uint32_t iteration = 0; iteration < count_of_request; ++iteration) {
        uint32_t begin = 0;
        uint32_t end = 0;
        std::cin >> begin >> end;
        segments.emplace_back(begin, end);
        std::cin >> pattern[iteration];
    }

    std::string answer;
    auto dictionary = bohr.FindAllOccurrencesOfPatternInText(text, pattern);
    for (uint32_t iteration = 0; iteration < pattern.size(); ++iteration) {
        auto occurrences_vector = dictionary[pattern[iteration]];
        bool is_segment_entry = false;
        for (auto& elem : occurrences_vector) {
            if (segments[iteration].IsThisNumberInSegment(elem) &&
                segments[iteration].IsThisNumberInSegment(elem + pattern[iteration].length() - 1)) {
                is_segment_entry = true;
                break;
            }
        }
        if (is_segment_entry) {
            answer += "+";
        } else {
            answer += "-";
        }
    }

    std::cout << answer << std::endl;

    return 0;
}