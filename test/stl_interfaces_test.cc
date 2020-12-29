#include <boost/stl_interfaces/iterator_interface.hpp>

#include <algorithm>
#include <array>
#include <vector>

template <typename Pred>
struct filtered_int_iterator
    : boost::stl_interfaces::iterator_interface<
          filtered_int_iterator<Pred>, std::bidirectional_iterator_tag, int> {
  filtered_int_iterator() : it_(nullptr) {}
  filtered_int_iterator(int *it, int *last, Pred pred)
      : it_(it), last_(last), pred_(std::move(pred)) {
    it_ = std::find_if(it_, last_, pred_);
  }

  filtered_int_iterator &operator++() {
    it_ = std::find_if(std::next(it_), last_, pred_);
    return *this;
  }

  int *base() const { return it_; }

private:
  friend boost::stl_interfaces::access;
  int *&base_reference() noexcept { return it_; }
  int *base_reference() const noexcept { return it_; }

  int *it_;
  int *last_;
  Pred pred_;
};

template <typename Pred>
filtered_int_iterator<Pred> make_filtered_int_iterator(int *it, int *last, Pred pred) {
  return filtered_int_iterator<Pred>(it, last, std::move(pred));
}

int main() {
  std::array<int, 8> ints = {{0, 1, 2, 3, 4, 5, 6, 7}};
  int *const ints_first = ints.data();
  int *const ints_last = ints.data() + ints.size();

  auto even = [](int x) { return (x % 2) == 0; };
  auto first = make_filtered_int_iterator(ints_first, ints_last, even);
  auto last = make_filtered_int_iterator(ints_last, ints_last, even);

  std::vector<int> ints_copy;
  std::copy(first, last, std::back_inserter(ints_copy));
  return ints_copy == (std::vector<int>{0, 2, 4, 6}) ? 0 : 1;
}
