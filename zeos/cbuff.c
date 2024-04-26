#include <cbuff.h>
#include <errno.h>

void cb_init(struct circular_buffer *cb)
{
    cb->read_pos = 0;
    cb->write_pos = 0;
    cb->count = 0;
    cb->size = 0;
}

void cb_add(struct circular_buffer *cb, char c)
{
    if (cb->write_pos - cb->read_pos < BUFFER_SIZE)
    {
        cb->data[cb->write_pos % BUFFER_SIZE] = c;
        //printk("\nEl caracter ");
        //printc(cb->data[cb->write_pos % BUFFER_SIZE]);
        //printk(" s'ha afegit a la posicio ");
        //print_int(cb->write_pos % BUFFER_SIZE);
        //printk(" del buffer circular.\n");
        ++cb->write_pos;
        ++cb->size;
    }
}

char cb_get(struct circular_buffer *cb)
{
    if (cb->read_pos < cb->write_pos)
    {
        char c = cb->data[cb->read_pos % BUFFER_SIZE];
        ++cb->read_pos;
        --cb->size;
        return c;
    }
    else
    {
        return '\0';
    }
}

void cb_print(struct circular_buffer *cbl)
{
    for (int i = 0; i < cbl->size; i++)
    {
        print_int(cbl->size);
        cb_get(&cbl);
    }
}