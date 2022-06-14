#pragma once

# include "Fraction.hpp"

# include <string>
# include <vector>
# include <map>

class Parse
{
private:
	std::vector<Token>	_tokens;

	int							_i;
	int							_len;

	int							_znak;
	Fraction					_arg;
	int							_degree;
	int							_assign;

	std::map<int, Fraction>		_pol;
	std::string					_x;

private:
	Parse();
	Parse(Parse const &copy);
	Parse &operator=(Parse const &copy);

	void _polinom_parse()
	{
		_znak = 1;
		_arg.setArg(1, 1);
		_degree = 0;

		int parse_begin = 0;

		while(_i < _len)
		{
			token_type type_i = _tokens[_i].getType();

			if (parse_begin == 0 && type_i == token_type::MINUS)
			{
				_check_minus_plus(_i);
				_znak *= -1;
			}
			else if (parse_begin == 0 && type_i == token_type::PLUS)
			{
				_check_minus_plus(_i);
			}
			else if (type_i == token_type::ASSIGN)
			{
				_check_assign(_i);
				_assign++;
			}
			else if (type_i == token_type::STAR)
			{
				_check_star(_i);
			}
			else if (type_i == token_type::SLASH)
			{
				_check_slash(_i);
			}
			else if (type_i == token_type::DEGREE)
			{
				_check_degree(_i);
			}
			else if (type_i == token_type::VARIABLE)
			{
				_check_variable(_i);
				parse_begin = 1;
				if (_i < _len - 1 && _tokens[_i + 1].getType() == token_type::DEGREE)
				{
					_i += 2;
					_degree += std::atoi(_tokens[_i].getLexem().c_str());
				}
				else
					_degree++;
			}
			else if (type_i == token_type::ARGUMENT)
			{
				_check_argument(_i);
				parse_begin = 1;
				Fraction tmp(_tokens[_i].getLexem());
				if (_i > 0 && _tokens[_i - 1].getType() == token_type::SLASH)
					_arg /= tmp;
				else
					_arg *= tmp;
			}

			if (parse_begin == 1 && \
					(type_i == token_type::MINUS ||\
					type_i == token_type::ASSIGN || \
					type_i == token_type::PLUS || \
					_i == _len - 1))
			{
				parse_begin = 0;

				_arg *= _znak;
				if (_pol.find(_degree) != _pol.end())
					_pol[_degree] += _arg;
				else
					_pol[_degree] = _arg;
				
				_arg.setArg(1, 1);
				_degree = 0;
				_znak = 1;
				if (_assign == 1)
					_znak *= -1;
				if (type_i == token_type::MINUS)
					_znak *= -1;
			}
			_i++;
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
		// if (i == 0)
		// 	;

		if (i == _len - 1)
			_check_handle_error("_check_minus_plus", i);

		if (i > 0)
		{
			token_type type_prev = _tokens[i - 1].getType();
			if (!(type_prev == token_type::PLUS || \
					type_prev == token_type::MINUS || \
					type_prev == token_type::ARGUMENT ||
					type_prev == token_type::VARIABLE))
				_check_handle_error("_check_minus_plus", i);
		}

		if (i < _len - 1)
		{
			token_type type_next = _tokens[i + 1].getType();
			if (!(type_next == token_type::PLUS || \
					type_next == token_type::MINUS || \
					type_next == token_type::ARGUMENT ||
					type_next == token_type::VARIABLE))
				_check_handle_error("_check_minus_plus", i + 1);
		}
	}

	void _check_assign(int i) const
	{
		if (i == 0)
			_check_handle_error("_check_assign", i);

		if (i == _len - 1)
			_check_handle_error("_check_assign", i);

		if (i > 0)
		{
			token_type type_prev = _tokens[i - 1].getType();
			if (!(type_prev == token_type::ARGUMENT ||
					type_prev == token_type::VARIABLE))
				_check_handle_error("_check_assign", i);
		}

		if (i < _len - 1)
		{
			token_type type_next = _tokens[i + 1].getType();
			if (!(type_next == token_type::PLUS || \
					type_next == token_type::MINUS || \
					type_next == token_type::ARGUMENT ||
					type_next == token_type::VARIABLE))
				_check_handle_error("_check_assign", i + 1);
		}

		if (_assign > 0)
			_check_handle_error("_check_assign", i);
	}

	void _check_star(int i) const
	{
		if (i == 0)
			_check_handle_error("_check_star", i);

		if (i == _len - 1)
			_check_handle_error("_check_star", i);

		if (i > 0)
		{
			token_type type_prev = _tokens[i - 1].getType();
			if (!(type_prev == token_type::ARGUMENT ||
					type_prev == token_type::VARIABLE))
				_check_handle_error("_check_star", i);
		}

		if (i < _len - 1)
		{
			token_type type_next = _tokens[i + 1].getType();
			if (!(type_next == token_type::ARGUMENT ||
					type_next == token_type::VARIABLE))
				_check_handle_error("_check_star", i + 1);
		}
	}

	void _check_slash(int i) const
	{
		if (i == 0)
			_check_handle_error("_check_slash", i);

		if (i == _len - 1)
			_check_handle_error("_check_slash", i);

		if (i > 0)
		{
			token_type type_prev = _tokens[i - 1].getType();
			if (!(type_prev == token_type::ARGUMENT))
				_check_handle_error("_check_star_slash", i);
		}

		if (i < _len - 1)
		{
			token_type type_next = _tokens[i + 1].getType();
			if (!(type_next == token_type::ARGUMENT))
				_check_handle_error("_check_star_slash", i + 1);
			
			if (std::stod(_tokens[i + 1].getLexem().c_str()) == 0)
				_check_handle_error("_check_slash", i + 1);
		}
	}

	void _check_degree(int i) const
	{
		if (i == 0)
			_check_handle_error("_check_degree", i);

		if (i == _len - 1)
			_check_handle_error("_check_degree", i);

		if (i > 0)
		{
			token_type type_prev = _tokens[i - 1].getType();
			if (!(type_prev == token_type::VARIABLE))
				_check_handle_error("_check_degree", i);
		}

		if (i < _len - 1)
		{
			token_type type_next = _tokens[i + 1].getType();
			if (!(type_next == token_type::ARGUMENT))
				_check_handle_error("_check_degree", i + 1);
		}
	}

	void _check_variable(int i)
	{
		if (_x.empty())
			_x = _tokens[i].getLexem();

		if (_x != _tokens[i].getLexem())
			_check_handle_error("_check_variable", i);

		// if (i == 0)
		// 	;

		// if (i == _len - 1)
		// 	;

		if (i > 0)
		{
			token_type type_prev = _tokens[i - 1].getType();
			if (!(type_prev == token_type::PLUS || \
					type_prev == token_type::MINUS || \
					type_prev == token_type::ASSIGN || \
					type_prev == token_type::STAR))
				_check_handle_error("_check_variable", i);
		}

		if (i < _len - 1)
		{
			token_type type_next = _tokens[i + 1].getType();
			if (!(type_next == token_type::PLUS || \
					type_next == token_type::MINUS || \
					type_next == token_type::ASSIGN || \
					type_next == token_type::DEGREE || \
					type_next == token_type::STAR || \
					type_next == token_type::SLASH))
				_check_handle_error("_check_variable", i + 1);
		
			if (type_next == token_type::DEGREE)
			{
				_check_degree(i + 1);
				_check_argument(i + 2);
				if (_tokens[i + 2].getLexem().find('.') != std::string::npos)
					_check_handle_error("_check_variable", i + 2);
			}
		}
	}

	void _check_argument(int i) const
	{
		// if (i == 0)
		// 	;

		// if (i == _len - 1)
		// 	;

		if (i > 0)
		{
			token_type type_prev = _tokens[i - 1].getType();
			if (!(type_prev == token_type::PLUS || \
					type_prev == token_type::MINUS || \
					type_prev == token_type::ASSIGN || \
					type_prev == token_type::STAR || \
					type_prev == token_type::SLASH || \
					type_prev == token_type::DEGREE))
				_check_handle_error("_check_argument", i);
		}

		if (i < _len - 1)
		{
			token_type type_next = _tokens[i + 1].getType();
			if (!(type_next == token_type::PLUS || \
					type_next == token_type::MINUS || \
					type_next == token_type::ASSIGN || \
					type_next == token_type::STAR || \
					type_next == token_type::SLASH))
				_check_handle_error("_check_argument", i + 1);
		}
	}

public:
	Parse(std::vector<Token> const &tokens)
	{
		_tokens = tokens;

		_assign = 0;
		_x = "";

		_i = 0;
		_len = _tokens.size();
		if (_len < 3)
			throw std::runtime_error("PARSE ERROR! token.size() < 3");

		_polinom_parse();
	}

	~Parse() {};

	std::map<int, Fraction> const getPolinom() const { return _pol; }
	std::string const getX() const { return _x; }

};
