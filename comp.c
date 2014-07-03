#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("snk compression\n");
    
    if (argc < 3)
    {
      printf("\nUsage: %s [input] [output] (d)\n",argv[0]);  
      printf("Warning: input must be divisible by 32 bytes,"
                 "\n\t and must be smaller than 65535 bytes.");
      return 0;     
    }
    FILE *fp,*fp2;
    int len,len1,len2;
    fp=fopen(argv[1],"rb");
    
    fseek(fp, 0L, SEEK_END);
    len = ftell(fp);
    len1 = len;
    fseek(fp, 0L, SEEK_SET);
    
    fp2=fopen(argv[2],"wb+");
    long d[7];
    if (fp == NULL || fp2 == NULL) {
            printf("File error maybe\n");
            return 0;
    }
	
	int x,y,i;
	
    if (argc > 3) {
        x = fgetc(fp);
		x <<= 8;
		x += fgetc(fp);
		x *= 32; /* number of bytes to decompress */
		len2 = x; /* result (original in len1) */
		len = x;
    
		i=0;
    
		x = fgetc(fp);
		fprintf(fp2, "%c",x);
		len--;
		while (len>0) {
			y = fgetc(fp);
			if (x==y) {
				fprintf(fp2, "%c",y);
				len--;
				y = fgetc(fp);
				if (y==255) i=1;
				for(;y>0&&len>0;y--)
					{ fprintf(fp2, "%c",x); len--; }
				if(i&&len>0) {x = fgetc(fp);fprintf(fp2, "%c",x);len--;i=0;}
			}
			else {
				fprintf(fp2, "%c",y);
				len--;
				x = y;
			}
		}
    printf("decompression complete; %d => %d bytes",len1,len2);
    fclose(fp); fclose(fp2);
    }
	
	else {
	
		/* header; word - length of file/0x20 <-- will only work with art
		ideas; nybble - spare space, 3*nybble - length/0x20
    
	    write header */
		len1 /= 32;
		fprintf(fp2, "%c",len1>>8);
		fprintf(fp2, "%c",len1);   
		len1 = len;
    
		x = fgetc(fp);
		for(;len>0;len--){
    
		y = fgetc(fp);
		if (x==y) {
			fprintf(fp2, "%c%c",x,x);
			i=0;
			x = fgetc(fp);
			len--;
			while (x==y&&i<255) {
				i++;
				x = fgetc(fp);
				len--;
				if(len<0) goto lolgoto;
            }
			fprintf(fp2, "%c",i);
        }
		else {
			fprintf(fp2, "%c",x);
			x=y;
        }
		}
lolgoto:
    len2 = ftell(fp2);
    printf("compression complete; %d => %d bytes",len1,len2);
    fclose(fp); fclose(fp2);
	}
    return 0;
}
