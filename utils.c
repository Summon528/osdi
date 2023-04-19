int strncmp(const char *s1, const char *s2, int n) {
  if (n == 0)
    return (0);
  do {
    if (*s1 != *s2++)
      return (*(unsigned char *)s1 - *(unsigned char *)--s2);
    if (*s1++ == 0)
      break;
  } while (--n != 0);
  return (0);
}

void *memset(void *ptr, int c, unsigned long len) {
  for (int i = 0; i < len; i++) {
    ((char *)ptr)[i] = (unsigned char)c;
  }
  return ptr;
}

int strlen(const char *a) {
  int cnt = 0;
  while (*a != '\0') {
    cnt++;
    a++;
  }
  return cnt;
}

void *memcpy(void *dest, const void *src, long n) {
  char *pDest = (char *)dest;
  const char *pSrc = (const char *)src;

  for (long i = 0; i < n; ++i) {
    pDest[i] = pSrc[i];
  }

  return dest;
}
