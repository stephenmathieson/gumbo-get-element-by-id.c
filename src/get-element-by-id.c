
#include <string.h>
#include "get-element-by-id.h"

/**
 * Get the first `GumboNode` with the given `id`
 * contained with the `document`.
 */

const GumboNode *
gumbo_get_element_by_id(const char *id, const GumboNode *document) {

  if (GUMBO_NODE_DOCUMENT != document->type
   && GUMBO_NODE_ELEMENT != document->type) {
    return NULL;
  }

  GumboAttribute *node_id =
    gumbo_get_attribute(&document->v.element.attributes, "id");
  if (node_id && 0 == strcmp(id, node_id->value)) {
    return document;
  }

  // iterate all children
  const GumboVector *children = &document->v.element.children;
  for (unsigned int i = 0; i < children->length; i++) {
    const GumboNode *node = gumbo_get_element_by_id(id, children->data[i]);
    if (node) return node;
  }

  return NULL;
}
