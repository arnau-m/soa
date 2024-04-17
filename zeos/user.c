#include <libc.h>

int pid;

char buff[24];

char *lorem = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec pellentesque libero rutrum mi rutrum, et volutpat ex faucibus. Integer fringilla velit ipsum, sed lacinia tellus feugiat ut. Pellentesque ultrices pretium cursus. Integer feugiat vitae justo a tempus. Vivamus in orci id erat auctor hendrerit a non nisl. Sed rutrum sodales nulla, id lacinia turpis. Sed ultrices orci quis nunc porta pellentesque. Fusce efficitur lobortis ligula. Nunc ullamcorper, enim at dictum rutrum, eros ipsum vehicula magna, sed lacinia purus sem vitae risus. Sed commodo tellus sed augue ultricies pharetra. Ut at dui justo. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Cras aliquet id odio sed mollis. Maecenas in dapibus lorem. Vivamus ut eros vel arcu vivERRa efficitur. Integer facilisis placerat rhoncus. Vestibulum quis nulla vitae ante rhoncus feugiat. Phasellus sagittis porta semper. Pellentesque ac tortor eu tellus maximus molestie at ut libero. Cras a volutpat dolor. Fusce egestas a ante sit amet euismod. Mauris viverra velit vel ipsum varius, vel finibus libero vulputate. Vivamus tincidunt lacus nec nulla luctus, in facilisis ligula pretium. Proin metus augue, hendrerit quis magna tempus, mattis varius eros. Proin condimentum dignissim massa, vel tempor eros aliquet ut. Quisque venenatis tellus ac mi interdum condimentum. Ut id elit ut nunc porta tincidunt in et massa. Phasellus mollis nunc ex, ac ultricies tellus gravida ac. Aliquam quam lorem, pharetra et eros ac, sodales suscipit nunc. Phasellus efficitur risus a nulla mollis efficitur. In sollicitudin pellentesque libero, non molestie odio porta nec. Integer metus nisl, cursus non sollicitudin sagittis, ornare pulvinar purus. Cras tincidunt elementum enim, nec vestibulum felis tempus at. Sed aliquam sed lacus id tincidunt. Interdum et malesuada fames ac ante ipsum primis in faucibus. Ut commodo risus molestie ex ullamcorper tempor. Sed rutrum accumsan viverra. Nulla in elit facilisis, finibus nulla vitae, tristique sem. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Cras egestas egestas erat et pretium. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Praesent maximus urna ut risus euismod, quis ultrices eros viverra. Nulla vulputate sodales sapien, eget vulputate elit lacinia at. Integer auctor non lorem vitae mattis. Duis quis risus augue. Nulla tristique auctor sem. Nulla at turpis ipsum. Vivamus congue, nibh at auctor luctus, ligula erat placerat ante, vitae tristique ante risus id justo. Aenean id nisl non eros venenatis iaculis non nec libero. Phasellus rhoncus, odio sit amet accumsan hendrerit, tortor nisi facilisis massa, feugiat aliquam ipsum diam non mi. Donec dignissim odio ut turpis feugiat, ac suscipit risus sagittis. Maecenas varius a mauris eu convallis. Aliquam tristique augue leo, eget tincidunt nibh blandit ut. In hac habitasse platea dictumst. Nam ac neque sit amet neque gravida aliquam. Aliquam finibus consequat ullamcorper. Quisque commodo lectus nisl, eu venenatis justo feugiat sit amet. Vivamus ullamcorper dolor lacinia nulla dictum interdum. Sed dictum libero nec libero ornare, eu tristique justo egestas. Mauris id nunc nec est condimentum facilisis. Praesent rhoncus et nibh vel fermentum. In nec lectus a nulla accumsan pulvinar. Nunc convallis pharetra dui, vel interdum metus convallis vitae. Phasellus ullamcorper ex quis venenatis pellentesque. Phasellus aliquam efficitur nisi id feugiat. Fusce aliquet tortor eu iaculis porttitor. Cras in nisi nec leo ullamcorper luctus. Donec sed elit sed neque sollicitudin cursus. Ut interdum vitae dui eget mollis. Donec in justo felis. Quisque ultricies volutpat congue. In libero lacus, cursus vitae ex non, scelerisque mollis purus. Donec vel justo libero. Phasellus et nibh massa. Pellentesque suscipit ante dolor. Sed vestibulum egestas felis et iaculis. Integer ut velit id leo mattis vulputate a at tellus. In ut nisl iaculis, commodo enim quis, scelerisque ex.";

int __attribute__((__section__(".text.main")))
main(void)
{
  /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
  /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  int fill = fork();
  if (fill != 0)
  {
    write(1, "\nSoc pare, el PID del meu fill es: ", strlen("\nSoc pare, el PID del meu fill es: "));
    itoa(fill, buff);
    write(1, buff, strlen(buff));
  }

  while (1)
  {
  }
}
