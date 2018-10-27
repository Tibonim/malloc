#include "header.h"

void		show_alloc_mem()
{
  t_header	*tmp;
  void		*end;
  void		*data_start;
  void		*data_end;

  if (start_sbrk() == end_sbrk(NULL))
    return ;
  tmp = start_sbrk();
  end = end_sbrk(NULL) - sizeof(t_header);
  printf("break : %p\n", (void *)end + sizeof(t_header));
  if (tmp->magic != 42)
    return ;
  while (tmp != end)
    {
      data_start = (void *)tmp + sizeof(t_header);
      data_end = (void *)tmp + (tmp->size * FRAG);
      if (tmp->is_free == FALSE)
	printf("%p - %p : %ld bytes\n", data_start, data_end,
	       data_end - data_start);
      tmp = data_end;
    }
}

void		add_first_malloc(size_t size, size_t end, void** tmp)
{
  void*		old;
  t_header*     header;

  old = *tmp;
  header = old;
  header->prev = NULL;
  old = old + sizeof(t_header);
  end = end - sizeof(t_header);
  header->size = size;
  header->is_free = FALSE;
  header->magic = MAGIC;
  set_inter_header(size * FRAG, old);
  set_end_header(old + size * FRAG, old + end - sizeof(t_header));
  *tmp = old;
}

void*		end_sbrk(void* ptr)
{
  static void*	p = NULL;

  if (p == NULL)
    p = start_sbrk();
  if (ptr != NULL)
    p = ptr;
  return (p);
}

void	del_page(int nb_page, int size_page, void* end, t_header* tmp)
{
  sbrk(-(nb_page * size_page));
  end_sbrk(end_sbrk(NULL) - (nb_page * size_page));
  end = end - (nb_page * size_page);
  if (tmp->is_free == FALSE)
    tmp = (void *)tmp + tmp->size * FRAG;
  set_end_header((void *)tmp + sizeof(t_header), end);
  tmp->size = (end - (void *)tmp) / FRAG;
}
