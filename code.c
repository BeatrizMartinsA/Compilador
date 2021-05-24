#include "globals.h"
#include "code.h"

void emitComment( char * c ) { 
  if (TraceCode) fprintf(code,"* %s\n",c);
}