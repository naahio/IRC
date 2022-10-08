# IRC

   The IRC protocol was developed over the last 4 years since it was
   first implemented as a means for users on a BBS to chat amongst
   themselves.
    The IRC protocol is a text-based protocol, with the simplest client
   being any socket program capable of connecting to the server.
   The IRC protocol has been developed on systems using the TCP/IP
   network protocol, although there is no requirement that this remain
   the only sphere in which it operates.
</p>
<p align="center">
<img width="586" alt="servers" src="https://user-images.githubusercontent.com/60394951/194696242-e25ec891-15d1-4884-b21a-6ea3e1a53b68.png"> 
</p>

# subject : ft_irc
    
   - the goal of the project is to create an irc server, where users (Clients) can exchange direct messages,
        and join group channels.
   - IRC server's are connected together, and has there own rules (Server to Server communication), 
        but we dont have to think about it because we are gonna be making a one server
        that is able to receive incomming connection from other Clients.

## Geting Started :
    
   > Server :
   - As what've been told before, the Server can connecte multiple Client with each other, and keep the other servers in touch,
   but we will not be handling the DSS communication.
   - Creating a server is basiclly creating a `Socket` with some specific rules :
       <details >
           <summary >socket : </summary>
                  Socket programming is a way of connecting two nodes on a network to communicate with each other.<br>
                  One socket(node) listens on a particular port at an IP, while the other socket reaches out to the other to form a connection.<br>
                  The server forms the listener socket while the client reaches out to the server.<br>
       </details> <br>
   - to be able to accept an incomming connection, our socket must have an `IP Adresse`, and an assigned `PORT`, which is one of our programme's paramaeters.
   - After having the two args, the type of the network protocol must be selected `TCP(v4 or v6)`, the rest be nothing more than a puzzle,
    all functions that we need are availble, easy to understand and maintain. <br>
    </p>
    <p align="center">
    ![diag](https://user-images.githubusercontent.com/60394951/194701310-c6ebb270-b36e-4c8d-b929-b35547102603.png)
    </p>
    
