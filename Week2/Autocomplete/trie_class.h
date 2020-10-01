#ifndef TRIE_CLASS_H
#define TRIE_CLASS_H

#include <unordered_map>
#include <map>
#include <set>
#include <memory>
#include <vector>

struct TrieNode {
  std::unordered_map<char,std::unique_ptr<TrieNode>> children;
  std::string word;
};

class Trie {
 public:
  Trie();
  void addWord(const std::string& word);
  std::vector<std::string> findSmallestWords(std::string& prefix);
 
 private:
  TrieNode* searchPrefix(const std::string& prefix);
  void findPrefixWords(const std::string prefix, std::set<std::string>& result);
  void findStrings(TrieNode* tmp, std::set<std::string>& strings, std::string current_string);
  
  std::unique_ptr<TrieNode> root{nullptr};
  std::map<char, std::vector<char>> chars;
  const int result_limit = 10;
};

#endif // TRIE_CLASS_H