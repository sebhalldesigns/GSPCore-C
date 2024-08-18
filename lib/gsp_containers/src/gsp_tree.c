#include <gsp_containers/gsp_tree.h>

#include <gsp_debug/gsp_debug.h>

#include <stdlib.h>
#include <stddef.h>

#define INITIAL_STACK_SIZE 10

typedef struct gtree_node_impl_t {
    struct gtree_node_impl_t* child;
    struct gtree_node_impl_t* sibling;
    uintptr_t data;
} gtree_node_impl_t;

gtree_node_impl_t* root_root = NULL;

gtree_node_t gsp_tree_create_root_node() {

    if (NULL == root_root) {
        root_root = (gtree_node_impl_t*)malloc(sizeof(gtree_node_impl_t));
        if (NULL == root_root) {
            gsp_debug_log(INFO, "Failed to allocate gtree_node_t");
            return NULL;
        } 

        root_root->child = NULL;
        root_root->sibling = NULL;
        root_root->data = NULL;

        gsp_debug_log(INFO, "Allocated gtree_node_t %lu", root_root);
        return (gtree_node_t)root_root;
    }

    gtree_node_impl_t* last_root = root_root;

    while (NULL != last_root->sibling) {
        last_root = last_root->sibling;
    }

    gtree_node_impl_t* new_root = (gtree_node_impl_t*)malloc(sizeof(gtree_node_impl_t));
    if (NULL == new_root) {
        gsp_debug_log(INFO, "Failed to allocate gtree_node_t");
        return NULL;
    } 

    new_root->child = NULL;
    new_root->sibling = NULL;
    new_root->data = NULL;

    last_root->sibling = new_root;

    gsp_debug_log(INFO, "Allocated gtree_node_t %lu", new_root);
    return (gtree_node_t)new_root;

}

gtree_node_t gsp_tree_create_sibling_node(gtree_node_t root) {

    if (!gsp_tree_check_node_exists(root)) {
        gsp_debug_log(FAIL, "Can't allocate gtree_node_t for invalid root");
        return NULL;
    }

    // Allocate memory for the new child node
    gtree_node_impl_t* new_sibling = (gtree_node_impl_t*)malloc(sizeof(gtree_node_impl_t));
    if (new_sibling == NULL) {
        gsp_debug_log(INFO, "Failed to allocate gtree_node_t");
        return NULL;
    }

    // Initialize the new child node
    new_sibling->child = NULL;
    new_sibling->sibling = NULL;
    new_sibling->data = NULL;

    gtree_node_impl_t* last_sibling = (gtree_node_impl_t*) root;

    while (last_sibling->sibling != NULL) {
        last_sibling = last_sibling->sibling;
    }

    last_sibling->sibling = new_sibling;

    gsp_debug_log(INFO, "Allocated gtree_node_t %lu as last sibling of %lu", (uintptr_t)new_sibling, (uintptr_t)root);
    return (gtree_node_t)new_sibling;

}

gtree_node_t gsp_tree_create_child_node(gtree_node_t parent) {

    if (!gsp_tree_check_node_exists(parent)) {
        gsp_debug_log(FAIL, "Can't allocate gtree_node_t for invalid parent");
        return NULL;
    }

     // Allocate memory for the new child node
    gtree_node_impl_t* new_child = (gtree_node_impl_t*)malloc(sizeof(gtree_node_impl_t));
    if (new_child == NULL) {
        gsp_debug_log(INFO, "Failed to allocate gtree_node_t");
        return NULL;
    }

    // Initialize the new child node
    new_child->child = NULL;
    new_child->sibling = NULL;
    new_child->data = NULL;

    // Add the new child node to the end of the parent's children list
    gtree_node_impl_t* parent_impl = (gtree_node_impl_t*)parent;
    if (parent_impl->child == NULL) {
        // If parent has no children, the new node is the first child
        parent_impl->child = new_child;
    } else {
        // Traverse to the end of the current children list
        gtree_node_impl_t* last_child = parent_impl->child;
        while (last_child->sibling != NULL) {
            last_child = last_child->sibling;
        }
        // Add the new node as the last sibling
        last_child->sibling = new_child;
    }

    gsp_debug_log(INFO, "Allocated gtree_node_t %lu as a child of %lu", (void*)new_child, (void*)parent_impl);
    return (gtree_node_t)new_child;
}


