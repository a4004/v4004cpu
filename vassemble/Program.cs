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
                        Logger.WriteTask($"  -> Parsing line {i + 1} as memory region");
                        Logger.OK();

                        continue;
                    }
                    if (line.Contains(":"))
                        line = line.Split(':')[1].Trim();

                    string[] tokens = line.Split(' ');

                    switch(tokens.Length)
                    {
                        case 1:
                            Logger.WriteTask($"  -> Parsing line {i + 1}");
                            Logger.OK();
                            SingleToken(tokens[0].Trim());
                            break;
                        case 2:
                            Logger.WriteTask($"  -> Parsing line {i + 1}");
                            Logger.OK();
                            DoubleToken(tokens[0].Trim(), tokens[1].Trim());
                            break;
                        case 3:
                            Logger.WriteTask($"  -> Parsing line {i + 1}");
                            Logger.OK();
                            TripleToken(tokens[0].Trim(), tokens[1].Trim(), tokens[2].Trim());
                            break;
                        default:
                            Logger.WriteTask($"  -> Parsing line {i + 1}");
                            Logger.WARN($"Unrecognized string of tokens \"{line}\"");
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

        static void TripleToken(string token1, string token2, string token3)
        {
            Logger.WriteTask($"  -> Parsing triple token \"{token1} {token2} {token3}\"");

            switch (token1)
            {
                case "add":
                    switch (token2)
                    {
                        case "ra":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x0DA0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x0DA1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x0DAA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x0DAB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x0DAC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x0DAD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rb":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x0DB0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x0DB1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x0DBA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x0DBB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x0DBC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x0DBD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rc":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x0DC0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x0DC1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x0DCA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x0DCB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x0DCC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x0DCD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rd":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x0DD0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x0DD1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x0DDA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x0DDB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x0DDC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x0DDD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        default:
                            Logger.FAIL($"Unrecognized first token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                case "sub":
                    switch (token2)
                    {
                        case "ra":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x0CA0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x0CA1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x0CAA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x0CAB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x0CAC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x0CAD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rb":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x0CB0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x0CB1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x0CBA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x0CBB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x0CBC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x0CBD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rc":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x0CC0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x0CC1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x0CCA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x0CCB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x0CCC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x0CCD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rd":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x0CD0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x0CD1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x0CDA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x0CDB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x0CDC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x0CDD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        default:
                            Logger.FAIL($"Unrecognized first token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                case "div":
                    switch (token2)
                    {
                        case "ra":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x0BA0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x0BA1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x0BAA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x0BAB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x0BAC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x0BAD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rb":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x0BB0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x0BB1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x0BBA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x0BBB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x0BBC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x0BBD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rc":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x0BC0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x0BC1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x0BCA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x0BCB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x0BCC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x0BCD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rd":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x0BD0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x0BD1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x0BDA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x0BDB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x0BDC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x0BDD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        default:
                            Logger.FAIL($"Unrecognized first token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                case "mul":
                    switch (token2)
                    {
                        case "ra":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x0AA0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x0AA1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x0AAA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x0AAB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x0AAC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x0AAD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rb":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x0AB0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x0AB1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x0ABA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x0ABB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x0ABC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x0ABD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rc":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x0AC0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x0AC1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x0ACA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x0ACB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x0ACC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x0ACD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rd":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x0AD0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x0AD1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x0ADA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x0ADB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x0ADC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x0ADD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        default:
                            Logger.FAIL($"Unrecognized first token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                case "and":
                    switch (token2)
                    {
                        case "ra":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x09A0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x09A1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x09AA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x09AB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x09AC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x09AD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rb":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x09B0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x09B1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x09BA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x09BB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x09BC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x09BD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rc":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x09C0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x09C1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x09CA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x09CB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x09CC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x09CD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rd":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x09D0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x09D1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x09DA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x09DB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x09DC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x09DD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        default:
                            Logger.FAIL($"Unrecognized first token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                case "or":
                    switch (token2)
                    {
                        case "ra":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x08A0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x08A1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x08AA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x08AB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x08AC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x08AD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rb":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x08B0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x08B1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x08BA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x08BB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x08BC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x08BD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rc":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x08C0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x08C1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x08CA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x08CB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x08CC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x08CD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rd":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x08D0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x08D1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x08DA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x08DB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x08DC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x08DD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        default:
                            Logger.FAIL($"Unrecognized first token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                case "xor":
                    switch (token2)
                    {
                        case "ra":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x07A0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x07A1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x07AA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x07AB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x07AC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x07AD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rb":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x07B0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x07B1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x07BA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x07BB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x07BC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x07BD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rc":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x07C0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x07C1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x07CA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x07CB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x07CC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x07CD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        case "rd":
                            if (token3.StartsWith("#"))
                            {
                                AddLEWord(0x07D0);
                                AddLEWord(DataParser.ResolveValueFromToken(token3));
                            }
                            else if (token2.StartsWith("&") || token2.StartsWith("_"))
                            {
                                AddLEWord(0x07D1);
                                AddLEWord(DataParser.ResolveAddressFromToken(token3, ref symbolTable));
                            }
                            else
                            {
                                switch (token2)
                                {
                                    case "ra":
                                        AddLEWord(0x07DA);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rb":
                                        AddLEWord(0x07DB);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rc":
                                        AddLEWord(0x07DC);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    case "rd":
                                        AddLEWord(0x07DD);
                                        AddLEWord(0x0300); // nop padding
                                        break;
                                    default:
                                        Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                                        break;
                                }
                            }
                            break;
                        default:
                            Logger.FAIL($"Unrecognized first token operand: \"{token2}\", skipping!");
                            break;
                    }
                    break;
                case "cpy":
                    byte cpy_opcode = 0x06;
                    byte cpy_opr0 = 0;
                    byte cpy_opr1 = 0;
                    switch (token2)
                    {
                        case "ra":
                            cpy_opr0 = 0xA;
                            break;
                        case "rb":
                            cpy_opr0 = 0xB;
                            break;
                        case "rc":
                            cpy_opr0 = 0xC;
                            break;
                        case "rd":
                            cpy_opr0 = 0xD;
                            break;
                        default:
                            Logger.FAIL($"Unrecognized first token operand: \"{token2}\", skipping!");
                            break;
                    }
                    switch (token3)
                    {
                        case "ra":
                            cpy_opr1 = 0xA;
                            break;
                        case "rb":
                            cpy_opr1 = 0xB;
                            break;
                        case "rc":
                            cpy_opr1 = 0xC;
                            break;
                        case "rd":
                            cpy_opr1 = 0xD;
                            break;
                        default:
                            Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                            break;
                    }
                    AddLEWord(DataParser.ResolveValueFromToken($"{cpy_opcode.ToString("X2")}{cpy_opr0.ToString("X1")}{cpy_opr1.ToString("X1")}"));
                    AddLEWord(0x0300); 
                    break;
                case "cmp":
                    byte cmp_opcode = 0x04;
                    byte cmp_opr0 = 0;
                    byte cmp_opr1 = 0;
                    switch (token2)
                    {
                        case "ra":
                            cmp_opr0 = 0xA;
                            break;
                        case "rb":
                            cmp_opr0 = 0xB;
                            break;
                        case "rc":
                            cmp_opr0 = 0xC;
                            break;
                        case "rd":
                            cmp_opr0 = 0xD;
                            break;
                        default:
                            Logger.FAIL($"Unrecognized first token operand: \"{token2}\", skipping!");
                            break;
                    }
                    switch (token3)
                    {
                        case "ra":
                            cmp_opr1 = 0xA;
                            break;
                        case "rb":
                            cmp_opr1 = 0xB;
                            break;
                        case "rc":
                            cmp_opr1 = 0xC;
                            break;
                        case "rd":
                            cmp_opr1 = 0xD;
                            break;
                        default:
                            Logger.FAIL($"Unrecognized second token operand: \"{token3}\", skipping!");
                            break;
                    }
                    AddLEWord(DataParser.ResolveValueFromToken($"{cmp_opcode.ToString("X2")}{cmp_opr0.ToString("X1")}{cmp_opr1.ToString("X1")}"));
                    AddLEWord(0x0300);
                    break;
            }
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
                    AddLEWord(0x0000); // null padding
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
                    if (token.Length > 0)
                        Logger.FAIL($"Unrecognized token: \"{token}\", skipping!");
                    else
                        Logger.WARN("Possible memory region within code section. Memory regions in source code are dangerous to use like this!");
                    break;
            }
        }
    }
}
