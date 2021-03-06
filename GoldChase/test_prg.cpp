#include "goldchase.h"
#include "Map.h"
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For amode constants */
#include <semaphore.h>
#include <errno.h>
#include<iostream>
#include <fstream>
#include <string>
#include<string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdlib.h>
#include<signal.h>
#include <sys/types.h>
#include <errno.h>
#include<string>
#include <mqueue.h>
#include <cstdlib>
#include<sstream>
#include<cstring>
#include<vector>
#include<utility>
#include<sys/socket.h>
#include<netdb.h>



using namespace std;
mqd_t fdarray[5];
string player_msgquename[5]={"/PLAYER_QUEUE_1","/PLAYER_QUEUE_2","/PLAYER_QUEUE_3","/PLAYER_QUEUE_4","/PLAYER_QUEUE_5"};
mqd_t readqueue_fd; //file descriptor.
mqd_t writequeue_fd; //message queue file descriptor
string mq_name="/PLAYER_QUEUE";
string mq_name2="/PLAYER_QUEUE";
int row_interrupt=0; //4 bytes
int col_interrupt=0; //4 bytes
Map *obj;
bool exit_flag =false;
int fifovar;
unsigned char *local_pointer;
unsigned char *local_pointer_client;
int globalCount;
int new_sockfd;
int sockfd;
sem_t* my_sem_ptr_client;
sem_t* my_sem_ptr;
int second_player_cols;
int second_player_rows;


struct GameBoard {
  int rows; //4 bytes
  int cols; //4 bytes
  unsigned char players;
  pid_t pid[5];
  int daemonID;
  unsigned char map[0];

};
GameBoard* goldmap;
GameBoard* goldmap2;
GameBoard* goldmap_client;

template<typename T>
int READ(int fd, T* obj_ptr, int count)
{
  char* addr=(char*)obj_ptr;
  //loop. Read repeatedly until count bytes are read in
  int bytesRead = 0;
  int totalBytesRead = 0;
  while (count > 0)
  {
	bytesRead = read(fd, addr + totalBytesRead, count);
    //std::cout << "bytes read = " << bytesRead << std::endl;
    if (bytesRead == -1)
    {
      if (errno != EINTR)
      {
        return -1;
      }
    }
	else
	{
		totalBytesRead += bytesRead;
		count -= totalBytesRead;
	}
  }
  return totalBytesRead;
}

template<typename T>
int WRITE(int fd, T* obj_ptr, int count)
{
  char* addr=(char*)obj_ptr;
  //loop. Write repeatedly until count bytes are written out
  int bytesWritten = 0;
  int totalBytesWritten = 0;
  while (count > 0)
  {
    //std::cout << "bytes written = " << bytesWritten << std::endl;
	bytesWritten = write(fd, addr + totalBytesWritten, count);
    if (bytesWritten == -1)
    {
      if (errno != EINTR)
      {
        return -1;
      }
    }
	else
	{
		totalBytesWritten += bytesWritten;
		count -= totalBytesWritten;
	}
  }
  return totalBytesWritten;
}


void sigusr2Daemonfunc_c(int)
{

  unsigned char playerCheck_msg;
  char temp;
  char* actual_msg;
  for(int i=0;i<5;i++)
{
    //read a message

  int err;
  char msg[121];
  memset(msg, 0, 121);//set all characters to '\0'
  while((err=mq_receive(fdarray[i], msg, 120, NULL))!=-1)
  {

    playerCheck_msg = G_SOCKMSG;
    if(i==0)
    {
      playerCheck_msg |= G_PLR0;
    }
    else if(i==1)
    {
      playerCheck_msg|=G_PLR1;
    }
    else if(i==2)
    {
      playerCheck_msg|=G_PLR2;
    }
    else if(i==3)
    {
      playerCheck_msg|=G_PLR3;
    }
    else if(i==4)
    {
      playerCheck_msg|=G_PLR4;
    }
    short msg_sizecount= strlen(msg);
    //actual_msg=msg;
    WRITE(sockfd,&playerCheck_msg,sizeof(playerCheck_msg));
    WRITE(sockfd,&msg_sizecount,sizeof(msg_sizecount));
    WRITE(sockfd,&msg,msg_sizecount);

  }
 }
}






void sigusr2Daemonfunc(int)
{


  unsigned char playerCheck_msg;
  char temp;
  char* actual_msg;
  for(int i=0;i<5;i++)
{
    //read a message

  int err;
  char msg[121];
  memset(msg, 0, 121);//set all characters to '\0'
  while((err=mq_receive(fdarray[i], msg, 120, NULL))!=-1)
  {

    playerCheck_msg = G_SOCKMSG;
    if(i==0)
    {
      playerCheck_msg |= G_PLR0;
    }
    else if(i==1)
    {
      playerCheck_msg|=G_PLR1;
    }
    else if(i==2)
    {
      playerCheck_msg|=G_PLR2;
    }
    else if(i==3)
    {
      playerCheck_msg|=G_PLR3;
    }
    else if(i==4)
    {
      playerCheck_msg|=G_PLR4;
    }
    short msg_sizecount= strlen(msg);
    actual_msg=msg;
    WRITE(new_sockfd,&playerCheck_msg,sizeof(playerCheck_msg));
    WRITE(new_sockfd,&msg_sizecount,sizeof(msg_sizecount));
    WRITE(new_sockfd,&msg,msg_sizecount);


  }
 }
}

void siguseDaemonfunc(int)
{
  unsigned char zero=0;

  short count=0;
  vector< pair<short,unsigned char> > pvec;
  for(short i=0; i<(goldmap2->rows*goldmap2->cols); i++)
  {
    if(goldmap2->map[i]!=local_pointer[i])
    {
      pair<short,unsigned char> aPair;
      aPair.first=i;
      aPair.second=goldmap->map[i];
      pvec.push_back(aPair);
      local_pointer[i]=goldmap2->map[i];
    //  count++;
    }
  }

  count=pvec.size();
  if(count>0)
  {

    WRITE(new_sockfd,&zero,sizeof(zero));
    WRITE(new_sockfd,&count,sizeof(count));
    for(int j=0;j<count;j++)
    {
      WRITE(new_sockfd,&pvec[j].first,sizeof(pvec[j].first));
      WRITE(new_sockfd,&pvec[j].second,sizeof(pvec[j].second));

    }
  }




}

void siguseDaemonfunc_c(int)
{
  unsigned char zero=0;

  short count=0;
  vector< pair<short,unsigned char> > pvec;
  for(short i=0; i<(goldmap_client->rows*goldmap_client->cols); ++i)
  {
    if(goldmap_client->map[i]!=local_pointer_client[i])
    {
      pair<short,unsigned char> aPair;
      aPair.first=i;
      aPair.second=goldmap_client->map[i];
      pvec.push_back(aPair);
      local_pointer_client[i]=goldmap_client->map[i];
    //  count++;
    }
  }

  count=pvec.size();
  if(count>0)
  {

    WRITE(sockfd,&zero,sizeof(zero));
    WRITE(sockfd,&count,sizeof(count));
    for(int j=0;j<count;j++)
    {
      WRITE(sockfd,&pvec[j].first,sizeof(pvec[j].first));
      WRITE(sockfd,&pvec[j].second,sizeof(pvec[j].second));

    }
  }


}

