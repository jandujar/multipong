#include "Red.h"
#include <iostream>
#include <stdlib.h>     /* atoi */
#include <cstring>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


#define MAX_LEN 512

Red::Red()
{
    //ctor
}

Red::~Red()
{
    //dtor
    SDLNet_FreePacket(udppacket);
}


int Red::inicia(){
    /* Inicializamos la Red */
    if(SDLNet_Init()==-1) {
        std::cout << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
        return -2;
    }else{
        std::cout << "Red Inicializada" << std::endl;
    }

    return 0;
}

int Red::iniciaServidor(int port){
    // create a listening TCP socket on port 9999 (server)
    IPaddress ip;

    std::cout << "Creando servidor : " << std::endl;

    //if(SDLNet_ResolveHost(&ip,NULL,port)==-1) {
    //    std::cout << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
    //    return -1;
    //}
    udpsock=SDLNet_UDP_Open(port);
    if(!udpsock) {
        std::cout << "SDLNet_UDP_Open: " << SDLNet_GetError() << std::endl;
        return -2;
    }

    if (!(udppacket = SDLNet_AllocPacket(PACKET_SIZE))) {
        std::cout << "SDLNet_AllocPacket:" << SDLNet_GetError() << std::endl;
    }

    std::cout << "Servidor creado : " << std::endl;

    return 0;
}
/*
int Red::esperaClientes(int nclientes, int numeroPlayers, int playerNumInicial){
    connectedClients = 0;
    int playerNum = playerNumInicial;

    std::cout << "Esperando cliente : " << std::endl;

    //Esperar conexion de usuarios
    while(connectedClients<nclientes){
        clientes[connectedClients]=SDLNet_TCP_Accept(tcpsock);
        if(clientes[connectedClients]) {
            //Enviamos el número de cliente
            std::cout << "Cliente conectado :" << connectedClients << std::endl;

            servidorEnviaNumeros(&clientes[connectedClients],numeroPlayers, playerNum);
            playerNum++;
            connectedClients++;
        }
    }

    return 0;
}
*/

int Red::iniciaCliente(std::string host, int port){
    // connect to localhost at port 9999 using TCP (client)
    if(SDLNet_ResolveHost(&serverIP,host.c_str(),port)==-1) {
        std::cout << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        return -1;
    }

    srand (time(NULL));
    int puerto = rand() % 100 + 1;

    std::cout << "Puerto: " << port+puerto << std::endl;
    udpsock=SDLNet_UDP_Open(port+puerto);
    if(!udpsock) {
        std::cout << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        return -1;
    }

    if (!(udppacket = SDLNet_AllocPacket(PACKET_SIZE))) {
        std::cout << "SDLNet_AllocPacket:" << SDLNet_GetError() << std::endl;
        return -1;
    }

    udppacket->address.host = serverIP.host;
    udppacket->address.port = serverIP.port;


    //Recibir numero cliente

    return 0;
}

int Red::envia(UDPsocket* cliente, char* msg){
    /*
    int len = strlen(msg)+1;
    int result=SDLNet_TCP_Send(*cliente,msg,len);
    if(result<len) {
        std::cout << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        // It may be good to disconnect sock because it is likely invalid now.
        return -1;
    }

    //std::cout << "Envio Mensaje " << msg << std::endl;
    */

    return 0;
}

int Red::recibe(UDPsocket* servidor, char * msg){
    /*
    int result;

    //std::cout << "Esperando Respuesta : " << std::endl;
    result=SDLNet_TCP_Recv(*servidor,msg,MAX_LEN);
    if(result<=0) {
        // An error may have occured, but sometimes you can just ignore it
        // It may be good to disconnect sock because it is likely invalid now.
        return -1;
    }else{
        //std::cout << "Mensaje recibido : " << msg << std::endl;
    }
    */
    return 0;
}

