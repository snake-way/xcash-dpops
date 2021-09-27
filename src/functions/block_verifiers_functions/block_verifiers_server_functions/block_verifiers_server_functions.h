#ifndef BLOCK_VERIFIERS_SERVER_FUNCTIONS_H_   /* Include guard */
#define BLOCK_VERIFIERS_SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

void server_received_data_XCASH_DPOPS_test_data(const int CLIENT_SOCKET, char* MESSAGE);
void server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs(const char* MESSAGE);
void server_receive_data_socket_block_verifiers_to_network_data_nodes_block_verifiers_current_time(const int CLIENT_SOCKET);
void server_receive_data_socket_main_network_data_node_to_block_verifier_start_block(const char* MESSAGE);
void server_receive_data_socket_main_node_to_node_message_part_4(const char* MESSAGE);
void server_receive_data_socket_node_to_node_majority(const char* MESSAGE);
void server_receive_data_socket_node_to_node(const char* MESSAGE);
void server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data(const char* MESSAGE);
void server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature(const char* MESSAGE);
#endif