#include <gsp_list/gsp_list.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gsp_debug/gsp_debug.h>


typedef struct gnode_impl_t {
    struct gnode_impl_t* next;
    uintptr_t data;
} gnode_impl_t;

typedef struct glist_impl_t {
    struct glist_impl_t* next;
    gnode_impl_t* nodes;
} glist_impl_t;



static glist_impl_t* lists = NULL;

glist_t gsp_list_create_list() {

    glist_impl_t* new_list = (glist_impl_t*)malloc(sizeof(glist_impl_t));

    if (new_list == NULL) {
        gsp_debug_log(FAIL, "Failed to allocate glist_t");
        return NULL;
    }

    // valid allocation

    new_list->next = NULL;
    new_list->nodes = NULL;

    if (NULL == lists) {
        // first glist allocated
        lists = new_list;
    } else {
        glist_impl_t* last_list = lists;

        while (NULL != last_list->next) {
            last_list = last_list->next;
        }

        last_list->next  = new_list;
    }

    gsp_debug_log(INFO, "Allocated glist_t at %lu", (uintptr_t)new_list);

    return (glist_t)new_list;
}

void gsp_list_destroy_list(glist_t list) {

    if (NULL == lists) {
        gsp_debug_log(WARNING, "No glist_t to destroy");
        return;
    } else if (list == lists) {
        lists = lists->next;
        free(list);
        gsp_debug_log(INFO, "Freed glist_t at %lu", (uintptr_t)list);
        return;
    }

    glist_impl_t* last_list = lists;

    while (NULL != last_list->next) {

        if (list == (glist_t)last_list->next) {
            last_list->next = last_list->next->next;
            free(list);
            gsp_debug_log(INFO, "Freed glist_t at %lu", (uintptr_t)list);
        } else {
            last_list = last_list->next;
        }

    }
}

gnode_t gsp_list_create_node(glist_t list) {

    if (!gsp_list_does_list_exist(list)) {
        gsp_debug_log(FAIL, "Can't create new gnode_t for invalid glist_t %lu", list);
        return NULL;
    }

    gnode_impl_t* new_node = (gnode_impl_t*)malloc(sizeof(gnode_impl_t));

    if (new_node == NULL) {
        gsp_debug_log(FAIL, "Failed to allocate gnode_t");
        return NULL;
    }

    // valid allocation

    new_node->next = NULL;
    new_node->data = NULL;

    if (NULL == ((glist_impl_t*)list)->nodes) {
        // first gnode allocated
        ((glist_impl_t*)list)->nodes = new_node;
    } else {
        gnode_impl_t* last_node = ((glist_impl_t*)list)->nodes;

        while (NULL != last_node->next) {
            last_node = last_node->next;
        }

        last_node->next  = new_node;
    }

    gsp_debug_log(INFO, "Allocated gnode_t at %lu", (uintptr_t)new_node);

    return (gnode_t)new_node;

    return NULL;
}

void gsp_list_destroy_node(glist_t list, gnode_t node) {

    if (!gsp_list_does_list_exist(list)) {
        gsp_debug_log(FAIL, "Can't destroy gnode_t %lu for invalid glist_t %lu", node, list);
        return NULL;
    }

    if (NULL == ((glist_impl_t*)list)->nodes) {
        gsp_debug_log(WARNING, "No glist_t to destroy");
        return;
    } else if (node == ((glist_impl_t*)list)->nodes) {
        ((glist_impl_t*)list)->nodes = ((glist_impl_t*)list)->nodes->next;
        free(node);
        gsp_debug_log(INFO, "Freed gnode_t at %lu", (uintptr_t)node);
        return;
    }

    gnode_impl_t* last_node = ((glist_impl_t*)list)->nodes;

    while (NULL != last_node->next) {

        if (node == (gnode_t)last_node->next) {
            last_node->next = last_node->next->next;
            free(node);
            gsp_debug_log(INFO, "Freed gnode_t at %lu", (uintptr_t)node);
        } else {
            last_node = last_node->next;
        }

    }
}

bool gsp_list_does_list_exist(glist_t list) {

    if (NULL == lists) {
        // no lists allocated
        return false;
    }

    glist_impl_t* last_list = lists;

    while (last_list != NULL) {
        
        if (list == last_list) {
            // found this list so escape
            return true;
        }

        last_list = last_list->next;
    }

    return false;
}

bool gsp_list_does_node_exist(glist_t list, gnode_t node) {

    if (!gsp_list_does_list_exist(list)) {
        gsp_debug_log(FAIL, "glist_t %lu doesn't exist", list);
        return false;
    }

    if (NULL == ((glist_impl_t*)list)->nodes) {
        // no nodes allocated
        return false;
    }

    gnode_impl_t* last_node = ((glist_impl_t*)list)->nodes;

    while (last_node != NULL) {
        
        if (node == last_node) {
            // found this node so escape
            return true;
        }

        last_node = last_node->next;
    }

    return false;
}

size_t gsp_list_get_node_count(glist_t list) {
    if (!gsp_list_does_list_exist(list)) {
        gsp_debug_log(FAIL, "Can't count invalid glist_t %lu", list);
        return 0;
    }   

    size_t count = 0;

    gnode_impl_t* last_node = ((glist_impl_t*)list)->nodes;

    while (last_node != NULL) {
        
        count++;

        last_node = last_node->next;
    }

    return count;
}

uintptr_t gsp_list_get_node_data(glist_t list, gnode_t node) {

    if (!gsp_list_does_node_exist(list, node)) {
        gsp_debug_log(FAIL, "gnode_t %lu doesn't exist in glist_t %lu", node, list);
        return NULL;
    }

    return ((gnode_impl_t*)node)->data;
}

void gsp_list_set_node_data(glist_t list, gnode_t node, uintptr_t data) {

    if (!gsp_list_does_node_exist(list, node)) {
        gsp_debug_log(FAIL, "gnode_t %lu doesn't exist in glist_t %lu", node, list);
        return;
    }

    ((gnode_impl_t*)node)->data = data;
}