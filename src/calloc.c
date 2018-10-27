#include "header.h"

void		*calloc(size_t nmemb, size_t size)
{
  char		*str;
  unsigned int	i;

  i = 0;
  if ((str = malloc(nmemb * size)) == NULL)
    return (str);
  pthread_mutex_lock(&lock);
  while (i < nmemb * size)
    {
      str[i] = 0;
      i++;
    }
  pthread_mutex_unlock(&lock);
  return (str);
}