void sighupDaemonfunc(int)
{


  unsigned char playerCheck;
  playerCheck = G_SOCKPLR;
  for(int i=0;i<5;i++)
  {
    if(goldmap2->pid[i]!=0)
    {
        if(i==0)
        {
          playerCheck |= G_PLR0;
        }
        else if(i==1)
        {
          playerCheck|=G_PLR1;
        }
        else if(i==2)
        {
          playerCheck|=G_PLR2;
        }
        else if(i==3)
        {
          playerCheck|=G_PLR3;
        }
        else if(i==4)
        {
          playerCheck|=G_PLR4;
        }
    }
  }
  //testing

  //testing ends
  WRITE(new_sockfd,&playerCheck,sizeof(playerCheck));

  //to unlink
  if(playerCheck==G_SOCKPLR)
  {
    sem_unlink("/Rohangoldchase");
    shm_unlink("/Rohangoldshare");
    exit(0);
  }
}

void sighupDaemonfunc_c(int)
{


  unsigned char playerCheck;
  playerCheck = G_SOCKPLR;
  for(int i=0;i<5;i++)
  {
    if(goldmap_client->pid[i]!=0)
    {
        if(i==0)
        {
          playerCheck|=G_PLR0;
        }
        else if(i==1)
        {
          playerCheck|=G_PLR1;
        }
        else if(i==2)
        {
          playerCheck|=G_PLR2;
        }
        else if(i==3)
        {
          playerCheck|=G_PLR3;
        }
        else if(i==4)
        {
          playerCheck|=G_PLR4;
        }
    }
  }
  //testing

  //testing ends
  WRITE(sockfd,&playerCheck,sizeof(playerCheck));

  //to unlink
  if(playerCheck==G_SOCKPLR)
  {
    sem_unlink("/Rohangoldchase");
    shm_unlink("/Rohangoldshare");
    exit(0);
  }
}





void handle_interrupt(int)
{
  obj->drawMap();
}
  int pipefd[2];

