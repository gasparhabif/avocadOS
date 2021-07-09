#include "proceso2.h"

int get_free_block()
{
    for (int i = 0; i < superbloque->blocks; i++)
    {
        if (!bitarray_test_bit(superbloque->bitmap, i))
        {
            return i;
        }
    }
}

void set_block(int block_index)
{
    bitarray_set_bit(superbloque->bitmap, block_index);
    memcpy(superbloque_file + 2 * sizeof(uint32_t), superbloque->bitmap->bitarray, superbloque->bitmap->size);
}

void clean_block(int block_index)
{
    bitarray_clean_bit(superbloque->bitmap, block_index);
    memcpy(superbloque_file + 2 * sizeof(uint32_t), superbloque->bitmap->bitarray, superbloque->bitmap->size);
}