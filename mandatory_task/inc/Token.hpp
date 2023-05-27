#pragma once

# include <string>
# include <iostream>

enum class token_type
{
	PLUS,		// +
	MINUS,		// -
	STAR,		// *
	SLASH,		// /
	DEGREE,		// ^
	VARIABLE,	// unknown X
	ARGUMENT,	// arguments
};

class Token
{
private:
	std::string		_lexeme;
	token_type		_type;

private:
	Token();

	token_type _which_token_type(std::string const &lexeme)
	{
		if (lexeme == "+")
			return token_type::PLUS;
		else if (lexeme == "-")
			return token_type::MINUS;
		else if (lexeme == "*")
			return token_type::STAR;
		else if (lexeme == "/")
			return token_type::SLASH;
		else if (lexeme == "^")
			return token_type::DEGREE;
		else if (_is_argument(lexeme))
			return token_type::ARGUMENT;
		else if (_is_variable(lexeme))
			return token_type::VARIABLE;
		else
			throw std::runtime_error("TOKEN ERROR! Unknown lexeme : " + lexeme);
	}

	bool _is_argument(std::string const &lexeme) const
	{
		int dot = 0;

		if (lexeme.front() == '.' || lexeme.back() == '.')
			return false;
		for (const auto &symbol : lexeme)
		{
			if (!isdigit(symbol) && symbol != '.')
				return false;
			if (symbol == '.')
				dot++;
			if (dot > 1)
				return false;
		}
		return true;
	}

	bool _is_variable(std::string const &lexeme) const
	{
		for (const auto &symbol : lexeme)
			if (!isalpha(symbol))
				return false;
		return true;
	}

public:
	Token(std::string lexeme)
	{
		_lexeme = lexeme;
		_type = _which_token_type(lexeme);
	}

	Token(Token const &copy) 
	{
		*this = copy;
	}

	Token &operator=(Token const &copy)
	{
		if (this == &copy)
			return *this;
		_lexeme = copy._lexeme;
		_type = copy._type;
		return *this;
	}

	~Token() {}

	const std::string&	getLexem() const { return _lexeme; }
	const token_type&	getType() const { return _type; }
};
