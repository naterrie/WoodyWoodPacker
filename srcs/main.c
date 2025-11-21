#include "woody.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		dprintf(2, "Usage: %s <file>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
