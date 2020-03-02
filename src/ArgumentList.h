#pragma once
#include <map>
#include <vector>
#include <string>

class ArgumentList
{
public:
	enum class ArgCode
	{
		Resolution = 0
	};

	const std::vector<std::wstring>* FindArguments(ArgCode _code)
	{
		auto it = m_argMap.find(_code);
		if (it != m_argMap.end())
			return &m_argMap.find(_code)->second;
		else
			return new std::vector<std::wstring>();
	}	
	
	static ArgumentList& Instance()
	{
		static ArgumentList* argumentList = new ArgumentList();
		return *argumentList;
	}

private:

	std::map<ArgCode, std::vector<std::wstring>> m_argMap;

	ArgumentList()
	{
		int argC;
		LPWSTR* szArgList = CommandLineToArgvW(GetCommandLineW(), &argC);
		for (int i = 0; i < argC; i++)
		{
			std::wstring str = (szArgList[i]);
			if (str == L"-Res")
			{
				m_argMap.emplace(ArgCode::Resolution, std::vector<std::wstring>{ szArgList[++i], szArgList[++i] });

			}
		}
			LocalFree(szArgList);
	}		
};