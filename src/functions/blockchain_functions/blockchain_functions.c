#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "variables.h"
#include "structures.h"
#include "define_macros_test.h"

#include "blockchain_functions.h"
#include "string_functions.h"
#include "network_daemon_functions.h"
#include "network_wallet_functions.h"
#include "vrf.h"
#include "crypto_vrf.h"
#include "VRF_functions.h"
#include "sha512EL.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: varint_encode
Description: Encodes varints for the get_block_template blocktemplate_blob
Parameters:
  number - The number to decode
  result - the string to store the result
Return: 1 if successfull, otherwise 0
-----------------------------------------------------------------------------------------------------------
*/

int varint_encode(long long int number, char *result, const size_t RESULT_TOTAL_LENGTH)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  size_t length;
  size_t count = 0;
  size_t count2 = 0;
  int binary_numbers[BITS_IN_BYTE] = {0,0,0,0,0,0,0,0};
  int binary_number_copy;
  long long int number_copy = number;  

  memset(data,0,sizeof(data));
  memset(result,0,RESULT_TOTAL_LENGTH);  

  // check if it should not be encoded
  if (number <= 0xFF)
  {
    snprintf(result,RESULT_TOTAL_LENGTH,"%02llx",number);
    return 1;
  }

  // convert the number to a binary string
  for (count = 0; number_copy != 0; count++)
  {
    if (number_copy % 2 == 1)
    {
      append_string(data,"1",sizeof(data));
    }
    else
    {
      append_string(data,"0",sizeof(data));
    }
    number_copy /= 2; 
  }

  // pad the string to a mulitple of 7 bits  
  for (count = strnlen(data,sizeof(data)); count % (BITS_IN_BYTE-1) != 0; count++)
  {
    append_string(result,"0",RESULT_TOTAL_LENGTH);
  }

  // reverse the string
  length = strnlen(data,sizeof(data));
  for (count = 0; count <= length; count++)
  {
    memcpy(result+strlen(result),&data[length - count],sizeof(char));
  }
  memset(data,0,sizeof(data));
  append_string(data,result,sizeof(data));
  memset(result,0,RESULT_TOTAL_LENGTH);

  /*
  convert each 7 bits to one byte
  set the first bit to 1 for all groups of 7 except for the first group of 7
  */
  length = strnlen(data,sizeof(data)) + (strnlen(data,sizeof(data)) / (BITS_IN_BYTE-1));

 for (count = 0, count2 = 0; count < length; count++)
 {
   if (count % BITS_IN_BYTE == 0 && count != 0)
   {
     // reverse the binary bits
     binary_number_copy = 0;       
     if (((binary_numbers[count2] >> 7) & 1U) == 1) {binary_number_copy |= 1UL << 0;} else {binary_number_copy &= ~(1UL << 0);}
     if (((binary_numbers[count2] >> 6) & 1U) == 1) {binary_number_copy |= 1UL << 1;} else {binary_number_copy &= ~(1UL << 1);}
     if (((binary_numbers[count2] >> 5) & 1U) == 1) {binary_number_copy |= 1UL << 2;} else {binary_number_copy &= ~(1UL << 2);}
     if (((binary_numbers[count2] >> 4) & 1U) == 1) {binary_number_copy |= 1UL << 3;} else {binary_number_copy &= ~(1UL << 3);}
     if (((binary_numbers[count2] >> 3) & 1U) == 1) {binary_number_copy |= 1UL << 4;} else {binary_number_copy &= ~(1UL << 4);}
     if (((binary_numbers[count2] >> 2) & 1U) == 1) {binary_number_copy |= 1UL << 5;} else {binary_number_copy &= ~(1UL << 5);}
     if (((binary_numbers[count2] >> 1) & 1U) == 1) {binary_number_copy |= 1UL << 6;} else {binary_number_copy &= ~(1UL << 6);}
     if (((binary_numbers[count2] >> 0) & 1U) == 1) {binary_number_copy |= 1UL << 7;} else {binary_number_copy &= ~(1UL << 7);}
     binary_numbers[count2] = binary_number_copy;
     count2++;
   } 
   if (count % BITS_IN_BYTE == 0)
   {
     binary_numbers[count2] = count == 0 ? binary_numbers[count2] & ~(1 << (count % BITS_IN_BYTE)) : binary_numbers[count2] | 1 << (count % BITS_IN_BYTE);
   }
   else
   {
     binary_numbers[count2] = strncmp(data + (count - (count2+1)),"1",1) == 0 ? binary_numbers[count2] | 1 << (count % BITS_IN_BYTE) : binary_numbers[count2] & ~(1 << (count % BITS_IN_BYTE));  
   }
 }

  // reverse the last binary_number
  length = strnlen(data,sizeof(data)) / BITS_IN_BYTE;
  binary_number_copy = 0;
  if (((binary_numbers[length] >> 7) & 1U) == 1) {binary_number_copy |= 1UL << 0;} else {binary_number_copy &= ~(1UL << 0);}
  if (((binary_numbers[length] >> 6) & 1U) == 1) {binary_number_copy |= 1UL << 1;} else {binary_number_copy &= ~(1UL << 1);}
  if (((binary_numbers[length] >> 5) & 1U) == 1) {binary_number_copy |= 1UL << 2;} else {binary_number_copy &= ~(1UL << 2);}
  if (((binary_numbers[length] >> 4) & 1U) == 1) {binary_number_copy |= 1UL << 3;} else {binary_number_copy &= ~(1UL << 3);}
  if (((binary_numbers[length] >> 3) & 1U) == 1) {binary_number_copy |= 1UL << 4;} else {binary_number_copy &= ~(1UL << 4);}
  if (((binary_numbers[length] >> 2) & 1U) == 1) {binary_number_copy |= 1UL << 5;} else {binary_number_copy &= ~(1UL << 5);}
  if (((binary_numbers[length] >> 1) & 1U) == 1) {binary_number_copy |= 1UL << 6;} else {binary_number_copy &= ~(1UL << 6);}
  if (((binary_numbers[length] >> 0) & 1U) == 1) {binary_number_copy |= 1UL << 7;} else {binary_number_copy &= ~(1UL << 7);}
  binary_numbers[length] = binary_number_copy;

  // create the varint encoded string
  for (count = 0, count2 = 0; count <= length; count++, count2 += 2)
  {
    snprintf(result+count2,RESULT_TOTAL_LENGTH,"%02x",binary_numbers[length-count] & 0xFF);
  }
  
  return 1;    
}



/*
-----------------------------------------------------------------------------------------------------------
Name: varint_decode
Description: Decodes varints for the get_block_template blocktemplate_blob
Parameters:
  varint - The varint to decode
Return: The decoded varint
-----------------------------------------------------------------------------------------------------------
*/

