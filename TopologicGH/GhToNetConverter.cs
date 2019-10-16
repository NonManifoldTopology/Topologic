// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Grasshopper.Kernel.Types;

namespace TopologicGH
{

    internal static class GhToNetConverter
    {
        public static Object ConvertObject(Object ghObject)
        {
            try
            {
                return ConvertGhObject((dynamic)ghObject);
            }
            catch (Exception)
            {
                return ghObject;
            }
        }

        public static String ConvertGhObject(GH_String ghString)
        {
            return ghString.Value;
        }

        public static double ConvertGhObject(GH_Number ghNumber)
        {
            return ghNumber.Value;
        }

        public static double ConvertGhObject(GH_Integer ghInteger)
        {
            return ghInteger.Value;
        }

        public static List<Object> ConvertList(List<Object> inputList)
        {
            List<Object> outputList = new List<Object>();
            foreach (Object inputObject in inputList)
            {
                outputList.Add(ConvertObject(inputObject));
            }
            return outputList;
        }

        public static Dictionary<String, Object> ConvertDictionary<KeyType>(Dictionary<KeyType, Object> inputDictionary)
        {
            Dictionary<String, Object> outputDictionary = new Dictionary<String, Object>();
            foreach (var inputPair in inputDictionary)
            {
                Object inputKey = inputPair.Key;
                String key = inputKey.ToString();

                Object inputValue = inputPair.Value;
                Object value = ConvertObject(inputValue);

                outputDictionary.Add(key, value);
            }
            return outputDictionary;
        }
    }

}
