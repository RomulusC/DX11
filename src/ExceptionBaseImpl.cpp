#include "Window.h"
#include <debugapi.h>
#include "ExceptionBaseImpl.h"
#include <sstream>
ExceptionBaseImpl::ExceptionBaseImpl(int _line, const char* _file) noexcept
	:line(_line)
	,file(_file)
{}

const char* ExceptionBaseImpl::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType()<< "!" << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* ExceptionBaseImpl::GetType() const noexcept
{
	return "Unknown Exception";
}

int ExceptionBaseImpl::GetLine() const noexcept
{
	return line;
}

const std::string& ExceptionBaseImpl::GetFile() const noexcept
{
	return file;
}

std::string ExceptionBaseImpl::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File]: " << file << std::endl
		<< "[Line]: " << line;
	return oss.str();
}
