#pragma once

/*
	*Gloomy.cc
 	*https://github.com/Chase1803
	
	Copyright (c) 2022 Chase1803
	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:
	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
 */

static std::string ReadFNamePool(int key) 
{
	uint64_t NamePoolChunk = read<uint64_t>(g_pid, g_base_address + 0xCCB6E80 + (8 * (uint32_t)((int)(key) >> 16)) + 16) + (unsigned int)(4 * (uint16_t)key);
	int nameLength = read<uint16_t>(g_pid, NamePoolChunk) >> 6; 
	char buff[1024];

	if ((uint32_t)nameLength)
	{
		for (int x = 0; x < nameLength; ++x)
		{
			buff[x] = read<char>(g_pid, NamePoolChunk + 4 + x);
		}

		char* v2 = buff; int v4 = nameLength, v5 = 0, v7; __int64 v6 = 30i64; char v8;
              
		if (v4)
		{
			do
			{
				v7 = v5 | v6;
				++v2;
				++v5;
				v8 = ~(BYTE)v7;
				v6 = (unsigned int)(2 * v7);
				*(BYTE*)(v2 - 1) ^= v8;
			} while (v5 < v4);
		} buff[nameLength] = '\0'; return std::string(buff);
	}
	else return "";
}
 
static std::string GetNameFromFName(int key)
{
	uint64_t NamePoolChunk = read<uint64_t>(g_pid, g_base_address + 0xCCB6E80 + (8 * (uint32_t)((int)(key) >> 16)) + 16) + (unsigned int)(4 * (uint16_t)key);

	if (read<uint16_t>(g_pid, NamePoolChunk) < 64)
	{
		auto a1 = read<DWORD>(g_pid, NamePoolChunk + 4);
		return ReadFNamePool(a1);
	}

	else
	{
		return ReadFNamePool(key);
	}
}
