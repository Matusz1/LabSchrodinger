#ifndef _UTIL_H_
#define _UTIL_H_

/* message is printed if malloc/realloc failed, unless there is no message */

void* msg_malloc(size_t size, const char* message);
void* msg_realloc(void* p, size_t size, const char* message);

#endif /* _UTIL_H_ */
