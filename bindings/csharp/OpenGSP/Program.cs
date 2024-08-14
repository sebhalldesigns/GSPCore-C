using System;

using OpenGSP.GSPCore.Native;


public static class Program {


    public static void Main(string[] args) {
        Console.WriteLine("Hello Csharp!");

        var list1 = gsp_list.gsp_list_create_list();
        var list2 = gsp_list.gsp_list_create_list();
        var list3 = gsp_list.gsp_list_create_list();

        gsp_list.gsp_list_destroy_list(list3);
        gsp_list.gsp_list_destroy_list(list2);
        gsp_list.gsp_list_destroy_list(list1);
    }


}