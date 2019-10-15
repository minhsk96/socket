#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <ctype.h>
#define PORT 8080 

//https://viblo.asia/p/lap-trinh-socket-giao-tiep-client-va-server-bang-ngon-ngu-c-voi-giao-thuc-tcp-bWrZng29lxw
// https://pwnpp4fun.wordpress.com/2016/03/01/client-server-don-gian
// https://voer.edu.vn/m/xay-dung-ung-dung-client-server-voi-socket/c8be33e3
// https://kipalog.com/posts/Tim-hieu-ve-lap-trinh-socket---Buoc-dau-lam-quen
void ToUp( char *p ) 
{ 
    while( *p ) 
    { 
        *p=toupper( *p ); 
        p++; 
    } 
} 

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; // sockaddr_in là struct  đặc biệt dùng riêng để lưu thông tin địa chỉ kết nốt của thiết bị đầu cuối kết nối vào socket.
    int opt = 1; 
    int addrlen = sizeof(address); 
    char mess_from_client[225];
    char buffer[1024] = {0}; 
    char *hello = "Hello from server";
    char *update = "update from server";
    char *a = "abc";
    int continu = 1;
    //tao socket
    // tao file mo ta soket các tham số trong đó theo thứ tự là họ giao thức, kiểu socket, kiểu giao thức.
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    }  
    //printf("%d\n",server_fd);
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY: là địa chỉ ip đặc biệt
    address.sin_port = htons( PORT );  //gan cong la 8080   
    // bind 
    // Gán địa chỉ cho socket. các tham số lần lượt là mô tả socket vừa tạo, con trỏ chỏ đến địa chỉ socket, độ lớn địa chỉ
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
    //listen, chi dinh socket lang nghe ket noi
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    //accept, chap nhan ket noi
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
   while(continu == 1){    
	    char str_cli_ip[INET_ADDRSTRLEN];
	    struct sockaddr_in* ip_client = (struct sockaddr_in*)&address;
	    inet_ntop(AF_INET, &ip_client->sin_addr, str_cli_ip, INET_ADDRSTRLEN); // chuyển đổi địa chỉ ip v4 or v6 thành một chuỗi định dạng tiêu chuẩn
	    printf("ipclient: %s\n", str_cli_ip );
	    char str_cli_port[INET_ADDRSTRLEN];
	    printf("port: %d\n", ntohs(ip_client->sin_port));
	    printf("Tin nhan ban nhan dc tu client: \n");
	    //read, doc du lieu gan vao bien valread tra ve so byte ma no doc duoc
	    valread = read( new_socket, buffer, 1024);
	    

        if(strcmp( buffer, a)==0){
            printf("send news update");
            send(new_socket, update, strlen(update), 0 );
            //return 0;
        }
        //viet hoa
        ToUp(buffer); 
        //printf("-------%d",valread);
	    //gan bien hello tra ve cho client la buffer da viet hoa
	    hello = &buffer;
	    printf("%s\n",buffer ); 
	    send(new_socket, hello, strlen(hello), 0 ); 
    }
    close(new_socket);
    return 0; 
} 
