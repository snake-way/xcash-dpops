#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "define_macros_test.h"

#include "insert_database_functions.h"
#include "delete_database_functions.h"
#include "count_database_functions.h"
#include "read_database_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "string_functions.h"
#include "variables_test.h"
#include "delegate_server_functions.h"
#include "delegate_server_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: delegate_server_functions_test
Description: Test the delegate_server functions
Return: The number of passed delegate_server_functions test
-----------------------------------------------------------------------------------------------------------
*/

int delegate_server_functions_test(void)
{
  // Variables
  char data[BUFFER_SIZE];
  int count;
  struct reserve_proof reserve_proof;

  // define macros
  #define TEST_RESERVE_PROOF_DELEGATES_REGISTER "ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti" // the reserve proof used for testing the delegate regster function
  #define TEST_WALLET_DELEGATES_REGISTER "XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP" // the wallet used for testing the delegate regster function
  #define TEST_SIGNATURE_ADD_RESERVE_PROOF "SigV1N2NWu1Yzg4aVrhJUjZVcRZ52CcgUghX7x5Fn9W5drC4g8B9tZn7q4yJEKbiJMRFeNRPmSPT3rBSVkJnFWAGK5PA3" // the signature used for testing the delegate regster function
  #define TEST_SIGNATURE_ADD_RESERVE_PROOF_INVALID_DATA "SigV1TKQC5vgf5WHDdovhL61tSo1AceDZvYZLziZTdXF4RNbwBoaxhNSYKAjCjFUmds2awbXpo6neuT7TS4eRVF1yD7FV" // the signature used for testing the delegate regster function for Invalid data
  #define TEST_SIGNATURE_ADD_RESERVE_PROOF_INVALID_RESERVE_PROOF "SigV1C2USs6c2BpuD5zMz4zbx7VCgYYJ1QWcjcL6gFsTqA2pKZg7StgCUdx8d4RFXsxhuXaMdqUNrfK52zHrzseWye8Zc" // the signature used for testing the delegate regster function for Invalid reserve proof
  #define TEST_SIGNATURE_DELEGATES_REGISTER "SigV11Bg1s6XgfB4aT8zk5SRHrER6AfnZBwspW6j3GVYGZtETAJD8wCpaQyp2o5rizXvcWzJe3mJs6ZXpRWfMEVfc1dCA" // the signature used for testing the delegate regster function
  #define TEST_SIGNATURE_DELEGATES_REGISTER_INVALID_DATA "SigV13GLt56kfHutU2rTpGkKstj6pikWqgA7XHjAYt7ghXcoebkx9PZmGNvYgfX4XboMebX4NcL5JFPZrPWc22oyGxbtm" // the signature used for testing the delegate regster function for Invalid data
  #define TEST_SIGNATURE_DELEGATES_REGISTER_PUBLIC_ADDRESS_ALREADY_REGISTERED "SigV1g8M1JKBjBDWjM4JLwhXyQjNfNR9D62GH4SdRVLfqNKyDfjyNeuRaVmdTp31FgJyBXaQ76tA8kMwJTfaj9usFPvcT" // the signature used for testing the delegate regster function for The delegates public address is already registered
  #define TEST_SIGNATURE_DELEGATES_REGISTER_IP_ADDRESS_ALREADY_REGISTERED "SigV1GDgpbSWb9E5iaLVX4Uo2KtahPqsQ1Vza5VDSG8dyUKv3EZgGLZQDABoiA6V9sNZkemRGwLpXtrSAYaKNw4iJc72C" // the signature used for testing the delegate regster function for The delegates IP address is already registered
  #define TEST_SIGNATURE_DELEGATES_REGISTER_PUBLIC_KEY_ALREADY_REGISTERED "SigV12BycAnvf3b7iRgJ5wfuSUzZxbREV6qHWbNaDA7yCGwsHJWBEfYwgNhdFx1MbTmb1QwZC7fPx38jAecU2oJTWE6zG" // the signature used for testing the delegate regster function for The delegates public key is already registered
  #define TEST_SIGNATURE_DELEGATES_REGISTER_DELEGATES_NAME_ALREADY_REGISTERED "SigV1PjXNojdeT9FCJoPtUCcUWtMVNWzg5KJwCN8tyFDBQowXAW3zd2Kb5ZM8191H2p5NRcK2do9sEPjUUUstiYg3BYUx" // the signature used for testing the delegate regster function for The delegates name is already registered
  #define TEST_SIGNATURE_DELEGATES_UPDATE "SigV1GgWgHi6kk6hYJHfpzUWvBkYf21PPX1ZEdi8xU4ufFuQvPnmzVAFV5EMYybZQAvfGYQDHtSiMRgRqnHVaryMzxTho" // the signature used for testing the delegate update function
  #define TEST_SIGNATURE_DELEGATES_UPDATE_INVALID_ITEM "SigV1HT4BXSPaftj5o26by2evyN9RLvT1MyMme1ni4E8zuA9b4SVKeuFkcakYcycdEfH5SpKg3CG3p3gg1QgRsCAnQWPw" // the signature used for testing the delegate update function for Invalid item to update
  #define TEST_SIGNATURE_DELEGATES_UPDATE_INVALID_ITEM_VALUE "SigV1gM13gP3p6pmdRcM5mumi8C2A69LN4oM46PhLdQLd8o3bg6Yu3xEoXkX8uv71558fZRYAzRwUr3YbAGRgew24mya4" // the signature used for testing the delegate update function for Invalid item value to update
  #define TEST_SIGNATURE_DELEGATES_REMOVE "SigV1c34L3pqB73kJyWv4qNkz3VGS2SU8kE9dRdDXToAQSGVQDhBj93ZR9YLgn8g1WkDLh8G6EoWTgPbq6ChdYptqZjP9" // the signature used for testing the delegate remove function
 
  #define DELEGATES_SERVER_FUNCTIONS_TEST_ERROR(DATA,FUNCTION_NAME,TEST) \
  memcpy(result_test,DATA,sizeof(DATA)-1); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS); \
  sleep(5); \
  for (count = 0; count < error_message.total; count++) \
  { \
    if (strncmp(error_message.data[count],TEST,BUFFER_SIZE) == 0) \
    { \
      break; \
    } \
  } \
  if (count != error_message.total) \
  { \
    fprintf(stderr,"\033[1;32mPASSED! Test for %s checking for %s\033[0m\n",FUNCTION_NAME,TEST); \
    count_test++; \
  } \
  else \
  { \
    fprintf(stderr,"\033[1;31mFAILED! Test for %s checking for %s\033[0m\n",FUNCTION_NAME,TEST); \
  } \
  delete_database(database_name); \
  memset(result_test,0,sizeof(result_test)); \
  memset(data_test,0,sizeof(data_test)); \
  memset(data,0,sizeof(data));

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));
  count_test = 0;
  error_message.total = 0;

  // initialize the reserve_proof struct
  memset(reserve_proof.block_verifier_public_address,0,sizeof(reserve_proof.block_verifier_public_address));
  memset(reserve_proof.public_address_created_reserve_proof,0,sizeof(reserve_proof.public_address_created_reserve_proof));
  memset(reserve_proof.public_address_voted_for,0,sizeof(reserve_proof.public_address_voted_for));
  memset(reserve_proof.reserve_proof_amount,0,sizeof(reserve_proof.reserve_proof_amount));
  memset(reserve_proof.reserve_proof,0,sizeof(reserve_proof.reserve_proof));
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\ndelegate server functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,DELEGATE_SERVER_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  network_functions_test_error_settings = 2;
  delete_database(database_name);

  // block_verifiers_add_reserve_proof_check_if_data_is_valid
  memcpy(result_test,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF|" TEST_WALLET "|" TEST_RESERVE_PROOF_DELEGATES_REGISTER "|" TEST_WALLET_DELEGATES_REGISTER "|" TEST_SIGNATURE_ADD_RESERVE_PROOF "|",872);
  if (block_verifiers_add_reserve_proof_check_if_data_is_valid(result_test,&reserve_proof) == 1)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for block_verifiers_add_reserve_proof_check_if_data_is_valid\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for block_verifiers_add_reserve_proof_check_if_data_is_valid\033[0m\n");
  }

  // add_reserve_proof_remove_previous_vote
  delete_database(database_name);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" NETWORK_DATA_NODE_1_PUBLIC_ADDRESS "\",\"total_vote_count\":\"240000000\",\"IP_address\":\"" NETWORK_DATA_NODE_1_IP_ADDRESS "\",\"delegate_name\":\"delegate_name_1\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"" TEST_WALLET_PUBLIC_KEY_1 "\"}");
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_RESERVE_PROOFS_DATA_1);
  insert_document_into_collection_json(database_name,"reserve_proofs_5",DATABASE_COLLECTION_RESERVE_PROOFS_DATA_1);
  memcpy(result_test,"{\"public_address_created_reserve_proof\":\"" TEST_WALLET_1 "\"}",141);
  if (add_reserve_proof_remove_previous_vote((const char*)result_test) == 1 && read_document_field_from_collection(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_1 "\"}","total_vote_count",data_test) == 1 && strncmp(data_test,"0",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for add_reserve_proof_remove_previous_vote\033[0m\n");
    count_test++;
  }
  else
  {
    color_print(data_test,"red");
    fprintf(stderr,"\033[1;31mFAILED! Test for add_reserve_proof_remove_previous_vote\033[0m\n");
  }

  // test server_receive_data_socket_node_to_block_verifiers_add_reserve_proof
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET "\",\"total_vote_count\":\"50000000\",\"IP_address\":\"127.0.0.1\",\"delegate_name\":\"delegate_name_1\",\"about\":\"about\",\"website\":\"website\",\"team\":\"team\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"0\",\"server_specs\":\"server settings\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"" XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT_TEST "\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}");
  
  memcpy(result_test,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF|" TEST_WALLET "|" TEST_RESERVE_PROOF_DELEGATES_REGISTER "|" TEST_WALLET_DELEGATES_REGISTER "|" TEST_SIGNATURE_ADD_RESERVE_PROOF "|",872);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (read_document_field_from_collection(database_name,"reserve_proofs_1","{\"reserve_proof\":\"" TEST_RESERVE_PROOF_DELEGATES_REGISTER "\"}","total",result_test) == 1 && strncmp(result_test,"120000000",BUFFER_SIZE) == 0 && read_document_field_from_collection(database_name,"delegates","{\"public_address\":\"" TEST_WALLET "\"}","total_vote_count",data_test) == 1 && strncmp(data_test,"170000000",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for server_receive_data_socket_node_to_block_verifiers_add_reserve_proof\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for server_receive_data_socket_node_to_block_verifiers_add_reserve_proof\033[0m\n");
  }

  // test server_receive_data_socket_node_to_block_verifiers_add_reserve_proof
  delete_database(database_name);
  RESET_ERROR_MESSAGES;
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET "\",\"total_vote_count\":\"60000000\",\"IP_address\":\"127.0.0.1\",\"delegate_name\":\"delegate_name_1\",\"about\":\"about\",\"website\":\"website\",\"team\":\"team\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"0\",\"server_specs\":\"server settings\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"" XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT_TEST "\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}");
  insert_document_into_collection_json(database_name,"reserve_proofs_5","{\"public_address_created_reserve_proof\":\"" TEST_WALLET_DELEGATES_REGISTER "\",\"public_address_voted_for\":\"" TEST_WALLET "\",\"total\":\"10000000\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiub05pa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\"}");

  memcpy(result_test,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF|" TEST_WALLET "|" TEST_RESERVE_PROOF_DELEGATES_REGISTER "|" TEST_WALLET_DELEGATES_REGISTER "|" TEST_SIGNATURE_ADD_RESERVE_PROOF "|",872);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (count_documents_in_collection(database_name,"reserve_proofs_5","{\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiub05pa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\"}") == 0 && read_document_field_from_collection(database_name,"delegates","{\"public_address\":\"" TEST_WALLET "\"}","total_vote_count",data_test) == 1 && strncmp(data_test,"170000000",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for server_receive_data_socket_node_to_block_verifiers_add_reserve_proof checking for when a voter votes for another delegate and the old vote is cancelled and the old delegates total is adjusted\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for server_receive_data_socket_node_to_block_verifiers_add_reserve_proof checking for when a voter votes for another delegate and the old vote is cancelled and the old delegates total is adjusted\033[0m\n");
  }

  // test server_receive_data_socket_nodes_to_block_verifiers_register_delegates
  RESET_ERROR_MESSAGES;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE|delegate_name_2|192.168.2.0|a2f7a502d4128c0dbe650116ed77df269a6a3623e90b1b361e977d5bdb73c646|" TEST_WALLET_DELEGATES_REGISTER "|" TEST_SIGNATURE_DELEGATES_REGISTER "|",329);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  if (count_documents_in_collection(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_DELEGATES_REGISTER "\"}") == 1)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for server_receive_data_socket_nodes_to_block_verifiers_register_delegates\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for server_receive_data_socket_nodes_to_block_verifiers_register_delegates\033[0m\n");
  }

  // test server_receive_data_socket_nodes_to_block_verifiers_update_delegates
  RESET_ERROR_MESSAGES;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_DELEGATES_REGISTER "\",\"total_vote_count\":\"0\",\"IP_address\":\"127.0.0.1\",\"delegate_name\":\"delegate_name_1\",\"about\":\"about\",\"website\":\"website\",\"team\":\"team\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"0\",\"server_specs\":\"server settings\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"" XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT_TEST "\",\"public_key\":\"" NEXT_BLOCK_VERIFIERS_PUBLIC_KEY "\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE|about|data|" TEST_WALLET_DELEGATES_REGISTER "|" TEST_SIGNATURE_DELEGATES_UPDATE "|",245);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  if (read_document_field_from_collection(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_DELEGATES_REGISTER "\"}","about",result_test) == 1 && strncmp(result_test,"data",sizeof(result_test)) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for server_receive_data_socket_nodes_to_block_verifiers_update_delegates\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for server_receive_data_socket_nodes_to_block_verifiers_update_delegates\033[0m\n");
  }

  // check_vote_status
  delete_database(database_name);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" NETWORK_DATA_NODE_1_PUBLIC_ADDRESS "\",\"total_vote_count\":\"240000000\",\"IP_address\":\"" NETWORK_DATA_NODE_1_IP_ADDRESS "\",\"delegate_name\":\"delegate_name_1\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"" TEST_WALLET_PUBLIC_KEY_1 "\"}");
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_RESERVE_PROOFS_DATA_1);
  insert_document_into_collection_json(database_name,"reserve_proofs_5",DATABASE_COLLECTION_RESERVE_PROOFS_DATA_1);
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_CHECK_VOTE_STATUS|" TEST_WALLET_1 "|",144);
  send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  if (strncmp(data_test,"delegate_name: delegate_name_1, total: 120000000}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for check_vote_status\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for check_vote_status\033[0m\n");
  }

  // test check_for_valid_delegate_name
  if (check_for_valid_delegate_name("us1.xcash.foundation") == 1 && check_for_valid_delegate_name("(us1.xcash.foundation)") == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for check_for_valid_delegate_name\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for check_for_valid_delegate_name\033[0m\n");
  }

  // test check_for_valid_ip_address
  if (check_for_valid_ip_address("us1.xcash.foundation") == 1 && check_for_valid_ip_address("147.135.68.247") == 1 && check_for_valid_ip_address("127.0.0.1") == 0 && check_for_valid_ip_address("localhost") == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for check_for_valid_ip_address\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for check_for_valid_ip_address\033[0m\n");
  }

  // test server_receive_data_socket_nodes_to_block_verifiers_recover_delegates for public address and public key
  RESET_ERROR_MESSAGES;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));

  delete_database(database_name);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_2);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_3);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_4);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_5);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_6);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_7);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_8);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_9);
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_10 "\",\"total_vote_count\":\"3000000000\",\"IP_address\":\"delegate-recover-test-1.xcash.foundation\",\"delegate_name\":\"delegate_name_10\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"" TEST_WALLET_PUBLIC_KEY_10 "\"}");
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_1);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_2);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_3);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_4);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_5);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_6);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_7);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_8);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_9);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_10);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_11);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_12);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_13);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_14);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_15);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_16);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_17);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_18);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_19);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_20);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
  
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE|delegate-recover-test-1.xcash.foundation|",83);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (read_document_field_from_collection(database_name,"delegates","{\"IP_address\":\"delegate-recover-test-1.xcash.foundation\"}","public_address",result_test) == 1 && strncmp(result_test,TEST_WALLET_15,sizeof(result_test)) == 0 && read_document_field_from_collection(database_name,"delegates","{\"IP_address\":\"delegate-recover-test-1.xcash.foundation\"}","public_key",data_test) == 1 && strncmp(data_test,TEST_WALLET_PUBLIC_KEY_15,sizeof(data_test)) == 0 && count_documents_in_collection(database_name,"reserve_proofs_1","{\"public_address_voted_for\":\"" TEST_WALLET_15 "\"}") == 3)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for server_receive_data_socket_nodes_to_block_verifiers_recover_delegates for public address and public key\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for server_receive_data_socket_nodes_to_block_verifiers_recover_delegates for public address and public key\033[0m\n");
    exit(0);
  }

  // test server_receive_data_socket_nodes_to_block_verifiers_recover_delegates for public address
  RESET_ERROR_MESSAGES;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));

  delete_database(database_name);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_2);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_3);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_4);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_5);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_6);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_7);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_8);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_9);
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_10 "\",\"total_vote_count\":\"3000000000\",\"IP_address\":\"delegate-recover-test-2.xcash.foundation\",\"delegate_name\":\"delegate_name_10\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"" TEST_WALLET_PUBLIC_KEY_10 "\"}");
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_1);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_2);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_3);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_4);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_5);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_6);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_7);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_8);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_9);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_10);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_11);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_12);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_13);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_14);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_15);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_16);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_17);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_18);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_19);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_20);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
  
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE|delegate-recover-test-2.xcash.foundation|",83);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (read_document_field_from_collection(database_name,"delegates","{\"IP_address\":\"delegate-recover-test-2.xcash.foundation\"}","public_address",result_test) == 1 && strncmp(result_test,TEST_WALLET_15,sizeof(result_test)) == 0 && count_documents_in_collection(database_name,"reserve_proofs_1","{\"public_address_voted_for\":\"" TEST_WALLET_15 "\"}") == 3)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for server_receive_data_socket_nodes_to_block_verifiers_recover_delegates for public address\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for server_receive_data_socket_nodes_to_block_verifiers_recover_delegates for public address\033[0m\n");
    exit(0);
  }

  // test server_receive_data_socket_nodes_to_block_verifiers_recover_delegates for public key
  RESET_ERROR_MESSAGES;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));

  delete_database(database_name);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_2);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_3);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_4);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_5);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_6);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_7);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_8);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_9);
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_10 "\",\"total_vote_count\":\"3000000000\",\"IP_address\":\"delegate-recover-test-3.xcash.foundation\",\"delegate_name\":\"delegate_name_10\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"" TEST_WALLET_PUBLIC_KEY_10 "\"}");
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_1);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_2);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_3);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_4);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_5);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_6);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_7);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_8);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_9);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_10);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_11);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_12);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_13);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_14);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_15);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_16);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_17);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_18);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_19);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_20);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
  
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE|delegate-recover-test-3.xcash.foundation|",83);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (read_document_field_from_collection(database_name,"delegates","{\"IP_address\":\"delegate-recover-test-3.xcash.foundation\"}","public_key",data_test) == 1 && strncmp(data_test,TEST_WALLET_PUBLIC_KEY_15,sizeof(data_test)) == 0 && count_documents_in_collection(database_name,"reserve_proofs_1","{\"public_address_voted_for\":\"" TEST_WALLET_10 "\"}") == 3)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for server_receive_data_socket_nodes_to_block_verifiers_recover_delegates for public key\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for server_receive_data_socket_nodes_to_block_verifiers_recover_delegates for public key\033[0m\n");
    exit(0);
  }

  // test server_receive_data_socket_nodes_to_block_verifiers_recover_delegates for invalid data
  RESET_ERROR_MESSAGES;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));

  delete_database(database_name);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_2);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_3);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_4);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_5);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_6);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_7);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_8);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_9);
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_10 "\",\"total_vote_count\":\"3000000000\",\"IP_address\":\"delegate-recover-test-4.xcash.foundation\",\"delegate_name\":\"delegate_name_10\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"" TEST_WALLET_PUBLIC_KEY_10 "\"}");
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_1);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_2);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_3);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_4);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_5);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_6);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_7);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_8);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_9);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_10);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_11);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_12);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_13);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_14);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_15);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_16);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_17);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_18);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_19);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_20);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
  
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE|delegate-recover-test-4.xcash.foundation|",83);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (read_document_field_from_collection(database_name,"delegates","{\"IP_address\":\"delegate-recover-test-4.xcash.foundation\"}","public_address",result_test) == 1 && strncmp(result_test,TEST_WALLET_10,sizeof(result_test)) == 0 && read_document_field_from_collection(database_name,"delegates","{\"IP_address\":\"delegate-recover-test-4.xcash.foundation\"}","public_key",data_test) == 1 && strncmp(data_test,TEST_WALLET_PUBLIC_KEY_10,sizeof(data_test)) == 0 && count_documents_in_collection(database_name,"reserve_proofs_1","{\"public_address_voted_for\":\"" TEST_WALLET_10 "\"}") == 3)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for server_receive_data_socket_nodes_to_block_verifiers_recover_delegates for invalid data\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for server_receive_data_socket_nodes_to_block_verifiers_recover_delegates for invalid data\033[0m\n");
    exit(0);
  }

  // test server_receive_data_socket_nodes_to_block_verifiers_recover_delegates for invalid data
  RESET_ERROR_MESSAGES;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));

  delete_database(database_name);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_2);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_3);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_4);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_5);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_6);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_7);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_8);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_9);
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_10 "\",\"total_vote_count\":\"3000000000\",\"IP_address\":\"delegate-recover-test-5.xcash.foundation\",\"delegate_name\":\"delegate_name_10\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"" TEST_WALLET_PUBLIC_KEY_10 "\"}");
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_1);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_2);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_3);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_4);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_5);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_6);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_7);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_8);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_9);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_10);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_11);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_12);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_13);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_14);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_15);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_16);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_17);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_18);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_19);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DATA_20);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
  
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE|delegate-recover-test-5.xcash.foundation|",83);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (read_document_field_from_collection(database_name,"delegates","{\"IP_address\":\"delegate-recover-test-5.xcash.foundation\"}","public_address",result_test) == 1 && strncmp(result_test,TEST_WALLET_10,sizeof(result_test)) == 0 && read_document_field_from_collection(database_name,"delegates","{\"IP_address\":\"delegate-recover-test-5.xcash.foundation\"}","public_key",data_test) == 1 && strncmp(data_test,TEST_WALLET_PUBLIC_KEY_10,sizeof(data_test)) == 0 && count_documents_in_collection(database_name,"reserve_proofs_1","{\"public_address_voted_for\":\"" TEST_WALLET_10 "\"}") == 3)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for server_receive_data_socket_nodes_to_block_verifiers_recover_delegates for no data\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for server_receive_data_socket_nodes_to_block_verifiers_recover_delegates for no data\033[0m\n");
    exit(0);
  }


  
  // check all errors for each test
  delete_database(database_name);
  RESET_ERROR_MESSAGES;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));

  // block_verifiers_add_reserve_proof_check_if_data_is_valid
  memset(reserve_proof.block_verifier_public_address,0,sizeof(reserve_proof.block_verifier_public_address));
  memset(reserve_proof.public_address_created_reserve_proof,0,sizeof(reserve_proof.public_address_created_reserve_proof));
  memset(reserve_proof.public_address_voted_for,0,sizeof(reserve_proof.public_address_voted_for));
  memset(reserve_proof.reserve_proof_amount,0,sizeof(reserve_proof.reserve_proof_amount));
  memset(reserve_proof.reserve_proof,0,sizeof(reserve_proof.reserve_proof));
  memcpy(result_test,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF|" TEST_WALLET "|" TEST_RESERVE_PROOF_DELEGATES_REGISTER "|" TEST_WALLET_DELEGATES_REGISTER "|SigV1N2NWu1Yzg4aVrhJUjZVcRZ52CcgUghX7x5Fn9W5drC4g8B9tZn7q4yJEKbiJMRFeNRPmSPT3rBSVkJnFWAGK5PA1|",872);
  if (block_verifiers_add_reserve_proof_check_if_data_is_valid(result_test,&reserve_proof) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for block_verifiers_add_reserve_proof_check_if_data_is_valid checking Could not verify the message\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for block_verifiers_add_reserve_proof_check_if_data_is_valid checking Could not verify the message\033[0m\n");
  }
  
  memset(reserve_proof.block_verifier_public_address,0,sizeof(reserve_proof.block_verifier_public_address));
  memset(reserve_proof.public_address_created_reserve_proof,0,sizeof(reserve_proof.public_address_created_reserve_proof));
  memset(reserve_proof.public_address_voted_for,0,sizeof(reserve_proof.public_address_voted_for));
  memset(reserve_proof.reserve_proof_amount,0,sizeof(reserve_proof.reserve_proof_amount));
  memset(reserve_proof.reserve_proof,0,sizeof(reserve_proof.reserve_proof));
  memcpy(result_test,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF|delegate_name_1|" TEST_RESERVE_PROOF_DELEGATES_REGISTER "|" TEST_WALLET_DELEGATES_REGISTER "|" TEST_SIGNATURE_ADD_RESERVE_PROOF "|",789);
  if (block_verifiers_add_reserve_proof_check_if_data_is_valid(result_test,&reserve_proof) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for block_verifiers_add_reserve_proof_check_if_data_is_valid checking for invalid data\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for block_verifiers_add_reserve_proof_check_if_data_is_valid checking for invalid data\033[0m\n");
  }
  
  memset(reserve_proof.block_verifier_public_address,0,sizeof(reserve_proof.block_verifier_public_address));
  memset(reserve_proof.public_address_created_reserve_proof,0,sizeof(reserve_proof.public_address_created_reserve_proof));
  memset(reserve_proof.public_address_voted_for,0,sizeof(reserve_proof.public_address_voted_for));
  memset(reserve_proof.reserve_proof_amount,0,sizeof(reserve_proof.reserve_proof_amount));
  memset(reserve_proof.reserve_proof,0,sizeof(reserve_proof.reserve_proof));
  memcpy(result_test,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF|" TEST_WALLET "|" TEST_RESERVE_PROOF_DELEGATES_REGISTER "|" TEST_WALLET "|" TEST_SIGNATURE_ADD_RESERVE_PROOF "|",872);
  if (block_verifiers_add_reserve_proof_check_if_data_is_valid(result_test,&reserve_proof) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for block_verifiers_add_reserve_proof_check_if_data_is_valid checking for invalid reserve proof\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for block_verifiers_add_reserve_proof_check_if_data_is_valid checking for invalid reserve proof\033[0m\n");
  }
  
  memset(reserve_proof.block_verifier_public_address,0,sizeof(reserve_proof.block_verifier_public_address));
  memset(reserve_proof.public_address_created_reserve_proof,0,sizeof(reserve_proof.public_address_created_reserve_proof));
  memset(reserve_proof.public_address_voted_for,0,sizeof(reserve_proof.public_address_voted_for));
  memset(reserve_proof.reserve_proof_amount,0,sizeof(reserve_proof.reserve_proof_amount));
  memset(reserve_proof.reserve_proof,0,sizeof(reserve_proof.reserve_proof));
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_RESERVE_PROOFS_DATA_1);
  memcpy(result_test,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF|" TEST_WALLET "|" TEST_RESERVE_PROOF_DELEGATES_REGISTER "|" TEST_WALLET_DELEGATES_REGISTER "|" TEST_SIGNATURE_ADD_RESERVE_PROOF "|",872);
  if (block_verifiers_add_reserve_proof_check_if_data_is_valid(result_test,&reserve_proof) == 8)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for block_verifiers_add_reserve_proof_check_if_data_is_valid checking for reserve proof is already in the database\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for block_verifiers_add_reserve_proof_check_if_data_is_valid checking for reserve proof is already in the database\033[0m\n");
  }

  

  // server_receive_data_socket_nodes_to_block_verifiers_register_delegates
  DELEGATES_SERVER_FUNCTIONS_TEST_ERROR("NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE|delegate_name_2|192.168.2.0|a2f7a502d4128c0dbe650116ed77df269a6a3623e90b1b361e977d5bdb73c646|","server_receive_data_socket_nodes_to_block_verifiers_register_delegates","Could not verify the message}"); 
  RESET_ERROR_MESSAGES;

  DELEGATES_SERVER_FUNCTIONS_TEST_ERROR("NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE|delegate_name_2|DATA|a2f7a502d4128c0dbe650116ed77df269a6a3623e90b1b361e977d5bdb73c646|" TEST_WALLET_DELEGATES_REGISTER "|" TEST_SIGNATURE_DELEGATES_REGISTER_INVALID_DATA "|","server_receive_data_socket_nodes_to_block_verifiers_register_delegates","Invalid data}");
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_DELEGATES_REGISTER "\",\"total_vote_count\":\"0\",\"IP_address\":\"127.0.0.1\",\"delegate_name\":\"delegate_name_1\",\"about\":\"about\",\"website\":\"website\",\"team\":\"team\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"0\",\"server_specs\":\"server settings\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"" XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT_TEST "\",\"public_key\":\"" NEXT_BLOCK_VERIFIERS_PUBLIC_KEY "\"}");
  RESET_ERROR_MESSAGES;

  DELEGATES_SERVER_FUNCTIONS_TEST_ERROR("NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE|delegate_name_2|192.168.2.0|a2f7a502d4128c0dbe650116ed77df269a6a3623e90b1b361e977d5bdb73c646|" TEST_WALLET_DELEGATES_REGISTER "|" TEST_SIGNATURE_DELEGATES_REGISTER_PUBLIC_ADDRESS_ALREADY_REGISTERED "|","server_receive_data_socket_nodes_to_block_verifiers_register_delegates","The delegates public address is already registered}"); 
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET "\",\"total_vote_count\":\"0\",\"IP_address\":\"127.0.0.1\",\"delegate_name\":\"delegate_name_1\",\"about\":\"about\",\"website\":\"website\",\"team\":\"team\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"0\",\"server_specs\":\"server settings\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"" XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT_TEST "\",\"public_key\":\"" NEXT_BLOCK_VERIFIERS_PUBLIC_KEY "\"}");
  RESET_ERROR_MESSAGES;

  DELEGATES_SERVER_FUNCTIONS_TEST_ERROR("NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE|delegate_name_2|127.0.0.1|a2f7a502d4128c0dbe650116ed77df269a6a3623e90b1b361e977d5bdb73c646|" TEST_WALLET_DELEGATES_REGISTER "|" TEST_SIGNATURE_DELEGATES_REGISTER_IP_ADDRESS_ALREADY_REGISTERED "|","server_receive_data_socket_nodes_to_block_verifiers_register_delegates","The delegates IP address is already registered}"); 
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET "\",\"total_vote_count\":\"0\",\"IP_address\":\"127.0.0.1\",\"delegate_name\":\"delegate_name_1\",\"about\":\"about\",\"website\":\"website\",\"team\":\"team\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"0\",\"server_specs\":\"server settings\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"" XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT_TEST "\",\"public_key\":\"" NEXT_BLOCK_VERIFIERS_PUBLIC_KEY "\"}");
  RESET_ERROR_MESSAGES;
  
  DELEGATES_SERVER_FUNCTIONS_TEST_ERROR("NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE|delegate_name_2|192.168.2.0|" NEXT_BLOCK_VERIFIERS_PUBLIC_KEY "|" TEST_WALLET_DELEGATES_REGISTER "|" TEST_SIGNATURE_DELEGATES_REGISTER_PUBLIC_KEY_ALREADY_REGISTERED "|","server_receive_data_socket_nodes_to_block_verifiers_register_delegates","The delegates public key is already registered}"); 
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA);
  RESET_ERROR_MESSAGES;
  
  DELEGATES_SERVER_FUNCTIONS_TEST_ERROR("NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE|delegate_name_1|192.168.2.0|a2f7a502d4128c0dbe650116ed77df269a6a3623e90b1b361e977d5bdb73c646|" TEST_WALLET_DELEGATES_REGISTER "|" TEST_SIGNATURE_DELEGATES_REGISTER_DELEGATES_NAME_ALREADY_REGISTERED "|","server_receive_data_socket_nodes_to_block_verifiers_register_delegates","The delegates name is already registered}"); 
  RESET_ERROR_MESSAGES;

  // test server_receive_data_socket_nodes_to_block_verifiers_update_delegates
  DELEGATES_SERVER_FUNCTIONS_TEST_ERROR("NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE|about|data|" TEST_WALLET_DELEGATES_REGISTER "|000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000|","server_receive_data_socket_nodes_to_block_verifiers_update_delegates","Could not verify the message}"); 
  RESET_ERROR_MESSAGES;

  DELEGATES_SERVER_FUNCTIONS_TEST_ERROR("NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE|data|data|" TEST_WALLET_DELEGATES_REGISTER "|" TEST_SIGNATURE_DELEGATES_UPDATE_INVALID_ITEM "|","server_receive_data_socket_nodes_to_block_verifiers_update_delegates","Invalid item to update}"); 
  RESET_ERROR_MESSAGES;

  DELEGATES_SERVER_FUNCTIONS_TEST_ERROR("NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE|IP_address|0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000|" TEST_WALLET_DELEGATES_REGISTER "|" TEST_SIGNATURE_DELEGATES_UPDATE_INVALID_ITEM_VALUE "|","server_receive_data_socket_nodes_to_block_verifiers_update_delegates","Invalid item value to update}"); 
  RESET_ERROR_MESSAGES;

  DELEGATES_SERVER_FUNCTIONS_TEST_ERROR("NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE|about|data|" TEST_WALLET_DELEGATES_REGISTER "|" TEST_SIGNATURE_DELEGATES_UPDATE "|","server_receive_data_socket_nodes_to_block_verifiers_update_delegates","The delegate could not be updated from the database}"); 
  RESET_ERROR_MESSAGES;

  // check_vote_status
  delete_database(database_name);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" NETWORK_DATA_NODE_1_PUBLIC_ADDRESS "\",\"total_vote_count\":\"240000000\",\"IP_address\":\"" NETWORK_DATA_NODE_1_IP_ADDRESS "\",\"delegate_name\":\"delegate_name_1\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"" TEST_WALLET_PUBLIC_KEY_1 "\"}");
  insert_document_into_collection_json(database_name,"reserve_proofs_5",DATABASE_COLLECTION_RESERVE_PROOFS_DATA_5);
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_CHECK_VOTE_STATUS|" TEST_WALLET_1 "|",144);
  send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  if (strncmp(data_test,"Error: The public address does not have an active vote}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for check_vote_status for no vote\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for check_vote_status for no vote\033[0m\n");
  }

  // test the check_for_valid_delegate_fee
  if (check_for_valid_delegate_fee("2") == 1 && check_for_valid_delegate_fee("2.5") == 1)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for check_for_valid_delegate_fee\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for check_for_valid_delegate_fee\033[0m\n");
  }

  if (check_for_valid_delegate_fee("200") == 0 && check_for_valid_delegate_fee("1.5000000") == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for check_for_valid_delegate_fee checking for invalid fee structures\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for check_for_valid_delegate_fee checking for invalid fee structures\033[0m\n");
  }

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\ndelegate server functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,DELEGATE_SERVER_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;
  
  #undef TEST_RESERVE_PROOF_DELEGATES_REGISTER
  #undef TEST_WALLET_DELEGATES_REGISTER
  #undef TEST_SIGNATURE_ADD_RESERVE_PROOF
  #undef TEST_SIGNATURE_ADD_RESERVE_PROOF_INVALID_DATA
  #undef TEST_SIGNATURE_ADD_RESERVE_PROOF_INVALID_RESERVE_PROOF
  #undef TEST_SIGNATURE_DELEGATES_REGISTER
  #undef TEST_SIGNATURE_DELEGATES_REGISTER_INVALID_DATA
  #undef TEST_SIGNATURE_DELEGATES_REGISTER_PUBLIC_ADDRESS_ALREADY_REGISTERED
  #undef TEST_SIGNATURE_DELEGATES_REGISTER_IP_ADDRESS_ALREADY_REGISTERED
  #undef TEST_SIGNATURE_DELEGATES_REGISTER_PUBLIC_KEY_ALREADY_REGISTERED
  #undef TEST_SIGNATURE_DELEGATES_REGISTER_DELEGATES_NAME_ALREADY_REGISTERED
  #undef TEST_SIGNATURE_DELEGATES_UPDATE
  #undef TEST_SIGNATURE_DELEGATES_UPDATE_INVALID_ITEM
  #undef TEST_SIGNATURE_DELEGATES_UPDATE_INVALID_ITEM_VALUE
  #undef TEST_SIGNATURE_DELEGATES_REMOVE
  #undef DELEGATES_SERVER_FUNCTIONS_TEST_ERROR
}