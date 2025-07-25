#include "ScalarConverter.hpp"

ScalarConverter::ScalarConverter(){}

ScalarConverter::~ScalarConverter(){}

ScalarConverter::ScalarConverter(const ScalarConverter& src)
{
    *this = src;
}

ScalarConverter & ScalarConverter::operator=(const ScalarConverter& src)
{
    if (this != &src){
        *this = src;
    }
    return *this;
}

static void convertIntoChar(double value) {
    std::cout << "char: ";
    if (std::isnan(value) || std::isinf(value) || value < 0 || value > 127)
        std::cout << "impossible" << std::endl;
    else if (!std::isprint(static_cast<char>(value)))
        std::cout << "Non displayable" << std::endl;
    else
        std::cout << "'" << static_cast<char>(value) << "'" << std::endl;
}

static void convertIntoInt(double value) {
    std::cout << "int: ";
    if (std::isnan(value) || std::isinf(value) ||
        value < INT_MIN || value > INT_MAX)
        std::cout << "impossible" << std::endl;
    else
        std::cout << static_cast<int>(value) << std::endl;
}

static void convertIntoFloat(double value) {
    std::cout << "float: ";
    if (std::isnan(value))
        std::cout << "nanf" << std::endl;
    else if (std::isinf(value)) {
		if (value > 0)
			std::cout << "+inff" << std::endl;
		else
			std::cout << "-inff" << std::endl;
	}
    else {
        std::cout << static_cast<float>(value);
        if (value == static_cast<int>(value))
            std::cout << ".0";
        std::cout << "f" << std::endl;
    }
}

static void convertIntoDouble(double value) {
    std::cout << "double: ";
    if (std::isnan(value))
        std::cout << "nan" << std::endl;
    else if (std::isinf(value)) {
		if (value > 0)
			std::cout << "+inff" << std::endl;
		else
			std::cout << "-inff" << std::endl;
	}
    else {
        std::cout << value;
        if (value == static_cast<int>(value))
            std::cout << ".0";
		std::cout << std::endl;
    }
}

void ScalarConverter::convert(const std::string &input) {
    std::string data = input;
    if (!data.empty() && data[data.size() - 1] == 'f')
        data = data.substr(0, data.size() - 1);
    if (data.length() == 3 && data[0] == '\'' && data[2] == '\'') {
        char ch = data[1];
        std::cout << "char: " << ch << std::endl;
        double d = static_cast<double>(ch);
        convertIntoInt(d);
        convertIntoFloat(d);
        convertIntoDouble(d);
        return;
    }
    double nbr = std::atof(data.c_str());
    convertIntoChar(nbr);
    convertIntoInt(nbr);
    convertIntoFloat(nbr);
    convertIntoDouble(nbr);
}
