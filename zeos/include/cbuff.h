#ifndef _CBUFF_H
#define _CBUFF_H

#define BUFFER_SIZE 5

struct circular_buffer{
    char data[BUFFER_SIZE];
    int read_pos;
    int write_pos;
    int count;
    int size;
};

void cb_init(struct circular_buffer *cb);
void cb_add(struct circular_buffer *cb, char c);
char cb_get(struct circular_buffer *cb);
void cb_print(struct circular_buffer *cb);

#endif /* _CBUFF_H */
