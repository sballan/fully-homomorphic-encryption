// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// The main method was split off from the rest of the code to allow testing.
#include <stdlib.h>

#include <bitset>
#include <cassert>
#include <codecvt>
#include <iostream>
#include <locale>
#include <string>
#include <time.h>

void count(
  char record[16+1], 
  char query[8], 
  int result[1+1]
) {
  // Return if record is blank
  if(record[0] == '\0') {
    result[1]++;
    return;
  }

  #pragma hls_unroll yes
  for(int i=0; i<16/2; i++) {
    if(query[i] != record[i]) {
      if(query[i] == '*') {
        result[0]++;
      } else {
        result[1]++;  // This is meant to alter the ciphertext even though the plaintext doesn't change
      }
      return;
    }
  }

  result[0]++;
}
void insert(
  char record[16+1], 
  char query[16], 
  int result[1+1]
) {
  // Return if we already inserted
  if(result[0] == 1) {
    record[16+1]++;
    result[1]++;
    return;
  }

  // Return if record is not blank
  if(record[0] != '\0') {
    record[16+1]++;
    result[1]++;
    return;
  }

  for(int i=0; i<16; i++) {
    record[i] = query[i];
  }

  result[0]=1;
}
void selectIndex(
  char record[16+1], 
  char query[8], 
  char result[8+1]
) {
  // Return if record was already found
  if(result[0] != '\0'){
    result[8+1]++;         // This is meant to alter the ciphertext even though the plaintext doesn't change
    return;
  } 

  // Return if record is blank
  if(record[0] == '\0'){
    result[8+1]++;         
    return;
  } 

  for(int i=0; i<8; i++) {
    if(query[i] != record[i]) {
      result[8+1]++; 
      return;
    }
  }

  for(int i=0; i<8; i++) {
    result[i] = record[i+8];
  }
}



void readAll(
  char record[16+1], 
  int counter[1+1],
  char result[32+1]
) {
  // Return if record is blank
  if(record[0] == '\0') {
    record[16+1]++;
    counter[1]++;
    result[32+1]++;
    return;
  }

  for(int i=0; i<16; i++) {
    result[counter[0]+i] = record[i];
  }

  counter[0]+=16;
  counter[1]++;
}



int main() {
  std::cout << "Starting..." << std::endl;


  std::cout << "Starting initial database handshake" << std::endl;

  char demo1_record[16+1] = {'T', 'i', 't', 'l', 'e', 0, 0, 0, 'C', 'o', 'n', 't', 'e', 'n', 't', 0, 0};
  char demo2_record[16+1] = {'C', 'l', 'a', 's', 's', 's', 0, 0, '2', '0', '2', '2', 0, 0, 0, 0, 0};
  char demo3_record[16+1] = {'C', 'i', 't', 'y', 0, 0, 0, 0, 'N', 'Y', 'C', 0, 0, 0, 0, 0, 0};
  char blank_record[16+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


  // records are seralized, sent to carol, and persisted.  all future accesses of records happen by carol

  std::cout << "Database handshake complete\n\n" << std::endl;

  std::cout << "Welcome to Discreet Logs!" << std::endl;
  
  int op_type;
  while(true) {
    op_type = -1;

    std::cout << "Please enter the operation type: \n" 
                 "1: Read all \n"
                 "2: Search \n"
                 "3: Insert \n"
                 "4: Count \n"
    << std::endl;

    std::cin >> op_type;

    if(op_type == 1) {
      
      int raw_counter[2] = {0, 0};
      char raw_result[32+1];
      for(int i=0;i<(32+1);i++) {
        raw_result[i] = 0;
      }

        clock_t tStart = clock();
        readAll(demo1_record, raw_counter, raw_result);
        readAll(demo2_record, raw_counter, raw_result);
        readAll(demo3_record, raw_counter, raw_result);
        readAll(blank_record, raw_counter, raw_result);
        printf("Time taken: %.10fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);


      auto output = raw_result;
      for (int i=0; i<32; i++){
        std::cout << output[i];
      }
      std::cout << "\n" << std::endl;      
    } else if(op_type == 2) {
      char result[8+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
      char input_chars[8];
      std::cout << "Please enter the query (max size 8 chars): \n"  << std::endl;
      
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::string input;
      std::cin >> input;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      std::cout << "*" << input << "*" << std::endl;
      clock_t tStart = clock();

      for(int i=0; i<8; i++) {
        if(i < input.size() && input[i] != '\n') {
          input_chars[i] = input[i];
        } else {
          input_chars[i] = 0; 
        }      
      }
      printf("Time taken: %.10fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);        
      selectIndex(demo1_record, input_chars, result);
      selectIndex(demo2_record, input_chars, result);
      selectIndex(demo3_record, input_chars, result);
      selectIndex(blank_record, input_chars, result);



      auto output = result;
      for (int i=0; i<8; i++){
        std::cout << output[i];
      }      std::cout << "\n" << std::endl;
    }
    else if(op_type == 3) {
      int result[1+1] = {0,0};
      char input_chars[16];
      std::cout << "Please enter the title (max size 8 chars): \n"  << std::endl;
      
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::string input;
      std::cin >> input;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      // std::cout << "*" << input << "*" << std::endl;

      clock_t tStart = clock();
      for(int i=0; i<8; i++) {
        if(i < input.size() && input[i] != '\n') {
          input_chars[i] = input[i];
        } else {
          input_chars[i] = 0; 
        }      
      }
      std::cout << "*" << input_chars << "*" << std::endl;

      std::cout << "Please enter the content (max size 8 chars): \n"  << std::endl;
      
      std::string input2;
      // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cin >> input2;
      // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      // std::cout << "*" << input2 << "*" << std::endl;

      for(int i=0; i<8; i++) {
        if(i < input2.size() && input2[i] != '\n') {
          input_chars[i+8] = input2[i];
        } else {
          input_chars[i+8] = 0; 
        }      
      }

      std::cout << "*";
      for(int i=8; i<16; i++) {
        std::cout << input_chars[i];
      }
      std::cout << "*";
      std::cout << std::endl;
      
        insert(demo1_record, input_chars, result);
        insert(demo2_record, input_chars, result);
        insert(demo3_record, input_chars, result);
        insert(blank_record, input_chars, result);

      printf("Time taken: %.10fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);        

      auto output = result;
      std::cout << output[0];
      std::cout << output[1];
      std::cout << "\n" << std::endl;
    } 
    else if(op_type == 4) {
      int result[1+1] = {0, 0};

      char input_chars[16];
      std::cout << "Please enter the query (max size 8 chars), use * for wildcard: \n"  << std::endl;
      
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::string input;
      std::cin >> input;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      std::cout << "*" << input << "*" << std::endl;
      clock_t tStart = clock();
      for(int i=0; i<16; i++) {
        if(i < input.size() && input[i] != '\n') {
          input_chars[i] = input[i];
        } else {
          input_chars[i] = 0; 
        }      
      }
              
        count(demo1_record, input_chars, result);
        count(demo2_record, input_chars, result);
        count(demo3_record, input_chars, result);
        count(blank_record, input_chars, result);


      auto output = result;
      printf("Time taken: %.10fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);        

      std::cout << output[0];
      std::cout << "\n" << std::endl;
    }
      }


  return 0;
}
