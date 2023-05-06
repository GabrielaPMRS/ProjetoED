/**
 * @brief Seta um bit.
 *
 * @param c O byte onde o bit será setado.
 * @param i A posição no byte que o bit será setado.
 * @return Retorna o byte setado.
 */
unsigned char set_bit(unsigned char c, int i)
{
    unsigned char mask = 1 << i;
    return mask | c;
}

/**
 * @brief Avalia se o bit, numa posição especifica, está setado.
 *
 * @param c O byte em que o bit sendo avaliado está.
 * @param i A posição do bit sendo avaliado.
 * @return Retorna 1 se o byte estiver setado, 0 se não.
 */
int is_bit_i_set(unsigned char c, int i)
{
    unsigned char mask = 1 << i;
    unsigned char res = mask & c;
    return res != 0;
}