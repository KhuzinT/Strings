#include <iostream>
#include <string>
#include <vector>
#include <memory>

const uint32_t kAlphabetSize = 26;
const char kBeginOfAlphabet = 'a';

class Bohr {
private:
    struct Node {
        std::vector<std::shared_ptr<Node>> next_vertex;

        uint32_t count_string_that_end_at_this_vertex = 0;

        char symbol = 0;

        explicit Node(const uint32_t& alphabet_size, const char& current_symbol) {
            next_vertex.resize(alphabet_size, nullptr);
            count_string_that_end_at_this_vertex = 0;
            symbol = current_symbol;
        }
    };

    std::shared_ptr<Node> root_;

    uint32_t alphabet_size_ = 0;

    char symbol_of_beginning_of_alphabet_ = 0;

public:
    explicit Bohr(const uint32_t& alphabet_size, const char& symbol_of_beginning_of_alphabet) {
        alphabet_size_ = alphabet_size;
        root_ = std::make_shared<Node>(alphabet_size, '#');
        symbol_of_beginning_of_alphabet_ = symbol_of_beginning_of_alphabet;
    }

    void AddNewString(const std::string& str) {
        std::shared_ptr<Node> current_vertex = root_;

        for (uint32_t iteration = 0; iteration < str.length(); ++iteration) {
            char symbol = str[iteration];
            uint32_t position = symbol - symbol_of_beginning_of_alphabet_;

            if (current_vertex->next_vertex[position] == nullptr) {
                current_vertex->next_vertex[position] = std::make_shared<Node>(alphabet_size_, symbol);
            }

            current_vertex = current_vertex->next_vertex[position];
        }

        current_vertex->count_string_that_end_at_this_vertex += 1;
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
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string request_type;
    std::string word;

    Bohr dictionary(kAlphabetSize, kBeginOfAlphabet);

    while (true) {
        std::cin >> request_type;
        if (request_type == "#") {
            break;
        }
        if (request_type == "+") {
            std::cin >> word;
            dictionary.AddNewString(word);
        } else if (request_type == "?") {
            std::cin >> word;
            bool is_word_in_bohr = dictionary.IsThisStringInBohr(word);
            if (is_word_in_bohr) {
                std::cout << "YES" << std::endl;
            } else {
                std::cout << "NO" << std::endl;
            }
        } else {
            std::cout << "¯\\_(ツ)_/¯" << std::endl;
            break;
        }
    }

    return 0;
}