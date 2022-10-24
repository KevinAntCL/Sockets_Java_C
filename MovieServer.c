#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    ///////////////
    char nombrePelicula[40], mensajeProtocolo[40], opcionConsulta[40];
    char *comma, *position;
    char number[40], busqueda[40], ch, resultadoBusqueda[40];
    //////////////
    FILE *fp = fopen("movies.csv", "r");
    const char s[2] = ",";
    char *token;
    int i, bandera = 0, opcionConsultaEntero;
    //////////////

    if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
    if (newsockfd < 0) 
          error("ERROR on accept");

    /////////////////////////////////////////////////////
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
     
    // RECIBIDO Y HACEMOS SPLIT POR COMAS A LO RECIBIDO.
    strcpy(mensajeProtocolo, buffer);
    comma = strchr(mensajeProtocolo, ',');
    position = mensajeProtocolo;
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
    strcpy(nombrePelicula, position);
    /////////////////////////////////////////////////////
    
    opcionConsultaEntero = atoi(opcionConsulta);
    if(fp != NULL)
    {
        char line[400];
        while(fgets(line, sizeof line, fp) != NULL)
        {
            token = strtok(line, s);
            for(i=0;i<8;i++)
            {
                if(i<7)
                {   
                    //printf("%s",token);
                    if(i==0 && strcmp(token, nombrePelicula)==0)
                    {            
                        bandera = 1;
                    }
                    if(i==opcionConsultaEntero && bandera == 1)
                    {
                        strcpy(busqueda, token);
                        strcpy(resultadoBusqueda, "1");
                        bandera = 0;
                    }
                    token = strtok(NULL,s);
                }     
            }
        }
        fclose(fp);
    }
    /////////////////////////////////////////////////////

    strcat(resultadoBusqueda, ",");
    strcat(resultadoBusqueda, busqueda);

    /////////////////////////////////////////////////////

    bzero(buffer,256);
    strcpy(buffer, resultadoBusqueda);
    n = write(newsockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");

    /////////////////////////////////////////////////////
    close(newsockfd);
    close(sockfd);

    return 0; 
}