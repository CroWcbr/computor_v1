#pragma once

# include "Token.hpp"

# include <string>
# include <vector>

class Lexer
{
private:
	std::string			_polinom;
	std::vector<Token>	_tokens;

private:
	Lexer();
	Lexer(Lexer const &copy);
	Lexer &operator=(Lexer const &copy);

	void _split()
	{
		std::string	tmp_lexeme;
		for (int i = 0, len = _polinom.size(); i < len; i++)
		{

			char symbol = _polinom[i];
			if (_is_separate_symbol(symbol))
			{
				if (!(tmp_lexeme.empty()))
				{
					Token new_token(tmp_lexeme);
					_tokens.push_back(new_token);
					tmp_lexeme.clear();
				}
				if (!isspace(symbol))
				{
					std::string current_symbol(1, symbol);
					Token new_token(current_symbol);
					_tokens.push_back(new_token);
				}
			}
			else
				tmp_lexeme += symbol;
		}

		if (!(tmp_lexeme.empty()))
		{
			Token new_token(tmp_lexeme);
			_tokens.push_back(new_token);
			tmp_lexeme.clear();
		}
	}

	bool _is_separate_symbol(char symbol)
	{
		return  symbol == '=' || \
				symbol == '+' || \
				symbol == '-' || \
				symbol == '*' || \
				symbol == '^' || \
				isspace(symbol);
	}

public:
	Lexer(std::string const &polinom)
	{
		_polinom = polinom;
		_split();
	}

	~Lexer() {};

	std::vector<Token> const &getTokens() const { return _tokens; }

};
