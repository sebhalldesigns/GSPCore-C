using GSP.NET.Core.Native;

public static class Program {
    public static void Main(string[] args) {
        glist_t list1 = gsp_list.gsp_list_create_list();
        glist_t list2 = gsp_list.gsp_list_create_list();
        glist_t list3 = gsp_list.gsp_list_create_list();

        gnode_t node1 = gsp_list.gsp_list_create_node(list1);
        gnode_t node2 = gsp_list.gsp_list_create_node(list1);
        gnode_t node3 = gsp_list.gsp_list_create_node(list1);

        gsp_list.gsp_list_set_node_data(list1, node1, 1);
        gsp_list.gsp_list_set_node_data(list1, node2, 2);
        gsp_list.gsp_list_set_node_data(list1, node3, 3);

        Console.WriteLine($"{gsp_list.gsp_list_get_node_data(list1, node1)}");
        Console.WriteLine($"{gsp_list.gsp_list_get_node_data(list2, node2)}");
        Console.WriteLine($"{gsp_list.gsp_list_get_node_data(list1, node3)}");

        Console.WriteLine($"List1 count {gsp_list.gsp_list_get_node_count(list1)}");

        gsp_list.gsp_list_destroy_node(list1, node1);
        gsp_list.gsp_list_destroy_node(list1, node2);
        gsp_list.gsp_list_destroy_node(list1, node3);

        gsp_list.gsp_list_destroy_list(list1);
        gsp_list.gsp_list_destroy_list(list2);
        gsp_list.gsp_list_destroy_list(list3);

    }
}