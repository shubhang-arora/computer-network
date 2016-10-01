Streaming Audio/Video Over UDP

Commands to run the server
* cd computer network/lab3/server
* gcc server.c -o server
* ./server IP_ADDR FILENAME.MP3  
[IP_ADDR of the machine on which server is running and FILENAME of the file which server will send.]  

Commands to run the client 
* cd computer network/lab3/client
* gcc client.c -o server
* ./client IP_ADDR FILENAME.MP3 <br>
[IP_ADDR of the machine on which server is running and FILENAME of the file in which recieved file will be saved on the client side.]
