#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "define_macros_test.h"

#include "delete_database_functions.h"
#include "insert_database_functions.h"
#include "organize_functions.h"

#include "variables_test.h"
#include "organize_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: organize_functions_test
Description: Test the organize functions
Return: The number of passed organize_functions test
-----------------------------------------------------------------------------------------------------------
*/

int organize_functions_test(void)
{
  // variables 
  char* reserve_proofs[5];
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  int count;

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  count_test = 0;

  // initialize the reserve_proofs array 
  for (count = 0; count < 5; count++)
  {
    reserve_proofs[count] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));

    if (reserve_proofs[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }

  // initialize the delegates struct
  for (count = 0; count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    delegates[count].public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    delegates[count].total_vote_count = (char*)calloc(100,sizeof(char));
    delegates[count].IP_address = (char*)calloc(256,sizeof(char));
    delegates[count].delegate_name = (char*)calloc(MAXIMUM_BUFFER_SIZE_DELEGATES_NAME+1,sizeof(char));
    delegates[count].about = (char*)calloc(1025,sizeof(char));
    delegates[count].website = (char*)calloc(256,sizeof(char));
    delegates[count].team = (char*)calloc(256,sizeof(char));
    delegates[count].shared_delegate_status = (char*)calloc(10,sizeof(char));
    delegates[count].delegate_fee = (char*)calloc(11,sizeof(char));
    delegates[count].server_specs = (char*)calloc(1025,sizeof(char));
    delegates[count].block_verifier_score = (char*)calloc(10,sizeof(char));
    delegates[count].online_status = (char*)calloc(10,sizeof(char));
    delegates[count].block_verifier_total_rounds = (char*)calloc(10,sizeof(char));
    delegates[count].block_verifier_online_total_rounds = (char*)calloc(10,sizeof(char));
    delegates[count].block_verifier_online_percentage = (char*)calloc(10,sizeof(char));
    delegates[count].block_producer_total_rounds = (char*)calloc(10,sizeof(char));
    delegates[count].block_producer_block_heights = (char*)calloc(50000,sizeof(char));
    delegates[count].public_key = (char*)calloc(VRF_PUBLIC_KEY_LENGTH+1,sizeof(char));
    if (delegates[count].public_address == NULL || delegates[count].total_vote_count == NULL || delegates[count].IP_address == NULL || delegates[count].delegate_name == NULL || delegates[count].about == NULL || delegates[count].website == NULL || delegates[count].team == NULL || delegates[count].shared_delegate_status == NULL || delegates[count].delegate_fee == NULL || delegates[count].server_specs == NULL || delegates[count].block_verifier_score == NULL || delegates[count].online_status == NULL || delegates[count].block_verifier_total_rounds == NULL || delegates[count].block_verifier_online_total_rounds == NULL || delegates[count].block_verifier_online_percentage == NULL || delegates[count].block_producer_total_rounds == NULL || delegates[count].block_producer_block_heights == NULL || delegates[count].public_key == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }

  memcpy(delegates[0].public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH);
  memcpy(delegates[0].total_vote_count,"5",2);
  memcpy(delegates[0].IP_address,NETWORK_DATA_NODE_1_IP_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_1_IP_ADDRESS_PRODUCTION)-1);
  memcpy(delegates[0].delegate_name,"delegates_xcash_foundation",26);
  memcpy(delegates[0].about,"Official X-Network node",23);
  memcpy(delegates[0].website,NETWORK_DATA_NODE_1_IP_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_1_IP_ADDRESS_PRODUCTION)-1);
  memcpy(delegates[0].team,"X-Network Team",14);
  memcpy(delegates[0].shared_delegate_status,"solo",4);
  memcpy(delegates[0].delegate_fee,"0",sizeof(char));
  memcpy(delegates[0].server_specs,"",0);
  memcpy(delegates[0].block_verifier_score,"0",sizeof(char));
  memcpy(delegates[0].online_status,"false",5);
  memcpy(delegates[0].block_verifier_total_rounds,"0",sizeof(char));
  memcpy(delegates[0].block_verifier_online_total_rounds,"0",sizeof(char));
  memcpy(delegates[0].block_verifier_online_percentage,"0",sizeof(char));
  memcpy(delegates[0].block_producer_total_rounds,"0",sizeof(char));
  memcpy(delegates[0].block_producer_block_heights,"0",sizeof(char));
  memcpy(delegates[0].public_key,NETWORK_DATA_NODE_1_PUBLIC_KEY_PRODUCTION,VRF_PUBLIC_KEY_LENGTH);

  memcpy(delegates[1].public_address,NETWORK_DATA_NODE_2_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH);
  memcpy(delegates[1].total_vote_count,"500",3);
  memcpy(delegates[1].IP_address,NETWORK_DATA_NODE_2_IP_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_2_IP_ADDRESS_PRODUCTION)-1);
  memcpy(delegates[1].delegate_name,"europe1_xcash_foundation",24);
  memcpy(delegates[1].about,"Official X-Network node",23);
  memcpy(delegates[1].website,NETWORK_DATA_NODE_2_IP_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_2_IP_ADDRESS_PRODUCTION)-1);
  memcpy(delegates[1].team,"X-Network Team",14);
  memcpy(delegates[1].shared_delegate_status,"solo",4);
  memcpy(delegates[1].delegate_fee,"0",sizeof(char));
  memcpy(delegates[1].server_specs,"",0);
  memcpy(delegates[1].block_verifier_score,"0",sizeof(char));
  memcpy(delegates[1].online_status,"false",5);
  memcpy(delegates[1].block_verifier_total_rounds,"0",sizeof(char));
  memcpy(delegates[1].block_verifier_online_total_rounds,"0",sizeof(char));
  memcpy(delegates[1].block_verifier_online_percentage,"0",sizeof(char));
  memcpy(delegates[1].block_producer_total_rounds,"0",sizeof(char));
  memcpy(delegates[1].block_producer_block_heights,"0",sizeof(char));
  memcpy(delegates[1].public_key,NETWORK_DATA_NODE_2_PUBLIC_KEY_PRODUCTION,VRF_PUBLIC_KEY_LENGTH);

  memcpy(delegates[2].public_address,NETWORK_DATA_NODE_3_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH);
  memcpy(delegates[2].total_vote_count,"15",2);
  memcpy(delegates[2].IP_address,NETWORK_DATA_NODE_3_IP_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_3_IP_ADDRESS_PRODUCTION)-1);
  memcpy(delegates[2].delegate_name,"europe2_xcash_foundation",24);
  memcpy(delegates[2].about,"Official X-Network node",23);
  memcpy(delegates[2].website,NETWORK_DATA_NODE_3_IP_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_3_IP_ADDRESS_PRODUCTION)-1);
  memcpy(delegates[2].team,"X-Network Team",14);
  memcpy(delegates[2].shared_delegate_status,"solo",4);
  memcpy(delegates[2].delegate_fee,"0",sizeof(char));
  memcpy(delegates[2].server_specs,"",0);
  memcpy(delegates[2].block_verifier_score,"0",sizeof(char));
  memcpy(delegates[2].online_status,"false",5);
  memcpy(delegates[2].block_verifier_total_rounds,"0",sizeof(char));
  memcpy(delegates[2].block_verifier_online_total_rounds,"0",sizeof(char));
  memcpy(delegates[2].block_verifier_online_percentage,"0",sizeof(char));
  memcpy(delegates[2].block_producer_total_rounds,"0",sizeof(char));
  memcpy(delegates[2].block_producer_block_heights,"0",sizeof(char));
  memcpy(delegates[2].public_key,NETWORK_DATA_NODE_3_PUBLIC_KEY_PRODUCTION,VRF_PUBLIC_KEY_LENGTH);

  memcpy(delegates[3].public_address,NETWORK_DATA_NODE_4_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH);
  memcpy(delegates[3].total_vote_count,"15",2);
  memcpy(delegates[3].IP_address,NETWORK_DATA_NODE_4_IP_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_4_IP_ADDRESS_PRODUCTION)-1);
  memcpy(delegates[3].delegate_name,"europe3_xcash_foundation",24);
  memcpy(delegates[3].about,"Official X-Network node",23);
  memcpy(delegates[3].website,NETWORK_DATA_NODE_4_IP_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_4_IP_ADDRESS_PRODUCTION)-1);
  memcpy(delegates[3].team,"X-Network Team",14);
  memcpy(delegates[3].shared_delegate_status,"solo",4);
  memcpy(delegates[3].delegate_fee,"0",sizeof(char));
  memcpy(delegates[3].server_specs,"",0);
  memcpy(delegates[3].block_verifier_score,"0",sizeof(char));
  memcpy(delegates[3].online_status,"false",5);
  memcpy(delegates[3].block_verifier_total_rounds,"0",sizeof(char));
  memcpy(delegates[3].block_verifier_online_total_rounds,"0",sizeof(char));
  memcpy(delegates[3].block_verifier_online_percentage,"0",sizeof(char));
  memcpy(delegates[3].block_producer_total_rounds,"0",sizeof(char));
  memcpy(delegates[3].block_producer_block_heights,"0",sizeof(char));
  memcpy(delegates[3].public_key,NETWORK_DATA_NODE_4_PUBLIC_KEY_PRODUCTION,VRF_PUBLIC_KEY_LENGTH);

  memcpy(delegates[4].public_address,NETWORK_DATA_NODE_5_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH);
  memcpy(delegates[4].total_vote_count,"1000",4);
  memcpy(delegates[4].IP_address,NETWORK_DATA_NODE_5_IP_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_5_IP_ADDRESS_PRODUCTION)-1);
  memcpy(delegates[4].delegate_name,"asia1_xcash_foundation",22);
  memcpy(delegates[4].about,"Official X-Network node",23);
  memcpy(delegates[4].website,NETWORK_DATA_NODE_5_IP_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_5_IP_ADDRESS_PRODUCTION)-1);
  memcpy(delegates[4].team,"X-Network Team",14);
  memcpy(delegates[4].shared_delegate_status,"solo",4);
  memcpy(delegates[4].delegate_fee,"0",sizeof(char));
  memcpy(delegates[4].server_specs,"",0);
  memcpy(delegates[4].block_verifier_score,"0",sizeof(char));
  memcpy(delegates[4].online_status,"false",5);
  memcpy(delegates[4].block_verifier_total_rounds,"0",sizeof(char));
  memcpy(delegates[4].block_verifier_online_total_rounds,"0",sizeof(char));
  memcpy(delegates[4].block_verifier_online_percentage,"0",sizeof(char));
  memcpy(delegates[4].block_producer_total_rounds,"0",sizeof(char));
  memcpy(delegates[4].block_producer_block_heights,"0",sizeof(char));
  memcpy(delegates[4].public_key,NETWORK_DATA_NODE_5_PUBLIC_KEY_PRODUCTION,VRF_PUBLIC_KEY_LENGTH);

  memcpy(delegates[5].public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH);
  memcpy(delegates[5].total_vote_count,"5000",4);
  memcpy(delegates[5].IP_address,NETWORK_DATA_NODE_1_IP_ADDRESS,sizeof(NETWORK_DATA_NODE_1_IP_ADDRESS)-1);
  memcpy(delegates[5].delegate_name,"delegate_name_1",15);
  memcpy(delegates[5].about,"Official X-Network node",23);
  memcpy(delegates[5].website,"",0);
  memcpy(delegates[5].team,"X-Network Team",14);
  memcpy(delegates[5].shared_delegate_status,"solo",4);
  memcpy(delegates[5].delegate_fee,"0",sizeof(char));
  memcpy(delegates[5].server_specs,"",0);
  memcpy(delegates[5].block_verifier_score,"0",sizeof(char));
  memcpy(delegates[5].online_status,"false",5);
  memcpy(delegates[5].block_verifier_total_rounds,"0",sizeof(char));
  memcpy(delegates[5].block_verifier_online_total_rounds,"0",sizeof(char));
  memcpy(delegates[5].block_verifier_online_percentage,"0",sizeof(char));
  memcpy(delegates[5].block_producer_total_rounds,"0",sizeof(char));
  memcpy(delegates[5].block_producer_block_heights,"0",sizeof(char));
  memcpy(delegates[5].public_key,TEST_WALLET_PUBLIC_KEY_1,VRF_PUBLIC_KEY_LENGTH);

  memcpy(delegates[6].public_address,NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH);
  memcpy(delegates[6].total_vote_count,"100",3);
  memcpy(delegates[6].IP_address,NETWORK_DATA_NODE_2_IP_ADDRESS,sizeof(NETWORK_DATA_NODE_2_IP_ADDRESS)-1);
  memcpy(delegates[6].delegate_name,"delegate_name_2",15);
  memcpy(delegates[6].about,"Official X-Network node",23);
  memcpy(delegates[6].website,"",0);
  memcpy(delegates[6].team,"X-Network Team",14);
  memcpy(delegates[6].shared_delegate_status,"solo",4);
  memcpy(delegates[6].delegate_fee,"0",sizeof(char));
  memcpy(delegates[6].server_specs,"",0);
  memcpy(delegates[6].block_verifier_score,"0",sizeof(char));
  memcpy(delegates[6].online_status,"false",5);
  memcpy(delegates[6].block_verifier_total_rounds,"0",sizeof(char));
  memcpy(delegates[6].block_verifier_online_total_rounds,"0",sizeof(char));
  memcpy(delegates[6].block_verifier_online_percentage,"0",sizeof(char));
  memcpy(delegates[6].block_producer_total_rounds,"0",sizeof(char));
  memcpy(delegates[6].block_producer_block_heights,"0",sizeof(char));
  memcpy(delegates[6].public_key,TEST_WALLET_PUBLIC_KEY_2,VRF_PUBLIC_KEY_LENGTH);

  memcpy(delegates[7].public_address,NETWORK_DATA_NODE_3_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH);
  memcpy(delegates[7].total_vote_count,"50",2);
  memcpy(delegates[7].IP_address,NETWORK_DATA_NODE_3_IP_ADDRESS,sizeof(NETWORK_DATA_NODE_3_IP_ADDRESS)-1);
  memcpy(delegates[7].delegate_name,"delegate_name_3",15);
  memcpy(delegates[7].about,"Official X-Network node",23);
  memcpy(delegates[7].website,"",0);
  memcpy(delegates[7].team,"X-Network Team",14);
  memcpy(delegates[7].shared_delegate_status,"solo",4);
  memcpy(delegates[7].delegate_fee,"0",sizeof(char));
  memcpy(delegates[7].server_specs,"",0);
  memcpy(delegates[7].block_verifier_score,"0",sizeof(char));
  memcpy(delegates[7].online_status,"false",5);
  memcpy(delegates[7].block_verifier_total_rounds,"0",sizeof(char));
  memcpy(delegates[7].block_verifier_online_total_rounds,"0",sizeof(char));
  memcpy(delegates[7].block_verifier_online_percentage,"0",sizeof(char));
  memcpy(delegates[7].block_producer_total_rounds,"0",sizeof(char));
  memcpy(delegates[7].block_producer_block_heights,"0",sizeof(char));
  memcpy(delegates[7].public_key,TEST_WALLET_PUBLIC_KEY_3,VRF_PUBLIC_KEY_LENGTH);

  memcpy(delegates[8].public_address,NETWORK_DATA_NODE_4_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH);
  memcpy(delegates[8].total_vote_count,"55",2);
  memcpy(delegates[8].IP_address,NETWORK_DATA_NODE_4_IP_ADDRESS,sizeof(NETWORK_DATA_NODE_4_IP_ADDRESS)-1);
  memcpy(delegates[8].delegate_name,"delegate_name_4",15);
  memcpy(delegates[8].about,"Official X-Network node",23);
  memcpy(delegates[8].website,"",0);
  memcpy(delegates[8].team,"X-Network Team",14);
  memcpy(delegates[8].shared_delegate_status,"solo",4);
  memcpy(delegates[8].delegate_fee,"0",sizeof(char));
  memcpy(delegates[8].server_specs,"",0);
  memcpy(delegates[8].block_verifier_score,"0",sizeof(char));
  memcpy(delegates[8].online_status,"false",5);
  memcpy(delegates[8].block_verifier_total_rounds,"0",sizeof(char));
  memcpy(delegates[8].block_verifier_online_total_rounds,"0",sizeof(char));
  memcpy(delegates[8].block_verifier_online_percentage,"0",sizeof(char));
  memcpy(delegates[8].block_producer_total_rounds,"0",sizeof(char));
  memcpy(delegates[8].block_producer_block_heights,"0",sizeof(char));
  memcpy(delegates[8].public_key,TEST_WALLET_PUBLIC_KEY_4,VRF_PUBLIC_KEY_LENGTH);

  memcpy(delegates[9].public_address,NETWORK_DATA_NODE_5_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH);
  memcpy(delegates[9].total_vote_count,"10000",5);
  memcpy(delegates[9].IP_address,NETWORK_DATA_NODE_5_IP_ADDRESS,sizeof(NETWORK_DATA_NODE_5_IP_ADDRESS)-1);
  memcpy(delegates[9].delegate_name,"delegate_name_5",15);
  memcpy(delegates[9].about,"Official X-Network node",23);
  memcpy(delegates[9].website,"",0);
  memcpy(delegates[9].team,"X-Network Team",14);
  memcpy(delegates[9].shared_delegate_status,"solo",4);
  memcpy(delegates[9].delegate_fee,"0",sizeof(char));
  memcpy(delegates[9].server_specs,"",0);
  memcpy(delegates[9].block_verifier_score,"0",sizeof(char));
  memcpy(delegates[9].online_status,"false",5);
  memcpy(delegates[9].block_verifier_total_rounds,"0",sizeof(char));
  memcpy(delegates[9].block_verifier_online_total_rounds,"0",sizeof(char));
  memcpy(delegates[9].block_verifier_online_percentage,"0",sizeof(char));
  memcpy(delegates[9].block_producer_total_rounds,"0",sizeof(char));
  memcpy(delegates[9].block_producer_block_heights,"0",sizeof(char));
  memcpy(delegates[9].public_key,TEST_WALLET_PUBLIC_KEY_5,VRF_PUBLIC_KEY_LENGTH);

  // copy the reserve proofs
  memcpy(reserve_proofs[0],"RESERVE_PROOF_TEST_DATA_2",25);
  memcpy(reserve_proofs[1],"RESERVE_PROOF_TEST_DATA_4",25);
  memcpy(reserve_proofs[2],"RESERVE_PROOF_TEST_DATA_1",25);
  memcpy(reserve_proofs[3],"RESERVE_PROOF_TEST_DATA_3",25);
  memcpy(reserve_proofs[4],"RESERVE_PROOF_TEST_DATA_5",25);
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\norganize functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,ORGANIZE_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
 
  // test for organize_invalid_reserve_proofs_settings
  qsort(reserve_proofs,sizeof(reserve_proofs)/sizeof(reserve_proofs[0]),sizeof(char*),organize_invalid_reserve_proofs_settings);
  if (strncmp(reserve_proofs[0],"RESERVE_PROOF_TEST_DATA_5",BUFFER_SIZE_RESERVE_PROOF) == 0 && strncmp(reserve_proofs[1],"RESERVE_PROOF_TEST_DATA_4",BUFFER_SIZE_RESERVE_PROOF) == 0 && strncmp(reserve_proofs[2],"RESERVE_PROOF_TEST_DATA_3",BUFFER_SIZE_RESERVE_PROOF) == 0 && strncmp(reserve_proofs[3],"RESERVE_PROOF_TEST_DATA_2",BUFFER_SIZE_RESERVE_PROOF) == 0 && strncmp(reserve_proofs[4],"RESERVE_PROOF_TEST_DATA_1",BUFFER_SIZE_RESERVE_PROOF) == 0)
  {   
    color_print("PASSED! Test for organize_invalid_reserve_proofs_settings","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for organize_invalid_reserve_proofs_settings","red");
  }

  // test for organize_delegates_settings
  production_settings = 1;
  qsort(delegates,MAXIMUM_AMOUNT_OF_DELEGATES,sizeof(struct delegates),organize_delegates_settings);
  if (strncmp(delegates[0].public_address,NETWORK_DATA_NODE_5_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 && strncmp(delegates[1].public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 && strncmp(delegates[2].public_address,NETWORK_DATA_NODE_5_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0 && strncmp(delegates[3].public_address,NETWORK_DATA_NODE_2_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0 && strncmp(delegates[4].public_address,NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 && strncmp(delegates[5].public_address,NETWORK_DATA_NODE_4_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 && strncmp(delegates[6].public_address,NETWORK_DATA_NODE_3_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 && strncmp(delegates[7].public_address,NETWORK_DATA_NODE_3_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0 && strncmp(delegates[8].public_address,NETWORK_DATA_NODE_4_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0 && strncmp(delegates[9].public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0)
  {   
    color_print("PASSED! Test for organize_delegates_settings","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for organize_delegates_settings","red");
  }

  delete_database(database_name);
  production_settings = 0;

  // reset the delegates struct
  RESET_DELEGATES_STRUCT;
  INITIALIZE_DATABASE_DATA(1);

  // test for organize_delegates_settings
  if (organize_delegates(delegates) != 0 && strncmp(delegates[0].public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 && strncmp(delegates[1].public_address,NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 && strncmp(delegates[2].public_address,NETWORK_DATA_NODE_3_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 && strncmp(delegates[3].public_address,NETWORK_DATA_NODE_4_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 && strncmp(delegates[4].public_address,NETWORK_DATA_NODE_5_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0)
  {   
    color_print("PASSED! Test for organize_delegates","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for organize_delegates","red");
  }
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);
  RESET_ERROR_MESSAGES;

  // reset the reserve proofs
  for (count = 0; count < 5; count++)
  {
    pointer_reset(reserve_proofs[count]);
  }

  // reset the delegates struct
  for (count = 0; count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    pointer_reset(delegates[count].public_address);
    pointer_reset(delegates[count].total_vote_count);
    pointer_reset(delegates[count].IP_address);
    pointer_reset(delegates[count].delegate_name);
    pointer_reset(delegates[count].about);
    pointer_reset(delegates[count].website);
    pointer_reset(delegates[count].team);
    pointer_reset(delegates[count].shared_delegate_status);
    pointer_reset(delegates[count].delegate_fee);
    pointer_reset(delegates[count].server_specs);
    pointer_reset(delegates[count].block_verifier_score);
    pointer_reset(delegates[count].online_status);
    pointer_reset(delegates[count].block_verifier_total_rounds);
    pointer_reset(delegates[count].block_verifier_online_total_rounds);
    pointer_reset(delegates[count].block_verifier_online_percentage);
    pointer_reset(delegates[count].block_producer_total_rounds);
    pointer_reset(delegates[count].block_producer_block_heights);
    pointer_reset(delegates[count].public_key);
  }

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\norganize functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,ORGANIZE_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;
}