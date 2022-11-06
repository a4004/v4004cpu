using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace vassemble
{
    internal sealed class Logger
    {
        public static ConsoleColor defaultColor;
        private static int errorCount = 0;
        private static int warningCount = 0;
        public static void OK()
        {
            Console.Write("[");
            Console.ForegroundColor = ConsoleColor.Green;
            Console.Write("OK");
            Console.ForegroundColor = defaultColor;
            Console.WriteLine("]");
        }
        public static void FAIL(string message)
        {
            Console.Write("[");
            Console.ForegroundColor = ConsoleColor.Red;
            Console.Write("FAIL");
            Console.ForegroundColor = defaultColor;
            Console.WriteLine($"] -> {message}");
            errorCount += 1;
        }
        public static void WARN(string message)
        {
            Console.Write("[");
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.Write("WARN");
            Console.ForegroundColor = defaultColor;
            Console.WriteLine($"] -> {message}");
            warningCount += 1;
        }
        public static void WriteTask(string task)
        {
            Console.Write(task + "... ");
        }
        public static void Finish()
        {
            Console.WriteLine($"===> Build Finished with {errorCount} errors and {warningCount} warnings.");
        }
    }
}
