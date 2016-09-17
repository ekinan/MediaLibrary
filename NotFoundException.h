#ifndef _NOT_FOUND_EXCEPTION_H
#define _NOT_FOUND_EXCEPTION_H

#include <stdexcept>
#include <string>

/*
Exception class used in the present project
*/

class NotFoundException : public std::logic_error
{
public:
	NotFoundException(const std::string& message = "");
};

#endif
