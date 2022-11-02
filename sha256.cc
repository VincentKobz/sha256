#include "sha256.hh"
#include <iostream>
#include <vector>
#include <cstring>
#include <sstream>
#include <iomanip>

auto Sha256::rotate_right(uint32_t c, int nb)
{
    return (c >> nb) | ((c << (32 - nb)) & 0xffffffff);
}

auto Sha256::shift_right(uint32_t c, int nb)
{
    return c >> nb;
}

auto Sha256::operator_1(uint32_t nb1, uint32_t nb2, uint32_t nb3)
{
    return nb1 ^ nb2 ^ nb3;
}

auto Sha256::ch_op(uint32_t nb1, uint32_t nb2, uint32_t nb3)
{
    return (nb1 & nb2) ^ (~nb1 & nb3);
}

auto Sha256::maj_op(uint32_t nb1, uint32_t nb2, uint32_t nb3)
{
    return (nb1 & nb2) ^ (nb1 & nb3) ^ (nb2 & nb3);
}

auto Sha256::s1_op(uint32_t nb)
{
    return rotate_right(nb, 6) ^ rotate_right(nb, 11) ^ rotate_right(nb, 25);
}

auto Sha256::s0_op(uint32_t nb)
{
    return rotate_right(nb, 2) ^ rotate_right(nb, 13) ^ rotate_right(nb, 22);
}

std::vector<unsigned char> Sha256::preprocessing(char *input)
{
    long long len = std::strlen(input) * 8;
    std::vector<unsigned char> data(input, input + len / 8);


    data.push_back(0x80);

    while ((data.size() + 8) % 64 != 0)
    {
        data.push_back(0x00);
    }

    for (int i = 0; i < 8; i++)
    {
        data.push_back(len >> (64 - 8 * (i + 1)));
    }

    return data;
}

void Sha256::hash(std::vector<unsigned char> vec)
{
    for (auto j = 0; j < vec.size(); j += 64)
    {
        std::vector<uint32_t> new_vec;
        for (auto i = j; i < j + 64; i += 4)
        {
            uint32_t new_data = (vec[i] << 24) | (vec[i + 1] << 16) | (vec[i + 2] << 8) | vec[i + 3];
            new_vec.push_back(new_data);
        }

        for (auto i = 16; i < 64; i++)
        {
            auto s0 = rotate_right(new_vec[i - 15], 7) ^ rotate_right(new_vec[i - 15], 18) ^ shift_right(new_vec[i - 15], 3);
            auto s1 = rotate_right(new_vec[i - 2], 17) ^ rotate_right(new_vec[i - 2], 19) ^ shift_right(new_vec[i - 2], 10);
            auto value = s0 + s1 + new_vec[i - 16] + new_vec[i - 7];

            new_vec.push_back(value);
        }

        auto a = SQUARE_ROOT[0];
        auto b = SQUARE_ROOT[1];
        auto c = SQUARE_ROOT[2];
        auto d = SQUARE_ROOT[3];
        auto e = SQUARE_ROOT[4];
        auto f = SQUARE_ROOT[5];
        auto g = SQUARE_ROOT[6];
        auto h = SQUARE_ROOT[7];

        for (auto i = 0; i < 64; i++)
        {
            auto s1 = s1_op(e);
            auto s0 = s0_op(a);
            auto maj = maj_op(a, b, c);
            auto ch = ch_op(e, f, g);

            auto tmp1 = s1 + ch + h + new_vec[i] + CONSTANTS[i];
            auto tmp2 = s0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + tmp1;
            d = c;
            c = b;
            b = a;
            a = tmp1 + tmp2;
        }

        SQUARE_ROOT[0] += a;
        SQUARE_ROOT[1] += b;
        SQUARE_ROOT[2] += c;
        SQUARE_ROOT[3] += d;
        SQUARE_ROOT[4] += e;
        SQUARE_ROOT[5] += f;
        SQUARE_ROOT[6] += g;
        SQUARE_ROOT[7] += h;
    }
}

std::string Sha256::exec(char *input)
{
    auto data = preprocessing(input);
    hash(data);

    std::ostringstream oss;
    for (int i = 0; i < 8; i++)
    {
        oss << std::hex << std::setfill('0') << std::setw(2) << SQUARE_ROOT[i];
    }

    return oss.str();
}

