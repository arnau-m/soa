#include <cbuff.h>
#include <errno.h>

void cb_init(struct circular_buffer *cb)
{
    cb->read_pos = 0;
    cb->write_pos = 0;
    cb->count = 0;
    cb->size = BUFFER_SIZE;
}

void cb_add(struct circular_buffer *cb, char c)
{
    if (cb->write_pos - cb->read_pos < BUFFER_SIZE)
    {
        cb->data[cb->write_pos % BUFFER_SIZE] = c;
        printk("\nEl caracter ");
        printc(cb->data[cb->write_pos % BUFFER_SIZE]);
        printk(" s'ha afegit a la posicio ");
        print_int(cb->write_pos);
        printk(" del buffer circular.\n");
        ++cb->write_pos;
    }
    else
    {
        printk("\nS'ha arribat al final del buffer, llegeix d'aquest per alliverar espai\n");
    }
}

char cb_get(struct circular_buffer *cb)
{
    if (cb->read_pos < cb->write_pos)
    {
        char c = cb->data[cb->read_pos % BUFFER_SIZE];
        printk("\nObtenim el caracter: ");
        printc(cb->data[cb->read_pos % BUFFER_SIZE]);
        printk(" de la posicio ");
        print_int(cb->read_pos % BUFFER_SIZE);
        printk(" del buffer l'alliberem");
        ++cb->read_pos;
        return c;
    }
    else
    {
        printk("\nNo hi ha mes elements a llegir");
        return '\0';
    }
}