size_t varint_decode(size_t varint)
{
  // Variables
  int length = 0;
  int count = 0;
  int counter = 0;
  int bytecount = 0;
  size_t number = 1;
  int start = 0;

  // get the length
  if (varint <= 0xFF)
  {
    return varint;
  }
  else if (varint > 0xFF && varint < 0xFFFF)
  {
    length = 2;
  }
  else if (varint >= 0xFFFF && varint < 0xFFFFFF)
  {
    length = 3;
  }
  else if (varint >= 0xFFFFFF && varint < 0xFFFFFFFF)
  {
    length = 4;
  }
  else if (varint >= 0xFFFFFFFF && varint < 0xFFFFFFFFFF)
  {
    length = 5;
  }
  else if (varint >= 0xFFFFFFFFFF && varint < 0xFFFFFFFFFFFF)
  {
    length = 6;
  }
  else if (varint >= 0xFFFFFFFFFFFF && varint < 0xFFFFFFFFFFFFFF)
  {
    length = 7;
  }
  else
  {
    length = 8;
  }

  // create a byte array for the varint
  char bytes[length];

  for (count = 0; count < length; count++)
  {
    // convert each byte to binary and read the bytes in reverse order
    bytes[count] = ((varint >> (BITS_IN_BYTE * count)) & 0xFF);
  }
    
  for (count = 0, counter = (BITS_IN_BYTE-1), bytecount = 0, start = 0; count < length * BITS_IN_BYTE; count++)
  {
    // loop through each bit until you find the first 1. for every bit after this:
    // if 0 then number = number * 2;
    // if 1 then number = (number * 2) + 1;
    // dont use the bit if its the first bit
    if (counter != (BITS_IN_BYTE-1))
    {
      if (bytes[bytecount] & (1 << counter)) 
      {
        if (start == 1)
        {
          number = (number * 2) + 1;
        }
        start = 1;
      }
      else
      {
        if (start == 1)
        {
          number = number * 2;
        }
      } 
    }
      
    if (counter == 0) 
    {
      counter = (BITS_IN_BYTE-1);
      bytecount++;
    }
    else
    {
      counter--;
    }
  }
 return number;    
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_generated_supply
Description: Gets the generated supply up to the given block height
Parameters:
  BLOCK_HEIGHT - The given block height
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

double get_generated_supply(const size_t BLOCK_HEIGHT)
{
  // Variables
  double generated_supply = FIRST_BLOCK_MINING_REWARD + XCASH_PREMINE_TOTAL_SUPPLY;
  size_t count;

  for (count = 2; count < BLOCK_HEIGHT; count++)
  {
    generated_supply = count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT ? generated_supply + (XCASH_TOTAL_SUPPLY - generated_supply) / XCASH_EMMISION_FACTOR : generated_supply + (XCASH_TOTAL_SUPPLY - generated_supply) / XCASH_DPOPS_EMMISION_FACTOR;
  }
  return generated_supply;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: network_block_string_to_blockchain_data
Description: Converts a network_block_string string to the blockchain_data struct
Parameters:
  DATA - The network_block_string
  BLOCK_HEIGHT - The block height of the network block string, 0 to get the current block height
  BLOCK_VERIFIERS_TOTAL - The maximum amount of block verifiers
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int network_block_string_to_blockchain_data(const char* DATA, const char* BLOCK_HEIGHT, const int BLOCK_VERIFIERS_TOTAL)
{
  // Variables
  char block_height[MAXIMUM_NUMBER_SIZE];
  char data2[BUFFER_SIZE];
  char* data3;
  char* message_copy1;
  size_t count;
  size_t count2;
  size_t count3;
  size_t number;

  // define macros
  #define NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"network_block_string_to_blockchain_data",39); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0; 

  memset(block_height,0,sizeof(block_height));
  memset(data2,0,sizeof(data2));

  RESET_BLOCKCHAIN_DATA;

  // network_version
  blockchain_data.network_version_data_length = sizeof(NETWORK_VERSION)-1;
  count+= blockchain_data.network_version_data_length;
  memcpy(blockchain_data.network_version_data,DATA,blockchain_data.network_version_data_length);

  // timestamp
  blockchain_data.timestamp_data_length = TIMESTAMP_LENGTH;
  count+= blockchain_data.timestamp_data_length;
  memcpy(blockchain_data.timestamp_data,&DATA[count-blockchain_data.timestamp_data_length],blockchain_data.timestamp_data_length);
  blockchain_data.timestamp = varint_decode((size_t)strtol(blockchain_data.timestamp_data, NULL, 16));

  // previous_block_hash
  blockchain_data.previous_block_hash_data_length = BLOCK_HASH_LENGTH;
  count+= blockchain_data.previous_block_hash_data_length;
  memcpy(blockchain_data.previous_block_hash_data,&DATA[count-blockchain_data.previous_block_hash_data_length],blockchain_data.previous_block_hash_data_length);

  // nonce
  blockchain_data.nonce_data_length = sizeof(BLOCK_PRODUCER_NETWORK_BLOCK_NONCE)-1;
  count+= blockchain_data.nonce_data_length;
  memcpy(blockchain_data.nonce_data,&DATA[count-blockchain_data.nonce_data_length],blockchain_data.nonce_data_length);

  // block_reward_transaction_version
  blockchain_data.block_reward_transaction_version_data_length = sizeof(BLOCK_REWARD_TRANSACTION_VERSION)-1;
  count+= blockchain_data.block_reward_transaction_version_data_length;
  memcpy(blockchain_data.block_reward_transaction_version_data,&DATA[count-blockchain_data.block_reward_transaction_version_data_length],blockchain_data.block_reward_transaction_version_data_length);

  // unlock_block
  // get the current block height
  if (strncmp(BLOCK_HEIGHT,"0",1) == 0)
  {
    if (get_current_block_height(block_height) == 0)
    {
      NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Could not get the current block height, Invalid unlock_block");
    }
    sscanf(block_height, "%zu", &number);
  }
  else
  {
    sscanf(BLOCK_HEIGHT, "%zu", &number);
  }

  blockchain_data.unlock_block_data_length = (number + UNLOCK_BLOCK_AMOUNT) > (VARINT_DECODED_VALUE_END_2_BYTE-UNLOCK_BLOCK_AMOUNT) ? 8 : 6;  
  count+= blockchain_data.unlock_block_data_length;
  memcpy(blockchain_data.unlock_block_data,&DATA[count-blockchain_data.unlock_block_data_length],blockchain_data.unlock_block_data_length);
  blockchain_data.unlock_block = varint_decode((size_t)strtol(blockchain_data.unlock_block_data, NULL, 16));

  // block_reward_input
  blockchain_data.block_reward_input_data_length = sizeof(BLOCK_REWARD_INPUT)-1;
  count+= blockchain_data.block_reward_input_data_length;
  memcpy(blockchain_data.block_reward_input_data,&DATA[count-blockchain_data.block_reward_input_data_length],blockchain_data.block_reward_input_data_length);

  // vin_type
  blockchain_data.vin_type_data_length = sizeof(VIN_TYPE)-1;
  count+= blockchain_data.vin_type_data_length;
  memcpy(blockchain_data.vin_type_data,&DATA[count-blockchain_data.vin_type_data_length],blockchain_data.vin_type_data_length);

  // block_height
  blockchain_data.block_height_data_length = number > VARINT_DECODED_VALUE_END_2_BYTE ? 8 : 6;
  count+= blockchain_data.block_height_data_length;
  memcpy(blockchain_data.block_height_data,&DATA[count-blockchain_data.block_height_data_length],blockchain_data.block_height_data_length);
  blockchain_data.block_height = varint_decode((size_t)strtol(blockchain_data.block_height_data, NULL, 16));

  // block_reward_output
  blockchain_data.block_reward_output_data_length = sizeof(BLOCK_REWARD_OUTPUT)-1;
  count+= blockchain_data.block_reward_output_data_length;
  memcpy(blockchain_data.block_reward_output_data,&DATA[count-blockchain_data.block_reward_output_data_length],blockchain_data.block_reward_output_data_length);

  // block_reward
  // since the block reward could be any number because of transactions fees, get the position of BLOCKCHAIN_RESERVED_BYTES_START to get the length of the block reward
  if ((data3 = strstr(DATA,BLOCKCHAIN_RESERVED_BYTES_START)) == NULL && (data3 = strstr(DATA,BLOCKCHAIN_RESERVED_BYTES_DATA_HASH)) == NULL)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string, Invalid block_reward");
  }
  
  // if the block contained BLOCKCHAIN_RESERVED_BYTES_DATA_HASH subtract 136 since we already included the extra nonce tag and the reserve bytes size
  blockchain_data.block_reward_data_length = strstr(DATA,BLOCKCHAIN_RESERVED_BYTES_START) != NULL ? strlen(DATA) - strlen(data3) - count - 140 : strlen(DATA) - strlen(data3) - count - 136;
  count+= blockchain_data.block_reward_data_length;
  memcpy(blockchain_data.block_reward_data,&DATA[count-blockchain_data.block_reward_data_length],blockchain_data.block_reward_data_length);
  blockchain_data.block_reward = varint_decode((size_t)strtol(blockchain_data.block_reward_data, NULL, 16));  

  // stealth_address_output_tag
  blockchain_data.stealth_address_output_tag_data_length = sizeof(STEALTH_ADDRESS_OUTPUT_TAG)-1;
  count+= blockchain_data.stealth_address_output_tag_data_length;
  memcpy(blockchain_data.stealth_address_output_tag_data,&DATA[count-blockchain_data.stealth_address_output_tag_data_length],blockchain_data.stealth_address_output_tag_data_length);

  // stealth_address_output
  blockchain_data.stealth_address_output_data_length = STEALTH_ADDRESS_OUTPUT_LENGTH;
  count+= blockchain_data.stealth_address_output_data_length;
  memcpy(blockchain_data.stealth_address_output_data,&DATA[count-blockchain_data.stealth_address_output_data_length],blockchain_data.stealth_address_output_data_length);

  // extra_bytes_size
  blockchain_data.extra_bytes_size_data_length = 2;
  count+= blockchain_data.extra_bytes_size_data_length;
  memcpy(blockchain_data.extra_bytes_size_data,&DATA[count-blockchain_data.extra_bytes_size_data_length],blockchain_data.extra_bytes_size_data_length);
  blockchain_data.extra_bytes_size = varint_decode((size_t)strtol(blockchain_data.extra_bytes_size_data, NULL, 16));

  // transaction_public_key_tag
  blockchain_data.transaction_public_key_tag_data_length = sizeof(TRANSACTION_PUBLIC_KEY_TAG)-1;
  count+= blockchain_data.transaction_public_key_tag_data_length;
  memcpy(blockchain_data.transaction_public_key_tag_data,&DATA[count-blockchain_data.transaction_public_key_tag_data_length],blockchain_data.transaction_public_key_tag_data_length);

  // transaction_public_key
  blockchain_data.transaction_public_key_data_length = TRANSACTION_PUBLIC_KEY_LENGTH;
  count+= blockchain_data.transaction_public_key_data_length;
  memcpy(blockchain_data.transaction_public_key_data,&DATA[count-blockchain_data.transaction_public_key_data_length],blockchain_data.transaction_public_key_data_length);

  // extra_nonce_tag
  blockchain_data.extra_nonce_tag_data_length = sizeof(EXTRA_NONCE_TAG)-1;
  count+= blockchain_data.extra_nonce_tag_data_length;
  memcpy(blockchain_data.extra_nonce_tag_data,&DATA[count-blockchain_data.extra_nonce_tag_data_length],blockchain_data.extra_nonce_tag_data_length);

  // reserve_bytes_size
  blockchain_data.reserve_bytes_size_data_length = 2;
  count+= blockchain_data.reserve_bytes_size_data_length;
  memcpy(blockchain_data.reserve_bytes_size_data,&DATA[count-blockchain_data.reserve_bytes_size_data_length],blockchain_data.reserve_bytes_size_data_length);
  blockchain_data.reserve_bytes_size = varint_decode((size_t)strtol(blockchain_data.reserve_bytes_size_data, NULL, 16));

  if (strstr(DATA,BLOCKCHAIN_RESERVED_BYTES_DATA_HASH) == NULL)
  {
    // blockchain_reserve_bytes
    // skip the BLOCKCHAIN_RESERVED_BYTES_START
    count+= sizeof(BLOCKCHAIN_RESERVED_BYTES_START)-1;

    // block_producer_delegates_name
    message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
    blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length);
    // convert the hexadecimal string to a string
    for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length; count2++, number += 2)
    {
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,&blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data[number],2);
      blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name[count2] = (char)strtol(data2, NULL, 16);
    }
    count += blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length + sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1;
  
    // block_producer_public_address
    message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
    blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length);
    // convert the hexadecimal string to a string
    for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length; count2++, number += 2)
    {
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,&blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data[number],2);
      blockchain_data.blockchain_reserve_bytes.block_producer_public_address[count2] = (char)strtol(data2, NULL, 16);
    }
    count += blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length + sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1;
  
    // block_producer_node_backup_count
    message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
    blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length);
    // convert the hexadecimal string to a string
    for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length; count2++, number += 2)
    {
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,&blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data[number],2);
      blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count[count2] = (char)strtol(data2, NULL, 16);
    }
    count += blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length + sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1;

    // block_producer_backup_nodes_names
    message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
    blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length);
    // convert the hexadecimal string to a string
    for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length; count2++, number += 2)
    {
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,&blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data[number],2);
      blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names[count2] = (char)strtol(data2, NULL, 16);
    }
    count += blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length + sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1;

    // vrf_secret_key
    message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
    blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data_length);
    // convert the hexadecimal string to a string
    for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data_length; count2++, number += 2)
    {
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data[number],2);
      blockchain_data.blockchain_reserve_bytes.vrf_secret_key[count2] = (unsigned char)strtol(data2, NULL, 16);
    }
    count += blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data_length + sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1;

    // vrf_public_key
    message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
    blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length);
    // convert the hexadecimal string to a string
    for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length; count2++, number += 2)
    {
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_public_key_data[number],2);
      blockchain_data.blockchain_reserve_bytes.vrf_public_key[count2] = (unsigned char)strtol(data2, NULL, 16);
    }
    count += blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length + sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1;

    // vrf_alpha_string
    message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
    blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length);
    // convert the hexadecimal string to a string
    for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length; count2++, number += 2)
    {
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data[number],2);
      blockchain_data.blockchain_reserve_bytes.vrf_alpha_string[count2] = (unsigned char)strtol(data2, NULL, 16);
    }
    count += blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length + sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1;

    // vrf_proof
    message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
    blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length);
    // convert the hexadecimal string to a string
    for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length; count2++, number += 2)
    {
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_proof_data[number],2);
      blockchain_data.blockchain_reserve_bytes.vrf_proof[count2] = (unsigned char)strtol(data2, NULL, 16);
    }
    count += blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length + sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1;

    // vrf_beta_string
    message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
    blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length);
    // convert the hexadecimal string to a string
    for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length; count2++, number += 2)
    {
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data[number],2);
      blockchain_data.blockchain_reserve_bytes.vrf_beta_string[count2] = (unsigned char)strtol(data2, NULL, 16);
    }
    count += blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length + sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1;

    // vrf_data_round
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data_round,&DATA[count],sizeof(VRF_DATA)-1);
    count += (sizeof(VRF_DATA)-1) + (sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);

    // vrf_data
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data,&DATA[count],sizeof(VRF_DATA)-1);
    count += (sizeof(VRF_DATA)-1) + (sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);

    // block_verifiers_vrf_secret_key_data
    blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data_length = VRF_SECRET_KEY_LENGTH;
    for (count3 = 0; (int)count3 < BLOCK_VERIFIERS_TOTAL; count3++)
    { 
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count3],&DATA[count],blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data_length);
      count += blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data_length + sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1;
      // convert the hexadecimal string to a string
      for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data_length; count2++, number += 2)
      {
        memset(data2,0,strnlen(data2,BUFFER_SIZE));
        memcpy(data2,&blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count3][number],2);
        blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key[count3][count2] = (unsigned char)strtol(data2, NULL, 16);
      }    
    }

    // block_verifiers_vrf_public_key_data
    blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data_length = VRF_PUBLIC_KEY_LENGTH;
    for (count3 = 0; (int)count3 < BLOCK_VERIFIERS_TOTAL; count3++)
    { 
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count3],&DATA[count],blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data_length);
      count += blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data_length + sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1;
      // convert the hexadecimal string to a string
      for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data_length; count2++, number += 2)
      {
        memset(data2,0,strnlen(data2,BUFFER_SIZE));
        memcpy(data2,&blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count3][number],2);
        blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key[count3][count2] = (unsigned char)strtol(data2, NULL, 16);
      }    
    }

    // block_verifiers_random_data
    blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_length = RANDOM_STRING_LENGTH*2;
    for (count3 = 0; (int)count3 < BLOCK_VERIFIERS_TOTAL; count3++)
    { 
      memset(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count3],0,strlen(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count3]));
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count3],&DATA[count],blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_length);
      count += blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_length + sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1;
      // convert the hexadecimal string to a string
      for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_length; count2++, number += 2)
      {
        memset(data2,0,strnlen(data2,BUFFER_SIZE));
        memcpy(data2,&blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count3][number],2);
        blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_text[count3][count2] = (char)strtol(data2, NULL, 16);
      } 
    }

    // next_block_verifiers_public_address_data
    blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data_length = VRF_PUBLIC_KEY_LENGTH*2;
    for (count3 = 0; (int)count3 < BLOCK_VERIFIERS_TOTAL; count3++)
    { 
      memset(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count3],0,strlen(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count3]));
      memcpy(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count3],&DATA[count],blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data_length);
      count += blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data_length + sizeof(BLOCKCHAIN_DATA_SEGMENT_PUBLIC_ADDRESS_STRING_DATA)-1;
      // convert the hexadecimal string to a string
      for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data_length; count2++, number += 2)
      {
        memset(data2,0,strnlen(data2,BUFFER_SIZE));
        memcpy(data2,&blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count3][number],2);
        blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count3][count2] = (char)strtol(data2, NULL, 16);
      }
    }
 
    // previous block hash
    blockchain_data.blockchain_reserve_bytes.previous_block_hash_data_length = BLOCK_HASH_LENGTH;
    memcpy(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,blockchain_data.previous_block_hash_data,blockchain_data.blockchain_reserve_bytes.previous_block_hash_data_length);
    count += blockchain_data.blockchain_reserve_bytes.previous_block_hash_data_length + sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1;

    // current block signature data
    blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data_length = (VRF_PROOF_LENGTH + VRF_BETA_LENGTH)*2;
    for (count3 = 0; (int)count3 < BLOCK_VERIFIERS_TOTAL; count3++)
    { 
      memset(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count3],0,strlen(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count3]));
      memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count3],&DATA[count],blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data_length);
      count += blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data_length + sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1;
      // convert the hexadecimal string to a string
      for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data_length; count2++, number += 2)
      {
        memset(data2,0,strnlen(data2,BUFFER_SIZE));
        memcpy(data2,&blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count3][number],2);
        blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count3][count2] = (char)strtol(data2, NULL, 16);
      }
    }

    // set the count to the end of the reserve bytes
    message_copy1 = strstr(DATA,BLOCKCHAIN_RESERVED_BYTES_END);
    count = strnlen(DATA,BUFFER_SIZE) - strnlen(message_copy1,BUFFER_SIZE) + sizeof(BLOCKCHAIN_RESERVED_BYTES_END)-1;
  }
  else
  {
    message_copy1 = strstr(DATA,BLOCKCHAIN_RESERVED_BYTES_DATA_HASH);
    count = strnlen(DATA,BUFFER_SIZE) - strnlen(message_copy1,BUFFER_SIZE) + (sizeof(BLOCKCHAIN_RESERVED_BYTES_DATA_HASH)-1);
  }

  // ringct_version
  blockchain_data.ringct_version_data_length = sizeof(RINGCT_VERSION)-1;
  count+= blockchain_data.ringct_version_data_length;
  memcpy(blockchain_data.ringct_version_data,&DATA[count-blockchain_data.ringct_version_data_length],blockchain_data.ringct_version_data_length);

  // transaction_amount
  // get how many bytes are left in the network_block_string
  blockchain_data.transaction_amount_data_length = (strnlen(DATA,BUFFER_SIZE) - count) % TRANSACTION_LENGTH;
  count+= blockchain_data.transaction_amount_data_length;
  memcpy(blockchain_data.transaction_amount_data,&DATA[count-blockchain_data.transaction_amount_data_length],blockchain_data.transaction_amount_data_length);
  blockchain_data.transaction_amount = varint_decode((size_t)strtol(blockchain_data.transaction_amount_data, NULL, 16));

  // get all of the transactions
  for (number = 0; number < blockchain_data.transaction_amount; number++)
  {
    count+= TRANSACTION_LENGTH;
    memcpy(blockchain_data.transactions[number],&DATA[count-TRANSACTION_LENGTH],TRANSACTION_LENGTH);
  }
  return 1;
  
  #undef NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: blockchain_data_to_network_block_string
