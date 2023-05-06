unsigned char set_bit(unsigned char c, int i)
{
    unsigned char mask = 1 << i;
    return mask | c;
}

int is_bit_i_set(unsigned char c, int i)
{
    unsigned char mask = 1 << i;
    unsigned char res = mask & c;
    return res != 0;
}