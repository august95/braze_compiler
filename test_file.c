int printf(const char* format, ...);

int braze_test(int iterations)
{
  printf("compiled by braze");
  for (int i = 0; i < iterations; i = i + 1)
  {
    printf("!");
  }
}

int main()
{
  braze_test(3);
}