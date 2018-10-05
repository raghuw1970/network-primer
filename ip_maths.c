#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void get_broadcast_address(char* ip_add, int prefix, char output_buffer[]){
	unsigned a, b, c, d;
	printf("IP Address in context: %s \n", ip_add);
	printf("Mask in Context: %d \n", prefix);
	//Scan IP address into the 4 Octet values
	int success = sscanf(ip_add, "%3u.%3u.%3u.%3u", &a, &b, &c, &d);
	unsigned long mask = (0xFFFFFFFF << (32 - prefix)) & 0xFFFFFFFF;

	unsigned long m1 = (mask >> 24);
	unsigned long m2 = ((mask >> 16) & 0xFF);
	unsigned long m3 = ((mask >> 8) & 0xFF);
	unsigned long m4 = (mask & 0xFF);
	printf("Subnet Mask: %d.%d.%d.%d\n", m1, m2, m3, m4);

	printf("Inverted Subnet Mask %d.%d.%d.%d\n", ~m1 & 0xFF, ~m2 & 0xFF, ~m3 & 0xFF, ~m4 & 0xFF);

	//OR individual octet values of IP address with the inverted mask (integer mask obtained from left shifting by 24,16,8,0 bits 
	printf("Broadcast Address: %d.%d.%d.%d\n", a | (~m1) & 0xFF, b | (~m2) & 0xFF, c | (~m3) & 0xFF, d | (~m4) & 0xFF);
	sprintf(output_buffer, "%d.%d.%d.%d", a | (~m1) & 0xFF, b | (~m2) & 0xFF, c | (~m3) & 0xFF, d | (~m4) & 0xFF);
}


unsigned int get_ip_integeral_equivalent(char *ip_address){
	int a, b, c, d;
	int success = sscanf(ip_address, "%3u.%3u.%3u.%3u", &a, &b, &c, &d);
	//Left Shift bits by 24,16,8,0
	int inta = a << 24 ;
	int intb = b << 16 ;
	int intc = c << 8;
	int intd = d << 0;
	return(inta+intb+intc+intd);
}

void get_abcd_ip_format(unsigned int ip_address, char *output_buffer){
	//Right shift bits by 24,16,8,0
	sprintf(output_buffer, "%u.%u.%u.%u", 
		((ip_address >> 24) & 0xFF) ,
		((ip_address >> 16) & 0xFF) ,
		((ip_address >> 8) & 0xFF) ,
		((ip_address >> 0) & 0xFF) );
		
}

void get_network_id(char* ip_addr, char prefix, char* output_buffer){
	
	unsigned a, b, c, d;
	printf("IP Address in context: %s \n", ip_addr);
	printf("Mask in Context: %d \n", prefix);
	int success = sscanf(ip_addr, "%3u.%3u.%3u.%3u", &a, &b, &c, &d);
	unsigned long mask = (0xFFFFFFFF << (32 - prefix)) & 0xFFFFFFFF;

	unsigned long m1 = (mask >> 24);
	unsigned long m2 = ((mask >> 16) & 0xFF);
	unsigned long m3 = ((mask >> 8) & 0xFF);
	unsigned long m4 = (mask & 0xFF);
	printf("Subnet Mask: %d.%d.%d.%d\n", m1, m2, m3, m4);

	//AND individual octet values of IP address with the inverted mask (integer mask obtained from right shifting by 24,16,8,0 bits 
	printf("Network/Subnet Id: %d.%d.%d.%d\n", a & m1 , b & m2, c & m3, d & m4);
	sprintf(output_buffer, " %d.%d.%d.%d", a & m1 , b & m2, c & m3, d & m4);
}

unsigned int get_subnet_cardinality(char mask){
	//Formula is 2^(32-maskValue)-2;
	return pow(2,(32 - mask)) - 2;
}


int check_ip_subnet_membership(char* network_id, char prefix, char *check_ip){

	//Apply mask on ip address, if it results in the network id, then return 0, else return 1
	
	unsigned a, b, c, d;
	int success = sscanf(check_ip, "%3u.%3u.%3u.%3u", &a, &b, &c, &d);
	unsigned long mask = (0xFFFFFFFF << (32 - prefix)) & 0xFFFFFFFF;

	unsigned long m1 = (mask >> 24);
	unsigned long m2 = ((mask >> 16) & 0xFF);
	unsigned long m3 = ((mask >> 8) & 0xFF);
	unsigned long m4 = (mask & 0xFF);


	char* out_buffer;
	asprintf(&out_buffer, "%d.%d.%d.%d", a & m1 , b & m2, c & m3, d & m4);


	int check = strcmp(out_buffer, network_id);
	free(out_buffer);
	return check;
} 	

int main(){
	int PREFIX_LEN = 16;
	char ipadd_buffer[PREFIX_LEN];
	memset(ipadd_buffer, 0, PREFIX_LEN);
	char* ip_add = "10.1.23.10";
	int prefix = 20;
	get_broadcast_address(ip_add, prefix, ipadd_buffer);
	printf("Broadcast Address = %s\n", ipadd_buffer);
	char* ip_address = "10.1.23.10";
	printf("Integer equivalent of %s is %u\n" , ip_address, get_ip_integeral_equivalent(ip_address));

	ip_address = "192.168.2.10";
	printf("Integer equivalent of %s is %u\n" , ip_address, get_ip_integeral_equivalent(ip_address));

	unsigned int ip_add_int = 2058138165;
	char ipadd_buf[PREFIX_LEN];
	memset(ipadd_buf, 0, PREFIX_LEN);
	get_abcd_ip_format(ip_add_int, ipadd_buf);
	printf("Ip in A.B.C.D format is = %s \n", ipadd_buf);
	
	unsigned char mask = 20;
	ip_address = "192.168.2.10";

	get_network_id(ip_address, mask, ipadd_buf);
	printf("Network Id = %s\n", ipadd_buf);


	mask = 24;
	printf("Subnet cardinality for mask %u is %u\n", mask, get_subnet_cardinality(mask));

	char* network_id = "192.168.0.0";
	mask = 24;
	char* check_ip = "192.168.1.13";

	int result = check_ip_subnet_membership(network_id, mask, check_ip);
	if (result == 0)
		printf( "ip address = %s is a member of subnet %s/%u \n", check_ip, network_id, mask );
	else 
		printf( "ip address = %s is not a member of subnet %s/%u \n", check_ip, network_id, mask );
}
