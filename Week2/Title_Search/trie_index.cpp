#include "trie_index.h"
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <queue>

#include <iostream>

TrieIndex::TrieIndex() {
  root = new TrieIndexNode();
}

void TrieIndex::addTitle(std::string& title) {
  std::string::iterator new_end = 
        std::unique(title.begin(), title.end(),
        [=](char lhs, char rhs){ return (lhs == rhs) && (lhs == ' '); }
        );
  title.erase(new_end, title.end());

  std::vector<std::string> words = extractKeywords(title);
  
  std::string sorted_title;
  for (auto word : words) {
    sorted_title += word;
    sorted_title.push_back(' ');
  }
  sorted_title.pop_back();
 
  // Add this title to the set of titles that share the same words
  auto it = same_words_titles.find(sorted_title);
  if (it == same_words_titles.end()) {
    std::vector<std::string> vec_title = {title};
    same_words_titles.insert(std::make_pair(sorted_title, vec_title));
  }
  else {
    (it->second).push_back(title);
  }

  // Build the inverted index
  std::set<std::string> words_set;
  for (auto word : words) {
    words_set.insert(word);
  }

  
  for (auto it_set = words_set.begin(); it_set != words_set.end(); ++it_set) {
    auto it = inverted_index.find(*it_set);
    if (it == inverted_index.end()) {
      std::set<std::string> title_tmp;
      title_tmp.insert(sorted_title);
      inverted_index.insert(std::make_pair(*it_set,title_tmp));
    }
    else {
      (it->second).insert(sorted_title);
    }
  }

  // Add title and words to the titles_words map
  std::set<std::string> title_set;
  for (auto word : words) {
    title_set.insert(word);
  }
  titles_words.insert(std::make_pair(sorted_title,title_set));

  
}

std::list<std::string> 
TrieIndex::searchTitles(std::string& query) {
  if (query.empty()) {
    return {};
  }
  
  std::string::iterator new_end = 
        std::unique(query.begin(), query.end(),
        [=](char lhs, char rhs){ return (lhs == rhs) && (lhs == ' '); }
        );
  query.erase(new_end, query.end());

  std::list<std::string> result;

  std::vector<std::string> query_words = extractKeywords(query);  
  query_words = processQuery(query_words);  

  std::set<std::string> result_set;

  for (auto word : query_words) {
    auto it = inverted_index.find(word);
    if (it == inverted_index.end()) {
      return {};
    }
    for (auto it_title = (it->second).begin(); it_title != (it->second).end(); ++it_title) {   
        result_set.insert(*it_title);      
    }
  }

  std::list<std::string> candidates;

  for (auto it = result_set.begin(); it != result_set.end(); ++it) {
    candidates.push_back(*it);
  }

  for (auto title : candidates) {
    auto it_words = titles_words.find(title);
    if (it_words != titles_words.end()) {
      bool all_words = true;
      for (auto word : query_words) {
        auto it = (it_words->second).find(word);
        if (it == (it_words->second).end()) {
          all_words = false;
        }
      }
      if (all_words) {  
        auto it_orig = same_words_titles.find(title);
        if (it_orig != same_words_titles.end()) {  
          for (auto orig_title : (it_orig->second)) {  
            result.push_back(orig_title);
          }          
        }
      }
    }
  }

  result = orderResult(result);
  return result;
}

std::vector<std::string> 
TrieIndex::extractKeywords(std::string& title, bool sort) {
  std::istringstream page(title);
  std::vector<std::string> words(std::istream_iterator<std::string>{page},
                                 std::istream_iterator<std::string>());
  if (sort) {
    std::sort(words.begin(), words.end());  
  } 
  return words;
}

std::vector<std::string> TrieIndex::processQuery(std::vector<std::string>& query) {
  std::vector<std::string> result;
  std::set<std::string> query_set;
  for (auto word : query) {
    query_set.insert(word);
  }
  for (auto it = query_set.begin(); it != query_set.end(); ++it) {
    result.push_back(*it);
  }
  return result;
}

std::list<std::string> TrieIndex::orderResult(std::list<std::string>& orig_result) {
  std::list<std::string> result;
  std::vector<std::vector<std::string>> v_titles;

  for (auto title : orig_result) {
    std::vector<std::string> v_tmp = extractKeywords(title, false);
    v_titles.push_back(v_tmp);
  }

  std::sort(v_titles.begin(), v_titles.end(), [](const std::vector<std::string>& v1, const std::vector<std::string>& v2) {
    if (v1.size() == v2.size()) {
      std::string first;
      std::string second;
      for (auto word : v1) {
        first += word;
        first.push_back(' ');
      }
      first.pop_back();
      for (auto word : v2) {
        second += word;
        second.push_back(' ');
      }
      second.pop_back();
      return std::lexicographical_compare(first.begin(), first.end(), second.begin(), second.end());
    }
    else {
      return v1.size() < v2.size();  
    }   
  });
  
  for (auto title : v_titles) {
    std::string first;
    for (auto word : title) {
      first += word;
      first.push_back(' ');
    }
    first.pop_back();
    result.push_back(first);
  }

  if (result.size() > PAGES_LIMIT) {
    result.resize(PAGES_LIMIT);
  }
  return result;
}