#include "infection.h"

size_t	array_length(char **array)
{
	size_t	size;

	size = 0;
	while (array && array[size] != NULL)
		size++;
	return (size);
}
