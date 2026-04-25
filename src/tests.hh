#include "parsing.hh"
#include <stdlib.h>

void parse_header_from_file() {
  printf("Testing parse del header del fitxer simple \n");
  FILE* fptr = fopen("../assets/testfiles/SimpleMidi.mid", "r");
  assert(fptr != NULL);
  int i = 0, c;
  unsigned char* s = (unsigned char*)malloc(10000);
  while ((c = fgetc(fptr)) != EOF) {
    s[i] = c;
    i++;
  }
  
  Header h = parse_header(s, 12);
  printf("Parsed\n");
  assert(h.length == 6);
  assert(h.format == 1);
  assert(h.num_tracks == 2);
  assert(h.division == 480);
}
