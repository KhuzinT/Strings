#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
 
const size_t    SYMBOLS_AMOUNT  = 27;
const char      ALPHABET_OFFSET = 'a';
const char      SEPARATOR       = '$';
const int64_t INVALID_VALUE   = -1;
 
struct Suffix_tree_node
        {
    int64_t left; 
    int64_t right;
    int64_t parent;
    int64_t link;
 
    int64_t new_number;

    std::vector<int64_t> to;
 
    Suffix_tree_node(int64_t par_left, int64_t par_right, int64_t par_parent)
    {
        parent = par_parent;
        link = INVALID_VALUE;
 
        left = par_left;
        right = par_right;
 
        new_number = INVALID_VALUE;
 
        for (int i = 0; i < SYMBOLS_AMOUNT; ++i)
        {
            to.push_back(INVALID_VALUE);
        }
    }
 
    Suffix_tree_node()
    {
        parent = INVALID_VALUE;
        link   = INVALID_VALUE;
 
        left = 0;
        right = 0;
 
        new_number = INVALID_VALUE;
 
        for (int i = 0; i < SYMBOLS_AMOUNT; ++i)
        {
            to.push_back(INVALID_VALUE);
        }
    }
 
    int64_t length() const
    {
        return right - left;
    }
        };
 
struct Current_state
        {
    int64_t vertex;
    int64_t position;
        };
 
bool is_vertex(const std::vector<Suffix_tree_node> &tree, const Current_state &cur_state);
Current_state go(std::vector<Suffix_tree_node> &tree, const std::vector<char> &string, Current_state cur_state, int64_t par_left, int64_t par_right);
int64_t split(std::vector<Suffix_tree_node> &tree, const std::vector<char> &string, Current_state cur_state);
int64_t get_link (std::vector<Suffix_tree_node> &tree, const std::vector<char> &string, int64_t vertex);
void suffix_tree_add(std::vector<Suffix_tree_node> &tree, const std::vector<char> &string, Current_state &state, int64_t position, int64_t& last);
 
void renumber(std::vector<Suffix_tree_node> &tree, int64_t vertex, int64_t *number);
void print_tree(std::vector<Suffix_tree_node> &tree, int64_t vertex);
 
int main()
{
    std::string str;
    std::cin >> str;

    std::vector<char> string;
    for (auto &symbol : str)
    {
        string.push_back(symbol - ALPHABET_OFFSET + 1);
    }
    string.push_back(0);
    int64_t string_size = string.size();
 
    std::vector<Suffix_tree_node> tree;
    Current_state state = { 0, 0 };
 
    tree.push_back(Suffix_tree_node());
    int64_t last = 0;
    int64_t count = 0;
    for (int64_t i = 0; i < string_size; ++i)
    {
        suffix_tree_add(tree, string, state, i, last);
        auto suffix_link = tree[tree[last].link];
        count += (tree[last].length() - suffix_link.length() + 1);
        std::cout << count << '\n';
    }

    return 0;
}
 
bool is_vertex(const std::vector<Suffix_tree_node> &tree, const Current_state &cur_state)
{
    if (cur_state.position == tree[cur_state.vertex].length())
        return true;
    else 
        return false;
}
 
Current_state go(std::vector<Suffix_tree_node> &tree, const std::vector<char> &string, Current_state cur_state, int64_t par_left, int64_t par_right) 
{
    while (par_left < par_right)
    {
        if (is_vertex(tree, cur_state)) 
        {
            cur_state = { tree[cur_state.vertex].to[string[par_left]], 0 };
 
            if (cur_state.vertex == INVALID_VALUE)
                return cur_state;
        }
        else 
        {
            if (string[tree[cur_state.vertex].left + cur_state.position] != string[par_left])
            {
                return { INVALID_VALUE, INVALID_VALUE };
            }
 
            if (par_right - par_left < tree[cur_state.vertex].length() - cur_state.position)
                return { cur_state.vertex, cur_state.position + par_right - par_left };
 
            par_left = par_left + tree[cur_state.vertex].length() - cur_state.position;
            cur_state.position = tree[cur_state.vertex].length();
        }
    }
    return cur_state;
}
 
int64_t split(std::vector<Suffix_tree_node> &tree, const std::vector<char> &string, Current_state cur_state) 
{
    if (is_vertex(tree, cur_state))
    {
        return cur_state.vertex;
    }
 
    if (cur_state.position == 0)
        return tree[cur_state.vertex].parent;
 
    Suffix_tree_node vertex = tree[cur_state.vertex];
 
    int64_t node_number = tree.size();
    tree.push_back(Suffix_tree_node(vertex.left, vertex.left + cur_state.position, vertex.parent));
 
    tree[vertex.parent].to[string[vertex.left]] = node_number;
 
    tree[node_number].to[string[vertex.left + cur_state.position]] = cur_state.vertex;
    tree[cur_state.vertex].parent = node_number;
    tree[cur_state.vertex].left += cur_state.position;
 
    return node_number;
}
 
int64_t get_link(std::vector<Suffix_tree_node> &tree, const std::vector<char> &string, int64_t vertex) 
{
    if (tree[vertex].link != INVALID_VALUE)  
        return tree[vertex].link;
 
    if (tree[vertex].parent == INVALID_VALUE)
        return 0;
 
    int64_t last = get_link(tree, string, tree[vertex].parent);
    int64_t left = tree[vertex].left;
    if (tree[vertex].parent == 0)
        left++;
 
    Current_state state = go(tree, string, { last, tree[last].length() }, tree[vertex].left + (tree[vertex].parent == 0), tree[vertex].right);
 
    int64_t result = split(tree, string, state);
 
    tree[vertex].link = result;
    return tree[vertex].link;
}
 
void suffix_tree_add(std::vector<Suffix_tree_node> &tree, const std::vector<char> &string, Current_state &state, int64_t position, int64_t& last)
{
    while (true)
    {
        Current_state new_state = go(tree, string, state, position, position + 1);
 
        if (new_state.vertex != INVALID_VALUE) 
        {
            state = new_state;
            return;
        }
 
        int64_t middle = split(tree, string, state);
        int64_t leaf = tree.size();
 
        tree.push_back(Suffix_tree_node(position, string.size(), middle));
        tree[middle].to[string[position]] = leaf;
 
        state.vertex   = get_link(tree, string, middle);
        state.position = tree[state.vertex].length();
 
        if (!middle)  {
            break;
        }
    }
    last = state.vertex + state.position;
}
