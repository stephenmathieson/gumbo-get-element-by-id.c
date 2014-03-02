
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gumbo-parser/gumbo.h"
#include "get-element-by-id.h"

#define EQUALS(a, b) ({                               \
  if (0 != strcmp(a, b))  {                           \
    fprintf(stderr                                    \
      , "Assertion error: \"%s\" == \"%s\" (%s:%d)\n" \
      , a                                             \
      , b                                             \
      , __FILE__                                      \
      , __LINE__);                                    \
    abort();                                          \
  }                                                   \
})

#define TEST(name) \
  static void test_##name(void)

#define RUN_TEST(test) \
  test_##test(); \
  puts("    \e[92m✓ \e[90m" #test "\e[0m");

// read a file
static char *
read_file(const char *file) {
  FILE *fp = NULL;
  char *buffer = NULL;
  long size = 0;

  fp = fopen(file, "r");
  assert(fp);

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);

  fseek(fp, 0, SEEK_SET);

  buffer = (char *) calloc(size + 1, sizeof(char));
  assert(buffer);
  *buffer = '\0';
  fread(buffer, sizeof(char), size, fp);
  fclose(fp);

  return buffer;
}


TEST(simple_matching_ids) {
  char *html = NULL;
  GumboOutput *output = NULL;
  GumboAttribute *id = NULL;
  const GumboNode *element = NULL;

  html = read_file("./tests/simple.html");
  assert(html);
  output = gumbo_parse(html);
  free(html);
  assert(output);

  element = gumbo_get_element_by_id("foo", output->root);
  id = gumbo_get_attribute(&element->v.element.attributes, "id");
  EQUALS("foo", id->value);

  element = gumbo_get_element_by_id("head", output->root);
  id = gumbo_get_attribute(&element->v.element.attributes, "id");
  EQUALS("head", id->value);

  element = gumbo_get_element_by_id("bar", output->root);
  id = gumbo_get_attribute(&element->v.element.attributes, "id");
  EQUALS("bar", id->value);

  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

TEST(simple_non_matches) {
  char *html = NULL;
  GumboOutput *output = NULL;

  html = read_file("./tests/simple.html");
  assert(html);
  output = gumbo_parse(html);
  free(html);
  assert(output);

  assert(NULL == gumbo_get_element_by_id("baz", output->root));
  assert(NULL == gumbo_get_element_by_id("header", output->root));
  assert(NULL == gumbo_get_element_by_id("container2", output->root));

  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

TEST(google_homepage) {
  char *html = NULL;
  GumboOutput *output = NULL;
  GumboAttribute *id = NULL;
  const GumboNode *element = NULL;

  // test a much larger document
  html = read_file("./tests/google.html");
  assert(html);
  output = gumbo_parse(html);
  free(html);
  assert(output);

  element = gumbo_get_element_by_id("hplogo", output->root);
  id = gumbo_get_attribute(&element->v.element.attributes, "id");
  EQUALS("hplogo", id->value);

  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

int
main(void) {
  clock_t start = clock();

  printf("\n  \e[36m%s\e[0m\n", "gumbo_get_element_by_id");

  RUN_TEST(simple_matching_ids);
  RUN_TEST(simple_non_matches);
  RUN_TEST(google_homepage);

  printf("\n");
  printf("  \e[90mcompleted in \e[32m%.5fs\e[0m\n"
    , (float) (clock() - start) / CLOCKS_PER_SEC);
  printf("\n");
}
