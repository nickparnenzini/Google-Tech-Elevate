#ifndef TRIE_INDEX_H_
#define TRIE_INDEX_H_

#include <map>
#include <list>
#include <set>
#include <vector>

struct TrieIndexNode {
  std::map<std::string, TrieIndexNode*> children;
  std::list<std::string> titles;
};

class TrieIndex {
 public:
  TrieIndex();
  void addTitle(std::string& title); 
  std::list<std::string> searchTitles(std::string& query);

 private:
  std::vector<std::string> extractKeywords(std::string& title, bool sort=true);
  std::vector<std::string> processQuery(std::vector<std::string>& query);
  std::list<std::string> orderResult(std::list<std::string>& result);

  TrieIndexNode* root{nullptr};
  std::map<std::string,std::string> sorted_orig_map;
  std::map<std::string, std::list<std::string>> cache_query;
  std::map<std::string, std::set<std::string>> inverted_index;
  std::map<std::string, std::set<std::string>> titles_words;
  std::map<std::string, std::vector<std::string>> same_words_titles;

  const int PAGES_LIMIT = 10;
};

#endif // TRIE_INDEX_H_