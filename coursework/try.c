#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
//#define LEN sizeof(struct student)
struct testsdd{
	char str[15];
	int d1;
	int d2;
	struct testsdd *next;
};
typedef struct testsdd test;
int main()
{
	char filename[15];
	char s[15];
	FILE *fp;
	int x,y;
	test *start = NULL;
	test *n = (test*)malloc(sizeof(test));
	printf("Enter filename: ");
	scanf("%s",filename);
	if (( fp= fopen(filename, "r"))==NULL)
	{
		printf("Can't open.");
	}
	fscanf(fp,"%s",s);
	fscanf(fp,"%d",&x);
	fscanf(fp,"%d",&y);
	strcpy(n->str,s);
	n->d1 = x;
	n->d2 = y;
	n->next = NULL;
	start = n;
	printf("%s %d %d",n->str,n->d1,n->d2);
	
	return 0;
}




/*struct try1{
	int value;
	struct try1 *next;
};
typedef struct try1 tryy;
int main()
{
	tryy *start = NULL;
	tryy *n = malloc(sizeof(tryy));
	n->value = 3;
	n->next = NULL;
	start = n;
	printf("%d\n",n->value);
	printf("%d\n",start->value);
	return 0;
	
}*/





/*struct student *create(); //�������� 
void print(struct student *head);//��ӡ���� 

struct student
{
	int num;
	float score;
	struct student *next;
};
int n;//ȫ�ֱ�����������¼����˶������ݡ� 
void main()
{
	struct student *stu;
	
	stu = create();
	print(stu);
	
	printf("\n\n");
	system("pause");
}

struct student *create()
{
	struct student *head;
	struct student *p1,*p2;
	
	p1 = p2 = (struct student *)malloc(LEN);
	
	printf("Please enter the num: ");
	scanf("%d", &p1->num);
	printf("Please enter the score: ");
	scanf("%f", &p1->score);
	
	head = NULL;
	n = 0;
	
	while (p1->num)
	{
		n++;
		if(1==n)
		{
			head = p1;
		}
		else
		{
			p2->next = p1;
		}
		
		p2 = p1;
		p1 = (struct student *)malloc(LEN);
		
		printf("\nPlease enter the num: ");
		scanf("%d", &p1->num);
		printf("Please enter the score: ");
		scanf("%f", &p1->score);
	}
	p2->next = NULL;
	return head;
}*/





/*int main()
{
	char ch[]={};
	strcpy(ch, "you");
	printf("%s",ch);
}*/
