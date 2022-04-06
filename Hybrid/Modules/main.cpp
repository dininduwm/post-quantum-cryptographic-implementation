
#include "AES.h"


int main(int argc, char const *argv[])
{
	struct InputFile input = readFile("1.txt");
	writeFile(input.data, input.data_size, "test.txt");



	return 0;
}