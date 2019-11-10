#include <stdio.h>                                                                                                                                                                              
#include <string.h>                                                                                                                                                                             
int main(int argc, char *argv[])                                                                                                                                                                
{                                                                                                                                                                                               
        char buf[128];                                                                                                                                                                          
        FILE *fps, *fpd;                                                                                                                                                                        
        int num = 0;                                                                                                                                                                            
        char *ch;  
		if(argc < 3)                                                                                                                                                                            
        {                                                                                                                                                                                       
                printf("Usage:%s <src_file> <dst_file>\n", argv[0]);                                                                                                                                                                                        
        }   
        if((fps = fopen(argv[1], "r")) == NULL)                                                                                                                                                 
        {                                                                                                                                                                                       
                perror("fopen faile\n");                                                                                                                                                        
                return -1;                                                                                                                                                                      
        }                                                                                                                                                                                       
        if((fpd = fopen(argv[2], "w")) == NULL)                                                                                                                                                 
        {                                                                                                                                                                                       
                perror("fopen faile\n");                                                                                                                                                        
                return -1;                                                                                                                                                                      
        }                                                                                                                                                                                       
        memset(buf, 0, 128);                                                                                                                                                                    
        ch = fgets(buf, 128, fps);                                                                                                                                                              
        while(ch != NULL)                                                                                                                                                                       
        {                                                                                                                                                                                       
                fputs(buf, fpd);                                                                                                                                                                
                memset(buf, 0, 128);                                                                                                                                                            
                ch = fgets(buf, 128, fps);                                                                                                                                                      
        }                                                                                                                                                                                       
        fclose(fps);                                                                                                                                                                            
        fclose(fpd);                                                                                                                                                                            
    
        return 0;                                                                                                                                                                               
}                                                                                                                                                                                               
~     