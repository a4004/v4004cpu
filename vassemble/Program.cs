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
        static ConsoleColor defaultColor;
        static int errorCount = 0;
        static int warningCount = 0;

        static void OK()
        {
            Console.Write("[");
            Console.ForegroundColor = ConsoleColor.Green;
            Console.Write("OK");
            Console.ForegroundColor = defaultColor;
            Console.WriteLine("]");
        }
        static void FAIL(string message)
        {
            Console.Write("[");
            Console.ForegroundColor = ConsoleColor.Red;
            Console.Write("FAIL");
            Console.ForegroundColor = defaultColor;
            Console.WriteLine($"] -> {message}");
            errorCount += 1;
        }
        static void WARN(string message)
        {
            Console.Write("[");
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.Write("WARN");
            Console.ForegroundColor = defaultColor;
            Console.WriteLine($"] -> {message}");
            warningCount += 1;
        }
        static void WriteTask(string task)
        {
            Console.Write(task + "... ");
        }



        static void Main(string[] args)
        {
            defaultColor = Console.ForegroundColor;

            if (args.Length < 2)
            {
                Console.Error.WriteLine("Usage: vassemble <input file> <output file>");
                return;
            }

            WriteTask($"Checking input file");

            if (!File.Exists(args[0]))
            {
                FAIL("File does not exist!");
                Console.Error.WriteLine($"Failed to assemble the input file: {args[0]} due to an error. The file does not exist.");
                Console.Error.WriteLine("Usage: vassemble <input file> <output file>");
                return;
            }

            OK();

            try
            {
                WriteTask("Reading input file");
                string[] lines = File.ReadAllLines(args[0]);
                OK();

                List<byte> machineCode = new List<byte>();
                Dictionary<string, ushort> symbolTable = new Dictionary<string, ushort>();

                Console.WriteLine("Creating Symbol Table...");

                for (int i = 0; i < lines.Length; i++)
                {
                    if (!lines[i].Contains(":"))
                        continue;

                    string name = lines[i].Split(':')[0];
                    ushort addr = (ushort)(lines.ToList().IndexOf(lines[i]) * 4);

                    Console.WriteLine($"  -> Found Symbol: @{name} at 0x{addr.ToString("X4")}");

                    symbolTable.Add(name, addr);
                }

                WriteTask("Created Symbol Table");
                OK();

                Console.WriteLine("Processing the file...");

                for (int i = 0; i < lines.Length; i++)
                {
                    string line = lines[i].ToLower();
                     
                    if (line.Contains(":"))
                        line = line.Split(':')[1].Trim();

                    string[] tokens = line.Split(' ');

                    switch(tokens.Length)
                    {
                        case 1:
                            SingleWord(tokens[0], ref machineCode);
                            break;
                        case 2:
                            break;
                        case 4:
                            break;
                    }                   
                }

                WriteTask("Assembling the program");

                File.WriteAllBytes($"{args[0]}.v4004", machineCode.ToArray());

                OK();
            }
            catch (Exception e)
            {
                FAIL(e.Message);
                Console.Error.WriteLine($"Failed to assemble the input file due to a critical failure. {e}");
            }

            Console.WriteLine($"===> Build Finished\n\tErrors: {errorCount}\n\tWarnings: {warningCount}\n");
        }

        static void AddLEWord(ushort data, ref List<byte> machineCode)
        {
            machineCode.Add((byte)(data & 0x00FF));
            machineCode.Add((byte)((data & 0xFF00) >> 8));
        }
        static void SingleWord(string token, ref List<byte> machineCode)
        {
            WriteTask($"Parsing single token \"{token}\"");

            switch (token)
            {
                case "end":
                    AddLEWord(0xF100, ref machineCode);
                    OK();
                    break;
                case "ret":
                    AddLEWord(0x0101, ref machineCode);
                    OK();
                    break;
                case "nop":
                    AddLEWord(0x0300, ref machineCode);
                    OK();
                    break;
                default:
                    FAIL($"Unrecognized token: \"{token}\", skipping!");
                    break;
            }
        }


    }
}
