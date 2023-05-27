#pragma once

# include "Fraction.hpp"
# include "Token.hpp"

# include <string>
# include <vector>
# include <map>

class Parse
{
private:
	std::vector<Token> const	&_tokens;
	std::map<int, Fraction>		_pol;
	std::string					_x;

private:
	Parse();
	Parse(Parse const &copy);
	Parse &operator=(Parse const &copy);

	void _check_polinom()
	{
		if (!(_tokens[0].getType() == token_type::MINUS || \
				_tokens[0].getType() == token_type::ARGUMENT || \
				_tokens[0].getType() == token_type::VARIABLE))
			_check_handle_error("_check_first_symbol", 1);

		if (!(_tokens[_tokens.size() - 1].getType() == token_type::ARGUMENT || \
				_tokens[_tokens.size() - 1].getType() == token_type::VARIABLE))
			_check_handle_error("_check_last_symbol", _tokens.size() - 1);

		if (_tokens[0].getType() == token_type::VARIABLE)
			_x = _tokens[0].getLexem();
		
		for(int i = 1, len = _tokens.size() - 1; i < len; i++)
		{
			token_type type_i = _tokens[i].getType();
			if (type_i == token_type::MINUS || type_i == token_type::PLUS)
				_check_minus_plus(i);
			else if (type_i == token_type::STAR)
				_check_star(i);
			else if (type_i == token_type::SLASH)
				_check_slash(i);
			else if (type_i == token_type::DEGREE)
				_check_degree(i);
			else if (type_i == token_type::VARIABLE)
				_check_variable(i);
			else if (type_i == token_type::ARGUMENT)
				_check_argument(i);
			else
				throw std::runtime_error("NO ELSE POSSIBLE");
		}
	}

	void _check_handle_error(std::string msg, int i) const
	{
		std::string where_error;
		for (int ii = 0; ii <= i; ii++)
			where_error += _tokens[ii].getLexem();
		throw std::runtime_error("PARSE ERROR in : " + where_error + " : " + msg);
	}

	void _check_minus_plus(int i) const
	{
		token_type type_prev = _tokens[i - 1].getType();
		if (!(type_prev == token_type::ARGUMENT ||
				type_prev == token_type::VARIABLE))
			_check_handle_error("_check_minus_plus", i);

		token_type type_next = _tokens[i + 1].getType();
		if (!(type_next == token_type::ARGUMENT ||
				type_next == token_type::VARIABLE))
			_check_handle_error("_check_minus_plus", i + 1);
	}

	void _check_star(int i) const
	{
		token_type type_prev = _tokens[i - 1].getType();
		if (!(type_prev == token_type::ARGUMENT ||
				type_prev == token_type::VARIABLE))
			_check_handle_error("_check_star", i);

		token_type type_next = _tokens[i + 1].getType();
		if (!(type_next == token_type::ARGUMENT ||
				type_next == token_type::VARIABLE))
			_check_handle_error("_check_star", i + 1);
	}

	void _check_slash(int i) const
	{
		token_type type_prev = _tokens[i - 1].getType();
		if (!(type_prev == token_type::ARGUMENT || 
				type_prev == token_type::VARIABLE))
			_check_handle_error("_check_slash", i);

		token_type type_next = _tokens[i + 1].getType();
		if (!(type_next == token_type::ARGUMENT))
			_check_handle_error("_check_slash", i + 1);
		
		if (std::stod(_tokens[i + 1].getLexem().c_str()) == 0)
			_check_handle_error("_check_slash zero division", i + 1);
	}

	void _check_degree(int i) const
	{
		token_type type_prev = _tokens[i - 1].getType();
		if (!(type_prev == token_type::VARIABLE))
			_check_handle_error("_check_degree", i);

		token_type type_next = _tokens[i + 1].getType();
		if (!(type_next == token_type::ARGUMENT))
			_check_handle_error("_check_degree", i + 1);

		double tmp = std::stod(_tokens[i + 1].getLexem().c_str());
		if (tmp - static_cast<int>(tmp) != 0)
			_check_handle_error("_check_degree", i + 1);
	}

	void _check_variable(int i)
	{
		if (_x.empty())
			_x = _tokens[i].getLexem();
		if (_x != _tokens[i].getLexem())
			_check_handle_error("_check_variable", i);

		token_type type_prev = _tokens[i - 1].getType();
		if (!(type_prev == token_type::PLUS || \
				type_prev == token_type::MINUS || \
				type_prev == token_type::STAR))
			_check_handle_error("_check_variable", i);

		token_type type_next = _tokens[i + 1].getType();
		if (!(type_next == token_type::PLUS || \
				type_next == token_type::MINUS || \
				type_next == token_type::DEGREE || \
				type_next == token_type::STAR || \
				type_next == token_type::SLASH))
			_check_handle_error("_check_variable", i + 1);
	}

	void _check_argument(int i) const
	{
		token_type type_prev = _tokens[i - 1].getType();
		if (!(type_prev == token_type::PLUS || \
				type_prev == token_type::MINUS || \
				type_prev == token_type::STAR || \
				type_prev == token_type::SLASH || \
				type_prev == token_type::DEGREE))
			_check_handle_error("_check_argument", i);

		token_type type_next = _tokens[i + 1].getType();
		if (!(type_next == token_type::PLUS || \
				type_next == token_type::MINUS || \
				type_next == token_type::STAR || \
				type_next == token_type::SLASH))
			_check_handle_error("_check_argument", i + 1);
	}

	void _polinom_parse()
	{
		Fraction arg(1, 1);
		int degree = 0;
		int znak = 1;

		for (int i = 0, len = _tokens.size(); i < len; i++)
		{
			token_type type_i = _tokens[i].getType();

			if (type_i == token_type::VARIABLE)
			{
				if (i < len - 1 && _tokens[i + 1].getType() == token_type::DEGREE)
				{
					i += 2;
					degree += std::atoi(_tokens[i].getLexem().c_str());
				}
				else
					degree++;
			}
			else if (type_i == token_type::ARGUMENT)
			{
				Fraction tmp(_tokens[i].getLexem());
				if (i > 0 && _tokens[i - 1].getType() == token_type::SLASH)
					arg /= tmp;
				else
					arg *= tmp;
			}

			if (i == len - 1 || \
				type_i == token_type::MINUS || \
				type_i == token_type::PLUS)
			{
				if (_pol.find(degree) != _pol.end())
					_pol[degree] += arg * znak;
				else
					_pol[degree] = arg * znak;
				if (_pol[degree].toDouble() == 0)
					_pol.erase(_pol.find(degree));
				arg.setArg(1,1);
				degree = 0;
				znak = 1;
				if (type_i == token_type::MINUS)
					znak = -1;
			}
		}
	}

public:
	Parse(std::vector<Token> const &tokens):_tokens(tokens)
	{
		_check_polinom();
		_polinom_parse();
	}

	~Parse() {};

	const std::map<int, Fraction>&	getPolinom() const { return _pol; }
	const std::string&				getX() const { return _x; }
};
