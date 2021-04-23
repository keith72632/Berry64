void bzero(void * dest, unsigned int bytes)
{
    char * d = dest;
    while(bytes--){
        *d++ = 0;
    }
}