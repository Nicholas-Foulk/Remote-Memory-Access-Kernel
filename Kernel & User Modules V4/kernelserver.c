#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <net/sock.h>
#include <linux/string.h>
#include <linux/inet.h>

/*
Sendbuffer sends "Length" bytes from "Buffer" through the socket "sock".


*/

size_t SendBuffer(struct socket *sock, const char *Buffer, size_t Length)
{
struct msghdr msg;
mm_segment_t oldfs; // mm_segment_t is just a long
struct iovec iov; // structure containing a base addr. and length
int len2;

printk("Server:Entering SendBuffer Server\n");


msg.msg_name = 0;
msg.msg_namelen = 0;
msg.msg_iov = &iov;
msg.msg_iovlen = 1; //point to be noted
msg.msg_control = NULL;
msg.msg_controllen = 0;

msg.msg_flags = MSG_NOSIGNAL;//0/*MSG_DONTWAIT*/;

iov.iov_base = (char*) Buffer; // as we know that iovec is
iov.iov_len = (__kernel_size_t) Length; // nothing but a base addr and length



// #define get_fs() (current_thread_info()->addr_limit)
// similar for set_fs;
/*
Therefore this line sets the "fs" to KERNEL_DS and saves its old value
*/
oldfs = get_fs(); set_fs(KERNEL_DS);

/* Actual Sending of the Message */
len2 = sock_sendmsg(sock,&msg,(size_t)(Length));

/* retrieve the old value of fs (whatever it is)*/
set_fs(oldfs);


return len2;
}


/*
Recieves data from the socket "sock" and puts it in the 'Buffer'.
Returns the length of data recieved

The Calling function must do a:
Buffer = (char*) get_free_page(GFP_KERNEL);
or a kmalloc to allocate kernel's memory
(or it can use the kernel's stack space [very small] )

*/


size_t RecvBuffer(struct socket *sock, const char *Buffer, size_t Length)
{
struct msghdr msg;
struct iovec iov;

int len;
mm_segment_t oldfs;
printk("Server:Entering Rec. Buffer Server");
/* Set the msghdr structure*/
msg.msg_name = 0;
msg.msg_namelen = 0;
msg.msg_iov = &iov;
msg.msg_iovlen = 1;
msg.msg_control = NULL;
msg.msg_controllen = 0;
msg.msg_flags = 0;

/* Set the iovec structure*/
iov.iov_base = (void *) &Buffer[0];
iov.iov_len = (size_t)Length;

/* Recieve the message */
oldfs = get_fs(); set_fs(KERNEL_DS);
len = sock_recvmsg(sock,&msg,Length,0/*MSG_DONTWAIT*/); // let it wait if there is no message
set_fs(oldfs);
printk("Server msg sent:- %s", Buffer);
//if ((len!=-EAGAIN)&&(len!=0)){
printk("Server:RecvBuffer Recieved %i bytes \n",len);

printk("Server:finish recvbuffer");
return len;
}



/*
Sets up a server-side socket

1. Create a new socket
2. Bind the address to the socket
3. Start listening on the socket
*/

struct socket* set_up_server_socket(int port_no) {
struct socket *sock;
struct sockaddr_in sin;
unsigned int serv_addr;
int error, a;


serv_addr = 168430083;
/* First create a socket */
error = sock_create(PF_INET,SOCK_STREAM,IPPROTO_TCP,&sock) ;
if (error<0)
printk("Server:Error during creation of socket; terminating\n");



/* Now bind the socket */
printk("Server:successfully set up socket, now binding socket\n");
sin.sin_family = AF_INET;
sin.sin_addr.s_addr = htonl(serv_addr);
sin.sin_port = htons(port_no);

a = sock->ops->bind(sock,(struct sockaddr*)&sin,sizeof(sin));
if (a<0)
{
printk("Server:Error binding socket \n");
return 0;
}
printk("Server:successfully bind socket, now preparing to listen \n");
/* Now, start listening on the socket */
error=sock->ops->listen(sock,32);

if (error!=0)
printk("Server:Error listening on socket \n");

/* Now start accepting */
// Accepting is performed by the function server_accept_connection

return sock;
}


/*

Accepts a new connection (server calls this function)

1. Create a new socket
2. Call socket->ops->accept
3. return the newly created socket

*/

struct socket* server_accept_connection(struct socket* sock) {

struct socket* newsock;
int error;

/* Before accept: Clone the socket */

error = sock_create(PF_INET,SOCK_STREAM,IPPROTO_TCP,&newsock);
if (error<0)
printk("Server:Error during creation of the other socket; terminating\n");

newsock->type = sock->type;
newsock->ops=sock->ops;

/* Do the actual accept */

error = newsock->ops->accept(sock,newsock,0);


if (error<0) {
printk("Server:Error accepting socket\n") ;
return 0;
}
return newsock;
}




struct socket* set_up_client_socket( int IP_addr, int port_no)
{
struct socket * clientsock;
struct sockaddr_in sin;
int error, i;

/* First create a socket */
error = sock_create(PF_INET,SOCK_STREAM,IPPROTO_TCP,&clientsock);
if (error<0) {
printk("Server:Error during creation of socket; terminating\n");
return 0;
}

/* Now bind and connect the socket */
sin.sin_family = AF_INET;
sin.sin_addr.s_addr = htonl(IP_addr);
sin.sin_port = htons(port_no);

for(i=0;i<10;i++) {
error = clientsock->ops->connect(clientsock,(struct sockaddr*)&sin,sizeof(sin),0);
if (error<0) {
printk("Server:Error connecting client socket to server: %i, retrying .. %d \n",error, i);
if(i==10-1) {
printk("Server:Giving Up!\n"); return 0;
}
}
else break; //connected
}

return clientsock;

}

static int module7_init(void)
{
int port= 13394;
struct socket* serversocket;
struct socket* newsocks;
size_t recieving, lens;

char * Buffer;

Buffer = (char*)__get_free_page(GFP_KERNEL);

lens=strlen(Buffer); 

serversocket= set_up_server_socket(port);
newsocks= server_accept_connection(serversocket); 

recieving = RecvBuffer(newsocks, Buffer, lens);
if (recieving<=0)
{
printk(KERN_INFO "Server:error when receiving message");
}
else
{
printk(KERN_INFO "Server received msg: %s",Buffer);
}
return 0;
}
static void module7_exit(void)
{
printk(KERN_ALERT "Server:The struggle is fucken real!");
return;
}


module_init(module7_init);
module_exit(module7_exit);
MODULE_AUTHOR("thinh");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("blahblah");

