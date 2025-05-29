#include <fcgi_stdio.h>

int main() {
   while (FCGI_Accept() >= 0) {
       FCGI_printf("Content-type: text/html\n\n");
       FCGI_printf("<h1>Hello, World!</h1>");
       FCGI_printf("</body></html>");
   }
   return 0;
}