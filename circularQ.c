#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4//큐의 전체 크기 원형큐이고 만원일때를 판별하기 위해 실제 크기는 MAX_QUEUE_SIZE-1

typedef char element;//char을 element로 자료형지정
typedef struct {
	element queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType;//큐 구조체

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
	QueueType *cQ = createQueue();//큐 구조체를 하나 만든다.
	element data;//element 타입의 변수를 만든다. 큐의 데이터 입,출력에 쓰인다
	char command;

	printf("\n[----- [서종원] [2018038031] -----]\n");

	if(cQ==NULL){return -1;}//큐가 제대로 만들어지지않았다면 프로그램을 종료한다.

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

QueueType *createQueue()//큐 구조체 크기 만큼 공간을 할당받아 주소를 리턴한다.
{
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType));
	if(cQ==NULL){//동적할당을 실패하면 안내메시지 출력후 NULL값을 리턴한다.
		printf("동적할당을 실패하였습니다.");
		return NULL;
	}
	else{
		cQ->front = 0;//front와 rear를 0으로 초기화한다.
		cQ->rear = 0;
		return cQ;
	}
}

int freeQueue(QueueType *cQ)//동적할당받은 큐를 해제한다.
{
    if(cQ == NULL) return 1;
    free(cQ);
    return 1;
}

element getElement()//큐에 넣을 문자를 입력받는다.
{
	element item;
	printf("Input element = ");
	fflush(stdout);
	scanf(" %c", &item);
	return item;//입력받은 문자를 리턴한다.
}

int isEmpty(QueueType *cQ)//큐가 비었는지를 판단한다.
{
	if(cQ->rear==cQ->front){
		printf("Queue is empty\n");
		return 0;//rear와 front 가 같다면 비어있다고 판단하고 0을 리턴한다.
	}
	else//아니라면 -1을 리턴한다.
		return -1;
}

int isFull(QueueType *cQ)//큐가 꽉찼는지를 판단한다.
{
	if((cQ->rear+1)%MAX_QUEUE_SIZE == cQ->front){//rear+1에 모듈러연산을 한것이 front와 같다면 꽉찼다고 판단하고 0을 리턴한다.
		printf("Queue is full\n");
		return 0;
	}
	return -1;//아니라면 -1을 리턴한다.
}

void enQueue(QueueType *cQ, element item)//큐에 데이터를 집어넣는다.
{
	if(isFull(cQ)){//큐가 꽉차있지않다면
		cQ->rear=(cQ->rear+1)%MAX_QUEUE_SIZE;//rear를 먼저 모듈러연산하여 앞으로 옮기고
		cQ->queue[cQ->rear]=item;//큐의 rear자리에 데이터를 넣는다.
	}
}

void deQueue(QueueType *cQ, element *item)//큐에서 데이터를 꺼낸다.
{
	if(isEmpty(cQ)){//큐가 비어있지않다면
			*item=cQ->queue[cQ->front];//큐의 front 자리에 데이터를 받아 item에 대입하고
			cQ->front=(cQ->front+1)%MAX_QUEUE_SIZE;//front를 모둘러연산하여 앞으로 옮긴다.
		}
}

void printQ(QueueType *cQ)//큐를 출력한다.
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE;//처음과
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;//끝을 찾는다.

	printf("Circular Queue : [");

	i = first;
	while(i != last){//처음부터 끝까지 출력한다.
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}

void debugQ(QueueType *cQ)//디버그
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front) {//큐의 front 자리는 따로 명시한다.
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);//큐 구조체 안에 배열의 현재 상태를 출력한다.

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);//큐의 front와 rear를 출력한다.
}
