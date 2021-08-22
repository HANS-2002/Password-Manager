#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char custom[]="";
char A[]="LOREM";
char B[]="IPSUM";
char C[]="CONSE";
char D[]="CRATO";
int size=0;

struct customer {
    char Appweb[40];
    char Username[40];
    char password[41];
    char id[3];
}s[100];

void getFile(){
    FILE* fp = fopen("data.csv", "r");
    int i=0;
    if (!fp)
        printf("File is empty!!\n");
    else 
    {
        char buffer[256];
        int row = 0,column=0;
        while (fgets(buffer,256, fp)) 
        {
            column = 0;
            row++;
            if (row == 1)continue;
  
            char* value = strtok(buffer, ",\n");
  
            while (value) 
            {
                strcpy(s[i].id,value);
                value = strtok(NULL, ",\n");
                strcpy(s[i].Appweb,value);
                value = strtok(NULL, ",\n");
                strcpy(s[i].Username,value);
                value = strtok(NULL, ",\n");
                strcpy(s[i].password,value);
                value = strtok(NULL, ",\n");
                i++;
            }
        }
        fclose(fp);
    }
    size=i;
}

void putFile(){
    FILE* fp = fopen("data.csv", "w");
    fprintf(fp,"Id,App or web,Username,Password\n");
    for(int i=0;i<size;i++)
        fprintf(fp,"%s,%s,%s,%s\n",s[i].id,s[i].Appweb,s[i].Username,s[i].password);
    fclose(fp);
}

void encrypt(char *a){
    char n[40];
    int l=strlen(a);
    int count=0;
    for(int i=0;i<l;i++)
    {
        if(i%4==0)
        {
            n[i]=(a[i]+A[count]);   
        }
        else if(i%3==0)
        {
            n[i]=(a[i]+C[count]);
        }
        else if(i%2==0)
        {
            n[i]=(a[i]+B[count]);
        }
        else
        {
            n[i]=(a[i]+D[count]);
        }
        count++;
        if(count==5)
        count=0;
    }
    n[l]=';'-2*l;
    count=0;
    for(int i=l+1;i<39;i++)
    {
        if(i%4==0)
        {
            n[i]=(B[count]+A[count]);
            
        }
        else if(i%3==0)
        {
            n[i]=(B[count]+C[count]);
            
        }
        else if(i%2==0)
        {
            n[i]=(D[count]+A[count]);
            
        }
        else
        {
            n[i]=(B[count]+D[count]);
            
        }
        if(n[i]=='\n')
        n[i]='!';
        else if(n[i]=='\t')
        n[i]='"';
        else if(n[i]==',')
        n[i]='#';
        else if(n[i]==' ')
        n[i]='$';
        count++;
        if(count==5)
        count=0;
    }
    n[39]='\0';
    strcpy(a,n);
}

void decrypt(char *a){
    int l;
    for(int i=0;i<40;i++)
    {
        if((a[i]+2*i)==';')
        {
            l=i;
            break;
        }
    }
    char n[l];
    int count=0;
    for(int i=0;i<l;i++)
    {
        if(a[i]=='!')
        a[i]='\n';
        else if(a[i]=='*')
        a[i]='\t';
        else if(a[i]=='#')
        a[i]=',';
        else if(n[i]=='$')
        n[i]=' ';
        if(i%4==0)
        {
            n[i]=(a[i]-A[count]);
            
        }
        else if(i%3==0)
        {
            n[i]=(a[i]-C[count]);
            
        }
        else if(i%2==0)
        {
            n[i]=(a[i]-B[count]);
            
        }
        else
        {
            n[i]=(a[i]-D[count]);
            
        }
        count++;
        if(count==5)
        count=0;
    }
    n[l]='\0';
    strcpy(a,n);
}

void makeNew(){
    getFile();
    char webName[50],user[50];
    printf("Enter name of app or website: ");
    fflush(stdin);
    scanf("%s",webName);
    printf("Enter username: ");
    fflush(stdin);
    scanf("%s",user);
    for(int i=0;i<size;i++)
        if(!strcmp(s[i].Username,user) && !strcmp(s[i].Appweb,webName)){
            printf("Data already exists for user %s.\n",user);
            return;
        }
    strcpy(s[size].Appweb,webName);
    strcpy(s[size].Username,user);
    printf("Enter the password: ");
    fflush(stdin);
    scanf("%s",s[size].password);
    encrypt(s[size].password);
    char str[3];
    sprintf(str,"%d",size+1);
    strcpy(s[size].id,str);
    size++;
    putFile();
}

void show(){
    getFile();
    if(size==0){
        printf("No data found!\n");
        return;
    }
    char search[40];
    int flag=1;
    printf("Enter the name of ur app or website: ");
    scanf("%s",search);
    for(int i=0;i<size;i++){
        if(!strcmp(s[i].Appweb,search)){
            char pass[41];
            strcpy(pass,s[i].password);
            decrypt(pass);
            printf("\nUser number %d\n",i+1);
            printf("Username is: %s\nPassword is: %s\n\n",s[i].Username,pass);
            flag=0;
        }
    }
    if(flag)
        printf("No entry found.\n");
}

void editPass(){
    getFile();
    char webName[50],user[50];
    printf("Enter name of app or website: ");
    fflush(stdin);
    scanf("%s",webName);
    printf("Enter username: ");
    fflush(stdin);
    scanf("%s",user);
    for(int i=0;i<size;i++)
        if(!strcmp(s[i].Username,user) && !strcmp(s[i].Appweb,webName)){
            char pass[41],rePass[41];
            printf("Enter new pass: ");
            scanf("%s",pass);
            printf("Re-enter new pass: ");
            scanf("%s",rePass);
            if(!strcmp(pass,rePass)){
                encrypt(pass);
                strcpy(s[i].password,pass);
            }
            else{
                printf("Re enetered password is wrong!\n");
            }
            putFile();
            return;
        }
    printf("No entry found.\n");
}

void deleteAcc(){
    getFile();
    char webName[50],user[50];
    printf("Enter name of app or website: ");
    fflush(stdin);
    scanf("%s",webName);
    printf("Enter username to delete: ");
    fflush(stdin);
    scanf("%s",user);
    for(int i=0;i<size;i++)
        if(!strcmp(s[i].Username,user) && !strcmp(s[i].Appweb,webName)){
            for(int j=i;j<size-1;j++){
                strcpy(s[j].Appweb,s[j+1].Appweb);
                strcpy(s[j].Username,s[j+1].Username);
                strcpy(s[j].password,s[j+1].password);
            }
            size--;
            putFile();
            return;
        }
    printf("No entry found.\n");
}

int main(){
    while(1){
        int choice;
        printf("\n----------------Password Manager Menu----------------\n");
        printf("(1)Make a new entry.\n");
        printf("(2)Access Existing password.\n");
        printf("(3)Change Existing password.\n");
        printf("(4)Delete an account.\n");
        printf("(5)Exit.\n");
        printf("-----------------------------------------------------\n");
        printf("Enter a choice: ");
        scanf("%d",&choice);
        switch(choice)
        {
        case 1:
            makeNew();
            break;
        case 2:
            show();
            break;
        case 3:
            editPass();
            break;
        case 4:
            deleteAcc();
            break;
        case 5:
            return 0;
            break;
        default:
            printf("Enter a valid choice.\n");
            break;
        }
    }
    return 0;
}