int Red::clienteRecibeDatos(std::vector<Pala*>* palas, Bola* bola, SDL_Renderer* glRenderer, Marcador* marcador1, Marcador* marcador2){
    char buffer2[MAX_BUFFER];
    int numplayers;
    int tmp = 0;
    int tmpx,tmpy;
    int score1,score2;
    if (SDLNet_UDP_Recv(udpsock, udppacket)) {
        //printf("Datos leidos %s\n",(char*)udppacket->data);
        sscanf((char*)udppacket->data,"%d %d %d",&numplayers,&bola->getRect()->x,&bola->getRect()->y);
        while(tmp<numplayers){
            if(tmp >= (*palas).size() ){
                Pala* p = new Pala();
                p->Init((*palas).size(),glRenderer);
                IPaddress fakeaddress;
                fakeaddress.host = 0;
                fakeaddress.port = 0;
                p->SetIP(fakeaddress);
                (*palas).push_back(p);
            }

            tmp++;
        }
        if(numplayers == 2){
            //printf("Datos por leer %s",(char*)udppacket->data);
            sscanf((char*)udppacket->data,"%d %d %d %d %d %d %d %d %d %d %d\n",&numplayers,&score1,&score2,&bola->getRect()->x,&bola->getRect()->y,&(*palas)[0]->getRect()->x,&(*palas)[0]->getRect()->y, &(*palas)[0]->direccion_pala,&(*palas)[1]->getRect()->x,&(*palas)[1]->getRect()->y, &(*palas)[1]->direccion_pala);
            marcador1->setScore(score1);
            marcador2->setScore(score2);
        }else if(numplayers == 3){
            //printf("Datos por leer %s",(char*)udppacket->data);
            sscanf((char*)udppacket->data,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",&numplayers,&score1,&score2,&bola->getRect()->x,&bola->getRect()->y,
                   &(*palas)[0]->getRect()->x,&(*palas)[0]->getRect()->y, &(*palas)[0]->direccion_pala,
                   &(*palas)[1]->getRect()->x,&(*palas)[1]->getRect()->y, &(*palas)[1]->direccion_pala,
                   &(*palas)[2]->getRect()->x,&(*palas)[2]->getRect()->y, &(*palas)[2]->direccion_pala
                   );
            marcador1->setScore(score1);
            marcador2->setScore(score2);

        }else if(numplayers == 4){
            //printf("Datos por leer %s",(char*)udppacket->data);
            sscanf((char*)udppacket->data,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",&numplayers,&score1,&score2,&bola->getRect()->x,&bola->getRect()->y,
                   &(*palas)[0]->getRect()->x,&(*palas)[0]->getRect()->y, &(*palas)[0]->direccion_pala,
                   &(*palas)[1]->getRect()->x,&(*palas)[1]->getRect()->y, &(*palas)[1]->direccion_pala,
                   &(*palas)[2]->getRect()->x,&(*palas)[2]->getRect()->y, &(*palas)[2]->direccion_pala,
                   &(*palas)[3]->getRect()->x,&(*palas)[3]->getRect()->y, &(*palas)[3]->direccion_pala
                   );
           marcador1->setScore(score1);
           marcador2->setScore(score2);
        }


        /*
        bool flag = false;
        for (int i=0;i < (*palas).size(); i++) {

            if ((*palas)[i]->ipaddress.host == udppacket->address.host && (*palas)[i]->ipaddress.port == udppacket->address.port) {
                sscanf((char*)udppacket->data,"%d",&direccion);
                //std::cout << "Recibimos datos en servidor: " << udppacket->data << std::endl;
                (*palas)[i]->Update(deltaTime,(Direcction)direccion);
                flag = true;
                break;
            }
        }
        if (flag == false && (*palas).size()<maxClients) {

            Pala* p = new Pala();
            p->Init((*palas).size());
            p->SetIP(udppacket->address);
            (*palas).push_back(p);
        }
        */
    }
}

int Red::clienteRecibeNumeros(int *numeroJugadores, int *jugador){
    if(recibe(&udpsock,buffer)>=0){
        sscanf(buffer,"%d %d",numeroJugadores, jugador);
        std::cout << "Numero cliente recibido "<< *jugador << std::endl;
        //Recibimos el numero de jugador
        return 0;
    }else{
        return -1;
    }
}

int Red::servidorEnviaNumeros(UDPsocket * cliente, int numeroJugadores, int numeroCliente){
    /*
    sprintf(buffer,"%d %d",numeroJugadores, numeroCliente);

    int len = std::strlen(buffer)+1;
    int result=SDLNet_TCP_Send(*cliente,buffer,len);
    if(result<len) {
        printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        // It may be good to disconnect sock because it is likely invalid now.
        return -1;
    }

    std::cout << "Envio numero cliente " << buffer << std::endl;
    */

    return 0;
}



