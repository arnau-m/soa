/*
 * io.c -
 */

#include <io.h>

#include <types.h>

/**************/
/** Screen  ***/
/**************/

#define NUM_COLUMNS 80
#define NUM_ROWS 25

Byte x, y = 19;
int fg = 0;
int bg = 0;

/* Read a byte from 'port' */
Byte inb(unsigned short port)
{
  Byte v;

  __asm__ __volatile__("inb %w1,%0" : "=a"(v) : "Nd"(port));
  return v;
}

void printc(char c)
{
  __asm__ __volatile__("movb %0, %%al; outb $0xe9" ::"a"(c)); /* Magic BOCHS debug: writes 'c' to port 0xe9 */
  if (c == '\n')
  {
    x = 0;
    y = (y + 1) % NUM_ROWS;
  }
  else
  {
    /*Colors:
      0- Negre
      1- Blau
      2- Verd
      3- Blau clar
      4- Vermell
      5- Rosa
      6- Taronja
      7- Blanc
    */

    Word ch = (Word)(c & 0x00FF) | (256 * fg + 4096 * bg);
    Word *screen = (Word *)0xb8000;
    screen[(y * NUM_COLUMNS + x)] = ch;
    if (++x >= NUM_COLUMNS)
    {
      x = 0;
      y = (y + 1) % NUM_ROWS;
    }
  }
}

void printc_xy(Byte mx, Byte my, char c)
{
  Byte cx, cy;
  cx = x;
  cy = y;
  x = mx;
  y = my;
  printc(c);
  x = cx;
  y = cy;
}

void printk(char *string)
{
  int i;
  for (i = 0; string[i]; i++)
    printc(string[i]);
}

void cursor_xy(Byte mx, Byte my)
{
  x = mx;
  y = my;
}

void chg_color(int f, int b)
{
  fg = f;
  bg = b;
}

void print_int(int num)
{
  char buffer[12]; // Suficientemente grande para representar el número y el signo (si es negativo) y el carácter nulo terminador
  int i = 0;
  int is_negative = 0;

  // Manejo de números negativos
  if (num < 0)
  {
    is_negative = 1;
    num = -num; // Hacemos que el número sea positivo
  }

  // Convertir el número en una cadena de caracteres
  do
  {
    buffer[i++] = num % 10 + '0'; // Obtener el dígito menos significativo y convertirlo en carácter
    num /= 10;                    // Reducir el número
  } while (num != 0);

  // Añadir el signo negativo si es necesario
  if (is_negative)
  {
    buffer[i++] = '-';
  }

  // Imprimir los caracteres en orden inverso (debido a cómo se construyó el buffer)
  while (i > 0)
  {
    printc(buffer[--i]);
  }
}
