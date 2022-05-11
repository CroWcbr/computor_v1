#include "../include/Computor_v1.hpp"

int main(int argc, char **argv)
{
	try
	{
		Computor_v1 polinom(argc, argv);
		polinom.decision();
		polinom.polinom_print();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
