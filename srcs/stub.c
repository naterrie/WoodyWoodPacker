#include "woody.h"

static void    fake_entry() {
    printf("Original program running!\n");
    printf("Message: This is the original programm\n");
}

void    stub(t_encrypt_data *enc_data)
{
    write(1, "....WOODY....\n", 14);
	xtea_decrypt_buff(enc_data->path, enc_data->size + enc_data->padding, enc_data->key, enc_data->padding);

    void    (*entrypoint)() = (void(*)())fake_entry;
    entrypoint();
}