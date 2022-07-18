#pragma once

# include "Token.hpp"

# include <string>
# include <vector>
# include <iostream>

class Lexer
{
private:
	std::vector<Token>	_tokens;

private:
	Lexer();
	Lexer(Lexer const &copy);
	Lexer &operator=(Lexer const &copy);

	std::string _separate_digit_and_alpha(std::string const &polinom) const
	{	// 2x -> 2 *x && x2 -> x * 2
		std::string tmp = polinom;
		for(size_t i = 0; i < tmp.size() - 1; i++)
			if ((isdigit(tmp[i]) && isalpha(tmp[i + 1])) || \
					(isdigit(tmp[i + 1]) && isalpha(tmp[i])))
				tmp.insert(i + 1, "*");
		return tmp;
	}

	std::vector<Token> _split(std::string const &input)
	{
		std::string			tmp_lexeme;
		std::vector<Token>	tmp_token;
		for (int i = 0, len = input.size(); i < len; i++)
		{
			char symbol = input[i];
			if (_is_separate_symbol(symbol))
			{
				if (!(tmp_lexeme.empty()))
				{
					tmp_token.push_back(Token(tmp_lexeme));
					tmp_lexeme.clear();
				}
				if (!isspace(symbol))
				{
					tmp_lexeme = symbol;
					tmp_token.push_back(Token(tmp_lexeme));
					tmp_lexeme.clear();
				}
			}
			else
				tmp_lexeme += symbol;
		}

		if (!(tmp_lexeme.empty()))
			tmp_token.push_back(Token(tmp_lexeme));
		return tmp_token;
	}

	bool _is_separate_symbol(char symbol) const
	{
		return  symbol == '+' || \
				symbol == '-' || \
				symbol == '*' || \
				symbol == '/' || \
				symbol == '^' || \
				isspace(symbol);
	}

public:
	Lexer(std::string const &polinom)
	{
		std::string tmp_str = _separate_digit_and_alpha(polinom);

		size_t	equal_pos = tmp_str.find("=");
		if (equal_pos == std::string::npos)
			throw std::runtime_error("PARSE ERROR! There is no '='");

		_tokens = _split(tmp_str.substr(0, equal_pos));
		std::vector<Token>	right_tokens = _split(tmp_str.substr(++equal_pos));
		if (_tokens.size() == 0)
			throw std::runtime_error("PARSE ERROR! No LEFT part");
		if (right_tokens.size() == 0)
			throw std::runtime_error("PARSE ERROR! No RIGHT part");

		if (_tokens.front().getLexem() == "-")
			_tokens.insert(_tokens.begin(), Token("0"));
		for (int i = 0, len = right_tokens.size(); i < len; i++)
		{
			if (i == 0 && right_tokens[i].getLexem() != "-")
				_tokens.push_back(Token("-"));
			else if (i > 0 && (right_tokens[i].getLexem() == "+" || right_tokens[i].getLexem() == "-") && \
					(right_tokens[i - 1].getLexem() == "+" || right_tokens[i - 1].getLexem() == "-"))
				throw std::runtime_error("PARSE ERROR! Double sign in RIGHT part");
			
			if (right_tokens[i].getLexem() == "-")
				_tokens.push_back(Token("+"));
			else if (right_tokens[i].getLexem() == "+")
				_tokens.push_back(Token("-"));
			else
				_tokens.push_back(right_tokens[i]);
		}
	}

	~Lexer() {};

	std::vector<Token> const &getTokens() const { return _tokens; }
};
