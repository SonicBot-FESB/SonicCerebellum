template <class A, class B> class Pair {
public:
  A first;
  B second;
  Pair(A first, B second) {
    this->first = first;
    this->second = second;
  };
};