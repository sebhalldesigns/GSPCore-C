using System;

using OpenGSP.GSPCore.Native;


public static class Program {


    public static void Main(string[] args) {
        Console.WriteLine("Hello Csharp!");

        UIntPtr window = gsp_window.gsp_window_create_window();
        gsp_window.gsp_window_set_title(window, "C# window 👍🏽!");

        while (true) {
            gsp_window.gsp_window_poll_events();
        }
    }


}