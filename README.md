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
  - With this the Server is ready to go.
    
  > Client :
   - At this point we are waiting for any connection from a Client.
   - Several IRC clients exist. You have to choose one of them as a reference(`for 1337-kh students there is the LimeChat`).
   - Using your reference client with your server must be similar to using it with any official IRC server,
      which means the replays must be displayed correctly and clearly .
   - each action in made by the Client has it's reaply/result in the server like an asnwer to the request to which could be and error or a command's result .
   - While building the replays it would be better to use the terminal as a client, it makes the follow of the commands much better,
      you can use the `nc -c` command with the server IP_Adresse and PORT : 
      </p>
       <p align="center">
         <img width="272" alt="Screen Shot 2022-10-08 at 11 37 28 AM" src="https://user-images.githubusercontent.com/60394951/194703357-26d3780a-9200-44f2-ac4b-161df108818c.png">
      </p>
   - Using the terminal make it more easy when it comes to the what the Server sends and what the Client receives
      </p>
      <img width="1448" alt="Screen Shot 2022-10-08 at 11 41 25 AM" src="https://user-images.githubusercontent.com/60394951/194704210-453b5859-9c68-4c9c-b982-c79dc4587ae9.png">   
      <p align="right">
         <img width="606" alt="Screen Shot 2022-10-08 at 11 43 09 AM" src="https://user-images.githubusercontent.com/60394951/194703941-788f1782-4517-404b-a916-3fe7e783e97c.png">
      </p>
   - And that's it, it's up to you now, using the [RFC](https://www.rfc-editor.org/rfc/rfc1459.html#section-1), you will find everything you need to know and to do, so you can create your own IRC server.<br>
   - Following the subject, there are some specific things that much be done as a mandaroty parts: <br>
      > - You must be able to : <br>
      >     ```authenticate,
      >     set a nickname, a username, join a channel,
      >     send and receive private messages using your reference client.
      > - All the messages sent from one client to a channel have to be forwarded to
            every other client that joined the channel.
      > - You must have operators and regular users.
      > - Then, you have to implement the commands that are specific to operators.
      <p align="center">
         <img width="377" alt="Screen Shot 2022-10-08 at 11 53 34 AM" src="https://user-images.githubusercontent.com/60394951/194706548-58052970-ca83-4f69-8196-9094a6e37eb1.png">
           <img width="426" alt="Screen Shot 2022-10-08 at 11 53 01 AM" src="https://user-images.githubusercontent.com/60394951/194706564-0e895e59-a079-4fd2-97c7-2345f4998e5e.png">
            <img width="353" alt="Screen Shot 2022-10-08 at 11 53 46 AM" src="https://user-images.githubusercontent.com/60394951/194706575-b40f3152-8022-497b-9c1b-98c4d2b84551.png">
      </p>
   - it may be too littel compared to the **RFC**, but there is no limit to what we can add to the project, from all the channel's mode + the use's mode and the other command that make the server more usable. 

## Executing program :

   - simply `make` the code, and run `./ircserv localhost port` (you can change the ip and the port) and use your Client to connect :
   <p align="center">
      <img width="976" alt="Screen Shot 2022-10-08 at 1 11 41 PM" src="https://user-images.githubusercontent.com/60394951/194706889-0afc2f16-024f-4753-944c-4dcfdf3ba427.png">
   </p><br>
   - And Done you can communicate with others using the commands you created .

## Bonus :

   - For the bonus part there is two tasks : <br>
      > BOT .<br>
      > FILE TRANSFER .<br>
   > BOT :
   - there is no limit nor a specific thing to do, you are free to make the bot do anything you like, from a normal commands to some bigger task.
   - Our bot main job is a user tracker, each user connect to the server will be tracked and tranfered into a player with a **LEVEL, RANK, POINTS** and more.
   - so that the more the user is active with the logtime and the number for commands used, he gain some point that level up him.
   -  some levels gives the user a specific rules in the server, or a special channel with the user's nickname, where he is it's operator, even more when a player reach the max level, he becomes a server operator.
   -  we names our littel bot /lily, a pertty name for a littel prety bot \(^_^)/.
   -  using the limeChat or any other Client, you can send `profile` command to her, she will answer with the player's data. 
   <p align="center">
      <img width="407" alt="Screen Shot 2022-10-08 at 1 30 08 PM" src="https://user-images.githubusercontent.com/60394951/194707729-d4853678-c54d-4858-8874-32f54a086cae.png">
   </p><br>

   - And finally a location or search command that gives the location of a player in the Cluster `whereis` :
   <p align="center">
      <img width="333" alt="Screen Shot 2022-10-08 at 1 30 36 PM" src="https://user-images.githubusercontent.com/60394951/194707857-a592f5a9-30f8-4094-8a37-2f76d81c6f39.png">
<img width="341" alt="Screen Shot 2022-10-08 at 1 35 53 PM" src="https://user-images.githubusercontent.com/60394951/194707916-ccaacb5a-a6a8-4e37-bf57-3f0d400969fe.png">
   </p><br>
   > File Transfer :  
