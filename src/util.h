#ifndef UTIL_H
#define UTIL_H

void* get_page_address(void *ptr);
int my_log(int n);
void my_lock(void);
void my_unlock(void);

#endif /* !UTIL_H */