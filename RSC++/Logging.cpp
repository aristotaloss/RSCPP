#include "Logging.h"

void logf(const char *format, ...) {
	va_list args;
	va_start(args, format);

	time_t rawtime;
	struct tm *timeinfo = (tm *) malloc(sizeof(tm));
	char buffer[80];

	time(&rawtime);
	localtime_s(timeinfo, &rawtime);

	strftime(buffer, 80, "[%H:%M:%S] ", timeinfo);
	//puts(buffer);

	printf(buffer);
	vprintf(format, args);

	va_end(args);
}