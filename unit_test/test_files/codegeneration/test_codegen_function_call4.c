int function(int a, int b, char c, char d)
{
  int e = a;
  int f = b;
  c = 'c';
}

int main()
{
  int g = 0;

  char h = 'r';
  function(g, 20, h, 'f');
}