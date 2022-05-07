#ifndef FULLY_HOMOMORPHIC_ENCRYPTION_TRANSPILER_EXAMPLES_DISCREET_LOGS_DISCREET_LOGS_SELECT_INDEX_H
#define FULLY_HOMOMORPHIC_ENCRYPTION_TRANSPILER_EXAMPLES_DISCREET_LOGS_DISCREET_LOGS_SELECT_INDEX_H

const unsigned short MAX_SIZE = 16;

void selectIndex(
  char record[MAX_SIZE], 
  char query[MAX_SIZE], 
  char result[MAX_SIZE/2+1]
);

#endif  // FULLY_HOMOMORPHIC_ENCRYPTION_TRANSPILER_EXAMPLES_DISCREET_LOGS_DISCREET_LOGS_SELECT_INDEX_H
