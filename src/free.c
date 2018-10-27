#include "header.h"

void		check_page_empty(void *end)
{
  int		nb_page;
  int		size_page;
  size_t	free_total;
  t_header*	tmp;

  tmp = end;
  tmp = (void *)tmp->prev - sizeof(t_header);
  free_total = 1;
  while (tmp->prev && tmp->is_free == TRUE)
    {
      if (tmp->magic != MAGIC)
	return ;
      free_total += tmp->size;
      tmp = (void *)tmp->prev - sizeof(t_header);
    }
  size_page = getpagesize();
  nb_page = free_total * FRAG / size_page - 1;
  if (nb_page > 0)
    del_page(nb_page, size_page, end, tmp);
}

unsigned int	get_total_free(t_header **header, void *end,
			       unsigned int size_header)
{
  unsigned int	free_total;
  t_header	*tmp;

  free_total = size_header;
  tmp = *header;
  while (tmp != end && tmp->is_free == TRUE)
    {
      free_total += tmp->size;
      if (tmp->size == 0)
      	return (my_error("Error in free size\n"));
      tmp = (void *)tmp + (tmp->size * FRAG);
    }
  *header = tmp;
  return (free_total);
}

size_t		my_free(void *ptr, t_header *header)
{
  t_header	*tmp;
  void		*end;
  size_t	free_total;

  tmp = header;
  end = end_sbrk(NULL) - sizeof(t_header);
  if (ptr != NULL)
    {
      if (header->magic != MAGIC)
  	return (my_error("Invalid pointer passed for free\n"));
      if (header->is_free == FALSE)
  	{
	  tmp = (void *)header + header->size * FRAG;
  	  if ((free_total = get_total_free(&tmp, end, header->size)) == 1)
  	    return (1);
  	  header->size = free_total;
  	  header->is_free = TRUE;
  	  tmp->prev = (void *)header + sizeof(t_header);
  	  check_page_empty(end);
  	}
      else
  	return (my_error("Double free or corruption\n"));
    }
  return (0);
}

void		free(void *ptr)
{
  t_header	*header;

  pthread_mutex_lock(&lock);
  header = ptr - sizeof(t_header);
  my_free(ptr, header);
  pthread_mutex_unlock(&lock);
}