Description: Converts a blockchain_data struct to a network_block_string
Parameters:
  result - The network block data string
  BLOCK_VERIFIERS_TOTAL - The maximum amount of block verifiers
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int blockchain_data_to_network_block_string(char *result, const int BLOCK_VERIFIERS_TOTAL)
{
  // Variables
  size_t count = 0;
  size_t count2 = 0;
  size_t counter;

  // define macros  
  #define BLOCKCHAIN_RESERVED_BYTES_LENGTH_TEXT "7c424c4f434b434841494e5f52455345525645445f42595445535f4c454e4754487c"
  #define BLOCKCHAIN_EXTRA_BYTES_LENGTH_TEXT "7c424c4f434b434841494e5f45585452415f42595445535f4c454e4754487c"
  #define BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"blockchain_data_to_network_block_string",39); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0; 

  memset(result,0,strlen(result));
  
  // network_version
  blockchain_data.network_version_data_length = strnlen(blockchain_data.network_version_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.network_version_data,blockchain_data.network_version_data_length);
  
  // timestamp
  if (varint_encode((long long int)blockchain_data.timestamp,blockchain_data.timestamp_data,BUFFER_SIZE_NETWORK_BLOCK_DATA) == 0)
  {
    BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_ERROR("Could not create the varint for the timestamp");
  }
  blockchain_data.timestamp_data_length = strnlen(blockchain_data.timestamp_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.timestamp_data,blockchain_data.timestamp_data_length); 

  // previous_block_hash
  blockchain_data.previous_block_hash_data_length = strnlen(blockchain_data.previous_block_hash_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.previous_block_hash_data,blockchain_data.previous_block_hash_data_length);

  // nonce
  blockchain_data.nonce_data_length = strnlen(blockchain_data.nonce_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.nonce_data,blockchain_data.nonce_data_length);

  // block_reward_transaction_version
  blockchain_data.block_reward_transaction_version_data_length = strnlen(blockchain_data.block_reward_transaction_version_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.block_reward_transaction_version_data,blockchain_data.block_reward_transaction_version_data_length);

  // unlock_block
  if (varint_encode((long long int)blockchain_data.unlock_block,blockchain_data.unlock_block_data,BUFFER_SIZE_NETWORK_BLOCK_DATA) == 0)
  {
    BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_ERROR("Could not create the varint for the unlock block");
  }
  blockchain_data.unlock_block_data_length = strnlen(blockchain_data.unlock_block_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.unlock_block_data,blockchain_data.unlock_block_data_length); 

  // block_reward_input
  blockchain_data.block_reward_input_data_length = strnlen(blockchain_data.block_reward_input_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.block_reward_input_data,blockchain_data.block_reward_input_data_length);

  // vin_type
  blockchain_data.vin_type_data_length = strnlen(blockchain_data.vin_type_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.vin_type_data,blockchain_data.vin_type_data_length);

  // block_height
  if (varint_encode((long long int)blockchain_data.block_height,blockchain_data.block_height_data,BUFFER_SIZE_NETWORK_BLOCK_DATA) == 0)
  {
    BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_ERROR("Could not create the varint for the block height");
  }
  blockchain_data.block_height_data_length = strnlen(blockchain_data.block_height_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.block_height_data,blockchain_data.block_height_data_length); 

  // block_reward_output
  blockchain_data.block_reward_output_data_length = strnlen(blockchain_data.block_reward_output_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.block_reward_output_data,blockchain_data.block_reward_output_data_length);

  // block_reward
  if (varint_encode((long long int)blockchain_data.block_reward,blockchain_data.block_reward_data,BUFFER_SIZE_NETWORK_BLOCK_DATA) == 0)
  {
    BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_ERROR("Could not create the varint for the block reward");
  }
  blockchain_data.block_reward_data_length = strnlen(blockchain_data.block_reward_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.block_reward_data,blockchain_data.block_reward_data_length); 

  // stealth_address_output_tag
  blockchain_data.stealth_address_output_tag_data_length = strnlen(blockchain_data.stealth_address_output_tag_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.stealth_address_output_tag_data,blockchain_data.stealth_address_output_tag_data_length);

  // stealth_address_output
  blockchain_data.stealth_address_output_data_length = strnlen(blockchain_data.stealth_address_output_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.stealth_address_output_data,blockchain_data.stealth_address_output_data_length);

  // extra_bytes_size
  memset(blockchain_data.extra_bytes_size_data,0,strlen(blockchain_data.extra_bytes_size_data));
  memcpy(blockchain_data.extra_bytes_size_data,"a3",2);
  blockchain_data.extra_bytes_size = 163;
  blockchain_data.extra_bytes_size_data_length = 2;
  memcpy(result+strlen(result),blockchain_data.extra_bytes_size_data,blockchain_data.extra_bytes_size_data_length);

  // transaction_public_key_tag
  blockchain_data.transaction_public_key_tag_data_length = strnlen(blockchain_data.transaction_public_key_tag_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.transaction_public_key_tag_data,blockchain_data.transaction_public_key_tag_data_length);

  // transaction_public_key
  blockchain_data.transaction_public_key_data_length = strnlen(blockchain_data.transaction_public_key_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.transaction_public_key_data,blockchain_data.transaction_public_key_data_length);

  // extra_nonce_tag
  blockchain_data.extra_nonce_tag_data_length = strnlen(blockchain_data.extra_nonce_tag_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.extra_nonce_tag_data,blockchain_data.extra_nonce_tag_data_length);

  // reserve_bytes_size
  memset(blockchain_data.reserve_bytes_size_data,0,strlen(blockchain_data.reserve_bytes_size_data));
  memcpy(blockchain_data.reserve_bytes_size_data,"80",2);
  blockchain_data.reserve_bytes_size = DATA_HASH_LENGTH;
  blockchain_data.reserve_bytes_size_data_length = 2;
  memcpy(result+strlen(result),blockchain_data.reserve_bytes_size_data,blockchain_data.reserve_bytes_size_data_length);

  // blockchain_reserve_bytes
  memcpy(result+strlen(result),BLOCKCHAIN_RESERVED_BYTES_START,sizeof(BLOCKCHAIN_RESERVED_BYTES_START)-1);

  // block_producer_delegates_name  
  blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  for (count2 = 0, count = 0; count2 < blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length; count2++, count += 2)
  {
    snprintf(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data+count,BUFFER_SIZE_NETWORK_BLOCK_DATA,"%02x",blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name[count2] & 0xFF);
  }
  blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length *= 2;
  memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data,blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length);  
  memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);

  // block_producer_public_address  
  blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  for (count2 = 0, count = 0; count2 < blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length; count2++, count += 2)
  {
    snprintf(blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data+count,BUFFER_SIZE_NETWORK_BLOCK_DATA,"%02x",blockchain_data.blockchain_reserve_bytes.block_producer_public_address[count2] & 0xFF);
  }
  blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length *= 2;
  memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data,blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length);  
  memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);

  // block_producer_node_backup_count
  blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  if (strncmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"0",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,"30",2);
  }
  if (strncmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"1",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,"31",2);
  }
  if (strncmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"2",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,"32",2);
  }
  if (strncmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"3",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,"33",2);
  }
  if (strncmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"4",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,"34",2);
  }
  if (strncmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"5",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,"35",2);
  }
  blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length = 2;
  memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length);  
  memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);

  // block_producer_backup_nodes_names
  blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  for (count2 = 0, count = 0; count2 < blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length; count2++, count += 2)
  {
    snprintf(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data+count,BUFFER_SIZE_NETWORK_BLOCK_DATA,"%02x",blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names[count2] & 0xFF);
  }
  blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length *= 2;
  memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data,blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length);  
  memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);

  // vrf_secret_key
  blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data,VRF_SECRET_KEY_LENGTH);
  memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data,blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data_length);
  memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);

  // vrf_public_key
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data,VRF_PUBLIC_KEY_LENGTH);
  memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.vrf_public_key_data,blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length);
  memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);

  // vrf_alpha_string
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data,BUFFER_SIZE);
  memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data,blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length);
  memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);

  // vrf_proof
  blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_data,VRF_PROOF_LENGTH);
  memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.vrf_proof_data,blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length);
  memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);

  // vrf_beta_string
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data,VRF_BETA_LENGTH);
  memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data,blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length);
  memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);

  // vrf_data_round
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data_round,VRF_DATA,sizeof(VRF_DATA)-1);
  memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.vrf_data_round,sizeof(VRF_DATA)-1);
  memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);

  // vrf_data
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data,VRF_DATA,sizeof(VRF_DATA)-1);
  memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.vrf_data,sizeof(VRF_DATA)-1);
  memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);

  // block_verifiers_vrf_secret_key_data  
  for (count2 = 0; (int)count2 < BLOCK_VERIFIERS_TOTAL; count2++)
  {
    blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count2],VRF_SECRET_KEY_LENGTH);
    memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count2],blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data_length);
    memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);
  }

  // block_verifiers_vrf_public_key_data  
  for (count2 = 0; (int)count2 < BLOCK_VERIFIERS_TOTAL; count2++)
  {
    blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count2],VRF_PUBLIC_KEY_LENGTH);
    memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count2],blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data_length);
    memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);
  }

  // block_verifiers_random_data  
  for (count2 = 0; (int)count2 < BLOCK_VERIFIERS_TOTAL; count2++)
  {
    blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count2],RANDOM_STRING_LENGTH*2);
    memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count2],blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_length);
    (int)count2+1 == BLOCK_VERIFIERS_TOTAL ? memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_PUBLIC_ADDRESS_STRING_DATA,sizeof(BLOCKCHAIN_DATA_SEGMENT_PUBLIC_ADDRESS_STRING_DATA)-1) : memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);  
  }

  // next_block_verifiers_public_address_data  
  blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data_length = VRF_PUBLIC_KEY_LENGTH*2;
  for (count = 0; (int)count < BLOCK_VERIFIERS_TOTAL; count++)
  {
    // convert the string to hexadecimal
    for (count2 = 0, counter = 0; count2 < VRF_PUBLIC_KEY_LENGTH; count2++, counter += 2)
    {
      snprintf(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count]+counter,BUFFER_SIZE_NETWORK_BLOCK_DATA,"%02x",blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count][count2] & 0xFF);
    }
    memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count],blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data_length);  
    (int)count+1 != BLOCK_VERIFIERS_TOTAL ? memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_PUBLIC_ADDRESS_STRING_DATA,sizeof(BLOCKCHAIN_DATA_SEGMENT_PUBLIC_ADDRESS_STRING_DATA)-1) : memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);   
  }

  // previous block hash
  blockchain_data.blockchain_reserve_bytes.previous_block_hash_data_length = BLOCK_HASH_LENGTH;
  memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,blockchain_data.blockchain_reserve_bytes.previous_block_hash_data_length);
  memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);

  // block_validation_node_signature_data  
  blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data_length = (VRF_PROOF_LENGTH+VRF_BETA_LENGTH)*2;
  for (count = 0; (int)count < BLOCK_VERIFIERS_TOTAL; count++)
  {
    // convert the string to hexadecimal
    for (count2 = 0, counter = 0; count2 < VRF_PROOF_LENGTH+VRF_BETA_LENGTH; count2++, counter += 2)
    {
      snprintf(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count]+counter,BUFFER_SIZE_NETWORK_BLOCK_DATA,"%02x",blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count][count2] & 0xFF);
    }
    memcpy(result+strlen(result),blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data_length);  
    if ((int)count+1 != BLOCK_VERIFIERS_TOTAL)
    {
      memcpy(result+strlen(result),BLOCKCHAIN_DATA_SEGMENT_STRING,sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1);
    }
  } 
  memcpy(result+strlen(result),BLOCKCHAIN_RESERVED_BYTES_END,sizeof(BLOCKCHAIN_RESERVED_BYTES_END)-1);

  // ringct_version
  blockchain_data.ringct_version_data_length = strnlen(blockchain_data.ringct_version_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.ringct_version_data,blockchain_data.ringct_version_data_length);

  // transaction_amount
  if (varint_encode((long long int)blockchain_data.transaction_amount,blockchain_data.transaction_amount_data,BUFFER_SIZE_NETWORK_BLOCK_DATA) == 0)
  {
    BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_ERROR("Could not create the varint for the transaction amount");
  }
  blockchain_data.transaction_amount_data_length = strnlen(blockchain_data.transaction_amount_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+strlen(result),blockchain_data.transaction_amount_data,blockchain_data.transaction_amount_data_length); 

  // get all of the transactions
  for (count2 = 0; count2 < blockchain_data.transaction_amount; count2++)
  {
    memcpy(result+strlen(result),blockchain_data.transactions[count2],TRANSACTION_LENGTH);
  }

  return 1;

  #undef BLOCKCHAIN_RESERVED_BYTES_LENGTH_TEXT
  #undef BLOCKCHAIN_EXTRA_BYTES_LENGTH_TEXT
  #undef BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: add_data_hash_to_network_block_string
