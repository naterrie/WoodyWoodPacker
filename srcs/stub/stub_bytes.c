unsigned char srcs_stub_stub_bin[] = {
    0x48, 0xB8,                         /* movabs imm64, %rax */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* imm64 placeholder */
    0x48, 0x8B, 0x00,                   /* mov (%rax), %rax */
    0xFF, 0xE0                          /* jmp *%rax */
};

unsigned long srcs_stub_stub_bin_len = sizeof(srcs_stub_stub_bin);