int Red::servidorEnviaDatosATodos(std::vector<Pala*>* palas, char* msg){

    int numsent;
    for (int i=0;i < (*palas).size(); i++) {
        if((*palas)[i]->ipaddress.host!=0 && (*palas)[i]->ipaddress.port!=0){
            sprintf((char*)udppacket->data,"%s",msg);
            udppacket->len = strlen((char*)udppacket->data);
            //int len = strlen(buffer)+1;
            //std::cout << "Servidor envia DATOS " << (char*)udppacket->data << std::endl;
            udppacket->address.host = (*palas)[i]->ipaddress.host;
            udppacket->address.port = (*palas)[i]->ipaddress.port;

            numsent=SDLNet_UDP_Send(udpsock, -1, udppacket);
            if(!numsent) {
                std::cout << "SDLNet_UDP_Send: " << SDLNet_GetError() << std::endl;
                // do something because we failed to send
                // this may just be because no addresses are bound to the channel...
            }
        }
    }

    return 0;
}

int Red::clienteEnviaDireccion(int direccion){

    int numsent;

    sprintf((char*)udppacket->data,"%d",direccion);
    udppacket->len = strlen((char*)udppacket->data);
    //int len = strlen(buffer)+1;
    //std::cout << "Cliente envia direccion " << (char*)udppacket->data << std::endl;

    numsent=SDLNet_UDP_Send(udpsock, -1, udppacket);
    if(!numsent) {
        std::cout << "SDLNet_UDP_Send: " << SDLNet_GetError() << std::endl;
        // do something because we failed to send
        // this may just be because no addresses are bound to the channel...
    }

    /*
    sprintf(buffer,"%d %d",cliente, direccion);
    std::cout << "Cliente envia direccion " << buffer << std::endl;
    int len = strlen(buffer)+1;
    int result=SDLNet_TCP_Send(tcpsock,buffer,len);
    if(result<len) {
        std::cout << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        // It may be good to disconnect sock because it is likely invalid now.
        //return -1;
    }

    //std::cout << "Envio Mensaje " << msg << std::endl;
    */

    return 0;
}

int Red::servidorRecibeDatos(std::vector<Pala*>* palas, float deltaTime, int maxClients, SDL_Renderer* glRenderer){
    int direccion;
    int cliente;

    if (SDLNet_UDP_Recv(udpsock, udppacket)) {
        /*
        char *host;
        if(!(host=(char*)SDLNet_ResolveIP((const IPaddress*)&udppacket->address))) {
            printf("SDLNet_ResolveIP: %s\n", SDLNet_GetError());
        }
        std::cout << "Servidor recibe datos " << (char*)udppacket->data << std::endl;
        printf("%s\n", (char*)udppacket->data);
        printf("\tFrom ip: %s\n", host);
        printf("\tFrom port: %d\n", udppacket->address.port);
        */
        bool flag = false;
        for (int i=0;i < (*palas).size(); i++) {
            /*if(!(host=(char*)SDLNet_ResolveIP((const IPaddress*)&(*palas)[i]->ipaddress))) {
                printf("SDLNet_ResolveIP: %s\n", SDLNet_GetError());
            }else{
                printf("PALA ip: %s\n", host);
            }
                */
            if ((*palas)[i]->ipaddress.host == udppacket->address.host && (*palas)[i]->ipaddress.port == udppacket->address.port) {
                sscanf((char*)udppacket->data,"%d",&direccion);
                //std::cout << "Recibimos datos en servidor: " << udppacket->data << std::endl;
                (*palas)[i]->direccion_pala = (Direcction)direccion;
                //(*palas)[i]->Update(deltaTime,(Direcction)direccion);
                flag = true;
                break;
            }
        }
        if (flag == false && (*palas).size()<maxClients) {

            Pala* p = new Pala();
            p->Init((*palas).size(),glRenderer);
            p->SetIP(udppacket->address);
            (*palas).push_back(p);
        }
    }
    /*
    for(int i=0;i<connectedClients;i++){
        if(recibe(&clientes[i],buffer)>=0){
            //Recibimos datos de un cliente
            sscanf(buffer,"%d %d",&cliente, &direccion);
            std::cout << "Recibimos datos en servidor: " << buffer << std::endl;
            palas[cliente]->Update(deltaTime,(Direcction)direccion);

        }else{
            std::cout << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        }
    }
    */

    return 0;
}