void gsp_tree_destroy_node(gtree_node_t node) {
    
    if (!gsp_tree_check_node_exists(node)) {
        gsp_debug_log(FAIL, "Can't destroy invalid gtree_node_t");
        return;
    }

    // find previous sibling

    if ((gtree_node_t)root_root == node) {
        // this was the first node, so make it's sibling the root
        root_root = ((gtree_node_impl_t*)node)->sibling;
    } else {
        // code here
    }


    // Initialize stack for DFS traversal
    size_t stack_capacity = INITIAL_STACK_SIZE;
    size_t stack_size = 0;
    gtree_node_impl_t** stack = (gtree_node_impl_t**)malloc(sizeof(gtree_node_impl_t*) * stack_capacity);
    if (stack == NULL) {
        gsp_debug_log(INFO, "Failed to allocate memory for stack");
        return;
    }

    // Push the initial node onto the stack
    stack[stack_size++] = (gtree_node_impl_t*)node;

    while (stack_size > 0) {
        // Pop a node from the stack
        gtree_node_impl_t* current = stack[--stack_size];

        // Destroy all child nodes
        gtree_node_impl_t* child = current->child;
        while (child != NULL) {
            // Ensure stack has enough capacity
            if (stack_size >= stack_capacity) {
                stack_capacity += INITIAL_STACK_SIZE;
                stack = (gtree_node_impl_t**)realloc(stack, sizeof(gtree_node_impl_t*) * stack_capacity);
                if (stack == NULL) {
                    gsp_debug_log(INFO, "Failed to reallocate memory for stack");
                    return;
                }
            }

            // Push child node onto the stack
            stack[stack_size++] = child;

            // Move to the next sibling
            child = child->sibling;
        }

        // Free the current node
        free(current);
        gsp_debug_log(INFO, "Freed gtree_node_t %lu", (uintptr_t)current);

    }

    // Free the stack memory
    free(stack);
}

bool gsp_tree_check_node_exists(gtree_node_t node) {
    if (root_root == NULL || node == NULL) {
        return false;
    }

    size_t stack_depth = 0;
    const size_t stack_chunk_size = 10;
    size_t stack_allocation = stack_chunk_size;
    
    gtree_node_impl_t** stack = malloc(sizeof(gtree_node_impl_t*) * stack_allocation);
    if (stack == NULL) {
        gsp_debug_log(FAIL, "Failed to allocate memory for stack");
        return false;
    }

    // Initialize the stack with the root nodes
    size_t stack_index = 0;
    gtree_node_impl_t* root = root_root;
    while (root != NULL) {
        if (stack_index >= stack_allocation) {
            stack_allocation += stack_chunk_size;
            stack = realloc(stack, sizeof(gtree_node_impl_t*) * stack_allocation);
            if (stack == NULL) {
                gsp_debug_log(FAIL, "Failed to reallocate memory for stack");
                return false;
            }
        }
        stack[stack_index++] = root;
        root = root->sibling;
    }

    while (stack_depth < stack_index) {
        // Pop the top node from the stack
        gtree_node_impl_t* current = stack[--stack_index];

        // Check if the current node is the one we're searching for
        if (current == (gtree_node_impl_t*)node) {
            free(stack);
            return true;
        }

        // Push all the children of the current node onto the stack
        gtree_node_impl_t* child = current->child;
        while (child != NULL) {
            if (stack_index >= stack_allocation) {
                stack_allocation += stack_chunk_size;
                stack = realloc(stack, sizeof(gtree_node_impl_t*) * stack_allocation);
                if (stack == NULL) {
                    gsp_debug_log(FAIL, "Failed to reallocate memory for stack");
                    return false;
                }
            }
            stack[stack_index++] = child;
            child = child->sibling;
        }
    }

    // If we reach here, the node was not found in the tree
    free(stack);
    return false;
}

void gsp_tree_set_node_data(gtree_node_t node, uintptr_t data) {
    if (!gsp_tree_check_node_exists(node)) {
        gsp_debug_log(FAIL, "Can't set data for invalid gtree_node_t %lu", node);
        return;
    }

    ((gtree_node_impl_t*)node)->data = data;
}

uintptr_t gsp_tree_get_node_data(gtree_node_t node) {
    if (!gsp_tree_check_node_exists(node)) {
        gsp_debug_log(FAIL, "Can't get data for invalid gtree_node_t %lu", node);
        return NULL;
    }

    return ((gtree_node_impl_t*)node)->data;
}
