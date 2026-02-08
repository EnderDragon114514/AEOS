/*
AEOS v5.11.2 Kernel and TTy
This version is still a beta version(not stable),which means there will be some bugs,and it will be fixed in future updates
Note:STOP ASKING ME TO MAKE THE LIBRARY EXTENSION FOR RBVM!I WILL IMPLENT IT!
Bug report:send an email to lithium-offical@outlook.com or commit in github
*/

/*
Future update implentions:
1. RbVM graphics supports
2. Pagefile(similar to Linux swapfile or swap parition and Windows pagefile)
3. Hibernate(Requires pagefile)
4. More background process
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <dos.h>
extern "C"{
    #include "A:\memdef.h"
    #include "A:\memdef1.h"
    #include "A:\memdef2.h"
    #include "A:\memdef3.h"
}

int memi[maxProg][128];
float memf[maxProg][128];
char memc[maxProg][128];
int sectline[maxProg][64];//short interger to reduce memory usage
FILE* fin[maxProg];
int progs=0;
int clamp(int x,int a,int b) {
	if(x<a) {
		return a;
	}
	if(x>b) {
		return b;
	} else {
		return x;
	}
}
int get_uint()
{
	char inputBuf[64];
	int key=0,currentChr=0;
	memset(inputBuf,0,sizeof(inputBuf));
	while(1)
	{
		key=getch();
		if(key==8||key==127)
		{
			currentChr--;
			if(currentChr<0)
			{
				currentChr=0;
			}
			else
			{
				printf("\b \b");
				inputBuf[currentChr+1]=0;
			}

		}
		if(key>=48&&key<=57)
		{
			if(currentChr>=64)
			{
				currentChr=63;
				break;
			}
			inputBuf[currentChr++]=key;
			printf("%c",key);
		}
		if(key==3||key==9||key==10||key=='\r'||key=='\n')
		{
			inputBuf[currentChr]=0;
			break;
		}
		if(key==28)
		{
			return -2;
		}
	}
    return atoi(inputBuf);
}
int condition(int procid,int type, int id_a, const char *_cmd, int id_b) {
	if (type == 1) {
		if (strcmp(_cmd, "equ") == 0) {
			return memi[procid][id_a] == memi[procid][id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "grt") == 0) {
			return memi[procid][id_a] > memi[procid][id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "les") == 0) {
			return memi[procid][id_a] < memi[procid][id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "neq") == 0) {
			return memi[procid][id_a] != memi[procid][id_b] ? 1 : 0;
		} else {
			printf("Operation error\n");
			return -1;
		}
	} else if (type == 2) {
		if (strcmp(_cmd, "equ") == 0) {
			return memc[procid][id_a] == memc[procid][id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "grt") == 0) {
			return memc[procid][id_a] > memc[procid][id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "les") == 0) {
			return memc[procid][id_a] < memc[procid][id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "neq") == 0) {
			return memc[procid][id_a] != memc[procid][id_b] ? 1 : 0;
		} else {
			printf("Operation error\n");
			return -1;
		}
	} else if (type == 3) {
		if (strcmp(_cmd, "equ") == 0) {
			return memf[procid][id_a] == memf[procid][id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "grt") == 0) {
			return memf[procid][id_a] > memf[procid][id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "les") == 0) {
			return memf[procid][id_a] < memf[procid][id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "neq") == 0) {
			return memf[procid][id_a] != memf[procid][id_b] ? 1 : 0;
		} else {
			printf("Operation error\n");
			return -1;
		}
	} else {
		printf("Type error\n");
		return -1;
	}
}
void read_string(FILE *f, char *buffer) {
	int i = 0;
	int ch;
	while ((ch = fgetc(f)) != EOF && (ch == ' ' || ch == '\t'));
	if (ch == EOF) {
		buffer[0] = '\0';
		return;
	}
	while (ch != EOF && ch != '\n' && ch != '\r' && i < 64) {
		buffer[i++] = (char)ch;
		ch = fgetc(f);
	}
	buffer[i] = '\0';
}
void read_string_32(FILE *f, char *buffer) {
	int i = 0;
	int ch;
	while ((ch = fgetc(f)) != EOF && (ch == ' ' || ch == '\t'));
	if (ch == EOF) {
		buffer[0] = '\0';
		return;
	}
	while (ch != EOF && ch != '\n' && ch != '\r' && i < 32) {
		buffer[i++] = (char)ch;
		ch = fgetc(f);
	}
	buffer[i] = '\0';
}
int encode_procid(int pid, int section) {
	return 4 + (pid * 4) + section;
}
void decode_procid(int procid, int *pid, int *section) {
	if (procid <= 4) {
		*pid = procid;
		*section = -1;
	} else {
		int temp = procid - 4;
		*pid = (temp - 1) / 4;
		*section = temp % 4;
	}
}
int run(int procid,const char *_cmd) {
	char buffer[128];
	int a, b, c, d;
	if(procid>4&&procid<=20)
	{
		int procid_tr=0;
		int section_id=0;
        decode_procid(procid,&procid_tr,&section_id);
		if(section_id<=0)
		{
			printf("Stack error\n");
			return -1;
		}
        if(sectstate[procid_tr][section_id-1]==1)
		{
            if(sectline[procid_tr][section_id]<16)
			{
                read_string_32(fin[procid_tr],sect[procid_tr][section_id-1][sectline[procid_tr][section_id-1]]);
                sectline[procid_tr][section_id-1]++;
			}
            if(sectline[procid_tr][section_id-1]>=16||strcmp(sect[procid][section_id-1][clamp(sectline[procid_tr][section_id-1],0,31)],"section end")==0)
			{
				char *marker = ".SECTION_END";
				for(int i = 0; marker[i] != '\0' && i < 31; i++) {
					sect[procid_tr][section_id-1][clamp(sectline[procid_tr][section_id-1],0,31)][i] = marker[i];
				}
				sect[procid_tr][section_id-1][clamp(sectline[procid_tr][section_id-1],0,31)][strlen(marker)] = '\0';
                sectstate[procid_tr][section_id-1]=0;
			}
		}
        else if(sectstate[procid_tr][section_id-1]==2)
		{
            if(strcmp(sect[procid][section_id-1][sectline[procid_tr][section_id-1]],".SECTION_END")!=0)
			{
                run(procid_tr,sect[procid][section_id-1][sectline[procid_tr][section_id-1]]);
			}
			else
			{
                sectstate[procid_tr][section_id-1]=0;
			}
		}
	}
	else if(strcmp(_cmd,"section")==0)
	{
		char* op;
		int a;
		fscanf(fin[procid], "%s %d", op, &a);
		if(strcmp(op,"set")==0)
		{
			if(a<1||a>4)
			{
				printf("Stack error\n");
				return -1;
			}
            sectstate[procid][a-1]=1;
		}
		else if(strcmp(op,"run")==0)
		{
			if(a<1||a>4)
			{
				printf("Stack error\n");
				return -1;
			}
            sectline[procid][a-1]=0;
            sectstate[procid][a-1]=2;
		}
		else
		{
			printf("Invalid operation\n");
		}
	}
	else if (strcmp(_cmd, "nop") == 0) {
		read_string(fin[procid], buffer);
		return 1;
	} else if (strcmp(_cmd, "end") == 0) {
		return 0;
	} else if (strcmp(_cmd, "set") == 0) {
		fscanf(fin[procid], "%d %d %d", &a, &b, &c);
		if (a < 0 || a > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (c == 1) {
			memi[procid][a] = b;
		} else if (c == 2) {
			memc[procid][a] = (char)b;
		} else if (c == 3) {
			memf[procid][a] = (float)b;
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "copy") == 0) {
		fscanf(fin[procid], "%d %d %d", &a, &b, &c);
		if (a < 0 || a > 127 || b < 0 || b > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (c == 1) {
			memi[procid][b] = memi[procid][a];
		} else if (c == 2) {
			memc[procid][b] = memc[procid][a];
		} else if (c == 3) {
			memf[procid][b] = memf[procid][a];
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "convert") == 0) {
		fscanf(fin[procid], "%d %d %d %d", &a, &b, &c, &d);
		if (a < 0 || a > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (b == 1) {
			if (d == 1) {
				memi[procid][c] = memi[procid][a];
			} else if (d == 2) {
				memc[procid][c] = (char)memi[procid][a];
			} else if (d == 3) {
				memf[procid][c] = (float)memi[procid][a];
			} else {
				printf("Type error\n");
				return -1;
			}
		} else if (b == 2) {
			if (d == 1) {
				memi[procid][c] = (int)memc[procid][a];
			} else if (d == 2) {
				memc[procid][c] = memc[procid][a];
			} else if (d == 3) {
				memf[procid][c] = (float)memc[procid][a];
			} else {
				printf("Type error\n");
				return -1;
			}
		} else if (b == 3) {
			if (d == 1) {
				memi[procid][c] = (int)memf[procid][a];
			} else if (d == 2) {
				memc[procid][c] = (char)memf[procid][a];
			} else if (d == 3) {
				memf[procid][c] = memf[procid][a];
			} else {
				printf("Type error\n");
				return -1;
			}
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "plus") == 0) {
		fscanf(fin[procid], "%d %d %d %d", &a, &b, &c, &d);
		if (a < 0 || a > 127 || b < 0 || b > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (d == 1) {
			memi[procid][c] = memi[procid][a] + memi[procid][b];
		} else if (d == 2) {
			memc[procid][c] = memc[procid][a] + memc[procid][b];
		} else if (d == 3) {
			memf[procid][c] = memf[procid][a] + memf[procid][b];
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "minus") == 0) {
		fscanf(fin[procid], "%d %d %d %d", &a, &b, &c, &d);
		if (a < 0 || a > 127 || b < 0 || b > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (d == 1) {
			memi[procid][c] = memi[procid][a] - memi[procid][b];
		} else if (d == 2) {
			memc[procid][c] = memc[procid][a] - memc[procid][b];
		} else if (d == 3) {
            memf[procid][c] = memf[procid][a] - memf[procid][b];
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "times") == 0) {
		fscanf(fin[procid], "%d %d %d %d", &a, &b, &c, &d);
		if (a < 0 || a > 127 || b < 0 || b > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (d == 1) {
			memi[procid][c] = memi[procid][a] * memi[procid][b];
		} else if (d == 2) {
			memc[procid][c] = memc[procid][a] * memc[procid][b];
		} else if (d == 3) {
			memf[procid][c] = memf[procid][a] * memf[procid][b];
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "divide") == 0) {
		fscanf(fin[procid], "%d %d %d %d", &a, &b, &c, &d);
		if (a < 0 || a > 127 || b < 0 || b > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (d == 1) {
			if (memi[procid][b] != 0) {
				memi[procid][c] = memi[procid][a] / memi[procid][b];
			} else {
				printf("Divide by zero error\n");
				return -1;
			}
		} else if (d == 2) {
			if (memc[procid][b] != 0) {
				memc[procid][c] = memc[procid][a] / memc[procid][b];
			} else {
				printf("Divide by zero error\n");
				return -1;
			}
		} else if (d == 3) {
			if (memf[procid][b] != 0.0f) {
				memf[procid][c] = memf[procid][a] / memf[procid][b];
			} else {
				printf("Divide by zero error\n");
				return -1;
			}
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "mod") == 0) {
		fscanf(fin[procid], "%d %d %d", &a, &b, &c);
		if (a < 0 || a > 127 || b < 0 || b > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (memi[procid][b] != 0) {
			memi[procid][c] = memi[procid][a] % memi[procid][b];
		} else {
			printf("Divide by zero error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "get") == 0) {
		fscanf(fin[procid], "%d %d", &a, &b);
		if (a < 0 || a > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (b == 1) {
			char buf[64];char inputBuf[64];
			int key=0,currentChr=0;
			memset(inputBuf,0,sizeof(inputBuf));
			while(1)
			{
				key=getch();
				if(key==8||key==127)
				{
					currentChr--;
					if(currentChr<0)
					{
						currentChr=0;
					}
					else
					{
						printf("\b \b");
						inputBuf[currentChr+1]=0;
					}

				}
				if((key>=48&&key<=57)||key=='-')
				{
					if(currentChr>=64)
					{
						currentChr=63;
						break;
					}
					inputBuf[currentChr++]=key;
					printf("%c",key);
				}
				if(key==3||key==9||key==10||key=='\r'||key=='\n')
				{
					inputBuf[currentChr]=0;
					break;
				}
				if(key==26)
				{
					return -3;
				}
				if(key==28)
				{
					return -2;
				}
			}
			memi[procid][a]=atoi(inputBuf);
		} else if (b == 2) {
			int _a=getch();
			if(_a==26)
			{
				return -3;
			}
			if(_a==28)
			{
				return -2;
			}
			memc[procid][a]=_a;
		} else if (b == 3) {
			char buf[64];char inputBuf[64];
			int key=0,currentChr=0;
			memset(inputBuf,0,sizeof(inputBuf));
			while(1)
			{
				key=getch();
				if(key==8||key==127)
				{
					currentChr--;
					if(currentChr<0)
					{
						currentChr=0;
					}
					else
					{
						printf("\b \b");
						inputBuf[currentChr+1]=0;
					}

				}
                if((key>=48&&key<=57)||(key=='-')||(key=='.'))
				{
					if(currentChr>=64)
					{
						currentChr=63;
						break;
					}
					inputBuf[currentChr++]=key;
					printf("%c",key);
				}
				if(key==3||key==9||key==10||key=='\r'||key=='\n')
				{
					inputBuf[currentChr]=0;
					break;
				}
				if(key==26)
				{
					return -3;
				}
				if(key==28)
				{
					return -2;
				}
			}
			memf[procid][a]=atof(inputBuf);
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "put") == 0) {
		fscanf(fin[procid], "%d %d", &a, &b);
		if (a < 0 || a > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (b == 1) {
			printf("%d\n", memi[procid][a]);
		} else if (b == 2) {
			printf("%c\n", memc[procid][a]);
		} else if (b == 3) {
			printf("%f\n", memf[procid][a]);
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "sset") == 0) {
		fscanf(fin[procid], "%d", &a);
		read_string(fin[procid], buffer);
        if (a < 128 || a > 192) {
			printf("Memory error\n");
			return -1;
		}
		a -= 128;
		strncpy(mems[procid][a], buffer, 127);
		mems[procid][a][127] = '\0';
	} else if (strcmp(_cmd, "scopy") == 0) {
		fscanf(fin[procid], "%d %d", &a, &b);
        if (a < 128 || a > 192 || b < 128 || b > 192) {
			printf("Memory error\n");
			return -1;
		}
		a -= 128;
		b -= 128;
		strncpy(mems[procid][b], mems[procid][a], 127);
		mems[procid][b][127] = '\0';
	} else if (strcmp(_cmd, "splus") == 0) {
		fscanf(fin[procid], "%d %d %d", &a, &b, &c);
        if (a < 128 || a > 192 || b < 128 || b > 192 || c < 128 || c > 192) {
			printf("Memory error\n");
			return -1;
		}
		a -= 128;
		b -= 128;
		c -= 128;
		strncpy(mems[procid][c], mems[procid][a], 127);
		strncat(mems[procid][c], mems[procid][b], 127 - strlen(mems[procid][c]));
		mems[procid][c][127] = '\0';
	} else if (strcmp(_cmd, "sget") == 0) {
		fscanf(fin[procid], "%d", &a);
		if (a < 128 || a > 255) {
			printf("Memory error\n");
			return -1;
		}
		a -= 128;
		fflush(stdin);
		char buf[64];char inputBuf[64];
		int key=0,currentChr=0;
		memset(inputBuf,0,sizeof(inputBuf));
		while(1)
		{
			key=getch();
			if(key==8||key==127)
			{
				currentChr--;
				if(currentChr<0)
				{
					currentChr=0;
				}
				else
				{
					printf("\b \b");
					inputBuf[currentChr+1]=0;
				}

			}
			if(key>=32&&key<=126)
			{
				if(currentChr>=64)
				{
					currentChr=63;
					break;
				}
				inputBuf[currentChr++]=key;
				printf("%c",key);
			}
			if(key==3||key==9||key==10||key=='\r'||key=='\n')
			{
				inputBuf[currentChr]=0;
				break;
			}
			if(key==26)
			{
				return -3;
			}
			if(key==28)
			{
				return -2;
			}
		}
		mems[procid][a][strcspn(mems[procid][a], "\r\n")] = '\0';
	} else if (strcmp(_cmd, "sput") == 0) {
		fscanf(fin[procid], "%d", &a);
        if (a < 128 || a > 192) {
			printf("Memory error\n");
			return -1;
		}
		a -= 128;
		printf("%s\n", mems[procid][a]);
	} else if (strcmp(_cmd, "ssize") == 0) {
		fscanf(fin[procid], "%d %d", &a, &b);
        if (a < 128 || a > 192 || b < 0 || b > 127) {
			printf("Memory error\n");
			return -1;
		}
		a -= 128;
		memi[procid][b] = strlen(mems[procid][a]);
	} else if (strcmp(_cmd, "if") == 0) {
		int type, id_a, id_b;
		char op[10], cmd1[128], cmd2[128];
		fscanf(fin[procid], "%d %d %s %d", &type, &id_a, op, &id_b);
		if (id_a < 0 || id_a > 127 || id_b < 0 || id_b > 127) {
			printf("Memory error\n");
			return -1;
		}
		int ret = condition(procid,type, id_a, op, id_b);
		if (ret == -1) {
			return -1;
		} else if (ret == 1) {
			fscanf(fin[procid], "%s", cmd1);
			return run(procid,cmd1);
		} else if (ret == 0) {
			fscanf(fin[procid], "%s", cmd2);
			return run(procid,cmd2);
		} else {
			printf("Unknown error\n");
			return -1;
		}
	}
	else if(strcmp(_cmd,"exec")==0){
		  int a;
		  fscanf(fin[procid],"%d",&a);
          if(a<128||a>192)
		  {
				printf("Memory error");
				return -1;
		  }
		  int ret=run(procid,mems[procid][a]);
		  return ret;
	 }
	 else if (strcmp(_cmd, "while") == 0) {
		int type, id_a, id_b;
		char op[10], cmd[128];
		fscanf(fin[procid], "%d %d %s %d %s", &type, &id_a, op, &id_b, cmd);
		if (id_a < 0 || id_a > 127 || id_b < 0 || id_b > 127) {
			printf("Memory error\n");
			return -1;
		}
		while (1) {
			int con = condition(procid,type, id_a, op, id_b);
			if (con == -1) {
				return -1;
			} else if (con == 0) {
				break;
			} else if (con == 1) {
				int ret = run(procid,cmd);
				if (ret == -1) {
					return -1;
				} else if (ret == 0) {
					return 0;
				} else if (ret == 1) {
					continue;
				}
			}
		}
	} else if(strcmp(_cmd,"getkey")==0) {
		int id_a;
		fscanf(fin[procid],"%d",&id_a);
		if(id_a<0||id_a>127) {
			printf("Memory error\n");
			return -1;
		}
		memc[procid][id_a]=getch();
	}
	else if(strcmp(_cmd,"loadlib")==0)
	{
		char* libname;
		fscanf(fin[procid],"%s ",libname);
		if(strcmp(libname,"extmath.lib")==0)
		{
			fscanf(fin[procid],"%s",_cmd);
			if(strcmp(_cmd,"sqrt")==0)
			{
				int a,b,c;
				fscanf(fin[procid],"%d %d %d",&a,&b,&c);
				if(b<0||b>127||c<0||c>127)
				{
					printf("Memory error\n");
					return -1;
				}
				if(a==1)
				{
					memi[procid][c]=sqrt(memi[procid][b]);
				}
				else if(a==2)
				{
					memc[procid][c]=sqrt(memc[procid][b]);
				}
				else if(a==3)
				{
					memf[procid][c]=sqrt(memf[procid][b]);
				}
				else
				{
					printf("Type error\n");
					return -1;
				}
			}
			else if(strcmp(_cmd,"pow")==0)
			{
				int a,b,c,d;
				fscanf(fin[procid],"%d %d %d %d",&a,&b,&c,&d);
				if(b<0||b>127||c<0||c>127||d<0||d>127)
				{
					printf("Memory error\n");
					return -1;
				}
				if(a==1)
				{
					memi[procid][d]=pow(memi[procid][b],memi[procid][c]);
				}
				else if(a==2)
				{
					memc[procid][d]=pow(memc[procid][b],memc[procid][c]);
				}
				else if(a==3)
				{
					memf[procid][d]=pow(memf[procid][b],memf[procid][c]);
				}
				else
				{
					printf("Type error\n");
					return -1;
				}
			}
			else if(strcmp(_cmd,"sin")==0)
			{
				int a,b,c;
				fscanf(fin[procid],"%d %d %d",&a,&b,&c);
				if(b<0||b>127||c<0||c>127)
				{
					printf("Memory error\n");
					return -1;
				}
				if(a==1)
				{
					memi[procid][c]=sin(memi[procid][b]);
				}
				else if(a==2)
				{
					memc[procid][c]=sin(memc[procid][b]);
				}
				else if(a==3)
				{
					memf[procid][c]=sin(memf[procid][b]);
				}
				else
				{
					printf("Type error\n");
					return -1;
				}
			}
			else if(strcmp(_cmd,"cos")==0)
			{
				int a,b,c;
				fscanf(fin[procid],"%d %d %d",&a,&b,&c);
				if(b<0||b>127||c<0||c>127)
				{
					printf("Memory error\n");
					return -1;
				}
				if(a==1)
				{
					memi[procid][c]=cos(memi[procid][b]);
				}
				else if(a==2)
				{
					memc[procid][c]=cos(memc[procid][b]);
				}
				else if(a==3)
				{
					memf[procid][c]=cos(memf[procid][b]);
				}
				else
				{
					printf("Type error\n");
					return -1;
				}
			}
			else if(strcmp(_cmd,"tan")==0)
			{
				int a,b,c;
				fscanf(fin[procid],"%d %d %d",&a,&b,&c);
				if(b<0||b>127||c<0||c>127)
				{
					printf("Memory error\n");
					return -1;
				}
				if(a==1)
				{
					int a1=memi[procid][b];
					if (abs(memi[procid][b]) > PI/2) {
                        a1 = fmod(memi[procid][b],PI);
						if (a1 > (PI/2)) a1 -= PI;
						if (a1 < -1*(PI/2)) a1 += PI;
					}
					if (abs(abs(a1) - PI/2) < 1e-10) {
						printf("Tan calculation error\n");
						return -1;
					}
					if (abs(cos(a1)) < 1e-10) {
						printf("Tan calculation error\n");
						return -1;
					}
					memi[procid][c]=tan(memi[procid][b]);
				}
				else if(a==2)
				{
					int a2=memc[procid][b];
					if (abs(memc[procid][b]) > PI/2) {
                        a2 = fmod(memc[procid][b],PI);
						if (a2 > (PI/2)) a2 -= PI;
						if (a2 < -1*(PI/2)) a2 += PI;
					}
					if (abs(abs(a2) - PI/2) < 1e-7) {
						printf("Tan calculation error\n");
						return -1;
					}
					if (abs(cos(a2)) < 1e-7) {
						printf("Tan calculation error\n");
						return -1;
					}
					memc[procid][c]=tan(memc[procid][b]);
				}
				else if(a==3)
				{
					int a3=memf[procid][b];
					if (fabs(memf[procid][b]) > PI/2) {
						a3 = fmod(memf[procid][b],PI);
						if (a3 > (PI/2)) a3 -= PI;
						if (a3 < -1*(PI/2)) a3 += PI;
					}
					if (fabs(fabs(a3) - PI/2) < 1e-7) {
						printf("Tan calculation error\n");
						return -1;
					}
					if (fabs(cos(a3)) < 1e-7) {
						printf("Tan calculation error\n");
						return -1;
					}
					memf[procid][c]=tan(memf[procid][b]);
				}
				else
				{
					printf("Type error\n");
					return -1;
				}
			}
			else if(strcmp(_cmd,"log")==0)
			{
				int a,b,c;
				fscanf(fin[procid],"%d %d %d",&a,&b,&c);
				if(b<0||b>127||c<0||c>127)
				{
					printf("Memory error\n");
					return -1;
				}
				if(a==1)
				{
					if(memi[procid][b]<=0)
					{
						printf("log invalid parameter\n");
						return -1;
					}
					memi[procid][c]=log10(memi[procid][b]);
				}
				else if(a==2)
				{
					if(memc[procid][b]<=0)
					{
						printf("log invalid parameter\n");
						return -1;
					}
					memc[procid][c]=log10(memc[procid][b]);
				}
				else if(a==3)
				{
					if(memf[procid][b]<=0)
					{
						printf("log invalid parameter\n");
						return -1;
					}
					memf[procid][c]=log10(memf[procid][b]);
				}
				else
				{
					printf("Type error\n");
					return -1;
				}
			}
			else if(strcmp(_cmd,"ln")==0)
			{
				int a,b,c;
				fscanf(fin[procid],"%d %d %d",&a,&b,&c);
				if(b<0||b>127||c<0||c>127)
				{
					printf("Memory error\n");
					return -1;
				}
				if(a==1)
				{
					if(memi[procid][b]<=0)
					{
						printf("log invalid parameter\n");
						return -1;
					}
					memi[procid][c]=log(memi[procid][b]);
				}
				else if(a==2)
				{
					if(memc[procid][b]<=0)
					{
						printf("log invalid parameter\n");
						return -1;
					}
					memc[procid][c]=log(memc[procid][b]);
				}
				else if(a==3)
				{
					if(memf[procid][b]<=0)
					{
						printf("log invalid parameter\n");
						return -1;
					}
					memf[procid][c]=log(memf[procid][b]);
				}
				else
				{
					printf("Type error\n");
					return -1;
				}
			}else if(strcmp(_cmd,"getpi")==0)
			{
				int a,b;
				fscanf(fin[procid],"%d %d",&a,&b);
				if(b<0||b>127)
				{
					printf("Memory error\n");
					return -1;
				}
				if(a==1)
				{
					memi[procid][b]=PI;
				}
				else if(a==2)
				{
					memc[procid][b]=PI;
				}
				else if(a==3)
				{
					memf[procid][b]=PI;
				}
				else
				{
					printf("Type error\n");
					return -1;
				}
			}

		}
	}
	else if (strcmp(_cmd, "clear") == 0) {
		system("cls");
	} else {
		printf("Invalid operation\n");
		return -1;
	}
	return 1;
}
int rbvm(int procid) {
	char command[128];/*
	printf("Rubidium Assembly on RbVM v1.11.4 Release 4\n");
	printf("By Lithium4141\n");
	printf("(C)2022~2025 Lithium Project LLC\n");*/
	fin[procid] = fopen(fileloc[procid], "r");
	if (fin[procid] == NULL) {
		printf("Cannot open file: %s\n", fileloc[procid]);
		return -2;
	}
	while (!feof(fin[procid])) {
		if (fscanf(fin[procid], "%s", command) != 1) {
			break;
		}
		int inte=getkey();
		if(inte==28)
		{
			printf("%s forcely terminated by Ctrl-\\",fileloc[procid]);
			return -1;
		}
		else if(inte==26)
		{
			printf("%s stopped by Ctrl-Z",fileloc[procid]);
		}
		int ret=run(procid,command);
		if (ret == 1) {
			continue;
		} else if (ret == 0) {
			//printf("RbVM Stopped\n");
			fclose(fin[procid]);
			return 0;
		} else if (ret == -1) {
			printf("RbVM Terminated by an error\n");
			fclose(fin[procid]);
			return -1;
		} else if (ret==-2){
			printf("%s forcely terminated by Ctrl-\\",fileloc[procid]);
			fclose(fin[procid]);
		} else if(ret==-3){
			printf("%s stopped by Ctrl-Z",fileloc[procid]);
			return 3;
		}
	}
	fclose(fin[procid]);
	return 0;
}
int getkey()
{
	if(kbhit())
	{
		return getch();
	}
	else
	{
		return -1;
	}
}
int main()
{
	system("vga.exe");
    system("cls");                                     
    printf("      A         EEEEEEEE     OOOOO            SSSS   555555       11      11  \n");
    printf("     A A        .           O     O          S       5          ..11    ..11  \n");
    printf("    A   A       .          O       O        S        5            11      11  \n");
    printf("   A.....A      EEEEEEEE   O       O        S        555555       11      11  \n");
    printf("  A       A     .          O       O        S             5       11      11  \n");
    printf(" A         A    .           O     O        S              5  OO   11      11  \n");
    printf("A           A   EEEEEEEE     OOOOO     SSSS          555555  OO ..11..  ..11..\n");
    printf("\n");
    printf("||===\\\\                       ||      //||      \n");
    printf("||   ||            ||         ||        ||       \n");
    printf("||===//  //==\\\\   ====  //==  ||==\\\\    ||              _     _\n");
    printf("||       ||  ||    ||   ||    ||   ||   ||              _|   | |\n");
    printf("||       \\\\==\\\\     \\\\  \\\\==  ||   || ======       \\/   _| . |_|\n");
    printf("\n");
    printf("AEOS v5.11.2 Beta 5 Build 7049 Kernel Patch 1 Community Edition\n");
    printf("Starting up...[                                             ]");
    printf("\rStarting up...[");
    for(int i=1;i<=45;i++)
    {
        printf("#");
        if(i>=12&&i<=30)
        {
            delay(155);
        }
        if(i==20)
		{
			system("ctmouse.exe > NUL");
		}
        if(i<30)
        {
            delay(40);
        }
        delay(25);
    }
    delay(300);
    system("cls");
    printf("Initilazing AEOS v5.11\n");
	printf("OpenSRV is intilazing...\n");
	system("mode con cols=80 lines=25");
	printf("[ OK ] TTyDM Initilazation Success!\n");
	printf("[ OK ] GKDM Initilazation Success!\n");
	printf("[ OK ] DMS Intilazation Success!\n");
	getkey();
	/*
	 *	   for(int i=1;i<=5;i++)
	 *	   {
	 *			   printf("\r[*   ] Initilazing BGIDM...");
	 *			   getkey();
	 *			   delay(500);
	 *			   printf("\r[#*  ] Initilazing BGIDM...");
	 *			   getkey();
	 *			   delay(500);
	 *			   printf("\r[*#* ] Initilazing BGIDM...");
	 *			   getkey();
	 *			   delay(500);
	 *			   printf("\r[ *#*] Initilazing BGIDM...");
	 *			   getkey();
	 *			   delay(500);
	 *			   printf("\r[  *#] Initilazing BGIDM...");
	 *			   getkey();
	 *			   delay(500);
	 *			   printf("\r[   *] Initilazing BGIDM...");
	 *			   getkey();
	 *			   delay(500);
     *             printf("\r[    ] Initilazing BGIDM...");
	 *			   getkey();
	 *			   delay(500);
}
*/
	printf("[ OK ] BGIDM Initilazation Success!\n");
	printf("[ OK ] ARM Initilazation Success!\n");
	printf("[ OK ] AGeTTy Initilazation Success!\n");
	printf("[ OK ] InfDrv Service Started!\n");
	getkey();
	//delay(2000);
	getkey();
	printf("[ OK ] CuteMouse v1.6 DOS Initilazation Success!\n");
	printf("[FAIL] No Sound card driver found!\n");
	printf("[FAIL] No Network card driver found!\n");
	getkey();
    system("mkdir ROOTFS > NUL");
    system("mkdir SYSCFG > NUL");
	printf("[ OK ] RTFS Driver Initilazed Successfully!\n");
    printf("[ OK ] Mounted /SYSCFG\n");
    printf("[ OK ] Mounted /ROOTFS\n");
shell:
	printf("Welcome to AEOS 5.1!\n");
retry_getpwd:
    FILE* _usrcfg=fopen("SYSCFG\\passwd.cfg","r");
	char plen=0;
    char pswd[65];
	if(!_usrcfg)
	{
        printf("[AGeTTy:FATAL]Unable to read user configuration file:Disk I/O Error\n");
		printf("               Resetting to default configuration...\n");
		char* __buf;
        sprintf(__buf,"echo %c >SYSCFG\\passwd.cfg",1);
		system(__buf);
        free(__buf);
		goto retry_getpwd;
	}
	fscanf(_usrcfg,"%c",plen);
	plen--;
	if(plen>0)
	{
		for(int i=0;i<((char)plen);i++)
		{
            fscanf(_usrcfg,"%c",pswd[i]);
			pswd[i]+=31;
		}
	}
	fclose(_usrcfg);
    //printf("%d : %s",plen,pswd);
	while(1)
	{
		printf("AEOS51 login:");
		int key=0,currentChr=0;
		char inputBuf[65];
		memset(inputBuf,0,sizeof(inputBuf));
		while(1)
		{
			key=getch();
			if(key==8||key==127)
			{
				currentChr--;
				if(currentChr<0)
				{
					currentChr=0;
				}
				else
				{
					printf("\b \b");
					inputBuf[currentChr+1]=0;
				}
            }
			if(key>=32&&key<=126)
			{
				if(currentChr>=64)
				{
					currentChr=63;
					break;
				}
				inputBuf[currentChr++]=key;
				printf("%c",key);
			}
			if(key==3||key==9||key==10||key=='\r'||key=='\n')
			{
				inputBuf[currentChr]=0;
				break;
			}
		}
        if(strcmp(inputBuf,"root")==0&&plen<=0)
        {            
            printf("\n[AGeTTy:Warning]Password not set!Use \"passwd\" to set a new password!\n");
            goto loginok;
        }
        printf("\nPassword:");
        key=0,currentChr=0;
		char inputBuf1[65];
		memset(inputBuf1,0,sizeof(inputBuf1));
		while(1)
		{
			key=getch();
			if(key==8||key==127)
			{
				currentChr--;
				if(currentChr<0)
				{
					currentChr=0;
				}
				else
				{
					//printf("\b \b"); //no need for this line
					inputBuf1[currentChr+1]=0;
				}

			}
			if(key>=32&&key<=126)
			{
				if(currentChr>=64)
				{
					currentChr=63;
					break;
				}
				inputBuf1[currentChr++]=key;
				//printf("%c",key); //passwords should not be shown
			}
			if(key==3||key==9||key==10||key=='\r'||key=='\n')
			{
				inputBuf1[currentChr]=0;
				break;
			}
		}
		if(strcmp(inputBuf,"root")==0 && strcmp(inputBuf1,pswd)==0)
		{
			goto loginok;
		}
		printf("\nLogin incorrect\n");
	}
loginok:
	system("bgidm.exe");
	while(1)
	{
	agsh:
		printf("\nroot@AEOS51:ROOTTFS/ # ");
		int key=0,currentChr=0;
		char inputBuf[65];
		memset(inputBuf,0,sizeof(inputBuf));
		while(1)
		{
			key=getch();
			if(key==8||key==127)
			{
				currentChr--;
				if(currentChr<0)
				{
					currentChr=0;
				}
				else
				{
					printf("\b \b");
					inputBuf[currentChr+1]=0;
				}

			}
			if(key>=32&&key<=126)
			{
				if(currentChr>=64)
				{
					currentChr=63;
					break;
				}
				inputBuf[currentChr++]=key;
				printf("%c",key);
			}
			if(key==3||key==9||key==10||key=='\r'||key=='\n')
			{
				inputBuf[currentChr]=0;
				break;
			}
		}
		printf("\n");
		if(strcmp(inputBuf,"exit")==0)
		{
			system("cls");
			goto shell;
		}
		else if(strcmp(inputBuf,"ls")==0)
		{
            system("dir ROOTFS /b");
		}
		else if(strcmp(inputBuf,"ls -l")==0)
		{
            system("dir ROOTFS /p /b");
		}
		else if(strlen(inputBuf)>=6&&strncmp(inputBuf,"echo ",5)==0)
		{
			char* others=inputBuf+5;
			printf("%s\n",others);
		}
		else if(strlen(inputBuf)>=6&&strncmp(inputBuf,"exec ",5)==0)
		{
			char* others=inputBuf+5;
			char file[130]="ROOTFS\\";
			strcat(file,others);
			if(progs>=maxProg)
			{
				printf("Error:No enough memory to execute other programs!Close at least one program to continue...\n");
				continue;
			}
			progs++;
			strncpy(fileloc[progs-1], file, sizeof(fileloc[0])-1);
			int ret=rbvm(progs-1);
			if(ret==3)
			{
				continue;
			}
			progs--;
		}
		else if(strcmp(inputBuf,"clear")==0)
		{
			system("cls");
		}
		else if(strcmp(inputBuf,"shutdown")==0)
		{
			system("shutdown s");
		}
		else if(strcmp(inputBuf,"poweroff")==0)
		{
			system("shutdown s");
		}
		else if(strcmp(inputBuf,"restart")==0)
		{
			system("shutdown r");
		}
		else if(strcmp(inputBuf,"reboot")==0)
		{
			system("shutdown r");
		}
		else if(strcmp(inputBuf,"pwd")==0)
		{
			printf("ROOTFS/ \n");
		}
		else if(strcmp(inputBuf,"ver")==0)
		{
            printf("AEOS v5.11.2 Build 7049 Kernel patch 1\n");
			printf("root@AEOS51\n");
			printf("(C)Copyright 2022~2026 Lithium Project LLC\n");
			printf("By Lithium4141\n");
			printf("Release date:2026/2/4\n");
		}
		else if(strcmp(inputBuf,"ps")==0)
		{
			if(progs<=0)
			{
				printf("No process running\n");
				continue;
			}
			printf("Totally %d process running:\n");
			printf("PID | Process name\n");
			for(int i=1;i<=progs;i++)
			{
				printf("%d   | %s\n",i-1,fileloc[i-1]);
			}
		}
		else if(strcmp(inputBuf,"help")==0)
		{
			printf("Built in commands:\n");
			printf("exit       ls       echo       exec\n");
			printf("clear      shutdown poweroff   restart\n");
			printf("reboot     pwd      ver        ps\n");
			printf("help       kill     open       bgidm\n");
			printf("passwd     whoami   lsusr      arm\n");
            printf("syscfg\n");
			printf("Note:To stop a program,press Ctrl-\\\n");
			printf("     To leave a program in background,try press Ctrl-Z\n");
		}
		else if(strlen(inputBuf)>=6&&strncmp(inputBuf,"kill ",5)==0)
		{
			int pid=atoi(inputBuf+5);
			if(pid<0||pid>=progs)
			{
				printf("Invalid PID:%d\n",pid);
				continue;
			}
			printf("Killed %d:%s\n",pid,fileloc[pid]);
			memset(fileloc[pid],'\0',sizeof(fileloc[pid]));
			progs--;
			fclose(fin[pid]);
            if(pid<(progs))
			{
                for(int i=pid+1;i<=progs;i++)
				{
                    strcpy(fileloc[i-1],fileloc[i]);
					//note:debug codes are disabled
					//printf("[debug]Copying file pointer from %d-1(%d) to %d...\n",i,i-1,i);
					//printf("[debug]Address of fin[%d]:%d\n[debug]Address of fin[%d]:%d\n",i-1,&fin[i-1],i,&fin[i]);
                    //printf("[debug]Data of fin[%d]:%d\n[debug]Data of fin[%d]:%d\n",i-1,fin[i-1],i,fin[i]);
					memcpy(fin[i-1],fin[i],sizeof(fin[i]));
					memcpy(memi[i], memi[i+1], sizeof(memi[0]));
					memcpy(memf[i], memf[i+1], sizeof(memf[0]));
					memcpy(memc[i], memc[i+1], sizeof(memc[0]));
                    memcpy(mems[i], mems[i+1], sizeof(mems[0]));
					memcpy(sect[i], sect[i+1], sizeof(sect[0]));
					//printf("[debug]Operation done successfully!\n");
					//printf("[debug]Address of fin[%d]:%d\n[debug]Address of fin[%d]:%d\n",i-1,&fin[i-1],i,&fin[i]);
                    //printf("[debug]Data of fin[%d]:%d\n[debug]Data of fin[%d]:%d\n",i-1,fin[i-1],i,fin[i]);
				}
			}
		}
		else if(strlen(inputBuf)>=6&&strncmp(inputBuf,"open ",5)==0)
		{
			int pid=atoi(inputBuf+5);
			if(pid<0||pid>=progs)
			{
				printf("Invalid PID:%d\n",pid);
				continue;
			}
			printf("Openning %d:%s\n",pid,fileloc[pid]);
			rbvm(pid);
		}
        else if(strcmp(inputBuf,"passwd")==0)
		{
            FILE* _file=fopen("SYSCFG\\passwd.cfg","w");
			if(!_file)
			{
				printf("Failed to open user database file:Disk I/O Error\n");
			}
			else
			{
                char inputBuf1[65],inputBuf2[65];
                memset(inputBuf1,0,sizeof(inputBuf1));
                memset(inputBuf2,0,sizeof(inputBuf2));    
				while(1)
				{
					printf("Input new password:");
					int key=0,currentChr=0;
                    while(1)         
					{
						key=getch();
						if(key==8||key==127)
						{
							currentChr--;
							if(currentChr<0)
							{
								currentChr=0;
							}
							else
							{
								//printf("\b \b"); //no need for this line
								inputBuf1[currentChr+1]=0;
							}
						}
						if(key>=32&&key<=126)
						{
							if(currentChr>=64)
							{
								currentChr=63;
								break;
							}
							inputBuf1[currentChr++]=key;
							//printf("%c",key); //passwords should not be shown
						}
						if(key==3||key==9||key==10||key=='\r'||key=='\n')
						{
							inputBuf1[currentChr]=0;
							break;
						}
					}
					printf("\nRetype new password:");
                    int key1=0,currentChr1=0;
					while(1)
					{
                        key1=getch();
                        if(key1==8||key1==127)
						{
                            currentChr1--;
                            if(currentChr1<0)
							{
                                currentChr1=0;
							}
							else
							{
								//printf("\b \b"); //no need for this line
                                inputBuf2[currentChr1+1]=0;
							}
						}
                        if(key1==28)
						{
							printf("\"passwd\" forcely terminated by Ctrl-\\\n");
							goto agsh;
						}
                        if(key1>=32&&key1<=126)
						{
                            if(currentChr1>=64)
							{
                                currentChr1=63;
								break;
							}
                            inputBuf2[currentChr1++]=key1;
                            //printf("%c",key1); //passwords should not be shown
						}
                        if(key1==3||key1==9||key1==10||key1=='\r'||key1=='\n')
						{
                            inputBuf2[currentChr1]=0;
							break;
						}
					}
					if(strcmp(inputBuf1,inputBuf2)==0)
					{
						break;
					}
					else
					{
						printf("\nInput disagree\n");
					}
				}
                fclose(_file);
                char* __buf;
				for(int i=0;i<strlen(inputBuf1);i++)
                {            
                    inputBuf1[i]-=31;
				}
                sprintf(__buf,"echo %c%s > SYSCFG\\passwd.cfg",((char)strlen(inputBuf1)+1),inputBuf1);
                //printf("%s\n",__buf);
                printf("Reboot to take effects.\n");
                system(__buf);
                //fclose(_file);
			}
		}
		else if(strcmp(inputBuf,"whoami")==0)
		{
			printf("root\n");
		}
		else if(strcmp(inputBuf,"lsusr")==0)
		{
			printf("Users   |UID\n");
			printf("51INIT  |0\n");
			printf("OPENSRV |1\n");
			printf("root    |2\n");
		}
		else if(strlen(inputBuf)>=4&&strncmp(inputBuf,"arm ",4)==0)
		{
			char* others=inputBuf+4;
			//printf("%s\n",others);
			if(strncmp(others,"install",7)==0)
			{
				printf("Reading package database... Done\n");
				printf("[InfDrv:FATAL]:no valid network driver or device found!\n");
				printf("Error:No valid interface found!\n");
				printf("Please try again later\n");
			}
			else if(strncmp(others,"update-repo",11)==0)
			{
				printf("Downloading repository index from\"http://storage.lithiumproj.ink/arm-repo/repo.idx\"...\n");
				printf("[InfDrv:FATAL]:no valid network driver or device found!\n");
				printf("Operation failed.\n");
				printf("Building local package databse....\n");
				system("echo aeos-system-kernel > SYSCFG\\repo.idx");
				system("echo bgidm >> SYSCFG\\repo.idx");
				system("echo agsh >> SYSCFG\\repo.idx");
				system("echo infdrv >> SYSCFG\\repo.idx");
				system("echo opensrv >> SYSCFG\\repo.idx");
				system("echo bgidm-dos-video-driver >> SYSCFG\\repo.idx");
				system("echo cutemouse-dos-mouse-driver >> SYSCFG\\repo.idx");
				system("echo msdos-system-intilazer >> SYSCFG\\repo.idx");
				printf("Success!\n");
			}
			else if(strncmp(others,"update",6)==0)
			{
				printf("Reading package database... Done\n");
				printf("Getting version information from databse... Done\n");
				printf("Error:all packages up to date\n");
			}
			else if(strncmp(others,"remove ",7)==0)
			{
				printf("Reading package database... Done\n");
				char* otherz=others+7;
				if(strcmp(otherz,"aeos-system-kernel")==0)
				{
					 printf("Warning:This is your operating system kernel.Removing it can lead your operating system unable to boot!Confirm operation?[y/n]\n");
					 int k=getch();
					 if(k==28)
					 {
						 printf("\"arm remove aeos-system-kernel\" Terminated by Ctrl-\\\n");
						 goto agsh;
					 }
					 else if(k=='y'||k=='Y')
					 {
						 printf("Removing package binaries...\n");
						 int ret=system("del 51kern.exe");
						 if(ret==0)
						 {
							 printf("Success!\n");
						 }
						 else
						 {
							 printf("Error:Failed to remove package binaries.\n");
							 printf("Error code:%d",ret);
						 }
					 }
					 else
					 {
						 printf("Aborted.\n");
						 goto agsh;
					 }
				}
                else if(strcmp(otherz,"rbvm-runtime")==0||strcmp(otherz,"bgidm")==0||strcmp(otherz,"agsh")==0||strcmp(otherz,"infdrv")==0||strcmp(otherz,"opensrv")==0||strcmp(otherz,"cutemouse-dos-mouse-driver")==0||strcmp(otherz,"bgidm-dos-video-driver")==0)
				{
                    printf("Warning:This package is not an independent package but a dependency and requirement of \"aeos-system-kernel\".\n");
                    printf("Error:Package not found!\n");
				}
                else if(strcmp(otherz,"msdos-system-intilazer")==0)
				{
					printf("Warning:This is your operating system kernel.Removing it can lead your operating system unable to boot!Confirm operation?[y/n]\n");
					int k=getch();
					if(k==28)
					{
						printf("\"arm remove msdos-system-initilazer\" Terminated by Ctrl-\\\n");
						goto agsh;
					}
					else if(k=='y'||k=='Y')
					{
						printf("Removing package binaries...\n");
						int ret=system("del command.exe io.sys msdos.sys");
						if(ret==0)
						{
							printf("Success!\n");
						}
						else
						{
							printf("Error:Failed to remove package binaries.\n");
							printf("Error code:%d",ret);
						}
					}
					else
					{
						printf("Aborted.\n");
						goto agsh;
					}
                }
				else
				{
					printf("Error:package not found!\n");
				}
			}
			else if(strncmp(others,"clean-cache",11)==0)
			{
				printf("Reading package database... Done\n");
				printf("Scanning unused package...  Done\n");
				printf("Scanning cache files...     Done\n");
				printf("Successs!\n");
			}
			else if(strncmp(others,"list",4)==0)
			{
				system("type SYSCFG\\repo.idx");
			}
			else
			{
				printf("Error:Invalid parameter\n");
				printf("Usage:arm [install/update/update-repo/remove/clean-cache/list] <package-name> \n");
			}
		}
		else if(strlen(inputBuf)>=7&&strncmp(inputBuf,"syscfg ",7)==0)
		{
			char* others=inputBuf+7;
			if(strcmp(others,"bgidm")==0)
			{
                int a=1,b=0,c=60,d=1;
                printf("Select Rendering mode:1. Legacy frame rendering  2. Double-buffered rendering");
				while(1)
				{
					int k=getch();
					if(k=='1')
					{
						a=1;
						break;
					}
					else if(k=='2')
					{
						a=0;
						break;
					}
					else if(k==28)
					{
						printf("\"syscfg bgidm\" terminated by Ctrl-\\\n");
						goto agsh;
					}
				}
				if(a==0)
				{
                    printf("\nCopy mode for double-buffered rendering?[y/n]");
					while(1)
					{
						int k=getch();
						if(k=='y'||k=='Y')
						{
							b=1;
							break;
						}
						else if(k=='n'||k=='N')
						{
							b=0;
							break;
						}
						else if(k==28)
						{
							printf("\"syscfg bgidm\" terminated by Ctrl-\\\n");
							goto agsh;
						}
					}
				}
				printf("\nMaximum Render FPS:");
				c=get_uint();
				if(c==-2)
				{
					printf("\"syscfg bgidm\" terminated by Ctrl-\\\n");
					goto agsh;
				}
			redo:
				printf("\nRender tick delay(Minimum 1):");
				d=get_uint();
				if(c==0)
				{
					printf("Error:Invalid configuration\n");
					goto redo;
				}
				if(c==-2)
				{
					printf("\"syscfg bgidm\" terminated by Ctrl-\\\n");
					goto agsh;
				}
				char* buf;
				sprintf(buf,"echo %d %d %d %d > SYSCFG\\bgidm.cfg",a,b,c,d);
                system(buf);
			}
			else if(strcmp(others,"agetty")==0)
			{
				printf("Error:password can only modified by \"passwd\"\n");
			}
			else
			{
				printf("Usage:syscfg [bgidm/agetty]\n");
			}
		}
		else if(strcmp(inputBuf,"bgidm")==0)
		{
			system("bgidm.exe");
		}
		else
		{
			printf("AGSH:Unknown Command:%s\n",inputBuf);
		}
	}
}