Description: Adds the data hash to the network block string
Parameters:
network_block_string - The network_block_string
network_block_string_data_hash - The network block string with the data hash
Return: 0 if an error has occured or it is not verified, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int add_data_hash_to_network_block_string(const char* NETWORK_BLOCK_STRING, char *network_block_string_data_hash)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[DATA_HASH_LENGTH+SMALL_BUFFER_SIZE];
  int count;
  int count2;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // get the data hash of the network block string
  crypto_hash_sha512((unsigned char*)data,(const unsigned char*)NETWORK_BLOCK_STRING,(unsigned long long)strnlen(NETWORK_BLOCK_STRING,BUFFER_SIZE));

  // convert the SHA512 data hash to a string
  for (count2 = 0, count = 0; count2 < DATA_HASH_LENGTH / 2; count2++, count += 2)
  {
    snprintf(data2+count,MAXIMUM_NUMBER_SIZE,"%02x",data[count2] & 0xFF);
  }

  // copy the reserve bytes data hash
  memset(VRF_data.reserve_bytes_data_hash,0,strlen(VRF_data.reserve_bytes_data_hash));
  memcpy(VRF_data.reserve_bytes_data_hash,data2,DATA_HASH_LENGTH);

  // add the data hash to the network block string
  memset(data,0,strnlen(data,BUFFER_SIZE));
  memcpy(data,&NETWORK_BLOCK_STRING[(strnlen(NETWORK_BLOCK_STRING,BUFFER_SIZE)) - (strnlen(strstr(NETWORK_BLOCK_STRING,BLOCKCHAIN_RESERVED_BYTES_START),BUFFER_SIZE) - (sizeof(BLOCKCHAIN_RESERVED_BYTES_START)-1))],((strnlen(NETWORK_BLOCK_STRING,BUFFER_SIZE)) - (strnlen(strstr(NETWORK_BLOCK_STRING,BLOCKCHAIN_RESERVED_BYTES_END),BUFFER_SIZE) - (sizeof(BLOCKCHAIN_RESERVED_BYTES_END)-1))) - ((strnlen(NETWORK_BLOCK_STRING,BUFFER_SIZE)) - (strnlen(strstr(NETWORK_BLOCK_STRING,BLOCKCHAIN_RESERVED_BYTES_START),BUFFER_SIZE) - (sizeof(BLOCKCHAIN_RESERVED_BYTES_START)-1))) - (sizeof(BLOCKCHAIN_RESERVED_BYTES_END)-1));

  memset(network_block_string_data_hash,0,strnlen(network_block_string_data_hash,BUFFER_SIZE));
  memcpy(network_block_string_data_hash,NETWORK_BLOCK_STRING,strnlen(NETWORK_BLOCK_STRING,BUFFER_SIZE));
  
  // replace the reserve bytes with the network block string data hash
  string_replace(network_block_string_data_hash,BUFFER_SIZE,data,data2);
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: verify_network_block_data
Description: Verifies a blockchain_data struct
Parameters:
  BLOCK_VALIDATION_SIGNATURES_SETTINGS - 0 to not verify the block validation signatures, 1 to verify the block validation signatures. The consensus node when submitting the block should be the only time when the block validation signatures are verified
  PREVIOUS_BLOCK_HASH_SETTINGS - 0 to not verify the previous block hash, 1 to verify the previous block hash. The blockchain_test should be the only time when the previous block hash is not verified
  BLOCK_HEIGHT - The block height of the network block string, 0 to get the current block height
  BLOCK_VERIFIERS_TOTAL - The maximum amount of block verifiers
