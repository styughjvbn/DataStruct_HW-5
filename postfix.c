#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10 //스택의 크기
#define MAX_EXPRESSION_SIZE 20 //계산식의 크기

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum {
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */
int evalResult = 0;	   /* 계산 결과를 저장 */
int is_right_momular = 0; //계산식이 옳은지 판단
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
int is_right();//중위표기법으로 작성한 계산식이 알맞는 식인지 판별

int main()
{
	char command;

	printf("[----- [서종원] [2018038031] -----]\n\n");

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
			if(is_right_momular==0)//계산식이 알맞을 때 실행한다.
			toPostfix();
			else
				printf("The formula is wrong\n");
			break;
		case 'e': case 'E':
			if (is_right_momular == 0)//계산식이 알맞을 때 실행한다.
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

void postfixPush(char x)  //postfix로 변환을 위해 필요한 스택에 문자 x를 넣는다
{
	if (postfixStackTop == MAX_STACK_SIZE - 1)//스택이 꽉 찼다면안내메시지를 출력하고 함수를 종료한다.
		printf("postfixStack is fulled\n");
	else
		postfixStack[++postfixStackTop] = x;//스택의 top을 +1 을 먼저하고서 해당 인덱스에 전달받은 문자를 넣는다.
}

char postfixPop()//postfix로 변환을 위해 필요한 스택에서 문자 x를 빼서 리턴한다.
{
	char x;//문자를 받을 변수
	if (postfixStackTop == -1)//스택이 비어 있다면 '\n'을 리턴한다
		return '\0';
	else {//비어있지 않다면 top위치의 인덱스에 저장된 값을 x에 대입하고 top을 -1한다.
		x = postfixStack[postfixStackTop--];
	}
	return x;//문자를 리턴한다.
}

void evalPush(int x)//계산을 위해 필요한 스택에 정수 x를 넣는다.
{
	if (evalStackTop == MAX_STACK_SIZE - 1)//스택이 꽉 찼다면안내메시지를 출력하고 함수를 종료한다.
		printf("evalStack is fulled\n");
	else
		evalStack[++evalStackTop] = x;//스택의 top을 +1하고 top에 정수x를 저장한다
}

int evalPop()//계산을 위해 필요한 스택에서 정수 x를 빼서 리턴한다.
{
	if (evalStackTop == -1)//스택이 비어있다면
		return -1;//-1(비정상종료)를 리턴한다.
	else
		return evalStack[evalStackTop--];//비어있지않다면 top위치의 정수를 리턴하고 top을 -1한다.
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
	printf("Type the expression >>> ");
	fflush(stdout);
	scanf("%s", infixExp);
	if (strlen(infixExp)> MAX_EXPRESSION_SIZE-1) {//입력받은 계산식이 infixExp의 길이를 초과한다면 안내메시지를 출력하고 배열의 길이만큼 자른다.
		infixExp[MAX_EXPRESSION_SIZE - 1] = '\0';
		printf("\nIf you do this, it is difficult to expect the right answer.Please enter a length of %d\n\n", MAX_EXPRESSION_SIZE - 1);
	}
	is_right_momular = is_right();//입력받은 계산식이 알맞은지 판단한다.
	if (is_right_momular==-1)//계산식이 알맞지 않다면 안내메시지를 출력한다.
		printf("The formula is wrong\n");
}

precedence getToken(char symbol)//계산식에 저장된 문자가 피연산자인지, 무슨 연산자인지에 따라 우선순위를 리턴한다.
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

precedence getPriority(char x)//해당 문자의 우선순위를 리턴한다.
{
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0')//postfixExp이 비어있다면 첫문자를 복사하고
		strncpy(postfixExp, c, 1);
	else//아니라면 차례대로 이어붙힌다.
		strncat(postfixExp, c, 1);
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/*이 함수 안에서는 자체적인 우선순위를 부여한다. 이 함수에서의 우선순위는 이 함수 안에서의 자체적인 우선순위를 말한다.*/
	static int isp[] = { 0,0,0,0,2,2,3,3,0,0 };//계산을 위한 우선순위
	char* exp = infixExp;/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */
	precedence i;//토큰 받는 변수

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while (*exp != '\0')//계산식이 끝날때 까지 반복한다.
	{
		i = getToken(*exp);//계산식에서 읽은 문자의 토큰을 저장한다.
		switch (i) {
		case operand://문자가 피연산자라면 해당 문자를 postFixExp에 저장한다.
			charCat(exp);
			break;
		case rparen://문자가 ')'라면 postfixStack에서 '('가 나올때까지 pop한다.
			while (getToken(postfixStack[postfixStackTop]) != lparen) {
				x = postfixPop();
				charCat(&x);
			}
			postfixPop();//'('가 나올때까지 pop하다가 '('이 postfixStack의 top에 위치하면 '('를 삭제한다
			break;
		case lparen://문자가 '('라면 postfixStack에 push한다
			postfixPush(*exp);
			break;
		default://문자가 피연산자,'(',')' 이 아니라면 우선순서를 판단하고 알맞는 동작을 한다.
			while((postfixStackTop!=-1)&&isp[getPriority(postfixStack[postfixStackTop])] >= isp[getPriority(*exp)]){
				/*postfixStack이 비어있거나 postfixStack의 top에 있는 연산자가 문자보다 우선순위가 낮을 때까지 top자리의 연산자를 pop하고 한다.*/
				x = postfixPop();
				charCat(&x);
			}//postfixStack이 비어있거나 postfixStack의 top에 있는 연산자가 문자보다 우선순위가 낮을 때 해당 문자를 push한다.
			postfixPush(*exp);
			break;
		}
		exp++;//계산식의 다음 문자를 가르킨다.
	}
	while (postfixStackTop != -1) {//계산식이 다 끝나면 postfixStack에 남아있는 연산자들을 모두 pop한다.
		x = postfixPop();
		charCat(&x);
	}
}

void debug()//디버그
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);//중위표기법으로 계산식을 표현한다.
	printf("postExp =  %s\n", postfixExp);//후위표기법으로 계산식을 표현한다.
	printf("eval result = %d\n", evalResult);//후위표기법으로 계산식을 계산한 값을 표시한다.

	printf("postfixStack : ");//postfixStack에 저장된 내용을 살펴본다.
	for (int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");
}

void reset()//초기화
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for (int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';

	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation() {//후위표기법으로 표현한 계산식을 계산한다.
	char* exp = postfixExp;//후위표기법에서 문자를 읽는다.
	int op1, op2;//스택에서 pop받고 연산에 필요한 변수이다
	precedence i = getToken(*exp);//토큰받는 변수

	while (*exp != '\0') {//계산식의 마지막까지 반복한다.
		if (i == operand)//피연산자라면
			evalPush((int)*exp - 48);//아스키코드를 정수값으로 변경하여 evalStack에 push한다.
		else {//연산자라면 연산에 필요한 피연산자를 pop하고 계산한다.
			op2 = evalPop();// 스택 삭제 및 변수에 대입
			op1 = evalPop();// 스택 삭제 및 변수에 대입
			switch (i) {//연산자에 맞게 계산하고 그 값을 evalStack에 push한다.
			case plus: evalPush(op1 + op2); break;
			case minus: evalPush(op1 - op2); break;
			case times: evalPush(op1 * op2); break;
			case divide: evalPush(op1 / op2); break;
			}
		}
		exp++;//다음 문자를 읽는다.
		i = getToken(*exp);//다음 문자의 토큰을 구별한다.
	}
	evalResult = evalPop();//최종적으로 계산된 값을 알맞는 변수에 대입한다.
}

int is_right() {//중위표기법으로 입력받는 계산식이 알맞은지 검사한다.
	precedence before = getToken(infixExp[0]); //첫 문자와
	precedence after = 0; //두번째 문자부터 비교한다.
	int a = 0;//(의 개수를 구한다.
	int b = 0;//)의 개수를 구한다.

	if (before != operand&&before !=lparen)return -1;//계산식의 시작이 피연산자나 '('이 아니라면 잘못된 수식이다

	for (int i = 0; infixExp[i+1]!='\0'; i++) {
		before = getToken(infixExp[i]);//다음 문자를 검사한다.
		after = getToken(infixExp[i + 1]);
		switch (before) {
		case plus://앞문자가 +,-,*,/ 일때 뒷문자가 피연산자나 (가 아니라면 잘못된 수식이라 판단하고 -1을 리턴한다.
		case minus:
		case times:
		case divide:
			if (after != operand && after != lparen)
				return -1;
			break;
		case lparen://앞문자가 ( 일때 뒷문자가 피연산자나 (가 아니라면 잘못된수식이다
			a++;//(의 개수를 +1해준다
			if(after != operand && after!=lparen)
				return -1;
			break;
		case rparen:
			b++;//)의 개수를 +1해준다.
		default://앞문자가 피연산자나 )일때 뒷문자가 피연산자나 (라면 잘못된 수식이다
			if(after ==operand||after== lparen)
				return -1;
			break;
		}
	}
	if (after != operand && after != rparen)return -1;//계산식의 끝이 피연산자나 ')'이 아니라면 잘못된 수식이다
	if (after == rparen)b++;//계산식의 맨끝이 ')'일때 개수를 올려준다.
	if (a != b)
		return -1;//(와 )의 개수가 다르다면 잘못된 식이다.
	return 0;//모든 문자가 알맞다면 정상종료 0을 리턴한다.
}
