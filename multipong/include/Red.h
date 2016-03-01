#ifndef RED_H
#define RED_H
#include "SDL_net.h"
#include <vector>
#include <string.h>
#include <string>
#include <stdio.h>
#include "Constants.h"
#include "Pala.h"
#include "Bola.h"

class Red
{
    public:
        Red();
        virtual ~Red();

        int inicia();
        int iniciaServidor(int port);

        //int esperaClientes(int nclientes, int numeroPlayers, int playerNumInicial);
        int iniciaCliente(std::string host, int port);

        int clienteRecibeNumeros(int *numeroJugadores, int *jugador);
        int clienteRecibeDatos(std::vector<Pala*>* palas, Bola* bola, SDL_Renderer* glRenderer);
        int clienteEnviaDireccion(int direccion);

        int servidorEnviaNumeros(UDPsocket * cliente, int numeroJugadores, int numeroCliente);
        int servidorEnviaDatosATodos(std::vector<Pala*>* palas,char* msg);
        int servidorRecibeDatos(std::vector<Pala*>* palas, float deltaTime, int maxClients, SDL_Renderer* glRenderer);

    protected:
        char buffer[MAX_BUFFER];
        UDPsocket udpsock;
        UDPpacket* udppacket;
        //TCPsocket tcpsock;
        int connectedClients;
        TCPsocket clientes[MAX_PLAYERS];

        int envia(UDPsocket* cliente, char* msg);
        int recibe(UDPsocket* cliente, char * msg);
    private:

        IPaddress serverIP;
};

#endif // RED_H
