struct Inner {
  short value;
};

struct Outer {
  char tag;
  struct Inner inner;
  int count;
};

struct Outer global;

int main() {
  struct Outer local;
  local.inner.value = 7;
  global.count = local.inner.value;
  return global.count;
}
