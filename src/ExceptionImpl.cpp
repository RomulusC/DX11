#include "ExceptionImpl.h"
#include <sstream>
ExceptionImpl::ExceptionImpl(int _line, const char* _file) noexcept
	:line(_line)
	,file(_file)
{}

const char* ExceptionImpl::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType()<< "!" << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* ExceptionImpl::GetType() const noexcept
{
	return "Unknown Exception";
}

int ExceptionImpl::GetLine() const noexcept
{
	return line;
}

const std::string& ExceptionImpl::GetFile() const noexcept
{
	return file;
}

std::string ExceptionImpl::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File]: " << file << std::endl
		<< "[Line]: " << line;
	return oss.str();
}
