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
        static List<byte> machineCode = new List<byte>();

        static void Main(string[] args)
        {
            Logger.defaultColor = Console.ForegroundColor;

            if (args.Length < 2)
            {
                Console.Error.WriteLine("Usage: vassemble <input file> <output file>");
                return;
            }

            Logger.WriteTask($"Checking input file");

            if (!File.Exists(args[0]))
            {
                Logger.FAIL("File does not exist!");
                Console.Error.WriteLine($"Failed to assemble the input file: {args[0]} due to an error. The file does not exist.");
                Console.Error.WriteLine("Usage: vassemble <input file> <output file>");
                return;
            }

            Logger.OK();

            try
            {
                Logger.WriteTask("Reading input file");
                string[] lines = File.ReadAllLines(args[0]);
                Logger.OK();

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

                Logger.WriteTask("Created Symbol Table");
                Logger.OK();

                Console.WriteLine("Processing the file...");

                for (int i = 0; i < lines.Length; i++)
                {
                    string line = lines[i].ToLower();

                    if (line.StartsWith("_"))
                    {
                        Logger.WriteTask($"  -> Parsing line {i}");
                        Logger.WARN("Possible memory region within code section. Memory regions in source code are dangerous to use in this architecture.");
                    }
                    if (line.Contains(":"))
                        line = line.Split(':')[1].Trim();

                    string[] tokens = line.Split(' ');

                    switch(tokens.Length)
                    {
                        case 1:
                            SingleToken(tokens[0].Trim());
                            break;
                        case 2:
                            DoubleToken(tokens[0].Trim(), tokens[1].Trim());
                            break;
                        case 3:
                            break;
                    }                   
                }

                Logger.WriteTask("Assembling the program");

                ushort size = (ushort)machineCode.ToArray().Length;
                machineCode.Insert(0, (byte)(size & 0x00FF));
                machineCode.Insert(1, (byte)((size & 0xFF00) >> 8));

                File.WriteAllBytes($"{args[1]}", machineCode.ToArray());

                Logger.OK();
            }
            catch (Exception e)
            {
                Logger.FAIL(e.Message);
                Console.Error.WriteLine($"Failed to assemble the input file due to a critical failure. {e}");
            }

            Logger.Finish();
        }

        static void AddLEWord(ushort data)
        {
            machineCode.Add((byte)(data & 0x00FF));
            machineCode.Add((byte)((data & 0xFF00) >> 8));
        }

        static void DoubleToken(string token1, string token2)
        {
            Logger.WriteTask($"  -> Parsing double token \"{token1} {token2}\"");

            switch (token1)
            {
                case "call":
                    AddLEWord(0x0100);
                    AddLEWord(DataParser.ResolveAddressFromToken(token2, ref symbolTable));
                    Logger.OK();
                    break;
                case "tin":
                    AddLEWord(0xF000);
                    AddLEWord(DataParser.ResolveAddressFromToken(token2, ref symbolTable));
                    Logger.OK();
                    break;
                case "tout":
                    if (token2.StartsWith("#"))
                    {
                        AddLEWord(0xF100);
                        AddLEWord(DataParser.ResolveValueFromToken(token2));
                    }
                    else if (token2.StartsWith("&") || token2.StartsWith("_"))
                    {
                        AddLEWord(0xF101);
                        AddLEWord(DataParser.ResolveAddressFromToken(token2, ref symbolTable));
                    }
                    else
                    {
                        switch(token2)
                        {
                            case "ra":
                                AddLEWord(0xF10A);
                                AddLEWord(0x0300); // nop padding
                                break;
                            case "rb":
                                AddLEWord(0xF10B);
                                AddLEWord(0x0300); // nop padding
                                break;
                            case "rc":
                                AddLEWord(0xF10C);
                                AddLEWord(0x0300); // nop padding
                                break;
                            case "rd":
                                AddLEWord(0xF10D);
                                AddLEWord(0x0300); // nop padding
                                break;
                            default:
                                Logger.FAIL($"Unrecognized token operand: \"{token2}\", skipping!");
                                break;
                        }
                    }
                    Logger.OK();
                    break;
                case "lda":
                    if (token2.StartsWith("#"))
                    {
                        AddLEWord(0x0FA0);
                        AddLEWord(DataParser.ResolveValueFromToken(token2));
                    }
                    else if (token2.StartsWith("&") || token2.StartsWith("_"))
                    {
                        AddLEWord(0x0FA1);
                        AddLEWord(DataParser.ResolveAddressFromToken(token2, ref symbolTable));
                    }
                    Logger.OK();
                    break;
                case "ldb":
                    if (token2.StartsWith("#"))
                    {
                        AddLEWord(0x0FB0);
                        AddLEWord(DataParser.ResolveValueFromToken(token2));
                    }
                    else if (token2.StartsWith("&") || token2.StartsWith("_"))
                    {
                        AddLEWord(0x0FB1);
                        AddLEWord(DataParser.ResolveAddressFromToken(token2, ref symbolTable));
                    }
                    Logger.OK();
                    break;
                case "ldc":
                    if (token2.StartsWith("#"))
                    {
                        AddLEWord(0x0FC0);
                        AddLEWord(DataParser.ResolveValueFromToken(token2));
                    }
                    else if (token2.StartsWith("&") || token2.StartsWith("_"))
                    {
                        AddLEWord(0x0FC1);
                        AddLEWord(DataParser.ResolveAddressFromToken(token2, ref symbolTable));
                    }
                    Logger.OK();
                    break;
                case "ldd":
                    if (token2.StartsWith("#"))
                    {
                        AddLEWord(0x0FD0);
                        AddLEWord(DataParser.ResolveValueFromToken(token2));
                    }
                    else if (token2.StartsWith("&") || token2.StartsWith("_"))
                    {
                        AddLEWord(0x0FD1);
                        AddLEWord(DataParser.ResolveAddressFromToken(token2, ref symbolTable));
                    }
                    Logger.OK();
                    break;
                case "sta":
                    AddLEWord(0x0EA1);
                    AddLEWord(DataParser.ResolveAddressFromToken(token2, ref symbolTable));
                    Logger.OK();
                    break;
                case "stb":
                    AddLEWord(0x0EB1);
                    AddLEWord(DataParser.ResolveAddressFromToken(token2, ref symbolTable));
                    Logger.OK();
                    break;
                case "stc":
                    AddLEWord(0x0EC1);
                    AddLEWord(DataParser.ResolveAddressFromToken(token2, ref symbolTable));
                    Logger.OK();
                    break;
                case "std":
                    AddLEWord(0x0ED1);
                    AddLEWord(DataParser.ResolveAddressFromToken(token2, ref symbolTable));
                    Logger.OK();
                    break;
                case "push":
                    switch (token2)
                    {
                        case "ra":
                            AddLEWord(0x020A);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        case "rb":
                            AddLEWord(0x020B);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        case "rc":
                            AddLEWord(0x020C);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        case "rd":
                            AddLEWord(0x020D);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        default:
                            Logger.FAIL($"Unrecognized token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                case "pop":
                    switch (token2)
                    {
                        case "ra":
                            AddLEWord(0x021A);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        case "rb":
                            AddLEWord(0x021B);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        case "rc":
                            AddLEWord(0x021C);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        case "rd":
                            AddLEWord(0x021D);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        default:
                            Logger.FAIL($"Unrecognized token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                case "jmp":
                    AddLEWord(0x03F1);
                    AddLEWord(DataParser.ResolveAddressFromToken(token2, ref symbolTable));
                    Logger.OK();
                    break;
                case "jmz":
                    AddLEWord(0x03E1);
                    AddLEWord(DataParser.ResolveAddressFromToken(token2, ref symbolTable));
                    Logger.OK();
                    break;
                case "jmn":
                    AddLEWord(0x03D1);
                    AddLEWord(DataParser.ResolveAddressFromToken(token2, ref symbolTable));
                    Logger.OK();
                    break;
                case "jmo":
                    AddLEWord(0x03C1);
                    AddLEWord(DataParser.ResolveAddressFromToken(token2, ref symbolTable));
                    Logger.OK();
                    break;
                case "jpp":
                    AddLEWord(0x03B1);
                    AddLEWord(DataParser.ResolveAddressFromToken(token2, ref symbolTable));
                    Logger.OK();
                    break;
                case "not":
                    switch (token2)
                    {
                        case "ra":
                            AddLEWord(0x050A);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        case "rb":
                            AddLEWord(0x050B);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        case "rc":
                            AddLEWord(0x050C);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        case "rd":
                            AddLEWord(0x050D);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        default:
                            Logger.FAIL($"Unrecognized token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                case "lsh":
                    switch (token2)
                    {
                        case "ra":
                            AddLEWord(0x05AA);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        case "rb":
                            AddLEWord(0x05AB);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        case "rc":
                            AddLEWord(0x05AC);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        case "rd":
                            AddLEWord(0x05AD);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        default:
                            Logger.FAIL($"Unrecognized token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                case "rsh":
                    switch (token2)
                    {
                        case "ra":
                            AddLEWord(0x05BA);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        case "rb":
                            AddLEWord(0x05BB);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        case "rc":
                            AddLEWord(0x05BC);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        case "rd":
                            AddLEWord(0x05BD);
                            AddLEWord(0x0300); // nop padding
                            Logger.OK();
                            break;
                        default:
                            Logger.FAIL($"Unrecognized token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                default:
                    Logger.FAIL($"Unrecognized token: \"{token1}\", skipping!");
                    break;
            }
        }
        static void SingleToken(string token)
        {
            Logger.WriteTask($"  -> Parsing single token \"{token}\"");

            switch (token)
            {
                case "end":
                    AddLEWord(0x0000);
                    AddLEWord(0x0300); // nop padding
                    Logger.OK();
                    break;
                case "ret":
                    AddLEWord(0x0101);
                    AddLEWord(0x0300); // nop padding
                    Logger.OK();
                    break;
                case "nop":
                    AddLEWord(0x0300);
                    AddLEWord(0x0300); // nop padding
                    Logger.OK();
                    break;
                default:
                    Logger.FAIL($"Unrecognized token: \"{token}\", skipping!");
                    break;
            }
        }


    }
}
