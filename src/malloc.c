#include "header.h"

void*		start_sbrk()
{
  static void*	start = NULL;

  if (start == NULL)
    start = sbrk(0);
  return (start);
}

void*		size_already_malloc(size_t size, void* current)
{
  void*		end;
  t_header*	header;

  if (current == start_sbrk())
    return (NULL);
  header = current - sizeof(t_header);
  header = header->prev - sizeof(t_header);
  if (header->is_free == FALSE)
    return (NULL);
  header = start_sbrk();
  end = current;
  current = start_sbrk() + sizeof(t_header);
  while (current != end)
    {
      if (header->is_free == TRUE && header->size > size)
	{
	  set_header_inside(size, current, header);
	  return (current);
	}
      current = current + header->size * FRAG;
      header = current - sizeof(t_header);
    }
  return (NULL);
}

size_t		calc_nb_space(void* current, size_t size)
{
  size_t	size_page;

  if (current == start_sbrk())
    size = size  + sizeof(t_header);
  size_page = getpagesize();
  if (size % size_page == 0)
    return (size);
  else
    return ((size / size_page + 1) * size_page);
}

void*		make_new_malloc(size_t size, void *current)
{
  void*	        old;
  size_t	end;
  t_header*	header;

  end = calc_nb_space(current, size * FRAG);
  old = sbrk(end);
  if (old == (void *)-1)
    return (NULL);
  end_sbrk(old + end);
  if (old != start_sbrk())
    {
      header = old - sizeof(t_header);
      old = header->prev;
      header = old - sizeof(t_header);
      if (header->is_free == FALSE)
	old = old + header->size * FRAG;
      add_new_page(size, end, old, old - sizeof(t_header));
    }
  else
    add_first_malloc(size, end, &old);
  return (old);
}

void*		malloc(size_t size)
{
  void*		space;
  void*		current;

  pthread_mutex_lock(&lock);
  if (size == 0)
    {
      pthread_mutex_unlock(&lock);
      return (NULL);
    }
  current = end_sbrk(NULL);
  if (size % FRAG == 0)
    size = size / FRAG + 1;
  else
    size = size / FRAG + 2;
  if ((space = size_already_malloc(size, current)) == NULL)
    space = make_new_malloc(size, current);
  pthread_mutex_unlock(&lock);
  return (space);
}
