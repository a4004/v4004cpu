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
        static Dictionary<string, ushort> symbolTable = new Dictionary<string, ushort>();
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

                    if (line.StartsWith("_"))
                    {
                        WriteTask($"  -> Parsing line {i}");
                        WARN("Possible memory region within code section. Memory regions in source code are dangerous to use in this architecture.");
                    }
                    if (line.Contains(":"))
                        line = line.Split(':')[1].Trim();

                    string[] tokens = line.Split(' ');

                    switch(tokens.Length)
                    {
                        case 1:
                            SingleToken(tokens[0].Trim(), ref machineCode);
                            break;
                        case 2:
                            DoubleToken(tokens[0].Trim(), tokens[1].Trim(), ref machineCode);
                            break;
                        case 3:
                            break;
                    }                   
                }

                WriteTask("Assembling the program");

                ushort size = (ushort)machineCode.ToArray().Length;
                machineCode.Insert(0, (byte)(size & 0x00FF));
                machineCode.Insert(1, (byte)((size & 0xFF00) >> 8));

                File.WriteAllBytes($"{args[1]}", machineCode.ToArray());

                OK();
            }
            catch (Exception e)
            {
                FAIL(e.Message);
                Console.Error.WriteLine($"Failed to assemble the input file due to a critical failure. {e}");
            }

            Console.WriteLine($"===> Build Finished with {errorCount} errors and {warningCount} warnings.");
        }

        static void AddLEWord(ushort data, ref List<byte> machineCode)
        {
            machineCode.Add((byte)(data & 0x00FF));
            machineCode.Add((byte)((data & 0xFF00) >> 8));
        }

        static ushort ResolveValueFromToken(string token)
        {
            ushort result = ushort.Parse(token.Replace("#", ""), System.Globalization.NumberStyles.HexNumber);
            return result;
        }

        static ushort ResolveAddressFromToken(string token)
        {
            try
            {
                ushort result = ushort.Parse(token.Replace("&", ""), System.Globalization.NumberStyles.HexNumber);
                return result;
            }
            catch
            {
                if (symbolTable.TryGetValue(token, out ushort result))
                    return result;
                else
                    throw new ApplicationException($"Could not resolve the memory address from token \"{token}\"!");
            }
        }

        static void DoubleToken(string token1, string token2, ref List<byte> machineCode)
        {
            WriteTask($"  -> Parsing double token \"{token1} {token2}\"");

            switch (token1)
            {
                case "call":
                    AddLEWord(0x0100, ref machineCode);
                    AddLEWord(ResolveAddressFromToken(token2), ref machineCode);
                    OK();
                    break;
                case "tin":
                    AddLEWord(0xF000, ref machineCode);
                    AddLEWord(ResolveAddressFromToken(token2), ref machineCode);
                    OK();
                    break;
                case "tout":
                    if (token2.StartsWith("#"))
                    {
                        AddLEWord(0xF100, ref machineCode);
                        AddLEWord(ResolveValueFromToken(token2), ref machineCode);
                    }
                    else if (token2.StartsWith("&") || token2.StartsWith("_"))
                    {
                        AddLEWord(0xF101, ref machineCode);
                        AddLEWord(ResolveAddressFromToken(token2), ref machineCode);
                    }
                    else
                    {
                        switch(token2)
                        {
                            case "ra":
                                AddLEWord(0xF10A, ref machineCode);
                                break;
                            case "rb":
                                AddLEWord(0xF10B, ref machineCode);
                                break;
                            case "rc":
                                AddLEWord(0xF10C, ref machineCode);
                                break;
                            case "rd":
                                AddLEWord(0xF10D, ref machineCode);
                                break;
                            default:
                                FAIL($"Unrecognized token operand: \"{token2}\", skipping!");
                                break;
                        }
                    }
                    OK();
                    break;
                case "lda":
                    if (token2.StartsWith("#"))
                    {
                        AddLEWord(0x0FA0, ref machineCode);
                        AddLEWord(ResolveValueFromToken(token2), ref machineCode);
                    }
                    else if (token2.StartsWith("&") || token2.StartsWith("_"))
                    {
                        AddLEWord(0x0FA1, ref machineCode);
                        AddLEWord(ResolveAddressFromToken(token2), ref machineCode);
                    }
                    OK();
                    break;
                case "ldb":
                    if (token2.StartsWith("#"))
                    {
                        AddLEWord(0x0FB0, ref machineCode);
                        AddLEWord(ResolveValueFromToken(token2), ref machineCode);
                    }
                    else if (token2.StartsWith("&") || token2.StartsWith("_"))
                    {
                        AddLEWord(0x0FB1, ref machineCode);
                        AddLEWord(ResolveAddressFromToken(token2), ref machineCode);
                    }
                    OK();
                    break;
                case "ldc":
                    if (token2.StartsWith("#"))
                    {
                        AddLEWord(0x0FC0, ref machineCode);
                        AddLEWord(ResolveValueFromToken(token2), ref machineCode);
                    }
                    else if (token2.StartsWith("&") || token2.StartsWith("_"))
                    {
                        AddLEWord(0x0FC1, ref machineCode);
                        AddLEWord(ResolveAddressFromToken(token2), ref machineCode);
                    }
                    OK();
                    break;
                case "ldd":
                    if (token2.StartsWith("#"))
                    {
                        AddLEWord(0x0FD0, ref machineCode);
                        AddLEWord(ResolveValueFromToken(token2), ref machineCode);
                    }
                    else if (token2.StartsWith("&") || token2.StartsWith("_"))
                    {
                        AddLEWord(0x0FD1, ref machineCode);
                        AddLEWord(ResolveAddressFromToken(token2), ref machineCode);
                    }
                    OK();
                    break;
                case "sta":
                    AddLEWord(0x0EA1, ref machineCode);
                    AddLEWord(ResolveAddressFromToken(token2), ref machineCode);
                    OK();
                    break;
                case "stb":
                    AddLEWord(0x0EB1, ref machineCode);
                    AddLEWord(ResolveAddressFromToken(token2), ref machineCode);
                    OK();
                    break;
                case "stc":
                    AddLEWord(0x0EC1, ref machineCode);
                    AddLEWord(ResolveAddressFromToken(token2), ref machineCode);
                    OK();
                    break;
                case "std":
                    AddLEWord(0x0ED1, ref machineCode);
                    AddLEWord(ResolveAddressFromToken(token2), ref machineCode);
                    OK();
                    break;
                case "push":
                    switch (token2)
                    {
                        case "ra":
                            AddLEWord(0x020A, ref machineCode);
                            OK();
                            break;
                        case "rb":
                            AddLEWord(0x020B, ref machineCode);
                            OK();
                            break;
                        case "rc":
                            AddLEWord(0x020C, ref machineCode);
                            OK();
                            break;
                        case "rd":
                            AddLEWord(0x020D, ref machineCode);
                            OK();
                            break;
                        default:
                            FAIL($"Unrecognized token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                case "pop":
                    switch (token2)
                    {
                        case "ra":
                            AddLEWord(0x021A, ref machineCode);
                            OK();
                            break;
                        case "rb":
                            AddLEWord(0x021B, ref machineCode);
                            OK();
                            break;
                        case "rc":
                            AddLEWord(0x021C, ref machineCode);
                            OK();
                            break;
                        case "rd":
                            AddLEWord(0x021D, ref machineCode);
                            OK();
                            break;
                        default:
                            FAIL($"Unrecognized token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                case "jmp":
                    AddLEWord(0x03F1, ref machineCode);
                    AddLEWord(ResolveAddressFromToken(token2), ref machineCode);
                    OK();
                    break;
                case "jmz":
                    AddLEWord(0x03E1, ref machineCode);
                    AddLEWord(ResolveAddressFromToken(token2), ref machineCode);
                    OK();
                    break;
                case "jmn":
                    AddLEWord(0x03D1, ref machineCode);
                    AddLEWord(ResolveAddressFromToken(token2), ref machineCode);
                    OK();
                    break;
                case "jmo":
                    AddLEWord(0x03C1, ref machineCode);
                    AddLEWord(ResolveAddressFromToken(token2), ref machineCode);
                    OK();
                    break;
                case "jpp":
                    AddLEWord(0x03B1, ref machineCode);
                    AddLEWord(ResolveAddressFromToken(token2), ref machineCode);
                    OK();
                    break;
                case "not":
                    switch (token2)
                    {
                        case "ra":
                            AddLEWord(0x050A, ref machineCode);
                            OK();
                            break;
                        case "rb":
                            AddLEWord(0x050B, ref machineCode);
                            OK();
                            break;
                        case "rc":
                            AddLEWord(0x050C, ref machineCode);
                            OK();
                            break;
                        case "rd":
                            AddLEWord(0x050D, ref machineCode);
                            OK();
                            break;
                        default:
                            FAIL($"Unrecognized token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                case "lsh":
                    switch (token2)
                    {
                        case "ra":
                            AddLEWord(0x05AA, ref machineCode);
                            OK();
                            break;
                        case "rb":
                            AddLEWord(0x05AB, ref machineCode);
                            OK();
                            break;
                        case "rc":
                            AddLEWord(0x05AC, ref machineCode);
                            OK();
                            break;
                        case "rd":
                            AddLEWord(0x05AD, ref machineCode);
                            OK();
                            break;
                        default:
                            FAIL($"Unrecognized token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                case "rsh":
                    switch (token2)
                    {
                        case "ra":
                            AddLEWord(0x05BA, ref machineCode);
                            OK();
                            break;
                        case "rb":
                            AddLEWord(0x05BB, ref machineCode);
                            OK();
                            break;
                        case "rc":
                            AddLEWord(0x05BC, ref machineCode);
                            OK();
                            break;
                        case "rd":
                            AddLEWord(0x05BD, ref machineCode);
                            OK();
                            break;
                        default:
                            FAIL($"Unrecognized token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                default:
                    FAIL($"Unrecognized token: \"{token1}\", skipping!");
                    break;
            }
        }
        static void SingleToken(string token, ref List<byte> machineCode)
        {
            WriteTask($"  -> Parsing single token \"{token}\"");

            switch (token)
            {
                case "end":
                    AddLEWord(0x0000, ref machineCode);
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
