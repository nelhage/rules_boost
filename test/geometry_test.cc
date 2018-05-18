#include <vector>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/index/rtree.hpp>


namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

int
main() {
  typedef bg::model::point<float, 2, bg::cs::cartesian> point;
  typedef bg::model::box<point> box;
  typedef std::pair<box, unsigned> value;
  bgi::rtree< value, bgi::quadratic<16> > rtree;
  for (unsigned i = 0 ; i < 10 ; ++i) {
    box b(point(i + 0.0f, i + 0.0f), point(i + 0.5f, i + 0.5f));
    rtree.insert(std::make_pair(b, i));
  }
  box query_box(point(0, 0), point(5, 5));
  std::vector<value> result_s;
  rtree.query(bgi::intersects(query_box), std::back_inserter(result_s));
  return result_s.size() == 5;
}
