#include "header.h"

void*		cpy_mem(char* ptr, t_header* cur, size_t size)
{
  size_t	i;
  char*		adr;

  i = 0;
  pthread_mutex_unlock(&lock);
  adr = malloc((size - 1) * FRAG);
  while (i < (cur->size - 1) * FRAG)
    {
      adr[i] = ptr[i];
      i++;
    }
  free(ptr);
  return (adr);
}

void		realloc_ext(size_t size, t_header* cur,
			    t_header* next)
{
  t_header*	new;

  next->magic = MAGIC - 1;

  new = (void*)cur + size * FRAG;
  new->prev = cur;
  new->is_free = TRUE;
  new->size = next->size - (size - cur->size);
  new->magic = MAGIC;

  next = (void*)next + next->size * FRAG;
  next->prev = (void *)new + sizeof(t_header);

  cur->size = size;
  pthread_mutex_unlock(&lock);
}

void*		realloc(void *ptr, size_t size)
{
  t_header*     cur;
  t_header*	next;

  if (size == 0)
    return (ptr);
  if (start_sbrk() == end_sbrk(NULL) ||
      ptr > end_sbrk(NULL) || ptr < start_sbrk())
    return (malloc(size));
  pthread_mutex_lock(&lock);
  cur = ptr - sizeof(t_header);
  if (size % FRAG == 0)
    size = size / FRAG + 1;
  else
    size = size / FRAG + 2;
  next = ptr + cur->size * FRAG - sizeof(t_header);
  if (cur->magic == MAGIC && next->magic == MAGIC &&
      next->is_free == TRUE && next->size + cur->size - 1 >= size)
    {
      realloc_ext(size, cur, next);
      return (ptr);
    }
  else
    {
      return (cpy_mem(ptr, cur, size));
    }
}
