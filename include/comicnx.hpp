
#ifdef DEBUG
#define PRINTF(fmt, args...)    fprintf(stderr, fmt, ## args)
#else
#define PRINTF(fmt, args...)
#endif
