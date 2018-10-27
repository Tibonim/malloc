#ifndef _HEADER_H_
# define _HEADER_H_

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

#define MAGIC	(42)
#define	FRAG	(16)
#define TRUE	(1)
#define FALSE	(0)

typedef struct	s_header
{
  void		*prev;
  unsigned int	size;
  char		is_free;
  short		magic;
}		t_header;

pthread_mutex_t lock;

int	my_error(char *);
void	set_inter_header(size_t size, void* old);
void	set_end_header(void* old, t_header* header);
void	set_header_inside(size_t size, void* current, t_header* header);
void	add_new_page(size_t size, size_t end, void* current, t_header* header);
void    add_first_malloc(size_t size, size_t end, void** tmp);
void	show_alloc_mem();
void*	start_sbrk();
void*	end_sbrk(void* ptr);
void*	malloc(size_t size);
void	free(void *ptr);
void	del_page(int nb_page, int size_page, void* end, t_header* tmp);

#endif /* _HEADER_H_ */
