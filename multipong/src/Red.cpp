#include "Red.h"
#include <iostream>
#include <stdlib.h>     /* atoi */
#include <cstring>

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

    udpsock=SDLNet_UDP_Open(port+1);
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

int Red::clienteRecibeDatos(char* msg){
    if(recibe(&udpsock,msg)>=0){
        //Recibimos el numero de jugador
        return 0;
    }else{
        return -1;
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



int Red::servidorEnviaDatosATodos(char* msg){

    int len = std::strlen(msg)+1;

    for(int i=0; i<connectedClients; i++){
        //std::cout << "ENVIO : " << msg << std::endl;
        int result=SDLNet_TCP_Send(clientes[i],msg,len);
        if(result<len) {
            std::cout << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
            // It may be good to disconnect sock because it is likely invalid now.
            //return -1;
        }
    }

    return 0;
}

int Red::clienteEnviaDireccion(int direccion){

    int numsent;

    sprintf((char*)udppacket->data,"%d",direccion);
    //int len = strlen(buffer)+1;
    std::cout << "Cliente envia direccion " << (char*)udppacket->data << std::endl;

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

int Red::servidorRecibeDatos(std::vector<Pala*> palas, float deltaTime, int maxClients){
    int direccion;
    int cliente;

    if (SDLNet_UDP_Recv(udpsock, udppacket)) {
        bool flag = false;
        for (int i=0;i < palas.size(); i++) {
            if (palas[i]->ipaddress.host == udppacket->address.host && palas[i]->ipaddress.port == udppacket->address.port) {
                sscanf((char*)udppacket->data,"%d",&direccion);
                std::cout << "Recibimos datos en servidor: " << udppacket->data << std::endl;
                palas[i]->Update(deltaTime,(Direcction)direccion);
                flag = true;
                break;
            }
        }
        if (flag == false && palas.size()<maxClients) {
            Pala* p = new Pala();
            p->Init(palas.size());
            p->SetIP(udppacket->address);
            palas.push_back(p);
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
