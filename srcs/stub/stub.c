#include "woody.h"

static void    fake_entry() {
    printf("Original program running!\n");
    printf("Message: This is the original programm\n");
}

void    stub(t_woody_meta *metadata, unsigned char *encrypted_text)
{
    write(1, "....WOODY....\n", 14);

    xtea_decrypt_buff(encrypted_text, metadata->text_size, metadata->key);

    void    (*entrypoint)() = (void(*)())fake_entry;
    entrypoint();
}
