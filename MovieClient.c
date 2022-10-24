#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    /////////////////// 
    char nombrePelicula[40], mensajeProtocolo[40], opcionConsulta[40];
    char resultadoBusqueda[40];
    char buffer[256];
    ///////////////////
    char *comma, *position;
    char number[40], resultadoFinal[40], ch;
    ///////////////////
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    //////////////////////////////////////////////

    printf("Nombre de la pelicula a buscar: ");
    scanf(" %s", nombrePelicula);
    printf("\nOpciones de busqueda:\n- Genre            1\n- Lead Studio      2\n- Audience score   3\n- Profitability    4\n- Rotten Tomatoes  5\n- Worldwide Gross  6\n- Year             7\nElija la opcion: ");
    scanf(" %s", opcionConsulta);

    strcat(mensajeProtocolo, opcionConsulta);
    strcat(mensajeProtocolo, ",");
    strcat(mensajeProtocolo, nombrePelicula);

    // ENVIO DE DATOS AL SV
    bzero(buffer,256);
    strcpy(buffer, mensajeProtocolo);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    // Server recibe el mensaje y lo procesa, para luego mandar el resultado:
    ////////////////////////////////////////////////

    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    strcpy(resultadoBusqueda,buffer);

    ///////////////////////////////////////////////

    comma = strchr(resultadoBusqueda, ',');
    position = resultadoBusqueda;
    while (comma)
    {
        int i = 0;

        while(position < comma)
        {
            number[i] = *position;
            i++;
            position++;
        }
        number[i]= '\0';
        strcpy(opcionConsulta, number);
        position++;
        comma = strchr (position, ',');
    }
    strcpy(resultadoFinal, position);

    ///////////////////////////////////////////////
    printf("El resultado de la busqueda fue: \n");
    if(atoi(opcionConsulta)==1)
    {
        printf("EXITOSO: ");
        printf("%s\n", resultadoFinal);
    }
    else
    {
        printf("FALLIDO: No se encontró una pelicula con el nombre especificado.\n");
    }
    ///////////////////////////////////////////////

    close(sockfd);

    return 0;
}