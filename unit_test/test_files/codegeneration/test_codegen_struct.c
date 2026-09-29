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
  local.tag = 3;
  local.inner.value = 7;
  local.count = 42;
  global.tag = local.tag;
  global.inner.value = local.inner.value;
  global.count = local.count;
  return global.count;
}
