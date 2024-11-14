#include "tstree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static inline TSTree *TSTree_insert_base(TSTree *root, TSTree *node,
                                         const char *key, size_t len,
                                         void *value) {
  if (node == NULL) {
    node = (TSTree *)calloc(1, sizeof(TSTree));
    if (root == NULL) {
      root = node;
    }

    node->splitchar = *key;
  }

  if (*key < node->splitchar) {
    node->low = TSTree_insert_base(root, node->low, key, len, value);
  } else if (*key > node->splitchar) {
    node->high = TSTree_insert_base(root, node->high, key, len, value);
  } else {
    if (len > 1) {
      node->equal =
          TSTree_insert_base(root, node->equal, key + 1, len - 1, value);
    } else {
      assert(node->value == NULL && "Duplicate insert into tst");
      node->value = value;
    }
  }

  return node;
}

TSTree *TSTree_insert(TSTree *node, const char *key, size_t len, void *value) {
  return TSTree_insert_base(node, node, key, len, value);
}

void *TSTree_search(TSTree *root, const char *key, size_t len) {
  TSTree *node = root;
  size_t i = 0;

  while (i < len && node) {
    if (key[i] < node->splitchar) {
      node = node->low;
    } else if (key[i] > node->splitchar) {
      node = node->high;
    } else {
      i++;
      if (i < len) {
        node = node->equal;
      }
    }
  }

  return node ? node->value : NULL;
}

void *TSTree_search_prefix(TSTree *root, const char *key, size_t len) {
  if (len == 0) {
    return NULL;
  }

  TSTree *node = root;
  TSTree *last = NULL;
  size_t i = 0;

  while (i < len && node) {
    if (key[i] < node->splitchar) {
      node = node->low;
    } else if (key[i] > node->splitchar) {
      node = node->high;
    } else {
      i++;
      if (i < len) {
        if (node->value) {
          last = node;
        }
        node = node->equal;
      }
    }
  }

  node = node ? node : last;

  // traverse until we find the first value in the equal chain
  // this is then the first node with this prefix
  while (node && !node->value) {
    node = node->equal;
  }

  return node ? node->value : NULL;
}

void TSTree_traverse(TSTree *node, TSTree_traverse_cb cb, void *data) {
  if (!node)
    return;

  if (node->low)
    printf("going to LOW branch: splitchar: %c, value: %s\n", node->splitchar,
           (char *)node->value);
  TSTree_traverse(node->low, cb, data);

  if (node->equal) {
    printf("going to EQUAL branch: splitchar: %c, value: %s\n", node->splitchar,
           (char *)node->value);
    TSTree_traverse(node->equal, cb, data);
  }

  if (node->high)
    printf("going to HIGH branch: splitchar: %c, value: %s\n", node->splitchar,
           (char *)node->value);
  TSTree_traverse(node->high, cb, data);

  if (node->value) {
    printf("calling cb\n");
    cb(node->value, data, node->splitchar);
  }
}

void TSTree_destroy(TSTree *node) {
  if (node == NULL)
    return;

  if (node->low)
    TSTree_destroy(node->low);

  if (node->equal) {
    TSTree_destroy(node->equal);
  }

  if (node->high)
    TSTree_destroy(node->high);

  free(node);
}