void clientFunc(string clientip)
{

  //Create a semaphore for client
  //  int sockfd; //file descriptor for the socket
    int status; //for error checking
    int client_rows;
    int client_cols;

    unsigned char *client_map_fromsock_var;
    unsigned char playerSocClient;


      pipe(pipefd);

      if(fork()>0)//parent;
      {
        close(pipefd[1]); //close write, parent only needs read
        int val;
        READ(pipefd[0], &val, sizeof(val));
        if(val==0)
          write(1, "Failure!\n", sizeof("Failure!\n"));
        else
          write(1, "Success!\n", sizeof("Success!\n"));

        return;
      }
      if(fork()>0)
      {
        exit(0);
      }
      //child
      if(setsid()==-1)
        exit(1);
      for(int i = 0; i< sysconf(_SC_OPEN_MAX); ++i)
      {
        if(i!=pipefd[1])//close everything, except write
          close(i);
      }
      open("/dev/null", O_RDWR); //fd 0
      open("/dev/null", O_RDWR); //fd 1
      open("/dev/null", O_RDWR); //fd 2
      umask(0);
      chdir("/");


    //change this # between 2000-65k before using
    const char* portno="42425";//42425

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints)); //zero out everything in structure
    hints.ai_family = AF_UNSPEC; //don't care. Either IPv4 or IPv6
    hints.ai_socktype=SOCK_STREAM; // TCP stream sockets

    struct addrinfo *servinfo;
    //instead of "localhost", it could by any domain name
    if((status=getaddrinfo(clientip.c_str(), portno, &hints, &servinfo))==-1)
    {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      exit(1);
    }
    sockfd=socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    if((status=connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen))==-1)
    {
      perror("connect");
      exit(1);
    }
    //release the information allocated by getaddrinfo()
    freeaddrinfo(servinfo);
    new_sockfd=sockfd;

    // read from socket
    READ(sockfd, &client_rows,sizeof(client_rows) );
    READ(sockfd, &client_cols,sizeof(client_cols) );

    local_pointer_client=new unsigned char[client_rows*client_cols];
    client_map_fromsock_var = new unsigned char[client_rows*client_cols];
    unsigned char bit;
    for(int i=0;i<(client_rows*client_cols);i++)
    {

      READ(sockfd,&bit,sizeof(bit));
      client_map_fromsock_var[i]=bit;
    }
  //  fifovar=open("/home/rohanc/Desktop/611/ActualCode/rchordiya_mqueue/myfifo",O_WRONLY);


    for(int i=0;i<(client_rows*client_cols);i++)
    {
      local_pointer_client[i] =  client_map_fromsock_var[i];
    }

  my_sem_ptr= sem_open("/Rohangoldchase",O_CREAT|O_EXCL,S_IRUSR| S_IWUSR,1);


      int fd;
    fd= shm_open("/Rohangoldshare", O_RDWR|O_CREAT,S_IRUSR | S_IWUSR );

    if(ftruncate(fd, (client_rows*client_cols)+sizeof(GameBoard)))
    {
       perror("Error in truncate\n");
    }
    goldmap_client= (GameBoard*)mmap(NULL, (client_rows*client_cols)+sizeof(GameBoard), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    goldmap_client->daemonID=getpid();
//semaphore check
    goldmap_client->rows=client_rows;
    goldmap_client->cols=client_cols;
    for(int j=0;j<(client_rows*client_cols);j++)
    {
      goldmap_client->map[j]=local_pointer_client[j];
    }
    //signal trapping

    struct sigaction sighup_handler_c;
    sighup_handler_c.sa_handler=sighupDaemonfunc_c;
    sigemptyset(&sighup_handler_c.sa_mask);
    sighup_handler_c.sa_flags=0;
    sighup_handler_c.sa_restorer=NULL;
    sigaction(SIGHUP, &sighup_handler_c, NULL);

    struct sigaction siguse1_handler_c;
    siguse1_handler_c.sa_handler=siguseDaemonfunc_c;
    sigemptyset(&siguse1_handler_c.sa_mask);
    siguse1_handler_c.sa_flags=0;
    siguse1_handler_c.sa_restorer=NULL;
    sigaction(SIGUSR1, &siguse1_handler_c, NULL);

    struct mq_attr mq_attributes;
    mq_attributes.mq_flags=0;
    mq_attributes.mq_maxmsg=10;
    mq_attributes.mq_msgsize=120;

    struct sigaction siguse2_handler_c;
    //handle with this function
    siguse2_handler_c.sa_handler=sigusr2Daemonfunc_c;
    //zero out the mask (allow any signal to interrupt)
    sigemptyset(&siguse2_handler_c.sa_mask);
    siguse2_handler_c.sa_flags=0;
    //tell how to handle SIGINT
    sigaction(SIGUSR2, &siguse2_handler_c, NULL);


    READ(sockfd,&playerSocClient,sizeof(playerSocClient));

//Changed now

unsigned char player_bit[5]={G_PLR0, G_PLR1, G_PLR2, G_PLR3, G_PLR4};

 for(int i=0; i<5; ++i) //loop through the player bits
 {
   if((playerSocClient & player_bit[i]) && goldmap_client->pid[i]==0)
   {

     mq_name2=player_msgquename[i];
     if((fdarray[i]=mq_open(mq_name2.c_str(), O_RDONLY|O_CREAT|O_EXCL|O_NONBLOCK,
             S_IRUSR|S_IWUSR, &mq_attributes))==-1)
     {
       perror("mq_open");
       exit(1);
     }
     //set up message queue to receive signal whenever message comes in
     struct sigevent mq_notification_event;
     mq_notification_event.sigev_notify=SIGEV_SIGNAL;
     mq_notification_event.sigev_signo=SIGUSR2;
     mq_notify(fdarray[i], &mq_notification_event);

     goldmap_client->pid[i]=getpid();
     goldmap_client->players|=player_bit[i];
   }

 }

  int val=1;
  WRITE(pipefd[1], &val, sizeof(val));
  while(1)
  {
    unsigned char protocol_byte,changedMapbyte;
    short numberofchangedByte;
    short index;
    short msg_size;
    char actual_received_msg[121];
    char temp;
    string tempQueue = "";
    READ(sockfd, &protocol_byte,sizeof(protocol_byte));

    //Map refresh
    if(protocol_byte==0)
    {
      //read number of changed byte
      READ(sockfd,&numberofchangedByte,sizeof(numberofchangedByte));
      for(short k=0;k<numberofchangedByte;k++)
      {
        READ(sockfd,&index,sizeof(index));
        READ(sockfd,&changedMapbyte,sizeof(changedMapbyte));
        local_pointer_client[index]=changedMapbyte;
        goldmap_client->map[index]=changedMapbyte;

      }
      for(int k=0;k<5;k++)
      {
        if(goldmap_client->pid[k]!=0 && goldmap_client->pid[k]!=getpid())
        {
          kill(goldmap_client->pid[k],SIGUSR1);
        }
      }


    }

  //for player

    else if(protocol_byte & G_SOCKPLR)
    {

      for(int i=0; i<5; ++i) //loop through the player bits
      {
        // If player bit is on and shared memory ID is zero,
        // a player (from other computer) has joined:
        if((protocol_byte & player_bit[i]) && (goldmap_client->pid[i]==0))
        {
          //do 1.1 and 1.2
          struct mq_attr mq_attributes;
          mq_attributes.mq_flags=0;
          mq_attributes.mq_maxmsg=10;
          mq_attributes.mq_msgsize=120;

          //1.1
          mq_name=player_msgquename[i];
          if((fdarray[i]=mq_open(mq_name.c_str(), O_RDONLY|O_CREAT|O_EXCL|O_NONBLOCK,
                  S_IRUSR|S_IWUSR, &mq_attributes))==-1)
          {
            perror("mq_open");
            exit(1);
          }
          //set up message queue to receive signal whenever message comes in
          struct sigevent mq_notification_event;
          mq_notification_event.sigev_notify=SIGEV_SIGNAL;
          mq_notification_event.sigev_signo=SIGUSR2;
          mq_notify(fdarray[i], &mq_notification_event);

          //1.2
          goldmap_client->pid[i]=getpid();
          goldmap_client->players|=player_bit[i];
        }
        //If player bit is off and shared memory ID is not zero,
        //remote player has quit:
        else if(!(protocol_byte & player_bit[i]) && goldmap_client->pid[i]!=0)
        {
          //do 2.1 and 2.2
          //2.1
          mq_name=player_msgquename[i];
          mq_close(fdarray[i]);
          mq_unlink(mq_name.c_str());
          //2.2
          goldmap_client->pid[i]=0;
          goldmap_client->players&=~player_bit[i];
        }

      }
      if(protocol_byte==G_SOCKPLR)
      {
        //no players are left in the game.  Close and unlink the shared memory.
        //Close and unlink the semaphore.  Then exit the program.
        sem_unlink("/Rohangoldchase");
        shm_unlink("/Rohangoldshare");
        exit(0);

      }


    }
    else if(protocol_byte & G_SOCKMSG)
    {
      unsigned char temp_player;

        if(protocol_byte & G_PLR0)
        {
          temp_player=G_PLR0;
          tempQueue="/PLAYER_QUEUE_1";
        }
        else if(protocol_byte & G_PLR1)
        {
          temp_player=G_PLR1;
          tempQueue="/PLAYER_QUEUE_2";
        }
        else if(protocol_byte & G_PLR2)
        {
          temp_player=G_PLR2;
          tempQueue="/PLAYER_QUEUE_3";
        }
        else if(protocol_byte & G_PLR3)
        {
          temp_player=G_PLR3;
          tempQueue="/PLAYER_QUEUE_4";
        }
        else if(protocol_byte & G_PLR4)
        {
          temp_player=G_PLR4;
          tempQueue="/PLAYER_QUEUE_5";
        }
        READ(sockfd,&msg_size,sizeof(msg_size));
        READ(sockfd,&actual_received_msg,msg_size);


        mqd_t daemonwrite;
        if((daemonwrite=mq_open(tempQueue.c_str(), O_WRONLY|O_NONBLOCK))==-1)
        {
          perror("mq_open 1");
          exit(1);
        }
        //cerr << "fd=" << writequeue_fd << endl;
        char message_text[121];
        memset(message_text, 0, 121);
        strncpy(message_text, actual_received_msg, 120);
        if(mq_send(daemonwrite, message_text, strlen(message_text), 0)==-1)
        {
          perror("mq_send");
          exit(1);
        }
        mq_close(daemonwrite);

    }


  }

}

