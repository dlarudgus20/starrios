void kmain(void)
{
    char *video = (char *)0xb8000;
    const char *msg = "hello kmain";

    for (const char* ptr = msg; *ptr != '\0'; ++ptr)
    {
        *video++ = *ptr;
        *video++ = 0x7d;
    }

    while (1) __asm__ __volatile__ ( "hlt" );
}
