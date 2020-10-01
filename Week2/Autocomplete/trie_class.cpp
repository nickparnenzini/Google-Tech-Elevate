#include "trie_class.h"
#include <utility>
#include <algorithm>
#include <iostream>

Trie::Trie() {
  root = std::make_unique<TrieNode>();
}

void Trie::addWord(const std::string& word) {
    TrieNode* tmp = root.get();
    for (int i = 0; i < word.length(); i++) { 
      auto it = tmp->children.find(word[i]);
      if (it == tmp->children.end()) {
        std::unique_ptr<TrieNode> new_node = std::make_unique<TrieNode>();
        tmp->children.insert(std::make_pair(word[i], std::move(new_node)));
      }
      tmp = (tmp->children[word[i]]).get();
    }
    tmp->word = word;
}

std::vector<std::string> Trie::findSmallestWords(std::string& prefix) {
  std::set<std::string> ordered_result;
  std::vector<std::string> result;
  std::string tmp_string = prefix;
  std::set<std::string> prefix_found;
  
  for (int i = 0; i < tmp_string.length(); i++) {
    for (char c = 'a'; c <= 'z'; c++) {
      tmp_string[i] = c;
      if (searchPrefix(tmp_string) != nullptr) {
        prefix_found.insert(tmp_string);
      }
    }
    tmp_string[i] = prefix[i];
  }
  
    for (auto it = prefix_found.begin(); it != prefix_found.end(); ++it) {
    /*if (result.size() >= result_limit) {
      break;
    }*/
    findPrefixWords(*it, ordered_result);
  }
  
  for (auto it = ordered_result.begin(); it != ordered_result.end() ; ++it) {
    result.push_back(*it);
  }
  
  std::sort(result.begin(), result.end());
  
  if (result.size() > result_limit) {
    result.resize(result_limit);
  }

  return result;
}

TrieNode* Trie::searchPrefix(const std::string& prefix) {
  TrieNode* tmp = root.get();
  for (int i = 0; i < prefix.length(); i++) {
    auto it = tmp->children.find(prefix[i]);
    if (it == tmp->children.end()) {
      return nullptr;
    }
    tmp = (tmp->children[prefix[i]]).get();
  }
  return tmp;
}

void Trie::findPrefixWords(const std::string prefix, std::set<std::string>& result) {
    TrieNode* tmp = searchPrefix(prefix);
    if (tmp == nullptr) {
      return;
    }
    std::set<std::string> strings;
    std::string current_string(prefix);
    findStrings(tmp, strings, current_string);
    for (auto it = strings.begin(); it != strings.end(); ++it) {
      result.insert(*it);
    }
}

void Trie::findStrings(TrieNode* tmp, std::set<std::string>& strings, std::string current_string) {
  if (tmp == nullptr) {
      return;
  }
  if (!tmp->word.empty()) {
      strings.insert(current_string);
  }
  for (auto it = tmp->children.begin(); it != tmp->children.end(); ++it) {
      current_string.push_back(it->first);
      findStrings(it->second.get(), strings, current_string);
      current_string.pop_back();
  }
}