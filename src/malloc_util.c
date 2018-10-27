#include "header.h"

void		set_inter_header(size_t size, void* old)
{
  size_t	page_size;
  t_header*	header;

  page_size = getpagesize();
  header = old + size - sizeof(t_header);
  header->magic = MAGIC;
  header->prev = old;
  if (((long)old - (long)start_sbrk()) / page_size > 0)
    header->size = (page_size - (size % page_size)) / FRAG;
  else
    header->size = (page_size - (size % page_size + sizeof(t_header))) / FRAG;
  header->is_free = TRUE;
}

void	set_end_header(void* old, t_header* header)
{
  header->prev = old;
  header->size = 0;
  header->is_free = FALSE;
  header->magic = MAGIC;
}

void		set_header_inside(size_t size, void* current, t_header* header)
{
  t_header*	new_header;
  t_header*	next_header;

  new_header = current + size * FRAG - sizeof(t_header);
  next_header = current + header->size * FRAG - sizeof(t_header);
  next_header->prev = current + size * FRAG;

  new_header->magic = MAGIC;
  new_header->prev = current;
  new_header->size = header->size - size;
  new_header->is_free = TRUE;

  header->magic = MAGIC;
  header->size = size;
  header->is_free = FALSE;
}

void		add_new_page(size_t size, size_t end, void* current,
			     t_header* header)
{
  t_header*	h_end;
  t_header*	new_header;

  h_end = (void*)header + header->size * FRAG + end;
  new_header = current + size * FRAG - sizeof(t_header);

  new_header->magic = MAGIC;
  new_header->prev = current;
  new_header->size = ((void *)h_end - (void *)new_header) / FRAG;
  new_header->is_free = TRUE;

  set_end_header((void*)new_header + sizeof(t_header), h_end);

  header->magic = MAGIC;
  header->size = size;
  header->is_free = FALSE;
}
