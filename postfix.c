#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10 //������ ũ��
#define MAX_EXPRESSION_SIZE 20 //������ ũ��

/* stack ������ �켱����, lparen = 0 ���� ���� */
typedef enum {
	lparen = 0,  /* ( ���� ��ȣ */
	rparen = 9,  /* ) ������ ��ȣ*/
	times = 7,   /* * ���� */
	divide = 6,  /* / ������ */
	plus = 5,    /* + ���� */
	minus = 4,   /* - ���� */
	operand = 1 /* �ǿ����� */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression�� �����ϴ� �迭 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix�� ����� ���ڿ��� �����ϴ� �迭 */
char postfixStack[MAX_STACK_SIZE];	/* postfix�� ��ȯ�� ���� �ʿ��� ���� */
int evalStack[MAX_STACK_SIZE];		/* ����� ���� �ʿ��� ���� */

int postfixStackTop = -1;  /* postfixStack�� top */
int evalStackTop = -1;	   /* evalStack�� top */
int evalResult = 0;	   /* ��� ����� ���� */
int is_right_momular = 0; //������ ������ �Ǵ�
void postfixPush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();
int is_right();//����ǥ������� �ۼ��� ������ �˸´� ������ �Ǻ�

int main()
{
	char command;

	printf("[----- [������] [2018038031] -----]\n\n");

	do {
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		fflush(stdout);
		scanf(" %c", &command);

		switch (command) {
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			if(is_right_momular==0)//������ �˸��� �� �����Ѵ�.
			toPostfix();
			else
				printf("The formula is wrong\n");
			break;
		case 'e': case 'E':
			if (is_right_momular == 0)//������ �˸��� �� �����Ѵ�.
				evaluation();
			else
				printf("The formula is wrong\n");
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');

	return 1;
}

void postfixPush(char x)  //postfix�� ��ȯ�� ���� �ʿ��� ���ÿ� ���� x�� �ִ´�
{
	if (postfixStackTop == MAX_STACK_SIZE - 1)//������ �� á�ٸ�ȳ��޽����� ����ϰ� �Լ��� �����Ѵ�.
		printf("postfixStack is fulled\n");
	else
		postfixStack[++postfixStackTop] = x;//������ top�� +1 �� �����ϰ� �ش� �ε����� ���޹��� ���ڸ� �ִ´�.
}

char postfixPop()//postfix�� ��ȯ�� ���� �ʿ��� ���ÿ��� ���� x�� ���� �����Ѵ�.
{
	char x;//���ڸ� ���� ����
	if (postfixStackTop == -1)//������ ��� �ִٸ� '\n'�� �����Ѵ�
		return '\0';
	else {//������� �ʴٸ� top��ġ�� �ε����� ����� ���� x�� �����ϰ� top�� -1�Ѵ�.
		x = postfixStack[postfixStackTop--];
	}
	return x;//���ڸ� �����Ѵ�.
}

void evalPush(int x)//����� ���� �ʿ��� ���ÿ� ���� x�� �ִ´�.
{
	if (evalStackTop == MAX_STACK_SIZE - 1)//������ �� á�ٸ�ȳ��޽����� ����ϰ� �Լ��� �����Ѵ�.
		printf("evalStack is fulled\n");
	else
		evalStack[++evalStackTop] = x;//������ top�� +1�ϰ� top�� ����x�� �����Ѵ�
}

int evalPop()//����� ���� �ʿ��� ���ÿ��� ���� x�� ���� �����Ѵ�.
{
	if (evalStackTop == -1)//������ ����ִٸ�
		return -1;//-1(����������)�� �����Ѵ�.
	else
		return evalStack[evalStackTop--];//��������ʴٸ� top��ġ�� ������ �����ϰ� top�� -1�Ѵ�.
}

/**
 * infix expression�� �Է¹޴´�.
 * infixExp���� �Էµ� ���� �����Ѵ�.
 */
void getInfix()
{
	printf("Type the expression >>> ");
	fflush(stdout);
	scanf("%s", infixExp);
	if (strlen(infixExp)> MAX_EXPRESSION_SIZE-1) {//�Է¹��� ������ infixExp�� ���̸� �ʰ��Ѵٸ� �ȳ��޽����� ����ϰ� �迭�� ���̸�ŭ �ڸ���.
		infixExp[MAX_EXPRESSION_SIZE - 1] = '\0';
		printf("\nIf you do this, it is difficult to expect the right answer.Please enter a length of %d\n\n", MAX_EXPRESSION_SIZE - 1);
	}
	is_right_momular = is_right();//�Է¹��� ������ �˸����� �Ǵ��Ѵ�.
	if (is_right_momular==-1)//������ �˸��� �ʴٸ� �ȳ��޽����� ����Ѵ�.
		printf("The formula is wrong\n");
}

precedence getToken(char symbol)//���Ŀ� ����� ���ڰ� �ǿ���������, ���� ������������ ���� �켱������ �����Ѵ�.
{
	switch (symbol) {
	case '(': return lparen;
	case ')': return rparen;
	case '+': return plus;
	case '-': return minus;
	case '/': return divide;
	case '*': return times;
	default: return operand;
	}
}

precedence getPriority(char x)//�ش� ������ �켱������ �����Ѵ�.
{
	return getToken(x);
}

/**
 * �����ϳ��� ���޹޾�, postfixExp�� �߰�
 */
void charCat(char* c)
{
	if (postfixExp == '\0')//postfixExp�� ����ִٸ� ù���ڸ� �����ϰ�
		strncpy(postfixExp, c, 1);
	else//�ƴ϶�� ���ʴ�� �̾������.
		strncat(postfixExp, c, 1);
}

/**
 * infixExp�� ���ڸ� �ϳ��� �о�鼭 stack�� �̿��Ͽ� postfix�� �����Ѵ�.
 * ����� postfix�� postFixExp�� ����ȴ�.
 */
void toPostfix()
{
	/*�� �Լ� �ȿ����� ��ü���� �켱������ �ο��Ѵ�. �� �Լ������� �켱������ �� �Լ� �ȿ����� ��ü���� �켱������ ���Ѵ�.*/
	static int isp[] = { 0,0,0,0,2,2,3,3,0,0 };//����� ���� �켱����
	char* exp = infixExp;/* infixExp�� ���� �ϳ����� �б����� ������ */
	char x; /* �����ϳ��� �ӽ÷� �����ϱ� ���� ���� */
	precedence i;//��ū �޴� ����

	/* exp�� �������Ѱ��鼭, ���ڸ� �а� postfix�� ���� */
	while (*exp != '\0')//������ ������ ���� �ݺ��Ѵ�.
	{
		i = getToken(*exp);//���Ŀ��� ���� ������ ��ū�� �����Ѵ�.
		switch (i) {
		case operand://���ڰ� �ǿ����ڶ�� �ش� ���ڸ� postFixExp�� �����Ѵ�.
			charCat(exp);
			break;
		case rparen://���ڰ� ')'��� postfixStack���� '('�� ���ö����� pop�Ѵ�.
			while (getToken(postfixStack[postfixStackTop]) != lparen) {
				x = postfixPop();
				charCat(&x);
			}
			postfixPop();//'('�� ���ö����� pop�ϴٰ� '('�� postfixStack�� top�� ��ġ�ϸ� '('�� �����Ѵ�
			break;
		case lparen://���ڰ� '('��� postfixStack�� push�Ѵ�
			postfixPush(*exp);
			break;
		default://���ڰ� �ǿ�����,'(',')' �� �ƴ϶�� �켱������ �Ǵ��ϰ� �˸´� ������ �Ѵ�.
			while((postfixStackTop!=-1)&&isp[getPriority(postfixStack[postfixStackTop])] >= isp[getPriority(*exp)]){
				/*postfixStack�� ����ְų� postfixStack�� top�� �ִ� �����ڰ� ���ں��� �켱������ ���� ������ top�ڸ��� �����ڸ� pop�ϰ� �Ѵ�.*/
				x = postfixPop();
				charCat(&x);
			}//postfixStack�� ����ְų� postfixStack�� top�� �ִ� �����ڰ� ���ں��� �켱������ ���� �� �ش� ���ڸ� push�Ѵ�.
			postfixPush(*exp);
			break;
		}
		exp++;//������ ���� ���ڸ� ����Ų��.
	}
	while (postfixStackTop != -1) {//������ �� ������ postfixStack�� �����ִ� �����ڵ��� ��� pop�Ѵ�.
		x = postfixPop();
		charCat(&x);
	}
}

void debug()//�����
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);//����ǥ������� ������ ǥ���Ѵ�.
	printf("postExp =  %s\n", postfixExp);//����ǥ������� ������ ǥ���Ѵ�.
	printf("eval result = %d\n", evalResult);//����ǥ������� ������ ����� ���� ǥ���Ѵ�.

	printf("postfixStack : ");//postfixStack�� ����� ������ ���캻��.
	for (int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");
}

void reset()//�ʱ�ȭ
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for (int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';

	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation() {//����ǥ������� ǥ���� ������ ����Ѵ�.
	char* exp = postfixExp;//����ǥ������� ���ڸ� �д´�.
	int op1, op2;//���ÿ��� pop�ް� ���꿡 �ʿ��� �����̴�
	precedence i = getToken(*exp);//��ū�޴� ����

	while (*exp != '\0') {//������ ���������� �ݺ��Ѵ�.
		if (i == operand)//�ǿ����ڶ��
			evalPush((int)*exp - 48);//�ƽ�Ű�ڵ带 ���������� �����Ͽ� evalStack�� push�Ѵ�.
		else {//�����ڶ�� ���꿡 �ʿ��� �ǿ����ڸ� pop�ϰ� ����Ѵ�.
			op2 = evalPop();// ���� ���� �� ������ ����
			op1 = evalPop();// ���� ���� �� ������ ����
			switch (i) {//�����ڿ� �°� ����ϰ� �� ���� evalStack�� push�Ѵ�.
			case plus: evalPush(op1 + op2); break;
			case minus: evalPush(op1 - op2); break;
			case times: evalPush(op1 * op2); break;
			case divide: evalPush(op1 / op2); break;
			}
		}
		exp++;//���� ���ڸ� �д´�.
		i = getToken(*exp);//���� ������ ��ū�� �����Ѵ�.
	}
	evalResult = evalPop();//���������� ���� ���� �˸´� ������ �����Ѵ�.
}

int is_right() {//����ǥ������� �Է¹޴� ������ �˸����� �˻��Ѵ�.
	precedence before = getToken(infixExp[0]); //ù ���ڿ�
	precedence after = 0; //�ι�° ���ں��� ���Ѵ�.
	int a = 0;//(�� ������ ���Ѵ�.
	int b = 0;//)�� ������ ���Ѵ�.

	if (before != operand&&before !=lparen)return -1;//������ ������ �ǿ����ڳ� '('�� �ƴ϶�� �߸��� �����̴�

	for (int i = 0; infixExp[i+1]!='\0'; i++) {
		before = getToken(infixExp[i]);//���� ���ڸ� �˻��Ѵ�.
		after = getToken(infixExp[i + 1]);
		switch (before) {
		case plus://�չ��ڰ� +,-,*,/ �϶� �޹��ڰ� �ǿ����ڳ� (�� �ƴ϶�� �߸��� �����̶� �Ǵ��ϰ� -1�� �����Ѵ�.
		case minus:
		case times:
		case divide:
			if (after != operand && after != lparen)
				return -1;
			break;
		case lparen://�չ��ڰ� ( �϶� �޹��ڰ� �ǿ����ڳ� (�� �ƴ϶�� �߸��ȼ����̴�
			a++;//(�� ������ +1���ش�
			if(after != operand && after!=lparen)
				return -1;
			break;
		case rparen:
			b++;//)�� ������ +1���ش�.
		default://�չ��ڰ� �ǿ����ڳ� )�϶� �޹��ڰ� �ǿ����ڳ� (��� �߸��� �����̴�
			if(after ==operand||after== lparen)
				return -1;
			break;
		}
	}
	if (after != operand && after != rparen)return -1;//������ ���� �ǿ����ڳ� ')'�� �ƴ϶�� �߸��� �����̴�
	if (after == rparen)b++;//������ �ǳ��� ')'�϶� ������ �÷��ش�.
	if (a != b)
		return -1;//(�� )�� ������ �ٸ��ٸ� �߸��� ���̴�.
	return 0;//��� ���ڰ� �˸´ٸ� �������� 0�� �����Ѵ�.
}
