#pragma once

# include "Token.hpp"

# include <string>
# include <vector>
# include <map>

class Parse
{
private:
	std::vector<Token>	_tokens;

	int					_i;
	int					_len;

	int					_znak;
	double				_arg;
	int					_degree;
	int					_assign;

	std::map<int, double>	_pol;
	std::string				_x;

private:
	Parse();

	void _polinom_parse()
	{
		_znak = 1;
		_arg = 1;
		_degree = 0;

		int parse_begin = 0;
		int parse_end = 0;

		while(_i < _len)
		{
			token_type type_i = _tokens[_i].getType();

			if (type_i == token_type::MINUS)
			{
				_check_minus_plus(_i);
				if (parse_begin == 0)
					_znak *= -1;
			}
			else if (type_i == token_type::PLUS)
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
				{
					_degree++;
				}
			}
			else if (type_i == token_type::ARGUMENT)
			{
				_check_argument(_i);
				parse_begin = 1;
				_arg *= std::stod(_tokens[_i].getLexem().c_str());
			}

			if (parse_begin == 1 && \
					(type_i == token_type::MINUS ||\
					type_i == token_type::ASSIGN || \
					type_i == token_type::PLUS || \
					_i == _len - 1))
			{
				parse_begin = 0;

				if (_pol.find(_degree) != _pol.end())
					_pol[_degree] += _arg * _znak;
				else
					_pol[_degree] = _arg * _znak;
				
				_arg = 1;
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
		if (i == 0)
			;

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

		if (i == 0)
			;

		if (i == _len - 1)
			;

		if (i > 0)
		{
			token_type type_prev = _tokens[i - 1].getType();
			if (!(type_prev == token_type::PLUS || \
					type_prev == token_type::MINUS || \
					type_prev == token_type::ASSIGN || \
					type_prev == token_type::STAR))
				_check_handle_error("_check_variable", i);
		}

		int tmp_degree = _degree;
		if (i < _len - 1)
		{
			token_type type_next = _tokens[i + 1].getType();
			if (!(type_next == token_type::PLUS || \
					type_next == token_type::MINUS || \
					type_next == token_type::ASSIGN || \
					type_next == token_type::DEGREE || \
					type_next == token_type::STAR))
				_check_handle_error("_check_variable", i + 1);
		
			if (type_next == token_type::DEGREE)
			{
				_check_degree(i + 1);
				_check_argument(i + 2);
				tmp_degree += std::atoi(_tokens[i + 2].getLexem().c_str());
			}
		}
		else
		{
			tmp_degree++;
			if (tmp_degree > 2)
				_check_handle_error("_check_variable", i);
		}
	}

	void _check_argument(int i) const
	{
		if (i == 0)
			;

		if (i == _len - 1)
			;

		if (i > 0)
		{
			token_type type_prev = _tokens[i - 1].getType();
			if (!(type_prev == token_type::PLUS || \
					type_prev == token_type::MINUS || \
					type_prev == token_type::ASSIGN || \
					type_prev == token_type::STAR || \
					type_prev == token_type::DEGREE))
				_check_handle_error("1_check_argument", i);
		}

		if (i < _len - 1)
		{
			token_type type_next = _tokens[i + 1].getType();
			if (!(type_next == token_type::PLUS || \
					type_next == token_type::MINUS || \
					type_next == token_type::ASSIGN || \
					type_next == token_type::STAR))
				_check_handle_error("2_check_argument", i + 1);
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

	std::map<int, double> const getPolinom() const { return _pol; }
	std::string const getX() const { return _x; }

};
