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
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "Dictionary.h"

namespace Topologic {
	Net::Dictionary<String^, Object^>^ Dictionary::ByKeysValues(List<String^>^ keys, List<Object^>^ values)
	{
		if (keys->Count == 0 || values->Count == 0)
		{
			return nullptr;
		}

		if (keys->Count != values->Count)
		{
			throw gcnew Exception("The lists of keys and values do not have the same length.");
		}

		Net::Dictionary<String^, Object^>^ dictionary = gcnew Net::Dictionary<String^, Object^>();
		for (int i = 0; i < keys->Count; ++i)
		{
			dictionary->Add(keys[i], values[i]);
		}
		return dictionary;
	}

	Net::Dictionary<String^, Object^>^ Dictionary::SetValueAtKey(Net::Dictionary<String^, Object^>^ dictionary, String ^ key, Object ^ value)
	{
		Net::Dictionary<String^, Object^>^ copyDict = Copy(dictionary);

		copyDict[key] = value;
		return copyDict;
	}

	Object ^ Dictionary::ValueAtKey(Net::Dictionary<String^, Object^>^ dictionary, String ^ key)
	{
		try {
			Object^ value = nullptr;
			if(dictionary->TryGetValue(key, value))
			{
				return value;
			}

			return nullptr;
		}
		catch (ArgumentException^)
		{
			throw gcnew Exception("The key argument is null.");
		}
	}

	Net::Dictionary<String^, Object^>^ Dictionary::Copy(Net::Dictionary<String^, Object^>^ dictionary)
	{
		// https://stackoverflow.com/questions/139592/what-is-the-best-way-to-clone-deep-copy-a-net-generic-dictionarystring-t
		Net::Dictionary<String^, Object^>^ copy = gcnew Net::Dictionary<String^, Object^>(dictionary);
		return copy;
	}
}