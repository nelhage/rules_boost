#include <boost/graph/directed_graph.hpp>

int main(int, char*[])
{
  typedef boost::directed_graph<> Graph;
  Graph g;
  boost::graph_traits< Graph >::vertex_descriptor v0 = g.add_vertex();
  boost::graph_traits< Graph >::vertex_descriptor v1 = g.add_vertex();

  g.add_edge(v0, v1);

  return 0;
}
