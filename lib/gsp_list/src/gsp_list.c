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
    gnode_impl_t* node;
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
    new_list->node = NULL;

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

    gsp_debug_log(INFO, "Allocated glist_t OK %lu", (uintptr_t)new_list);

    return (glist_t)new_list;
}

void gsp_list_destroy_list(glist_t list) {

    if (NULL == lists) {
        gsp_debug_log(WARNING, "No lists to destroy");
        return;
    } else if (list == lists) {
        lists = lists->next;
        free(list);
        gsp_debug_log(INFO, "Freed glist_t %lu", (uintptr_t)list);
        return;
    }

    glist_impl_t* last_list = lists;

    while (NULL != last_list->next) {

        if (list == (glist_t)last_list->next) {
            last_list->next = last_list->next->next;
            free(list);
            gsp_debug_log(INFO, "Freed glist_t %lu", (uintptr_t)list);
        } else {
            last_list = last_list->next;
        }

    }
}

gnode_t gsp_list_create_node() {
    return NULL;
}

void gsp_list_destroy_node(gnode_t node) {

}

bool gsp_list_does_list_exist(glist_t list) {
    return false;
}

bool gsp_list_does_node_exist(glist_t list, gnode_t node) {
    return false;
}

void gsp_list_add_node(glist_t list, gnode_t node) {

}

void gsp_list_remove_node(glist_t list, gnode_t node) {

}

uintptr_t gsp_list_get_node_data(glist_t list, gnode_t node) {
    return NULL;
}

void gsp_list_set_node_data(glist_t list, gnode_t node, uintptr_t data) {

}