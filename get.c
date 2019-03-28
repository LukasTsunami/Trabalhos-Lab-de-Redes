#include "libs.h"

#define BUFFSIZ 1024
#define FILE_PERMISSIONS 0755

void eraseAllPreviousSocketData(struct addrinfo *socket_addr){
    const int which_byte_to_set_data=0;
    memset(socket_addr, which_byte_to_set_data, sizeof(struct addrinfo));
}

void setIfFamilyAddressIsIpv4OrIpv6(struct addrinfo *socket_addr, char protocol[]){
    if(strcmp(protocol,"IPV4")==0){
        socket_addr->ai_family = AF_INET;
        return;
    }

    if(strcmp(protocol,"IPV6")==0){
        socket_addr->ai_family = AF_INET6;
        return;
    }

    if(strcmp(protocol,"BOTH")==0){
        socket_addr->ai_family = AF_UNSPEC;
        return;
    }

    socket_addr->ai_family = AF_UNSPEC;
}

void setThatCallerHandlesOnlyTCP(struct addrinfo *socket_addr){
    socket_addr->ai_socktype = SOCK_STREAM;
}

void configureSocket(struct addrinfo *address_info_configuration_model){
    const int ACCEPT_ANY_PROTOCOL=0;
    const int NO_EXTRA_BEHAVIORS=0;

    eraseAllPreviousSocketData(address_info_configuration_model);
    setIfFamilyAddressIsIpv4OrIpv6(address_info_configuration_model, "BOTH");
	  setThatCallerHandlesOnlyTCP(address_info_configuration_model);
    address_info_configuration_model->ai_flags = NO_EXTRA_BEHAVIORS;
    address_info_configuration_model->ai_protocol = ACCEPT_ANY_PROTOCOL;
}

char* copyDynamicString(char* output_str, char* input_str)
{
    output_str = malloc(sizeof(char) * strlen(input_str));
    if (output_str == NULL) {
        printf("Not enough memory to copy dynamic string");
        const int generic_error_code = 1;
        exit(generic_error_code);
    }
    strcpy(output_str, input_str);
    return output_str;
}

int calculateNewBufferSize(){
    return BUFFSIZ - 1;
}

char* estabilishDataToSend(char *data_to_send, char *SOURCE_URI){
    data_to_send = malloc(sizeof(char) * (strlen(SOURCE_URI) + strlen("GET  HTTP/1.0\r\n\r\n")));
    strcpy(data_to_send, "GET ");
    strcat(data_to_send, SOURCE_URI);
    strcat(data_to_send, " HTTP/1.0\r\n\r\n");
    return data_to_send;
}

int main(int argc, char *argv[])
{
    int socket_identificator, on = 1, destination_file_identificator, number_of_received_arguments=argc, answer_status_code;
    const int NUMBER_OF_EXPECTED_ARGUMENTS=5, THROWS_ERROR_CODE=1, UNCONNECTED_SOCKET = -1, PROGRAM_SUCCESSFULL_EXECUTED_CODE=0;
	char buffer[BUFFSIZ],*data_to_send;
    struct addrinfo address_info_configuration_model;
    struct addrinfo *target_gotten_address_informations, *pointer_to_walk_along_address_infos;
    struct sockaddr_in *information_core;
    char *current_source_file = copyDynamicString(current_source_file, argv[0]);
    char *hostname = copyDynamicString(hostname, argv[1]);
    char *port = copyDynamicString(port, argv[2]);
    char *SOURCE_URI = copyDynamicString(SOURCE_URI, argv[3]);
    char *destination_file = copyDynamicString(destination_file, argv[4]);

	if(number_of_received_arguments < NUMBER_OF_EXPECTED_ARGUMENTS)
	{
		fprintf(stderr, "Usage: %s <hostname> <port> <path> <file>\n", current_source_file);
		exit(THROWS_ERROR_CODE);
    }

    configureSocket(&address_info_configuration_model);

    if((answer_status_code = getaddrinfo(hostname, port, &address_info_configuration_model, &target_gotten_address_informations)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(answer_status_code));
    }

    for(pointer_to_walk_along_address_infos = target_gotten_address_informations; pointer_to_walk_along_address_infos != NULL; pointer_to_walk_along_address_infos = pointer_to_walk_along_address_infos->ai_next)
    {
        information_core = (struct sockaddr_in *) pointer_to_walk_along_address_infos->ai_addr;
        printf("\t------------------------\n");
        printf("\tAddress: %s\n", inet_ntoa((struct in_addr) information_core->sin_addr));
        socket_identificator = socket(pointer_to_walk_along_address_infos->ai_family, pointer_to_walk_along_address_infos->ai_socktype, IPPROTO_TCP);
        setsockopt(socket_identificator, IPPROTO_TCP, TCP_NODELAY, (const char *) &on, sizeof(int));

        if(socket_identificator == UNCONNECTED_SOCKET)
            continue;
        if(connect(socket_identificator, pointer_to_walk_along_address_infos->ai_addr, pointer_to_walk_along_address_infos->ai_addrlen) != -1)
            break;
        close(socket_identificator);
    }

    if (pointer_to_walk_along_address_infos == NULL)
    {
        fprintf(stderr, "It was not possible to estabilish connection\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(target_gotten_address_informations);
    data_to_send = estabilishDataToSend(data_to_send, SOURCE_URI);
    write(socket_identificator, data_to_send, strlen(data_to_send));
	bzero(buffer, BUFFSIZ);
    destination_file_identificator = open(destination_file, O_WRONLY | O_APPEND | O_CREAT, FILE_PERMISSIONS);

	while(read(socket_identificator, buffer, calculateNewBufferSize()) != 0)
	{
		write(destination_file_identificator, buffer, strlen(buffer));
		bzero(buffer, BUFFSIZ);
	}

	shutdown(socket_identificator, SHUT_RDWR);
	close(socket_identificator);

	return(PROGRAM_SUCCESSFULL_EXECUTED_CODE);
}