void daeFunc()
{
  if(fork()>0)
  {
    //I'm the parent, leave the function
    return ;
  }

  if(fork()>0)
    exit(0);
  if(setsid()==-1)
    exit(1);
  for(int i = 0; i< sysconf(_SC_OPEN_MAX); ++i)
    close(i);

  open("/dev/null", O_RDWR); //fd 0
  open("/dev/null", O_RDWR); //fd 1
  open("/dev/null", O_RDWR); //fd 2
  umask(0);
  chdir("/");




struct sigaction sighup_handler;
sighup_handler.sa_handler=sighupDaemonfunc;
sigemptyset(&sighup_handler.sa_mask);
sighup_handler.sa_flags=0;
sighup_handler.sa_restorer=NULL;
sigaction(SIGHUP, &sighup_handler, NULL);

struct sigaction siguse1_handler;
siguse1_handler.sa_handler=siguseDaemonfunc;
sigemptyset(&siguse1_handler.sa_mask);
siguse1_handler.sa_flags=0;
siguse1_handler.sa_restorer=NULL;
sigaction(SIGUSR1, &siguse1_handler, NULL);


struct mq_attr mq_attributes;
mq_attributes.mq_flags=0;
mq_attributes.mq_maxmsg=10;
mq_attributes.mq_msgsize=120;


struct sigaction siguse2_handler;
//handle with this function
siguse2_handler.sa_handler=sigusr2Daemonfunc;
//zero out the mask (allow any signal to interrupt)
sigemptyset(&siguse2_handler.sa_mask);
siguse2_handler.sa_flags=0;
//tell how to handle SIGINT
sigaction(SIGUSR2, &siguse2_handler, NULL);


//for socket initialization

/* code */
int sockfd; //file descriptor for the socket
int status; //for error checking


//change this # between 2000-65k before using
const char* portno="42425";
struct addrinfo hints;
memset(&hints, 0, sizeof(hints)); //zero out everything in structure
hints.ai_family = AF_UNSPEC; //don't care. Either IPv4 or IPv6
hints.ai_socktype=SOCK_STREAM; // TCP stream sockets
hints.ai_flags=AI_PASSIVE; //file in the IP of the server for me

struct addrinfo *servinfo;
if((status=getaddrinfo(NULL, portno, &hints, &servinfo))==-1)
{
  fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
  exit(1);
}
sockfd=socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

/*avoid "Address already in use" error*/
int yes=1;
if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))==-1)
{
  perror("setsockopt");
  exit(1);
}

//We need to "bind" the socket to the port number so that the kernel
//can match an incoming packet on a port to the proper process
if((status=bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen))==-1)
{
  perror("bind");
  exit(1);
}
//when done, release dynamically allocated memory
freeaddrinfo(servinfo);

if(listen(sockfd,1)==-1)
{
  perror("listen");
  exit(1);
}

//printf("Blocking, waiting for client to connect\n");

struct sockaddr_in client_addr;
socklen_t clientSize=sizeof(client_addr);

do
{
  new_sockfd=accept(sockfd, (struct sockaddr*) &client_addr, &clientSize);
} while(new_sockfd==-1 && errno==EINTR);



if(new_sockfd==-1)
{
  exit(1);
}

//Sock player

int fd;
  fd = shm_open("/Rohangoldshare", O_RDWR,S_IRUSR | S_IWUSR |S_IRWXU );
  int daemon_cols;
  int daemon_rows;
  read(fd, &daemon_rows, sizeof(int));
  read(fd, &daemon_cols, sizeof(int));
  goldmap2= (GameBoard*)mmap(NULL, (daemon_rows*daemon_cols + sizeof(GameBoard)), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  goldmap2->daemonID=getpid();
  local_pointer=new unsigned char[daemon_rows*daemon_cols];
  for(int i=0;i < daemon_rows*daemon_cols;i++)
  {
    local_pointer[i] = goldmap2->map[i];
  }

unsigned char playerSoc;
playerSoc = G_SOCKPLR;

for(int i=0;i<5;i++)
{
  if(goldmap->pid[i]!=0)
  {
      if(i==0)
      {
        playerSoc|=G_PLR0;
      }
      if(i==1)
      {
        playerSoc|=G_PLR1;
      }
      if(i==2)
      {
        playerSoc|=G_PLR2;
      }
      if(i==3)
      {
        playerSoc|=G_PLR3;
      }
      if(i==4)
      {
        playerSoc|=G_PLR4;
      }
  }
}

//write(row col)

WRITE(new_sockfd, &daemon_rows, sizeof(daemon_rows));
WRITE(new_sockfd, &daemon_cols, sizeof(daemon_cols));
for(int i=0;i<daemon_rows*daemon_cols;i++)
{
    WRITE(new_sockfd, &local_pointer[i], sizeof(local_pointer[i]));
}

WRITE(new_sockfd, &playerSoc, sizeof(playerSoc));
//write(map)
//write(player)

while(1)
{
  unsigned char protocol_byte,changedMapbyte;
  short numberofchangedByte;
  short index;
  short msg_size;
  char actual_received_msg[121];
  char temp;
  unsigned char player_bit[5]={G_PLR0, G_PLR1, G_PLR2, G_PLR3,G_PLR4};
  string tempQueue = "";

  READ(new_sockfd, &protocol_byte,sizeof(protocol_byte));

  //Map refresh
  if(protocol_byte==0)
  {
    //read number of changed byte
    READ(new_sockfd,&numberofchangedByte,sizeof(numberofchangedByte));
    for(short k=0;k<numberofchangedByte;k++)
    {
      READ(new_sockfd,&index,sizeof(index));
      READ(new_sockfd,&changedMapbyte,sizeof(changedMapbyte));
      local_pointer[index]=changedMapbyte;
      goldmap2->map[index]=changedMapbyte;


    }
    for(int k=0;k<5;k++)
    {
      if(goldmap2->pid[k]!=0 && goldmap2->pid[k]!=getpid())
      {
        kill(goldmap2->pid[k],SIGUSR1);
      }
    }


  }

//for player

  else if(protocol_byte & G_SOCKPLR)
  {

    for(int i=0; i<5; ++i) //loop through the player bits
    {
      // If player bit is on and shared memory ID is zero,
      // a player (from other computer) has joined:
      if(protocol_byte & player_bit[i] && goldmap2->pid[i]==0)
      {
        //do 1.1 and 1.2
        //1.1
        struct mq_attr mq_attributes;
        mq_attributes.mq_flags=0;
        mq_attributes.mq_maxmsg=10;
        mq_attributes.mq_msgsize=120;

        mq_name=player_msgquename[i];
        if((fdarray[i]=mq_open(mq_name.c_str(), O_RDONLY|O_CREAT|O_EXCL|O_NONBLOCK,
                S_IRUSR|S_IWUSR, &mq_attributes))==-1)
        {
          perror("mq_open");
          exit(1);
        }
        //set up message queue to receive signal whenever message comes in
        struct sigevent mq_notification_event;
        mq_notification_event.sigev_notify=SIGEV_SIGNAL;
        mq_notification_event.sigev_signo=SIGUSR2;
        mq_notify(fdarray[i], &mq_notification_event);

        //1.2
        goldmap2->pid[i]=getpid();
        goldmap2->players|=player_bit[i];
      }
      //If player bit is off and shared memory ID is not zero,
      //remote player has quit:
      else if(!(protocol_byte & player_bit[i]) && goldmap2->pid[i]!=0)
      {
        //do 2.1 and 2.2
        //2.1
        mq_name=player_msgquename[i];
        mq_close(fdarray[i]);
        mq_unlink(mq_name.c_str());
        //2.2
        goldmap2->pid[i]=0;
        goldmap2->players&=~player_bit[i];
      }
    //  goldmap2->players=protocol_byte;
    }
    if(protocol_byte==G_SOCKPLR)
    {
      //no players are left in the game.  Close and unlink the shared memory.
      //Close and unlink the semaphore.  Then exit the program.
      sem_unlink("/Rohangoldchase");
      shm_unlink("/Rohangoldshare");
      exit(0);

    }


  }
  else if(protocol_byte & G_SOCKMSG)
  {
    unsigned char temp_player;

      if(protocol_byte & G_PLR0)
      {
        temp_player=G_PLR0;
        tempQueue="/PLAYER_QUEUE_1";
      }
      else if(protocol_byte & G_PLR1)
      {
        temp_player=G_PLR1;
        tempQueue="/PLAYER_QUEUE_2";
      }
      else if(protocol_byte & G_PLR2)
      {
        temp_player=G_PLR2;
        tempQueue="/PLAYER_QUEUE_3";
      }
      else if(protocol_byte & G_PLR3)
      {
        temp_player=G_PLR3;
        tempQueue="/PLAYER_QUEUE_4";
      }
      else if(protocol_byte & G_PLR4)
      {
        temp_player=G_PLR4;
        tempQueue="/PLAYER_QUEUE_5";
      }
      READ(new_sockfd,&msg_size,sizeof(msg_size));
      READ(new_sockfd,&actual_received_msg,msg_size);



      mqd_t daemonwrite;
      if((daemonwrite=mq_open(tempQueue.c_str(), O_WRONLY|O_NONBLOCK))==-1)
      {
        perror("mq_open 1");
        exit(1);
      }
      //cerr << "fd=" << writequeue_fd << endl;
      char message_text[121];
      memset(message_text, 0, 121);
      strncpy(message_text, actual_received_msg, 120);
      if(mq_send(daemonwrite, message_text, strlen(message_text), 0)==-1)
      {
        perror("mq_send");
        exit(1);
      }
      mq_close(daemonwrite);

  }



}


}

