#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4//ť�� ��ü ũ�� ����ť�̰� �����϶��� �Ǻ��ϱ� ���� ���� ũ��� MAX_QUEUE_SIZE-1

typedef char element;//char�� element�� �ڷ�������
typedef struct {
	element queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType;//ť ����ü

QueueType *createQueue();
int freeQueue(QueueType *cQ);
int isEmpty(QueueType *cQ);
int isFull(QueueType *cQ);
void enQueue(QueueType *cQ, element item);
void deQueue(QueueType *cQ, element* item);
void printQ(QueueType *cQ);
void debugQ(QueueType *cQ);
element getElement();

int main(void)
{
	QueueType *cQ = createQueue();//ť ����ü�� �ϳ� �����.
	element data;//element Ÿ���� ������ �����. ť�� ������ ��,��¿� ���δ�
	char command;

	printf("\n[----- [������] [2018038031] -----]\n");

	if(cQ==NULL){return -1;}//ť�� ����� ����������ʾҴٸ� ���α׷��� �����Ѵ�.

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		fflush(stdout);
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D':
			deQueue(cQ, &data);
			break;
		case 'p': case 'P':
			printQ(cQ);
			break;
		case 'b': case 'B':
			debugQ(cQ);
			break;
		case 'q': case 'Q':
   	        freeQueue(cQ);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');


	return 1;
}

QueueType *createQueue()//ť ����ü ũ�� ��ŭ ������ �Ҵ�޾� �ּҸ� �����Ѵ�.
{
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType));
	if(cQ==NULL){//�����Ҵ��� �����ϸ� �ȳ��޽��� ����� NULL���� �����Ѵ�.
		printf("�����Ҵ��� �����Ͽ����ϴ�.");
		return NULL;
	}
	else{
		cQ->front = 0;//front�� rear�� 0���� �ʱ�ȭ�Ѵ�.
		cQ->rear = 0;
		return cQ;
	}
}

int freeQueue(QueueType *cQ)//�����Ҵ���� ť�� �����Ѵ�.
{
    if(cQ == NULL) return 1;
    free(cQ);
    return 1;
}

element getElement()//ť�� ���� ���ڸ� �Է¹޴´�.
{
	element item;
	printf("Input element = ");
	fflush(stdout);
	scanf(" %c", &item);
	return item;//�Է¹��� ���ڸ� �����Ѵ�.
}

int isEmpty(QueueType *cQ)//ť�� ��������� �Ǵ��Ѵ�.
{
	if(cQ->rear==cQ->front){
		printf("Queue is empty\n");
		return 0;//rear�� front �� ���ٸ� ����ִٰ� �Ǵ��ϰ� 0�� �����Ѵ�.
	}
	else//�ƴ϶�� -1�� �����Ѵ�.
		return -1;
}

int isFull(QueueType *cQ)//ť�� ��á������ �Ǵ��Ѵ�.
{
	if((cQ->rear+1)%MAX_QUEUE_SIZE == cQ->front){//rear+1�� ��ⷯ������ �Ѱ��� front�� ���ٸ� ��á�ٰ� �Ǵ��ϰ� 0�� �����Ѵ�.
		printf("Queue is full\n");
		return 0;
	}
	return -1;//�ƴ϶�� -1�� �����Ѵ�.
}

void enQueue(QueueType *cQ, element item)//ť�� �����͸� ����ִ´�.
{
	if(isFull(cQ)){//ť�� ���������ʴٸ�
		cQ->rear=(cQ->rear+1)%MAX_QUEUE_SIZE;//rear�� ���� ��ⷯ�����Ͽ� ������ �ű��
		cQ->queue[cQ->rear]=item;//ť�� rear�ڸ��� �����͸� �ִ´�.
	}
}

void deQueue(QueueType *cQ, element *item)//ť���� �����͸� ������.
{
	if(isEmpty(cQ)){//ť�� ��������ʴٸ�
			*item=cQ->queue[cQ->front];//ť�� front �ڸ��� �����͸� �޾� item�� �����ϰ�
			cQ->front=(cQ->front+1)%MAX_QUEUE_SIZE;//front�� ��ѷ������Ͽ� ������ �ű��.
		}
}

void printQ(QueueType *cQ)//ť�� ����Ѵ�.
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE;//ó����
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;//���� ã�´�.

	printf("Circular Queue : [");

	i = first;
	while(i != last){//ó������ ������ ����Ѵ�.
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}

void debugQ(QueueType *cQ)//�����
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front) {//ť�� front �ڸ��� ���� ����Ѵ�.
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);//ť ����ü �ȿ� �迭�� ���� ���¸� ����Ѵ�.

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);//ť�� front�� rear�� ����Ѵ�.
}
