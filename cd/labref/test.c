int main()
{
  int array[11];
  printf("Write down your ID number!\n");
  for(int i=0;i<id_length;i++)
  scanf("%d", &array[i]);
  if (array[0]==1)
  {
    printf("\nThis person is a male.");
  }
  else if (array[0]==2)
  {
    printf("\nThis person is a female.");
  }
  return 0;
}