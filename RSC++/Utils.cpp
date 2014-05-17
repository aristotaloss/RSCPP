#include "Utils.h"


Utils::Utils()
{
}


Utils::~Utils()
{
}

long Utils::nameToLong(char *name) {
	long l = 0L;
	int str_len = strlen(name);
	for (int i = 0; i < str_len && i < 12; i++) {
		char c = name[i];
		l *= 37L;
		if (c >= 'A' && c <= 'Z')
			l += (1 + c) - 65;
		else if (c >= 'a' && c <= 'z')
			l += (1 + c) - 97;
		else if (c >= '0' && c <= '9')
			l += (27 + c) - 48;
	}

	for (; l % 37L == 0L && l != 0L; l /= 37L);
	return l;
}
