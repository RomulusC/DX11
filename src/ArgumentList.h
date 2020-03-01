#pragma once
#include <map>
#include <vector>
#include <string>

class ArgumentList
{
public:
	enum ARGCODE
	{
		RESOLUTION = 0
	};
	const std::vector<std::wstring>* FindArguments(ARGCODE _code)
	{
		auto it = argMap.find(_code);
		if (it != argMap.end())
			return &argMap.find(_code)->second;
		else
			return new std::vector<std::wstring>();
	}	
	
	static ArgumentList& Instance()
	{
		static ArgumentList* argumentList = new ArgumentList();
		return *argumentList;
	}

private:

	std::map<int, std::vector<std::wstring>> argMap;	

	ArgumentList()
	{
		int argC;
		LPWSTR* szArgList = CommandLineToArgvW(GetCommandLineW(), &argC);
		for (int i = 0; i < argC; i++)
		{
			std::wstring str = (szArgList[i]);
			if (str == L"-Res")
			{
				argMap.emplace(ARGCODE::RESOLUTION, std::vector<std::wstring>{ szArgList[++i], szArgList[++i] });

			}
		}
			LocalFree(szArgList);
	}	
	
};