void read_message(int)
{

  struct sigevent mq_notification_event;
  mq_notification_event.sigev_notify=SIGEV_SIGNAL;
  mq_notification_event.sigev_signo=SIGUSR2;
  mq_notify(readqueue_fd, &mq_notification_event);

  //read a message
  int err;
  char msg[121];
  memset(msg, 0, 121);//set all characters to '\0'
  while((err=mq_receive(readqueue_fd, msg, 120, NULL))!=-1)
  {
    //cout << "Message received: " << msg << endl;
    obj->postNotice(msg);
    memset(msg, 0, 121);//set all characters to '\0'
  }

  if(errno!=EAGAIN)
  {
    perror("mq_receive");
    exit(1);
  }
}


void sendMessage(string message, int local)
{
  string tempQueue = "";
//int this_player;
    if(local == 0)
    {
      tempQueue="/PLAYER_QUEUE_1";
    }
    else if(local == 1)
    {
      tempQueue="/PLAYER_QUEUE_2";
    }
    else if(local == 2)
    {
      tempQueue="/PLAYER_QUEUE_3";
    }
    else if(local == 3)
    {
      tempQueue="/PLAYER_QUEUE_4";
    }
    else if(local == 4)
    {
      tempQueue="/PLAYER_QUEUE_5";
    }
    int this_player;
    const char *x;
    string str = "";
    string str2 = "";

    x=message.c_str();

    if((writequeue_fd=mq_open(tempQueue.c_str(), O_WRONLY|O_NONBLOCK))==-1)
    {
      perror("mq_open 1");
      exit(1);
    }
    //cerr << "fd=" << writequeue_fd << endl;
    char message_text[121];
    memset(message_text, 0, 121);
    strncpy(message_text, x, 120);
    if(mq_send(writequeue_fd, message_text, strlen(message_text), 0)==-1)
    {
      perror("mq_send");
      exit(1);
    }
    mq_close(writequeue_fd);
}


void broadcast(string msg, bool winning, int playerNumber)
{
  string str2 = "";
  string final = "";

  if(winning == false){
  str2= "Player "+ to_string(playerNumber+1)+" Says ";
  //string final = "";
  final = str2 + obj -> getMessage();
//  cerr << " NALMCLFDF" << endl;
  }
else{
  //cout << " akjkjahsdasdlakdjad" << endl;
  final = "Player " + to_string(playerNumber+1) + " has Won!";
  }
  //str=goldMine.getMessage
  for(int i = 0; i < 5; i++)
  {
     if(goldmap->pid[i]!=0 && goldmap->pid[i]!=getpid())
      sendMessage(final,i);
  }
}

void clean_up(int)
{

  exit_flag = true;
  return;
}

void initialize_players()
{

  struct mq_attr mq_attributes;
  mq_attributes.mq_flags=0;
  mq_attributes.mq_maxmsg=10;
  mq_attributes.mq_msgsize=120;
  if((readqueue_fd=mq_open(mq_name.c_str(), O_RDONLY|O_CREAT|O_EXCL|O_NONBLOCK,
          S_IRUSR|S_IWUSR, &mq_attributes))==-1)
  {
    perror("mq_open");
    exit(1);
  }
  //set up message queue to receive signal whenever message comes in
  struct sigevent mq_notification_event;
  mq_notification_event.sigev_notify=SIGEV_SIGNAL;
  mq_notification_event.sigev_signo=SIGUSR2;
  mq_notify(readqueue_fd, &mq_notification_event);

}



