#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <algorithm>
#include <set>
#include <climits>

#include "code.h"

using elevate::Edge;
using elevate::Node;

using namespace std;

typedef std::pair<int,std::string> iPair;

class Flight {
 public:
  string departs;
  string arrives;
  int price;

  Flight(string departs_, string arrives_, int price_) {
    departs = departs_;
    arrives = arrives_;
    price = price_;
  }
};

class Route {
 public:
  std::list<string> airports;
  int price;

  Route(list<string> airports_, int price_) {
    airports = airports_;
    price = price_;
  }

  void addAirport(std::string& airport) {
    airports.push_back(airport);
  }

  bool operator<(const Route& that)  const {
    return this->price < that.price;
  }
};

class Compare
{
public:
    bool operator() (Route& r1, Route& r2)
    {
        return r1.price > r2.price;
    }
};

class Graph {
 public:
  std::map<std::string, Node> nodes;

  Graph() {}
  
  void addEdge(Flight f) {
    auto it = nodes.find(f.departs);
    if (it != nodes.end()) {
      Edge e = Edge(f.price, it->second);
      (it->second).connections.insert(std::make_pair(f.arrives,e));
    }
    else {
      Node n = Node(f.departs);
      Edge e = Edge(f.price, n);
      n.connections.insert(std::make_pair(f.arrives, e));
      nodes.insert(std::make_pair(f.departs, n));
    }
  }
};

std::list<Route> sortResult(std::list<Route>& result) {
  std::vector<Route> v_result;
  std::list<Route> l_result;
  
  for (auto r : result) {
    v_result.push_back(r);
  }

  std::sort(v_result.begin(), v_result.end(), [](const Route& r1, const Route& r2) {
    if (r1.price == r2.price) {
      if (r1.airports.size() == r2.airports.size()) {
        std::string first;
        for (auto aiport : r1.airports) {
          first += aiport;
        }
        std::string second;
        for (auto airport : r2.airports) {
          second += airport;
        }
        return std::lexicographical_compare(first.begin(), first.end(), second.begin(), second.end());   
      }
      else {
        return r1.airports.size() < r2.airports.size();
      }
    }
    else {
      return r1.price < r2.price;
    }
  });

  for (auto r : v_result) {
    l_result.push_back(r);
  }

  return l_result;
}

std::list<Route> find_connections(std::list<Flight> flights, string start,
                                  string end, int limit) {
                                    
  if (start == end) {
    return {};
  }
  std::list<Route> routes;
  Graph g;
  std::set<std::string> airports;
  std::map<std::string,int> distance;
  
  for (auto flight : flights) {
    g.addEdge(flight);
    airports.insert(flight.departs);
    airports.insert(flight.arrives);  
  }
  
  if (g.nodes.find(start) == g.nodes.end() || airports.find(end) == airports.end()) {
    return {};
  }
  
  // Priority queue for the resulting connections by price.
  std::priority_queue<Route, std::vector<Route>, Compare> result;
  Route r = Route(std::list<std::string>{start}, 0);
  result.push(r);
  
  while (!result.empty()) {
    Route curr_route = result.top();
    result.pop();
    std::string end_airport = curr_route.airports.back();
    int current_price = curr_route.price;
    if (end_airport == end) {
      routes.push_back(curr_route); 
      if (routes.size() == limit) {
        break;
      }
    }
    else {
      if (routes.size() == limit) {
        break;
      }
      auto it = g.nodes.find(end_airport);
      if (it != g.nodes.end()) {
        Node curr_node = it->second;
        for (auto it_node = curr_node.connections.begin(); it_node != curr_node.connections.end(); ++it_node) {
          std::string stop = it_node->first;
          int updated_cost = curr_route.price + it_node->second.price;
          Route r(curr_route.airports, curr_route.price);
          r.addAirport(stop);
          r.price = updated_cost;
          result.push(r);           
        }
      }
    }
  }

  routes = sortResult(routes);
  return routes;
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  int number_of_flights;
  int limit;
  string departs;
  string arrives;
  int price;
  string source;
  string destination;

  list<Flight> flights;
  std::cin >> number_of_flights;
  for (int i = 0; i < number_of_flights; i++) {
    std::cin >> departs;
    std::cin >> arrives;
    std::cin >> price;
    Flight f = Flight(departs, arrives, price);
    flights.push_back(f);
  }
  std::cin >> limit;
  std::cin >> source;
  std::cin >> destination;

  std::list<Route> connections =
      find_connections(flights, source, destination, limit);
      
  if (!connections.empty()) {
    for (auto route : connections) {
      for (auto it = route.airports.begin(); it != route.airports.end(); ++it) {
        std::cout << *it << " ";
      }
      std::cout << route.price << std::endl;
    }  
  }
  else {
    std::cout << "<no solution>" << std::endl;
  }
  
  return 0;
};