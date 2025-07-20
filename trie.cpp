#include <bits/stdc++.h>
#define TRIE_CHARACTERS 63 // 52 Alphabets + 10 Digits + Whitespace

template <class T>
struct TrieNode;
template <class T>
class Trie;

template <class T>
struct TrieNode
{
    T *value;
    bool isEnd;
    TrieNode<T> *next[TRIE_CHARACTERS]; // A to Z , a to z , 0 to 9 , ' '

    TrieNode()
    {
        this->value = NULL;
        this->isEnd = false;
        for (int i = 0; i < TRIE_CHARACTERS; i++)
            this->next[i] = NULL;
    }
    TrieNode(T *T_object)
    {
        this->value = T_object;
        this->isEnd = false;
        for (int i = 0; i < TRIE_CHARACTERS; i++)
            this->next[i] = NULL;
    }
};

template <class T>
class Trie
{
private:
    TrieNode<T> *root;
    int findIndex(char ch)
    {
        if ('A' <= ch && ch <= 'Z')
            return (int)(ch - 'A'); // 0 to 25
        else if ('a' <= ch && ch <= 'z')
            return (int)(ch - 'a' + 26); // 26 to 51
        else if ('0' <= ch && ch <= '9')
            return (int)(ch - '0' + 52); // 52 to 61
        else if (ch == ' ')
            return 63 - 1; // 62
        else
            return -1;
    }
    bool isEmpty(TrieNode<T> *root)
    {
        for (int i = 0; i < TRIE_CHARACTERS; i++)
        {
            if (root->next[i] != NULL)
            {
                return false;
            }
        }
        return true;
    }
    void dfs(TrieNode<T> *node, std::string &current, std::vector<std::pair<std::string, T *>> &result) const
    {
        if (!node)
            return;

        if (node->isEnd)
            result.push_back({current, node->value});

        for (int i = 0; i < TRIE_CHARACTERS; i++)
        {
            if (node->next[i])
            {
                char ch;
                if (i >= 0 && i <= 25)
                    ch = 'A' + i;
                else if (i >= 26 && i <= 51)
                    ch = 'a' + (i - 26);
                else if (i >= 52 && i <= 61)
                    ch = '0' + (i - 52);
                else if (i == 62)
                    ch = ' ';

                current.push_back(ch);
                dfs(node->next[i], current, result);
                current.pop_back();
            }
        }
    }
    void clear(TrieNode<T> *root)
    {
        for (auto &next : root->next)
            if (next)
                clear(next);
        delete root;
    }

public:
    Trie()
    {
        root = new TrieNode<T>;
    }
    ~Trie()
    {
        clear(root);
    }

    void insert(const std::string key, T *val)
    {
        TrieNode<T> *movingPtr = this->root;
        for (int i = 0; i < key.size(); i++)
        {
            int index = findIndex(key[i]);
            if (movingPtr->next[index] == NULL)
            {
                movingPtr->next[index] = new TrieNode<T>;
            }
            movingPtr = movingPtr->next[index];
        }
        movingPtr->isEnd = true;
        movingPtr->value = val;
    }
    bool search(const std::string key)
    {
        TrieNode<T> *movingPtr = this->root;
        for (int i = 0; i < key.size(); i++)
        {
            int index = findIndex(key[i]);
            if (movingPtr->next[index] == NULL)
                return false;
            movingPtr = movingPtr->next[index];
        }
        return movingPtr->isEnd;
    }
    T *getValue(const std::string key)
    {
        TrieNode<T> *movingPtr = this->root;
        for (char ch : key)
        {
            int index = findIndex(ch);
            if (movingPtr->next[index] == NULL)
                return NULL;
            movingPtr = movingPtr->next[index];
        }

        return movingPtr->isEnd ? movingPtr->value : NULL;
    }
    TrieNode<T> *deleteHelper(TrieNode<T> *root, const std::string key, int depth)
    {
        if (root == NULL)
        {
            return NULL;
        }
        if (depth == key.size())
        {
            if (root->isEnd)
            {
                root->isEnd = false;
            }
            if (isEmpty(root))
            {
                delete root;
                root = NULL;
            }
            return root;
        }
        int index = findIndex(key[depth]);
        root->next[index] = deleteHelper(root->next[index], key, depth + 1);
        if (isEmpty(root) && !root->isEnd)
        {
            delete root;
            root = NULL;
        }
        return root;
    }
    void erase(const std::string key)
    {
        deleteHelper(root, key, 0);
    }
    std::vector<std::pair<std::string, T *>> traverse() const
    {
        std::vector<std::pair<std::string, T *>> result;
        std::string current;
        dfs(root, current, result);
        return result;
    }

    // Mutable version (for insert + access)
    T *&operator[](const std::string &key)
    {
        TrieNode<T> *movingPtr = this->root;
        for (char ch : key)
        {
            int index = findIndex(ch);
            if (movingPtr->next[index] == NULL)
            {
                movingPtr->next[index] = new TrieNode<T>;
            }
            movingPtr = movingPtr->next[index];
        }
        movingPtr->isEnd = true;
        return movingPtr->value;
    }
    // Const version (read-only lookup only)
    T *operator[](const std::string &key) const
    {
        return this->getValue(key);
    }
};