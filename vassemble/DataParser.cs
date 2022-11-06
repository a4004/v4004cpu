using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace vassemble
{
    internal sealed class DataParser
    {


        public static ushort ResolveValueFromToken(string token)
        {
            ushort result = ushort.Parse(token.Replace("#", ""), System.Globalization.NumberStyles.HexNumber);
            return result;
        }

        public static ushort ResolveAddressFromToken(string token, ref Dictionary<string, ushort> symbolTable)
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
    }
}
