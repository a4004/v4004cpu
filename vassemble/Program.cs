using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace vassemble
{
    internal class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 1)
            {
                Console.Error.WriteLine("Usage: vassemble <input file>");
                return;
            }

            if (!File.Exists(args[0]))
            {
                Console.Error.WriteLine($"Failed to assemble the input file: {args[0]} due to an error. The file does not exist.");
                Console.Error.WriteLine("Usage: vassemble <input file>");
                return;
            }

            try
            {
                string[] lines = File.ReadAllLines(args[0]);
          
                List<ushort> machineCode = new List<ushort>();
                Dictionary<string, ushort> symbolTable = new Dictionary<string, ushort>();

                for (int i = 0; i < lines.Length; i++)
                {
                    if (!lines[i].Contains(":"))
                        continue;
                }
            }
            catch (Exception e)
            {
                Console.Error.WriteLine($"Failed to assemble the input file due to a critical failure. {e}");
            }
        }
    }
}
