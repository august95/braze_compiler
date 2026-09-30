int main()
{
  int* ptr;
  int val = 5;
  ptr = &val;
  *ptr = 6;
  int copy = val;

  int result = *ptr;
}