Return: 0 if an error has occured or it is not verified, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int verify_network_block_data(const int BLOCK_VALIDATION_SIGNATURES_SETTINGS, const int PREVIOUS_BLOCK_HASH_SETTINGS, const char* BLOCK_HEIGHT, const int BLOCK_VERIFIERS_TOTAL)
{
  // Variables
  char block_height[MAXIMUM_NUMBER_SIZE];
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char network_block_string[BUFFER_SIZE];
  char current_block_verifiers_public_address[(VRF_PUBLIC_KEY_LENGTH*2)+1];
  int vrf_data_verify_count = 0;
  size_t count;
  size_t count2;
  size_t number;

  // define macros
  #define VERIFY_NETWORK_BLOCK_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"verify_network_block_data",25); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0; 

  memset(block_height,0,sizeof(block_height));
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(network_block_string,0,sizeof(network_block_string));
  memset(current_block_verifiers_public_address,0,sizeof(current_block_verifiers_public_address));

  // network_version
  if (blockchain_data.network_version_data_length != sizeof(NETWORK_VERSION)-1 || strncmp(blockchain_data.network_version_data,NETWORK_VERSION,sizeof(NETWORK_VERSION)-1) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_version");
  } 

  // timestamp
  if (blockchain_data.timestamp_data_length != TIMESTAMP_LENGTH)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid timestamp");
  }

  // previous_block_hash
  if (PREVIOUS_BLOCK_HASH_SETTINGS == 1 && (blockchain_data.previous_block_hash_data_length != BLOCK_HASH_LENGTH || strncmp(blockchain_data.previous_block_hash_data,previous_block_hash,BLOCK_HASH_LENGTH) != 0))
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid previous block hash");
  } 
  
  // nonce
  if (blockchain_data.nonce_data_length != sizeof(BLOCK_PRODUCER_NETWORK_BLOCK_NONCE)-1 || (strncmp(blockchain_data.nonce_data,BLOCK_PRODUCER_NETWORK_BLOCK_NONCE,sizeof(BLOCK_PRODUCER_NETWORK_BLOCK_NONCE)-1) != 0 && strncmp(blockchain_data.nonce_data,CONSENSUS_NODE_NETWORK_BLOCK_NONCE,sizeof(BLOCK_PRODUCER_NETWORK_BLOCK_NONCE)-1) != 0))
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network block nonce");
  }

  // block_reward_transaction_version
  if (blockchain_data.block_reward_transaction_version_data_length != sizeof(BLOCK_REWARD_TRANSACTION_VERSION)-1 || strncmp(blockchain_data.block_reward_transaction_version_data,BLOCK_REWARD_TRANSACTION_VERSION,sizeof(BLOCK_REWARD_TRANSACTION_VERSION)-1) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid block_reward_transaction_version");
  }

  // unlock_block
  if (strncmp(BLOCK_HEIGHT,"0",1) == 0)
  {
    if (get_current_block_height(block_height) == 0)
    {
      VERIFY_NETWORK_BLOCK_DATA_ERROR("Could not get the current block height");
    }
    sscanf(block_height, "%zu", &number);
  }
  else
  {
    sscanf(BLOCK_HEIGHT, "%zu", &number);    
  }
  if ((blockchain_data.unlock_block <= (VARINT_DECODED_VALUE_END_2_BYTE-UNLOCK_BLOCK_AMOUNT) && blockchain_data.unlock_block_data_length != 6) || (blockchain_data.unlock_block > (VARINT_DECODED_VALUE_END_2_BYTE-UNLOCK_BLOCK_AMOUNT) && blockchain_data.unlock_block_data_length != 8) || blockchain_data.unlock_block != (number + UNLOCK_BLOCK_AMOUNT))
  { 
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid unlock_block");
  }

  // block_reward_input
  if (blockchain_data.block_reward_input_data_length != sizeof(BLOCK_REWARD_INPUT)-1 || strncmp(blockchain_data.block_reward_input_data,BLOCK_REWARD_INPUT,sizeof(BLOCK_REWARD_INPUT)-1) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid block_reward_input");
  }

  // vin_type
  if (blockchain_data.vin_type_data_length != sizeof(VIN_TYPE)-1 || strncmp(blockchain_data.vin_type_data,VIN_TYPE,sizeof(VIN_TYPE)-1) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid vin_type");
  }

  // block_height
  if (strncmp(BLOCK_HEIGHT,"0",1) == 0)
  {
    if ((blockchain_data.block_height <= VARINT_DECODED_VALUE_END_2_BYTE && blockchain_data.block_height_data_length != 6) || (blockchain_data.block_height > VARINT_DECODED_VALUE_END_2_BYTE && blockchain_data.block_height_data_length != 8) || blockchain_data.block_height != number)
    {
      VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid block_height");
    }
  }
  else
  {
    if ((blockchain_data.block_height <= VARINT_DECODED_VALUE_END_2_BYTE && blockchain_data.block_height_data_length != 6) || (blockchain_data.block_height > VARINT_DECODED_VALUE_END_2_BYTE && blockchain_data.block_height_data_length != 8))
    {
      VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid block_height");
    }
  }
  

  // block_reward_output
  if (blockchain_data.block_reward_output_data_length != sizeof(BLOCK_REWARD_OUTPUT)-1 || strncmp(blockchain_data.block_reward_output_data,BLOCK_REWARD_OUTPUT,sizeof(BLOCK_REWARD_OUTPUT)-1) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid block_reward_output");
  }

  // block_reward
  if ((blockchain_data.block_reward <= VARINT_DECODED_VALUE_END_4_BYTE && blockchain_data.block_reward_data_length != 10) || (blockchain_data.block_reward > VARINT_DECODED_VALUE_END_4_BYTE && blockchain_data.block_reward <= VARINT_DECODED_VALUE_START_6_BYTE && blockchain_data.block_reward_data_length != 12) || (blockchain_data.block_reward > VARINT_DECODED_VALUE_START_6_BYTE && blockchain_data.block_reward <= VARINT_DECODED_VALUE_START_7_BYTE && blockchain_data.block_reward_data_length != 14) || (blockchain_data.block_reward > VARINT_DECODED_VALUE_START_7_BYTE && blockchain_data.block_reward <= VARINT_DECODED_VALUE_END_7_BYTE+1 && blockchain_data.block_reward_data_length != 16) || (blockchain_data.block_reward > VARINT_DECODED_VALUE_END_7_BYTE+1 && blockchain_data.block_reward_data_length != 18))
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid block_reward");
  }

  // stealth_address_output_tag
  if (blockchain_data.stealth_address_output_tag_data_length != sizeof(STEALTH_ADDRESS_OUTPUT_TAG)-1 || strncmp(blockchain_data.stealth_address_output_tag_data,STEALTH_ADDRESS_OUTPUT_TAG,sizeof(STEALTH_ADDRESS_OUTPUT_TAG)-1) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid stealth_address_output_tag");
  }

  // stealth_address_output
  if (blockchain_data.stealth_address_output_data_length != STEALTH_ADDRESS_OUTPUT_LENGTH)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid stealth_address_output");
  }

  // extra_bytes_size
  if (blockchain_data.extra_bytes_size_data_length != 2 || blockchain_data.extra_bytes_size != 163 || (((blockchain_data.transaction_public_key_tag_data_length + blockchain_data.transaction_public_key_data_length - 2 + blockchain_data.extra_nonce_tag_data_length + blockchain_data.reserve_bytes_size_data_length) / 2) + blockchain_data.reserve_bytes_size) != blockchain_data.extra_bytes_size)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid extra_bytes_size");
  }

  // transaction_public_key_tag
  if (blockchain_data.transaction_public_key_tag_data_length != sizeof(TRANSACTION_PUBLIC_KEY_TAG)-1 || strncmp(blockchain_data.transaction_public_key_tag_data,TRANSACTION_PUBLIC_KEY_TAG,sizeof(TRANSACTION_PUBLIC_KEY_TAG)-1) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid transaction_public_key_tag");
  }

  // transaction_public_key
  if (blockchain_data.transaction_public_key_data_length != TRANSACTION_PUBLIC_KEY_LENGTH)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid transaction_public_key");
  }

  // extra_nonce_tag
  if (blockchain_data.extra_nonce_tag_data_length != sizeof(EXTRA_NONCE_TAG)-1 || strncmp(blockchain_data.extra_nonce_tag_data,EXTRA_NONCE_TAG,sizeof(EXTRA_NONCE_TAG)-1) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid extra_nonce_tag");
  }

  // reserve_bytes_size
  if (blockchain_data.reserve_bytes_size_data_length != 2 || blockchain_data.reserve_bytes_size != DATA_HASH_LENGTH)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid reserve_bytes_size");
  }

  // blockchain_reserve_bytes
  // block_producer_delegates_name
  if (blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length < MINIMUM_BUFFER_SIZE_DELEGATES_NAME*2 || blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length > MAXIMUM_BUFFER_SIZE_DELEGATES_NAME*2)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid block_producer_delegates_name");
  }

  // block_producer_public_address
  if (blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length != XCASH_WALLET_LENGTH*2 || strncmp(blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data,"584341",6) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid block_producer_public_address");
  }

  // block_producer_node_backup_count
  if (blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length != 2 || (strncmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"0",1) != 0 && strncmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"1",1) != 0 && strncmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"2",1) != 0 && strncmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"3",1) != 0 && strncmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"4",1) != 0 && strncmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"5",1) != 0))
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid block_producer_node_backup_count");
  }

  // block_producer_backup_nodes_names
  if (blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length < MINIMUM_BUFFER_SIZE_DELEGATES_BACKUP_NAMES*2 || blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length > MAXIMUM_BUFFER_SIZE_DELEGATES_BACKUP_NAMES*2 || string_count(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data,"2c") != 4)
  {  
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid block_producer_backup_nodes_names");
  }

  // vrf_secret_key  
  if (blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data_length != VRF_SECRET_KEY_LENGTH)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid vrf_secret_key");
  }

  // vrf_public_key  
  if (blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length != VRF_PUBLIC_KEY_LENGTH || crypto_vrf_is_valid_key((const unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_public_key) != 1)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid vrf_public_key");
  }

  // vrf_alpha_string
  // convert the previous block hash to hexadecimal
  memset(data2,0,strnlen(data2,BUFFER_SIZE));
  for (count = 0, number = 0; count < BLOCK_HASH_LENGTH; count++, number += 2)
  {
    snprintf(data2+number,MAXIMUM_NUMBER_SIZE,"%02x",blockchain_data.previous_block_hash_data[count] & 0xFF);
  }
  if ((int)blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length != ((RANDOM_STRING_LENGTH*2)*BLOCK_VERIFIERS_TOTAL) + (BLOCK_HASH_LENGTH*2) || strncmp(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data,data2,BLOCK_HASH_LENGTH*2) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid vrf_alpha_string");
  }

  // vrf_proof
  if (blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length != VRF_PROOF_LENGTH)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid vrf_proof");
  }

  // vrf_beta_string
  if (blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length != VRF_BETA_LENGTH)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid vrf_beta_string");
  }

  // vrf_data_round
  if (crypto_vrf_verify(blockchain_data.blockchain_reserve_bytes.vrf_beta_string,(const unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_public_key,(const unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_proof,(const unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data,(unsigned long long)strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data)) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid vrf_data_round");
  }
  memset(blockchain_data.blockchain_reserve_bytes.vrf_data_round,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data_round,11));
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data_round,VRF_DATA,sizeof(VRF_DATA)-1);

  // vrf_data
  memset(blockchain_data.blockchain_reserve_bytes.vrf_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data,11));
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data,VRF_DATA,sizeof(VRF_DATA)-1);

  // verify that all of the data to create the VRF data is correct
  memset(data,0,strlen(data));
  memcpy(data,blockchain_data.previous_block_hash_data,blockchain_data.previous_block_hash_data_length);
  for (count = 0; (int)count < BLOCK_VERIFIERS_TOTAL; count++)
  {
    if (strlen((char*)blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key[count]) == crypto_vrf_SECRETKEYBYTES && strlen((char*)blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key[count]) == crypto_vrf_PUBLICKEYBYTES && strlen(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_text[count]) == RANDOM_STRING_LENGTH)
    {
      memcpy(data+strlen(data),blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_text[count],RANDOM_STRING_LENGTH);
    }
    else
    {
      memcpy(data+strlen(data),GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1);
    }
  } 

  memset(data2,0,strlen(data2));
  // convert the vrf alpha string to a string
  for (count2 = 0, count = 0; count2 < strlen(data); count2++, count += 2)
  {
    snprintf(data2+count,BUFFER_SIZE,"%02x",data[count2] & 0xFF);
  }

  memset(data,0,strlen(data));
  crypto_hash_sha512((unsigned char*)data,(const unsigned char*)data2,strlen(data2));
  memset(data2,0,strlen(data2));

  // convert the SHA512 data hash to a string
  for (count2 = 0, count = 0; count2 < DATA_HASH_LENGTH / 2; count2++, count += 2)
  {
    snprintf(data2+count,BUFFER_SIZE,"%02x",data[count2] & 0xFF);
  }
  
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],network_data_nodes_list.network_data_nodes_public_address[0],XCASH_WALLET_LENGTH) == 0)
    {
      //counter = (int)count;
      break;
    }
  }
  
  /* Disable this part for now
  check what block verifiers vrf secret key and vrf public key to use
  if (main_network_data_node_create_block == 0)
  {
    if (strncmp(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[counter],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING_DATA)-1) != 0)
    {
      for (count = 0; count < DATA_HASH_LENGTH; count += 2)
      {
        memset(data,0,strlen(data));
        memcpy(data,&data2[count],2);
        counter = (int)strtol(data, NULL, 16); */
   
        /* if it is not in the range of 01 - FA or it has already been calculated then skip the byte
           This number needs to be evenly divisible by how many maximum block verifiers there will be
           This is so block verifiers in specific spots do not have more of a chance to be the block producer than others
           The goal is to use as many bytes as possible, since the more unused bytes, the more chance that it will run out of bytes when selecting the block producer
        */
        /*if (counter >= MINIMUM_BYTE_RANGE && counter <= MAXIMUM_BYTE_RANGE)
        {
          counter = counter % BLOCK_VERIFIERS_TOTAL; 

          // check if the block verifier created the data
          if (strncmp(VRF_data.block_verifiers_vrf_secret_key_data[counter],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA)-1) != 0 && strncmp(VRF_data.block_verifiers_vrf_public_key_data[counter],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA)-1) != 0 && strncmp(VRF_data.block_verifiers_random_data[counter],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1) != 0)
          {
            break;
          }
        }
      }
    }
    else
    {
      counter = 0;
    }

    // check if the selected vrf secret key and vrf public key are the same as the vrf_secret_key and vrf_public_key
    if (strncmp((char*)blockchain_data.blockchain_reserve_bytes.vrf_secret_key,(char*)blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key[counter],crypto_vrf_SECRETKEYBYTES) != 0 || strncmp((char*)blockchain_data.blockchain_reserve_bytes.vrf_public_key,(char*)blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key[counter],crypto_vrf_PUBLICKEYBYTES) != 0)
    {
      VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid VRF data");
    }
  }*/

  // previous_block_hash
  if (PREVIOUS_BLOCK_HASH_SETTINGS == 1 && (blockchain_data.blockchain_reserve_bytes.previous_block_hash_data_length != BLOCK_HASH_LENGTH || strncmp(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,previous_block_hash,BLOCK_HASH_LENGTH) != 0))
  { 
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid previous block hash");
  }  

  // block_validation_node_signature
  if (BLOCK_VALIDATION_SIGNATURES_SETTINGS == 1)
  {
    if (blockchain_data.block_height == XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
    {
      // create a network block string
      if (blockchain_data_to_network_block_string(network_block_string,BLOCK_VERIFIERS_TOTAL) == 0)
      {
        VERIFY_NETWORK_BLOCK_DATA_ERROR("Could not convert the blockchain struct to a network block string");
      }  

      // replace the main network data nodes block validation signature with the GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE_DATA
      string_replace(network_block_string,sizeof(network_block_string),blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[0],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE_DATA);
 
      for (count2 = 0; (int)count2 < BLOCK_VERIFIERS_TOTAL; count2++)
      {
        if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count2],network_data_nodes_list.network_data_nodes_public_address[0],XCASH_WALLET_LENGTH) == 0)
        {
          vrf_data_verify_count = VRF_data_verify(current_block_verifiers_list.block_verifiers_public_key[count2],blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[0],network_block_string) == 1 ? BLOCK_VERIFIERS_AMOUNT : 0;
        }
      }      
    }
    else
    {
      // create a network block string
      if (blockchain_data_to_network_block_string(network_block_string,BLOCK_VERIFIERS_TOTAL) == 0)
      {
        VERIFY_NETWORK_BLOCK_DATA_ERROR("Could not convert the blockchain struct to a network block string");
      }

      // replace the block validation signatures with the GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE_DATA
      for (count = 0; (int)count < BLOCK_VERIFIERS_TOTAL; count++)
      { 
        string_replace(network_block_string,sizeof(network_block_string),blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE_DATA);
      }

      // check if at least 67 of the next block verifiers in the previous block (or current block verifiers) signed the data in the current block
      for (count = 0, vrf_data_verify_count = 0; (int)count < BLOCK_VERIFIERS_TOTAL; count++)
      { 
        // check the signed data 
        if (VRF_data_verify(current_block_verifiers_list.block_verifiers_public_key[count],VRF_data.block_blob_signature[count],network_block_string) == 1)
        {
          vrf_data_verify_count++;
        }
      }
    }
    if (vrf_data_verify_count < BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      // run the double for loop to check all signatures with all block verifiers public address in case the order is off
      for (count = 0, vrf_data_verify_count = 0; (int)count < BLOCK_VERIFIERS_TOTAL; count++)
      { 
        for (count2 = 0; (int)count2 < BLOCK_VERIFIERS_TOTAL; count2++)
        { 
          // check the signed data 
          if (VRF_data_verify(current_block_verifiers_list.block_verifiers_public_key[count],VRF_data.block_blob_signature[count2],network_block_string) == 1)
          {
            vrf_data_verify_count++;
          }
        }
      }
      if (vrf_data_verify_count >= BLOCK_VERIFIERS_VALID_AMOUNT)
      {
        fprintf(stderr,"\033[1;32m%d / %d block verifiers from the previous block signatures are valid\n\033[0m\n",vrf_data_verify_count,BLOCK_VERIFIERS_VALID_AMOUNT);
      }
      else
      {
        fprintf(stderr,"\033[1;31m%d / %d block verifiers from the previous block signatures are valid\n\033[0m\n",vrf_data_verify_count,BLOCK_VERIFIERS_VALID_AMOUNT);
        VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string, The block was not signed by the required amount of block validation nodes from the previous block");
      }
    }
    else
    {
      fprintf(stderr,"\033[1;32m%d / %d block verifiers from the previous block signatures are valid\n\033[0m\n",vrf_data_verify_count,BLOCK_VERIFIERS_VALID_AMOUNT);
    }    
  }

  // ringct_version
  if (blockchain_data.ringct_version_data_length != sizeof(RINGCT_VERSION)-1 || strncmp(blockchain_data.ringct_version_data,RINGCT_VERSION,sizeof(RINGCT_VERSION)-1) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid ringct_version");
  }

  // transaction_amount
  if ((blockchain_data.transaction_amount <= 255 && blockchain_data.transaction_amount_data_length != 2) || (blockchain_data.transaction_amount > 255 && blockchain_data.transaction_amount <= VARINT_DECODED_VALUE_END_1_BYTE && blockchain_data.transaction_amount_data_length != 4) || (blockchain_data.transaction_amount > VARINT_DECODED_VALUE_END_1_BYTE && blockchain_data.transaction_amount_data_length != 6))
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid transaction_amount");
  }
  return 1;

  #undef VERIFY_NETWORK_BLOCK_DATA_ERROR
}
