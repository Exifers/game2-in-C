#include "cache.h"

struct *cache_entry cache_singleton() {
  static struct cache_entry cache_entry;
  if (!cache_entry) {
    cache_entry_init(
        &cache_entry,
        NULL,
        NULL,
        0
        );
  }
  return &cache_entry;
}

void cache_entry_init(
    struct cache_entry *this,
    char *filename,
    void *content,
    size_t content_length
    ) {
  this->filename = filename;
  this->content = content;
  this->content_length = content_length;
  this->next = NULL;
}

struct cache_entry *cache_entry_new(char *filename, void *content, size_t content_length) {
  struct cache_entry *cache_entry = malloc(sizeof(struct cache_entry));
  cache_entry_init(
      cache_entry,
      filename,
      content,
      content_length
  );
  return cache_entry;
}

void cache_add_entry(char *filename, void *content, size_t content_length) {
  struct *cache_entry cache = cache_singleton();

  struct *cache_entry cur = cache;
  while(cur->next) {
    cur = cur->next;
  }
  cur->next = cache_entry_new(
      filename,
      content,
      content_length
      );
}

void *cache_open_file(char *filename) {
  struct cache_entry *cache_entry = find_filename_in_cache(filename);
  if (!cache_entry) {
    size_t content_length;
    void *content = io_read_file_content(filename, &content_length);
    add_cache_entry(filename, content, content_length);
  }
}
