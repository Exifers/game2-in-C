#ifndef CACHE_H
#define CACHE_H

#include <SDL.h>
#include <stddef.h>
#include <stdio.h>

struct cache_entry {
  char *filename;
  void *content;
  size_t content_length;
  struct cache_entry *next;
}

struct *cache_entry cache_singleton();
void cache_entry_init(
    struct cache_entry *this,
    char *filename,
    void *content,
    size_t content_length
    );
struct cache_entry *cache_entry_new(
    char *filename,
    void *content,
    size_t content_length
    );
void *cache_open_file(char *filename);
void *cache_free_all();
struct cache_entry *find_filename_in_cache(char *filename);
void cache_add_entry(char *filename, void *content, size_t content_length);


#endif /* CACHE_H */
