#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"
#include "define_macros_test.h"

#include "blockchain_functions.h"
#include "block_verifiers_server_functions.h"
#include "block_verifiers_synchronize_server_functions.h"
#include "block_verifiers_synchronize_functions.h"
#include "block_verifiers_thread_server_functions.h"
#include "insert_database_functions.h"
#include "read_database_functions.h"
#include "file_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "organize_functions.h"
#include "string_functions.h"
#include "thread_functions.h"
#include "convert.h"
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
Name: server_received_data_XCASH_DPOPS_test_data
Description: Runs the code when the server receives the XCASH_DPOPS_test_data message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_received_data_XCASH_DPOPS_test_data(const int CLIENT_SOCKET, char* MESSAGE)
{
  // verify the message
  if (verify_data(MESSAGE,1) == 0)
  { 
    return;
  }
  else
  {
    if (send_data(CLIENT_SOCKET,(unsigned char*)MESSAGE,0,1,"") == 1)
    {
      network_functions_test_settings = 1;
      return;
    }
    else
    {
      return;
    }
  }  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS message
Parameters:
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs(const char* MESSAGE)
{
  // Variables
  char block_verifiers_public_address[XCASH_WALLET_LENGTH+1];
  char public_address[XCASH_WALLET_LENGTH+1];
  char reserve_proof[BUFFER_SIZE_RESERVE_PROOF+1];  
  char data3[MAXIMUM_NUMBER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  size_t count3;
  int count;
  int settings = 1;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_ERROR(settings) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs",84); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  } \
  return;

  // check if the time is valid
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
  if (current_UTC_date_and_time.tm_min % BLOCK_TIME != 3)
  {
    return;
  }

  memset(block_verifiers_public_address,0,sizeof(block_verifiers_public_address));
  memset(public_address,0,sizeof(public_address));
  memset(reserve_proof,0,sizeof(reserve_proof));
  memset(data3,0,sizeof(data3));

  // parse the message
  if (parse_json_data(MESSAGE,"public_address_that_created_the_reserve_proof",block_verifiers_public_address,sizeof(block_verifiers_public_address)) == 0 || strlen(block_verifiers_public_address) != XCASH_WALLET_LENGTH || strncmp(block_verifiers_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"public_address",public_address,sizeof(public_address)) == 0 || strlen(public_address) != XCASH_WALLET_LENGTH || strncmp(public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"reserve_proof",reserve_proof,sizeof(reserve_proof)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_ERROR("Could not parse the message");
  }

  // check if the reserve proof is unique 
  for (count3 = 0, settings = 1; (int)count3 < invalid_reserve_proofs.count; count3++)
  {
    if (strncmp(invalid_reserve_proofs.reserve_proof[count3],reserve_proof,sizeof(reserve_proof)) == 0)
    {
      settings = 0;
    }
  }
  
  if (settings == 1)
  {
    // check if the reserve proof is valid. check a few times to make sure it does not error since were already checking reserve proofs
    for (count = 0; count < 5; count++)
    {
      if (check_reserve_proofs(data3,block_verifiers_public_address,reserve_proof) == 0)
      {
        invalid_reserve_proofs.block_verifier_public_address[invalid_reserve_proofs.count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
        invalid_reserve_proofs.public_address_created_reserve_proof[invalid_reserve_proofs.count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
        invalid_reserve_proofs.public_address_voted_for[invalid_reserve_proofs.count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
        invalid_reserve_proofs.reserve_proof[invalid_reserve_proofs.count] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
        sscanf(data3,"%zu", &count3);     
        memcpy(invalid_reserve_proofs.block_verifier_public_address[invalid_reserve_proofs.count],public_address,XCASH_WALLET_LENGTH);
        memcpy(invalid_reserve_proofs.reserve_proof[invalid_reserve_proofs.count],reserve_proof,strnlen(reserve_proof,BUFFER_SIZE_RESERVE_PROOF));
        invalid_reserve_proofs.count++;
        break;
      }
      nanosleep((const struct timespec[]){{0, 200000000L}}, NULL);
    }  
  }
  return;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_network_data_nodes_block_verifiers_current_time
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME message
Parameters:
  CLIENT_SOCKET - The socket to send data to
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_block_verifiers_to_network_data_nodes_block_verifiers_current_time(const int CLIENT_SOCKET)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME_ERROR(settings) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_network_data_nodes_block_verifiers_current_time",93); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  } \
  send_data(CLIENT_SOCKET,(unsigned char*)"Could not update the delegates information}",0,0,""); \
  return;

  memset(data,0,sizeof(data));
  
  // create the message
  memcpy(data,"{\r\n \"message_settings\": \"NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_BLOCK_VERIFIERS_CURRENT_TIME\",\r\n \"current_time\": \"",112);
  snprintf(data+112,sizeof(data)-113,"%ld",time(NULL));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // sign_data
  if (sign_data(data) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME_ERROR("Could not sign_data");
  }
  send_data(CLIENT_SOCKET,(unsigned char*)data,0,1,"");
  return;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_main_network_data_node_to_block_verifier_start_block
Description: Runs the code when the server receives the MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK message
Parameters:
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_main_network_data_node_to_block_verifier_start_block(const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  size_t count;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK(settings) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_main_network_data_node_to_block_verifier_start_block",79); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  } \
  return;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // parse the message
  if (parse_json_data(MESSAGE,"database_data",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"public_address",data2,sizeof(data2)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK("Could not parse the data");
  }

  // make sure the message is coming from the main network data node
  if (strncmp(data2,network_data_nodes_list.network_data_nodes_public_address[backup_network_data_node_settings],XCASH_WALLET_LENGTH) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK("Invalid message");
  }

  // add the data to the database
  memset(data2,0,sizeof(data2));
  memcpy(data2,data,strlen(data)-2);
  get_reserve_bytes_database(count,0);
  memset(data,0,sizeof(data));
  memcpy(data,"reserve_bytes_",14);
  snprintf(data+strlen(data),MAXIMUM_NUMBER_SIZE,"%zu",count);
  if (insert_document_into_collection_json(database_name,data,data2) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK("Could not add the start block to the database");
  }  
  return;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_main_node_to_node_message_part_4
Description: Runs the code when the server receives the MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK message
Parameters:
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_main_node_to_node_message_part_4(const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[XCASH_WALLET_LENGTH+1];

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR(settings) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_main_node_to_node_message_part_4",59); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  } \
  return;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));
  
  // parse the message
  if (parse_json_data(MESSAGE,"block_blob",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"public_address",data2,sizeof(data2)) == 0 || strlen(data2) != XCASH_WALLET_LENGTH || strncmp(data2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not parse the data");
  }

  // check if the public_address is the correct main node
  if ((test_settings == 1) || (strncmp(current_round_part_backup_node,"0",1) == 0 && strncmp(main_nodes_list.block_producer_public_address,data2,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"1",1) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,data2,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"2",1) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,data2,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"3",1) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,data2,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"4",1) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,data2,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"5",1) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,data2,XCASH_WALLET_LENGTH) == 0))
  { 
    memcpy(VRF_data.block_blob,data,strnlen(data,BUFFER_SIZE));
  }
  else
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Invalid main node");
  }
  return;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_node_to_node_majority
Description: Runs the code when the server receives the NODES_TO_NODES_VOTE_MAJORITY_RESULTS message
Parameters:
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_node_to_node_majority(const char* MESSAGE)
{
  // Variables
  char data[100]; 
  char public_address[XCASH_WALLET_LENGTH+1];
  int count;
  int count2;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_MAJORITY_ERROR(settings) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_node_majority",48); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  } \
  return;

  memset(data,0,sizeof(data));

  // parse the public address
  if (parse_json_data(MESSAGE,"public_address",public_address,sizeof(public_address)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_MAJORITY_ERROR("Could not parse the data");
  }

  // find the block verifier count
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],public_address,XCASH_WALLET_LENGTH) == 0)
    {
      break;
    }
  }

  // parse the message
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memset(data,0,sizeof(data));
    memcpy(data,"vote_data_",10);
    snprintf(data+strlen(data),MAXIMUM_NUMBER_SIZE,"%d",count2+1);
    if (parse_json_data(MESSAGE,data,current_block_verifiers_majority_vote.data[count][count2],sizeof(current_block_verifiers_majority_vote.data[count][count2])) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_MAJORITY_ERROR("Could not parse the data");
    }
  }
  return;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_MAJORITY_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_node_to_node
Description: Runs the code when the server receives the NODES_TO_NODES_VOTE_RESULTS message
Parameters:
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_node_to_node(const char* MESSAGE)
{
  // Variables
  char data[10]; 
  char data2[DATA_HASH_LENGTH+1];
  char public_address[XCASH_WALLET_LENGTH+1];

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_ERROR(settings) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_node",39); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  } \
  return;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // parse the message
  if (parse_json_data(MESSAGE,"public_address",public_address,sizeof(public_address)) == 0 || parse_json_data(MESSAGE,"vote_settings",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"vote_data",data2,sizeof(data2)) == 0 || strlen(data2) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_ERROR("Could not parse the data");
  }

  // process the vote data
  pthread_mutex_lock(&vote_lock);
  if (strncmp(data,"valid",5) == 0 && strncmp(data2,current_round_part_vote_data.current_vote_results,DATA_HASH_LENGTH) == 0)
  {
    current_round_part_vote_data.vote_results_valid++;
  }
  else
  {
    current_round_part_vote_data.vote_results_invalid++;
  }
  pthread_mutex_unlock(&vote_lock);
  return;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA message
Parameters:
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data(const char* MESSAGE)
{
  // Variables  
  char public_address[XCASH_WALLET_LENGTH+1];
  char vrf_secret_key_data[VRF_SECRET_KEY_LENGTH+1];
  char vrf_public_key_data[VRF_PUBLIC_KEY_LENGTH+1];
  char random_data[RANDOM_STRING_LENGTH+1];
  char data[MAXIMUM_NUMBER_SIZE];
  unsigned char vrf_public_key[crypto_vrf_PUBLICKEYBYTES];
  unsigned char vrf_secret_key[crypto_vrf_SECRETKEYBYTES];
  int count;
  int counter;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_ERROR(settings) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data",70); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  } \
  return;

  memset(public_address,0,sizeof(public_address));
  memset(vrf_secret_key_data,0,sizeof(vrf_secret_key_data));
  memset(vrf_public_key_data,0,sizeof(vrf_public_key_data));
  memset(random_data,0,sizeof(random_data));
  memset(data,0,sizeof(data));  
  memset(vrf_public_key,0,sizeof(vrf_public_key));
  memset(vrf_secret_key,0,sizeof(vrf_secret_key));

  // parse the message
  if (parse_json_data(MESSAGE,"public_address",public_address,sizeof(public_address)) == 0 || strlen(public_address) != XCASH_WALLET_LENGTH || strncmp(public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"vrf_secret_key",vrf_secret_key_data,sizeof(vrf_secret_key_data)) == 0 || strlen(vrf_secret_key_data) != VRF_SECRET_KEY_LENGTH || parse_json_data(MESSAGE,"vrf_public_key",vrf_public_key_data,sizeof(vrf_public_key_data)) == 0 || strlen(vrf_public_key_data) != VRF_PUBLIC_KEY_LENGTH || parse_json_data(MESSAGE,"random_data",random_data,sizeof(random_data)) == 0 || strlen(random_data) != RANDOM_STRING_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_ERROR("Could not parse the data");
  }

  // convert the VRF secret key string to a VRF secret key
  for (counter = 0, count = 0; counter < VRF_SECRET_KEY_LENGTH; count++, counter += 2)
  {
    memset(data,0,strlen(data));
    memcpy(data,&vrf_secret_key_data[counter],2);
    vrf_secret_key[count] = (unsigned char)strtol(data, NULL, 16);
  } 

  // convert the VRF public key string to a VRF public key
  for (counter = 0, count = 0; counter < VRF_PUBLIC_KEY_LENGTH; count++, counter += 2)
  {
    memset(data,0,strlen(data));
    memcpy(data,&vrf_public_key_data[counter],2);
    vrf_public_key[count] = (unsigned char)strtol(data, NULL, 16);
  } 

  // process the vote data
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],public_address,XCASH_WALLET_LENGTH) == 0 && strncmp(VRF_data.block_verifiers_vrf_secret_key_data[count],"",1) == 0 && strncmp((char*)VRF_data.block_verifiers_vrf_secret_key[count],"",1) == 0 && strncmp(VRF_data.block_verifiers_vrf_public_key_data[count],"",1) == 0 && strncmp((char*)VRF_data.block_verifiers_vrf_public_key[count],"",1) == 0 && strncmp(VRF_data.block_verifiers_random_data[count],"",1) == 0)
    {
      memcpy(VRF_data.block_verifiers_vrf_secret_key_data[count],vrf_secret_key_data,VRF_SECRET_KEY_LENGTH);
      memcpy(VRF_data.block_verifiers_vrf_secret_key[count],vrf_secret_key,crypto_vrf_SECRETKEYBYTES);
      memcpy(VRF_data.block_verifiers_vrf_public_key_data[count],vrf_public_key_data,VRF_PUBLIC_KEY_LENGTH);
      memcpy(VRF_data.block_verifiers_vrf_public_key[count],vrf_public_key,crypto_vrf_PUBLICKEYBYTES);
      memcpy(VRF_data.block_verifiers_random_data[count],random_data,RANDOM_STRING_LENGTH);
    }
  }
  return;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE message
Parameters:
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature(const char* MESSAGE)
{
  // Variables
  char data[VRF_PROOF_LENGTH+VRF_BETA_LENGTH+1]; 
  char data2[XCASH_WALLET_LENGTH+1];
  int count;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_ERROR(settings) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature",82); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  } \
  return;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // parse the message
  if (parse_json_data(MESSAGE,"block_blob_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_PROOF_LENGTH+VRF_BETA_LENGTH || parse_json_data(MESSAGE,"public_address",data2,sizeof(data2)) == 0 || strlen(data2) != XCASH_WALLET_LENGTH || strncmp(data2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_ERROR("Could not parse the data");
  }

  // process the vote data
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],data2,XCASH_WALLET_LENGTH) == 0 && strncmp(VRF_data.block_blob_signature[count],"",1) == 0)
    {
      memcpy(VRF_data.block_blob_signature[count],data,VRF_PROOF_LENGTH+VRF_BETA_LENGTH);
    }
  }
  return;

  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_ERROR
}
