//
// Created by meteo on 07.12.2023.
//

//TODO топология 3 бинарное дерево поиска(не сбалансированное), тип команд 2(целочисленный словарь), тип проверки доступности узлов 3(heartbit time)
#include "iostream"
#include "BinaryTree.h"
#include "MyProcess.h"
#include "windows.h"
#include "cppzmq-master/zmq.hpp"

int main(){
    zmq::context_t context{1};

    zmq::socket_t socket{context, zmq::socket_type::rep};
    socket.bind("tcp://*:5555");

    const std::string data{"World"};

    for (;;)
    {
        zmq::message_t request;

        // receive a request from client
        socket.recv(request, zmq::recv_flags::none);
        std::cout << "Received " << request.to_string() << std::endl;

        // send the reply to the client
        socket.send(zmq::buffer(data), zmq::send_flags::none);
    }

    return 0;

}