#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "define_macro_functions.h"
#include "define_macros_test.h"

#include "count_database_functions.h"
#include "insert_database_functions.h"
#include "update_database_functions.h"
#include "delete_database_functions.h"
#include "thread_functions.h"

#include "variables_test.h"
#include "update_database_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: update_database_functions_test
Description: Test the update database functions
Return: The number of passed update database functions test
-----------------------------------------------------------------------------------------------------------
*/

int update_database_functions_test(void)
{   
  // Variables 
  pthread_t thread_id;

  // define macros
  #define MESSAGE "{\"message_settings\": \"XCASH_DPOPS_TEST_DATA\"}"
  #define MESSAGE_SETTINGS "{\"message_settings\": \"XCASH_DPOPS_DATA\"}"

  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);

  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nupdate database functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,UPDATE_DATABASE_FUNCTIONS_TEST,TEST_OUTLINE);

  // reset the variables
  memset(data_test,0,sizeof(data_test));
  count_test = 0;

  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE);
  // update a document in the collection
  if (update_document_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,MESSAGE_SETTINGS) == 0)
  {
    color_print("FAILED! Test for update_document_from_collection","red");
  }
  else
  {
    color_print("PASSED! Test for update_document_from_collection","green");
    count_test++;
  }

  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE);
  // update a document in the collection on a separate thread
  struct update_document_from_collection_thread_parameters update_document_from_collection_thread_parameters = {database_name,DATABASE_COLLECTION_TEST,MESSAGE,MESSAGE_SETTINGS};
  pthread_create(&thread_id, NULL, &update_document_from_collection_thread,(void *)&update_document_from_collection_thread_parameters);
  if (thread_settings(thread_id) == 0)
  {
    color_print("FAILED! Test for update_document_from_collection_thread","red");
  }
  else
  {
    color_print("PASSED! Test for update_document_from_collection_thread","green");
    count_test++;
  }

  // update multiple documents in the collection
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,"{\"message_settings\": \"XCASH_DPOPS_TEST_DATA_1\",\"public_address\": \"" NETWORK_DATA_NODE_1_PUBLIC_ADDRESS_PRODUCTION "\"}");
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,"{\"message_settings\": \"XCASH_DPOPS_TEST_DATA_2\",\"public_address\": \"" NETWORK_DATA_NODE_1_PUBLIC_ADDRESS_PRODUCTION "\"}");
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,"{\"message_settings\": \"XCASH_DPOPS_TEST_DATA_3\",\"public_address\": \"" NETWORK_DATA_NODE_2_PUBLIC_ADDRESS_PRODUCTION "\"}");
  if (update_multiple_documents_from_collection(database_name,DATABASE_COLLECTION_TEST,"{\"public_address\": \"" NETWORK_DATA_NODE_1_PUBLIC_ADDRESS_PRODUCTION "\"}","{\"public_address\": \"" NETWORK_DATA_NODE_2_PUBLIC_ADDRESS_PRODUCTION "\"}") == 0 || count_documents_in_collection(database_name,DATABASE_COLLECTION_TEST,"{\"public_address\": \"" NETWORK_DATA_NODE_2_PUBLIC_ADDRESS_PRODUCTION "\"}") != 3)
  {
    color_print("FAILED! Test for update_multiple_documents_from_collection","red");
  }
  else
  {
    color_print("PASSED! Test for update_multiple_documents_from_collection","green");
    count_test++;
  }

  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE);
  // update all document in the collection
  if (update_all_documents_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE) == 0)
  {
    color_print("FAILED! Test for update_all_documents_from_collection","red");
  }
  else
  {
    color_print("PASSED! Test for update_all_documents_from_collection","green");
    count_test++;
  }

  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE);
  // update all document in the collection on a separate thread
  struct update_all_documents_from_collection_thread_parameters update_all_documents_from_collection_thread_parameters = {database_name,DATABASE_COLLECTION_TEST,MESSAGE};
  pthread_create(&thread_id, NULL, &update_all_documents_from_collection_thread,(void *)&update_all_documents_from_collection_thread_parameters);
  if (thread_settings(thread_id) == 0)
  {
    color_print("FAILED! Test for update_all_documents_from_collection_thread","red");
  }
  else
  {
    color_print("PASSED! Test for update_all_documents_from_collection_thread","green");
    count_test++;
  }



  // check all errors for each test
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);
  RESET_ERROR_MESSAGES;

  // update_document_from_collection
  if (update_document_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,MESSAGE_SETTINGS) == 0 && strncmp(error_message.data[0],"The database collection does not exist",BUFFER_SIZE) == 0)
  {
    color_print("PASSED! Test for update_document_from_collection checking for The database collection does not exist","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for update_document_from_collection checking for The database collection does not exist","red");
  }
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE);
  RESET_ERROR_MESSAGES;

  if (update_document_from_collection(database_name,DATABASE_COLLECTION_TEST,"data",MESSAGE_SETTINGS) == 0 && strncmp(error_message.data[0],"Could not convert the data into a database document",BUFFER_SIZE) == 0)
  {
    color_print("PASSED! Test for update_document_from_collection checking for Could not convert the data into a database document","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for update_document_from_collection checking for Could not convert the data into a database document","red");
  }
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);
  RESET_ERROR_MESSAGES;

  // update_all_documents_from_collection
  if (update_all_documents_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE) == 0 && strncmp(error_message.data[0],"The database collection does not exist",BUFFER_SIZE) == 0)
  {
    color_print("PASSED! Test for update_all_documents_from_collection checking for The database collection does not exist","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for update_all_documents_from_collection checking for The database collection does not exist","red");
  }
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE);
  RESET_ERROR_MESSAGES;

  if (update_all_documents_from_collection(database_name,DATABASE_COLLECTION_TEST,"data") == 0 && strncmp(error_message.data[0],"Could not convert the data into a database document",BUFFER_SIZE) == 0)
  {
    color_print("PASSED! Test for update_all_documents_from_collection checking for Could not convert the data into a database document","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for update_all_documents_from_collection checking for Could not convert the data into a database document","red");
  }
  RESET_ERROR_MESSAGES;

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nupdate database functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,UPDATE_DATABASE_FUNCTIONS_TEST-count_test,TEST_OUTLINE);
  return count_test;
  
  #undef MESSAGE
  #undef MESSAGE_SETTINGS
}