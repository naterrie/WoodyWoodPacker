#include "woody.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		dprintf(2, "Usage: %s <file>\n", argv[0]);
		return (1);
	}

	return (0);
}