int main(int argc, const char* argv[])
{
  if(argc>1)
  {
    string ipvalue;
    sem_t* my_sem_client_check;
    my_sem_client_check= sem_open("/Rohangoldchase",O_RDWR,S_IRUSR|S_IWUSR,1);
    if(my_sem_client_check==SEM_FAILED)
    {
    	ipvalue=argv[1];
      clientFunc(ipvalue);
    }


  }



  int curPlayer;
  int curSig;
  bool flag=false;
  int colCount=0;
  int rowCount=0;
  int randomNumber;


  int mapSize,goldCount;
  string line;
  string toCount;
  int pl=-1;
  //signal initialization
  struct sigaction sig_handler;
  sig_handler.sa_handler=handle_interrupt;
  sigemptyset(&sig_handler.sa_mask);
  sig_handler.sa_flags=0;
  sig_handler.sa_restorer=NULL;
  sigaction(SIGUSR1, &sig_handler, NULL);

  struct sigaction action_to_take;
  //handle with this function
  action_to_take.sa_handler=read_message;
  //zero out the mask (allow any signal to interrupt)
  sigemptyset(&action_to_take.sa_mask);
  action_to_take.sa_flags=0;
  //tell how to handle SIGINT
  sigaction(SIGUSR2, &action_to_take, NULL);



  struct sigaction exit_handler;
  exit_handler.sa_handler=clean_up;
  sigemptyset(&exit_handler.sa_mask);
  exit_handler.sa_flags=0;
  sigaction(SIGINT, &exit_handler, NULL);
//  sigaction(SIGHUP, &exit_handler, NULL);
  sigaction(SIGTERM, &exit_handler, NULL);

  if(argc!=2)
  {
    ifstream myfile ("mymap.txt");
    if (myfile.is_open())
    {
      getline(myfile,line);
      goldCount=atoi(line.c_str());
      while ( getline(myfile,line) )
      {
         rowCount++;
         colCount=line.length();
      }
  }
  myfile.close();
}
  row_interrupt=rowCount;
  col_interrupt=colCount;
  second_player_rows=rowCount;
  second_player_cols=colCount;
  mapSize=colCount*rowCount;
  globalCount=mapSize;

  //if(argc!=2)
  my_sem_ptr= sem_open("/Rohangoldchase",O_CREAT|O_EXCL,S_IRUSR| S_IWUSR,1);
  if(my_sem_ptr==SEM_FAILED)
  {
    if(errno!=EEXIST)
    {
      perror("Error in semaphore");
      exit(1);
    }
  }

  if(my_sem_ptr!=SEM_FAILED)
  {
    sem_wait(my_sem_ptr);

    int fd;
    fd= shm_open("/Rohangoldshare", O_RDWR|O_CREAT,S_IRUSR | S_IWUSR );

    if(ftruncate(fd, (rowCount*colCount)+sizeof(GameBoard)))
    {
       perror("Error in truncate\n");
    }
    goldmap= (GameBoard*)mmap(NULL, (rowCount*colCount)+sizeof(GameBoard), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    goldmap->rows=rowCount;
    goldmap->cols=colCount;


    //reading file again
    int i=0;
    char myArray[mapSize+1],ch;
    ifstream myfile2("mymap.txt");
    if (myfile2.is_open())
    {
      getline(myfile2,line);
      while (!myfile2.eof())
      {
        myfile2.get(ch);
        if(ch!='\n')
        {
          myArray[i]=ch;
          i++;
        }
      }
         myArray[i]='\0';
    }

    myfile2.close();
    for(int x=0;x < mapSize;x++)
    {
      if(myArray[x]=='*')
      {
        goldmap->map[x]=G_WALL;
      }
      else if(myArray[x]==' ')
      {
        goldmap->map[x]=0;
      }
    }

    int track=0;
    srand (time(NULL));
    for(int g=0;g<goldCount;g++)
    {
      while(1)
      {
        randomNumber = rand() % mapSize + 1;
        if(track==0)
        {
          if(goldmap->map[randomNumber]!=G_WALL && goldmap->map[randomNumber]!=G_FOOL )
          {
            goldmap->map[randomNumber]=G_GOLD;
            track=1;
            break;
          }
          else
          {
            continue;
          }
        }
        if(track==1)
        {
          if(goldmap->map[randomNumber]!=G_WALL && goldmap->map[randomNumber]!=G_GOLD)
          {
            goldmap->map[randomNumber]=G_FOOL;
            break;
          }
          else
          {
            continue;
          }
         }
      }
    }
    for(int i = 0; i < 5; i ++)
    {
      goldmap -> pid[i] = 0;
    }
    while(1)
    {
      randomNumber = rand() % mapSize + 1;
      if(goldmap->map[randomNumber]!=G_WALL && goldmap->map[randomNumber]!=G_FOOL && goldmap->map[randomNumber]!=G_GOLD)
      {
         goldmap->map[randomNumber]=G_PLR0;
         goldmap->players=G_PLR0;
         pl=0;
         goldmap->pid[pl]=getpid();
         if(goldmap->daemonID!=0)
         {
           kill(goldmap->daemonID,SIGHUP);
         }

         curPlayer=G_PLR0;
         curSig=curPlayer;
//         if(curPlayer == G_PLR0)
         mq_name="/PLAYER_QUEUE_1";
         sem_post(my_sem_ptr);
         initialize_players();
         break;
      }
      else
      {
         continue;
      }

    }


}
else
{

  int fd;
  pl=-1;
  my_sem_ptr= sem_open("/Rohangoldchase",O_RDWR,S_IRUSR| S_IWUSR,1);
  sem_wait(my_sem_ptr);
  fd= shm_open("/Rohangoldshare", O_RDWR,S_IRUSR | S_IWUSR |S_IRWXU );

  read(fd, &second_player_rows, sizeof(int));
  read(fd, &second_player_cols, sizeof(int));
  randomNumber = rand() % (second_player_rows*second_player_cols)+1;
  goldmap= (GameBoard*)mmap(NULL, second_player_rows*second_player_cols + sizeof(GameBoard), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  globalCount= second_player_rows*second_player_cols;
  while(goldmap->map[randomNumber]!=0)
  {
    randomNumber = rand() % globalCount+1;
  }


  if((goldmap->players & G_PLR0) && (goldmap->pid[0]==0))
  {

    goldmap->map[randomNumber]=G_PLR0;
    goldmap->players|=G_PLR0;
    pl=0;

    curPlayer=G_PLR0;
    for(int i=0;i<5;i++)
    {
      if(goldmap->pid[i]!=0 && goldmap->pid[i]!=getpid())
      kill(goldmap->pid[i], SIGUSR1);
    }

    curSig=curPlayer;
    mq_name="/PLAYER_QUEUE_1";
    initialize_players();
  }

  else if((goldmap->players & G_PLR1)==0 && (goldmap->pid[1]==0))
  {
    goldmap->map[randomNumber]=G_PLR1;
    goldmap->players|=G_PLR1;
    pl=1;

    curPlayer=G_PLR1;
    for(int i=0;i<5;i++)
    {
      if(goldmap->pid[i]!=0 && goldmap->pid[i]!=getpid())
      kill(goldmap->pid[i], SIGUSR1);
    }


    curSig=curPlayer;

    mq_name="/PLAYER_QUEUE_2";
    initialize_players();
  }
  else if((goldmap->players & G_PLR2)==0 && (goldmap->pid[2]==0))
  {
    goldmap->map[randomNumber]=G_PLR2;
    goldmap->players|=G_PLR2;
    pl=2;


    curPlayer=G_PLR2;
    for(int i=0;i<5;i++)
    {
      if(goldmap->pid[i]!=0 && goldmap->pid[i]!=getpid())
      kill(goldmap->pid[i], SIGUSR1);
    }

    curSig=curPlayer;
    mq_name="/PLAYER_QUEUE_3";
    initialize_players();
  }
  else if((goldmap->players & G_PLR3)==0 && (goldmap->pid[3]==0))
  {
    goldmap->map[randomNumber]=G_PLR3;
    goldmap->players|=G_PLR3;
    pl=3;
    //goldmap->pid[3]=getpid();

    curPlayer=G_PLR3;
    for(int i=0;i<5;i++)
    {
      if(goldmap->pid[i]!=0 && goldmap->pid[i]!=getpid())
      kill(goldmap->pid[i], SIGUSR1);
    }

    curSig=curPlayer;
  //  if(curPlayer == G_PLR3)
    mq_name="/PLAYER_QUEUE_4";
    initialize_players();
  }
  else if((goldmap->players & G_PLR4)==0 && (goldmap->pid[4]==0))
  {
    goldmap->map[randomNumber]=G_PLR4;
    goldmap->players|=G_PLR4;
    pl=4;
    //goldmap->pid[4]=getpid();

    curPlayer=G_PLR4;
    for(int i=0;i<5;i++)
    {
      if(goldmap->pid[i]!=0 && goldmap->pid[i]!=getpid())
      kill(goldmap->pid[i], SIGUSR1);
    }

    curSig=curPlayer;
  //  if(curPlayer == G_PLR4)
    mq_name="/PLAYER_QUEUE_5";
    initialize_players();
  }
  else
  {
    cout<<"We are full"<<endl;
    sem_post(my_sem_ptr);
    return 0;
  }

  goldmap->pid[pl]=getpid();
  sem_post(my_sem_ptr);
}
      //For subsequent player
int key;
Map goldMine(goldmap->map,goldmap->rows,goldmap->cols);
obj=&goldMine;
goldMine.postNotice("This is a notice");
if(goldmap->daemonID==0)
{
  daeFunc();
}
else
{
//  cout<<"In daemon not 0 ready to send killsighup else"<<endl;
  kill(goldmap->daemonID,SIGHUP);

}
do
{

key=goldMine.getKey();
if(key=='b')
{
  string temp = "";
  int this_player = 0;
  int otherPlayers = 0;
  //temp = goldMine.getMessage();
  for(int i=0;i<5;i++)
  {
    if(goldmap->pid[i]==getpid())
    {
      this_player=i;
    }
    if((goldmap->pid[i]==0))
      otherPlayers++;
  }
string x = "";

  if(otherPlayers != 4)
  broadcast(x,false,this_player);
}
if(key=='m')
{
  sem_wait(my_sem_ptr);

 int local ;
  unsigned int msg_receive = goldMine.getPlayer(goldmap->players & ~curPlayer);
  if(msg_receive &  G_PLR0)
  {
    local = 0;

  }
  else if(msg_receive & G_PLR1)
  {
    local = 1;

  }
  else if(msg_receive & G_PLR2)
  {
    local = 2;

  }
  else if(msg_receive & G_PLR3)
  {
    local =3;

  }
  else if(msg_receive & G_PLR4)
  {
    local =4;

  }

string tempQueue;

  if(local == 0)
  {
    tempQueue="/PLAYER_QUEUE_1";
  }
  else if(local == 1)
  {
    tempQueue="/PLAYER_QUEUE_2";
  }
  else if(local == 2)
  {
    tempQueue="/PLAYER_QUEUE_3";
  }
  else if(local == 3)
  {
    tempQueue="/PLAYER_QUEUE_4";
  }
  else if(local == 4)
  {
    tempQueue="/PLAYER_QUEUE_5";
  }
  int this_player;
  const char *x;
  string str = "";
  string str2 = "";
  for(int i=0;i<5;i++)
  {
    if(goldmap->pid[i]==getpid())
    {

      this_player=i;
    }
  }

  if(msg_receive != 0){
  str2= "Player "+to_string(this_player+1)+" Says ";
  string final = "";
  final = str2 + goldMine.getMessage();

  x=final.c_str();

  if((writequeue_fd=mq_open(tempQueue.c_str(), O_WRONLY|O_NONBLOCK))==-1)
  {
    perror("mq_open 1");
    exit(1);
  }

  char message_text[121];
  memset(message_text, 0, 121);
  strncpy(message_text, x, 120);
  if(mq_send(writequeue_fd, message_text, strlen(message_text), 0)==-1)
  {
    perror("mq_send");
    exit(1);
  }
  mq_close(writequeue_fd);
}
  sem_post(my_sem_ptr);

}
if(key=='h')
{
  for(int i=0;i<5;i++)
  {
    if(goldmap->pid[i]!=0 && goldmap->pid[i]!=curPlayer)
    {
      kill(goldmap->pid[i], SIGUSR1);
    }
  }
  int newRow,newCol,ranVar,leftMov;
  newRow=randomNumber/second_player_cols;
  newCol=randomNumber%second_player_cols;
  ranVar=(newRow*second_player_cols)+newCol;
  leftMov=randomNumber-1;

  if(!((randomNumber)%second_player_cols==0))
  {

    if(goldmap->map[leftMov]!=G_WALL )
    {

      sem_wait(my_sem_ptr);
      goldmap->map[leftMov]|=curPlayer;
      goldmap->map[randomNumber]&=~curPlayer;

      sem_post(my_sem_ptr);
      randomNumber=leftMov;
    }
    goldMine.drawMap();
    if(goldmap->map[leftMov] & G_FOOL )
    {

      goldMine.postNotice("this is fools gold sorry");
    }
    else if(goldmap->map[leftMov] & G_GOLD )
    {

      goldmap->map[leftMov]&=~G_GOLD;
      goldMine.postNotice("You found the Gold");
      flag =true;
    }


  }
  else
  {
    if(flag)
    {
      sem_wait(my_sem_ptr);
      goldmap->players &= ~curPlayer;
      goldmap->map[randomNumber] &= ~curPlayer;
      int this_player =0;
      int otherPlayers = 0;
      for(int i=0;i<5;i++)
      {
        if(goldmap->pid[i]==getpid())
        {
          this_player=i;

        }
        if(goldmap->pid[i]== 0)
          otherPlayers++;
      }

        if(otherPlayers != 4)
          broadcast("",true,this_player);
        mq_close(readqueue_fd);

        int x = mq_unlink(mq_name.c_str());
        goldmap->pid[this_player]=0;

      if(goldmap->players==0)
      {

        sem_close(my_sem_ptr);
        sem_unlink("/Rohangoldchase");
        sem_unlink("/Rohangoldshare");
      }
      sem_post(my_sem_ptr);
      return 0;

    }
  }
}
if(key=='l')
{
  for(int i=0;i<5;i++)
  {
    if(goldmap->pid[i]!=0 && goldmap->pid[i]!=curPlayer)
    kill(goldmap->pid[i], SIGUSR1);
  }
  int newRow,newCol,ranVar,rightMov;
  newRow=randomNumber/second_player_cols;
  newCol=(randomNumber+1)%second_player_cols;
  ranVar=(newRow*second_player_cols)+newCol;
  rightMov=randomNumber+1;

  if(!((randomNumber+1)%second_player_cols==0))
  {


    if(goldmap->map[rightMov]!=G_WALL)
    {

      sem_wait(my_sem_ptr);
      goldmap->map[rightMov]|=curPlayer;
      goldmap->map[randomNumber]&=~curPlayer;
      sem_post(my_sem_ptr);
      randomNumber=rightMov;
    }
    goldMine.drawMap();
    if(goldmap->map[rightMov] & G_FOOL )
    {

      goldMine.postNotice("this is fools gold sorry");
    }
    else if(goldmap->map[rightMov] & G_GOLD )
    {

      goldmap->map[rightMov]&=~G_GOLD;
      goldMine.postNotice("You found the Gold ");
      flag =true;
    }

  }
  else
  {

    if(flag)
    {
      sem_wait(my_sem_ptr);
      goldmap->players &= ~curPlayer;
      goldmap->map[randomNumber] &= ~curPlayer;
      int this_player =0;
      int otherPlayers = 0;
      for(int i=0;i<5;i++)
      {
        if(goldmap->pid[i]==getpid())
        {
          this_player=i;
        }
        if(goldmap->pid[i]== 0)
          otherPlayers++;
      }

        if(otherPlayers != 4)
          broadcast("",true,this_player);
        mq_close(readqueue_fd);
        int x = mq_unlink(mq_name.c_str());
        goldmap->pid[this_player]=0;


      if(goldmap->players==0)
      {

        sem_close(my_sem_ptr);
        sem_unlink("/Rohangoldchase");
        shm_unlink("/Rohangoldshare");
      }
      sem_post(my_sem_ptr);
       return 0;
    }
  }
}
if(key=='j')
{
  for(int i=0;i<5;i++)
  {
    if(goldmap->pid[i]!=0 && goldmap->pid[i]!=curPlayer)
    kill(goldmap->pid[i], SIGUSR1);
  }

  int newRow,newCol,ranVar,downMov;
  newRow=randomNumber/second_player_cols;
  newCol=randomNumber+second_player_cols;
  ranVar=(newRow*second_player_cols)+newCol;
  downMov=newCol;

  if(newCol<second_player_rows*second_player_cols )
  {


    if(goldmap->map[downMov]!=G_WALL)
    {

      sem_wait(my_sem_ptr);
      goldmap->map[downMov]|=curPlayer;
      goldmap->map[randomNumber]&=~curPlayer;

      sem_post(my_sem_ptr);
      randomNumber=downMov;
    }
    goldMine.drawMap();
    if(goldmap->map[downMov] & G_FOOL )
    {

      goldMine.postNotice("this is fools gold sorry");
    }
    else if(goldmap->map[downMov] & G_GOLD )
    {

      goldmap->map[downMov]&=~G_GOLD;
      goldMine.postNotice("You found the Gold ");
      flag =true;
    }

  }
  else
  {
    if(flag)
    {
      sem_wait(my_sem_ptr);
      goldmap->players &= ~curPlayer;
      goldmap->map[randomNumber] &= ~curPlayer;

      int this_player =0;
      int otherPlayers = 0;
      for(int i=0;i<5;i++)
      {
        if(goldmap->pid[i]==getpid())
        {
          this_player=i;
        }
        if(goldmap->pid[i]== 0)
          otherPlayers++;
      }

        if(otherPlayers != 4)
          broadcast("",true,this_player);
        mq_close(readqueue_fd);
        int x = mq_unlink(mq_name.c_str());
        goldmap->pid[this_player]=0;

      if(goldmap->players==0)
      {

        sem_close(my_sem_ptr);
        sem_unlink("/Rohangoldchase");
        shm_unlink("/Rohangoldshare");
      }
      sem_post(my_sem_ptr);
      return 0;
    }
  }
}
if(key=='k')
{
  for(int i=0;i<5;i++)
  {
    if(goldmap->pid[i]!=0 && goldmap->pid[i]!=curPlayer)
    kill(goldmap->pid[i], SIGUSR1);
  }

  int newRow,newCol,ranVar,upMov;
  newRow=randomNumber/second_player_cols;
  newCol=randomNumber-second_player_cols;
  ranVar=(newRow*second_player_cols)+newCol;
  upMov=newCol;

  if(newCol>0)
  {

    if(goldmap->map[upMov]!=G_WALL )
    {

      sem_wait(my_sem_ptr);
      goldmap->map[upMov]|=curPlayer;
      goldmap->map[randomNumber]&=~curPlayer;

      sem_post(my_sem_ptr);
      randomNumber=upMov;
    }
    goldMine.drawMap();
    if(goldmap->map[upMov] & G_FOOL )
    {
      goldMine.postNotice("this is fools gold sorry");
    }
    else if(goldmap->map[upMov] & G_GOLD )
    {
      goldmap->map[upMov]&=~G_GOLD;
      goldMine.postNotice("You found the Gold ");
      flag =true;

    }

  }
  else
  {
    if(flag)
    {

      sem_wait(my_sem_ptr);
      goldmap->players &= ~curPlayer;
      goldmap->map[randomNumber] &= ~curPlayer;
      int this_player =0;
      int otherPlayers = 0;
      for(int i=0;i<5;i++)
      {
        if(goldmap->pid[i]==getpid())
        {
          this_player=i;
        }
        if(goldmap->pid[i]== 0)
          otherPlayers++;
      }

      if(otherPlayers != 4)
        broadcast("",true,this_player);
      mq_close(readqueue_fd);
      int x = mq_unlink(mq_name.c_str());
      goldmap->pid[this_player]=0;
      if(goldmap->players==0)
      {
        //unlink shared memory and semaphore
        sem_close(my_sem_ptr);
        sem_unlink("/Rohangoldchase");
        shm_unlink("/Rohangoldshare");
      }
        sem_post(my_sem_ptr);
      return 0;
    }
  }
}

}
while (key != 'Q' && exit_flag == false);

    //when quitting
sem_wait(my_sem_ptr);
goldmap->players &= ~curPlayer;
goldmap->map[randomNumber] &= ~curPlayer;
goldmap->pid[pl]=0;

for(int i=0;i<5;i++)
{
  if(goldmap->pid[i]!=0 && goldmap->pid[i]!=getpid())
  kill(goldmap->pid[i], SIGUSR1);
}

  kill(goldmap->daemonID,SIGHUP);
  mq_close(readqueue_fd);
  int x = mq_unlink(mq_name.c_str());
  if(x == -1)
    cerr << " UNLINK FAIL" << endl;
    sem_post(my_sem_ptr);
    //sem_close(my_sem_ptr);
    
/*  if(goldmap->players==0)
  {  //unlink shared memory and semaphore

  sem_close(my_sem_ptr);
  sem_unlink("/Rohangoldchase");
  shm_unlink("/Rohangoldshare");

}*/
return 0;

}
