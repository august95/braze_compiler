int main()
{
  int a = 0;
  int* ptr1 = &a;
  int** ptr2 = &ptr1;
  int c = **ptr2;
}
