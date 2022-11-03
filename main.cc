#include "sha256.hh"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: ./sha256 <string>\n";
        return 1;
    }

    sha_256 sha;

    std::string res = sha.exec(argv[1]);

    std::cout << res << std::endl;

    return 0;
}
