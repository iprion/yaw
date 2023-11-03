#pragma once


#include <sstream>

namespace std {

    template<typename T>
    inline void format_helper(std::ostringstream& oss, std::string_view& str, const T& value)
    {
        //find the {}
        std::size_t openBracket = str.find('{'); 
        if (openBracket == std::string::npos) { return; }
        std::size_t closeBracket = str.find('}', openBracket + 1);
        if (closeBracket == std::string::npos) { return; }

        //After this line, the output stream will be:
        //The output stream + the string view up to (but not including) {} + the value argument.
        oss << str.substr(0, openBracket) << value;

        //Change the string view reference so that subsequent recursive calls start after the {} that this function just took care of. 
        str = str.substr(closeBracket + 1);  
    }

    //Given a string with any number of {} and any number of arguments, return a string that has each subsequent {} replaced with one of the subsequent arguments.
    // e.g. format("Hello {} {}", world, 123) returns "Hello world 123"
    template<typename... Targs>
    inline std::string format(std::string_view str, Targs...args) //Define a function that takes a string_view and any number of arguments of type Targs.
    {
        std::ostringstream oss; //define an output string stream that allows us to insert characters into a string buffer. 
        (format_helper(oss, str, args), ...); //recursively call format_helper on each argument. This takes the string stream, a reference of the string view, and one of the arguments. 
        oss << str;
        return oss.str();
    }
}
