/*Yanin Niramai*/
/*260983558*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
struct ACCOUNT {
      int accountNumber;
      double startingBalance;
      double endingBalance;
      struct ACCOUNT *next;
};
void printList(struct ACCOUNT *head) {  
	struct ACCOUNT *x = head;
   	while(x != NULL) {
   	   printf("%d",x->accountNumber);
   	   printf(" %.2lf",x->startingBalance);
   	   printf(" %.2lf\n", x->endingBalance);
   	   x = x->next;
  	}
}
struct ACCOUNT *makeList(int accountNumber, double startingBalance, double endingBalance){
	struct ACCOUNT *x = (struct ACCOUNT*) malloc(sizeof(struct ACCOUNT));
	x->accountNumber = accountNumber;
	x->startingBalance=startingBalance;
	x->endingBalance= endingBalance;
	x->next = NULL;
	return x;
}
void insert1(struct ACCOUNT **head, struct ACCOUNT *nodetoinsert){
	nodetoinsert->next = *head;
	*head = nodetoinsert;

}

void reverse(struct ACCOUNT **head){
	struct ACCOUNT *prev = NULL;
	struct ACCOUNT *current = *head;
	struct ACCOUNT *next = NULL;
	while (current != NULL){
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	*head = prev;
}
void sort(struct ACCOUNT **head){
	int tempa;
	double tempb,tempc;
	struct ACCOUNT *temp1;
	struct ACCOUNT *temp2;
	for (temp1=*head;temp1!=NULL;temp1=temp1->next){
		for(temp2=temp1->next;temp2!=NULL;temp2=temp2->next){
			
			if(temp2->accountNumber < temp1->accountNumber){
				tempa = temp1->accountNumber;
				tempb = temp1->startingBalance;
				tempc = temp1->endingBalance;
				temp1->accountNumber = temp2->accountNumber;
				temp1->startingBalance = temp2->startingBalance;
				temp1->endingBalance = temp2->endingBalance;
				temp2->accountNumber = tempa;
				temp2->startingBalance = tempb;
				temp2->endingBalance = tempc;
			}
		}
	}
}

int findNum(struct ACCOUNT *head, int accountNum, char mode, float absoluteValueAmount){
	struct ACCOUNT *x = head;
        while(x != NULL) {
		if (x->accountNumber == accountNum){
			if (mode == 'd'){
				x->startingBalance = x->startingBalance+absoluteValueAmount;
			}
			if (mode =='w'){
				x->startingBalance = x->startingBalance-absoluteValueAmount;
				if (x->startingBalance < 0){
					printf("Balance below zero error (account, mode, transaction, startingBalance before): %d %c %f %.2f", accountNum, mode, absoluteValueAmount, x->startingBalance);
					x->startingBalance = 0;	
				}
			}
			return 0;
		}
		x = x->next;
	}
	printf("Account not found (account, mode, amount): %d, %c, %.2f\n", accountNum, mode, absoluteValueAmount);
}

void checkEqual(struct ACCOUNT *head){
	struct ACCOUNT *x = head;
        while(x != NULL) {
		double a;
	       	a = floor((100*x->startingBalance)/100);
		double b;
	        b= floor((100*x->endingBalance)/100);
		if (a != b){
			printf("End of day balances do not agree (account, starting, ending): %d, %.2lf, %.2lf\n", x->accountNumber, x->startingBalance, x->endingBalance);
		}
		x  = x->next;
	}
}

int main(int argc, char *argv[])
{
	if (argc != 3){
		printf("Wrong number of arguments!");
		printf("Program syntax: ./tv STATE TRANSACTIONS\n");
        	printf("Legal usage examples: ./tv state.csv transaction.csv\n");
        	printf("./tv ../state.csv /data/log/transaction.csv\n");
		exit (1);
	}
	char *filename1 = argv[1];
	char *filename2 = argv[2];
	if (fopen(argv[1], "r") == NULL && fopen(argv[2], "r") == NULL){
		printf("Unable to open filename %s\n",filename1);
		printf("Unable to open filename %s\n",filename2);
		printf("Program syntax: ./tv STATE TRANSACTIONS\n");
       		printf("Legal usage examples: ./tv state.csv transaction.csv\n");
        	printf("./tv ../state.csv /data/log/transaction.csv\n");
		exit (2);
	}
	else if (fopen(argv[1], "r") == NULL){
		printf("Unable to open filename %s\n",filename1);
                printf("Program syntax: ./tv STATE TRANSACTIONS\n");
                printf("Legal usage examples: ./tv state.csv transaction.csv\n");
                printf("./tv ../state.csv /data/log/transaction.csv\n");
                exit (2);
        }
	else if (fopen(argv[2], "r") == NULL){
		printf("Unable to open filename %s\n",filename2);
        	printf("Program syntax: ./tv STATE TRANSACTIONS\n");
                printf("Legal usage examples: ./tv state.csv transaction.csv\n");
                printf("./tv ../state.csv /data/log/transaction.csv\n");
		exit (2);
	}
	FILE *file1 = fopen(argv[1], "r");
	FILE *file2= fopen(argv[2], "r");
	int c = fgetc(file1);
	int b = fgetc(file2);
	if (c== EOF&& b!=EOF){
		puts("File state.csv is empty. Unable to validate transaction.csv.");
		exit (3);
	}
	struct ACCOUNT *head=NULL;
	struct ACCOUNT *tmp;
	char  array[1000]="";
        fgets(array,1000, file1);
        for(int i=0; i<1000;i++){
                if (array[i] == 44){
                        array[i] = ' ';
                }
        }
        int i = 0;
	int accountNumber;
	double startingBalance;
	double endingBalance;
        while(fgets(array, 1000, file1)){
                sscanf(array, "%d %lf %lf", &accountNumber, &startingBalance, &endingBalance);
		tmp =makeList(accountNumber, startingBalance, endingBalance);
		if (i==0){
			insert1(&head, tmp);
		}
		else{
			if (tmp->accountNumber == head->accountNumber){
				printf("Duplicate account number [account, start, end] : %d, %.2f, %.2f\n", accountNumber, startingBalance, endingBalance);
				continue;
			}
			else{
				insert1(&head, tmp);
			}
		}
		i++;

	}
	reverse(&head);
	sort(&head);
	fclose(file1);

	char newarr[1000]="";
        fgets(newarr,1000, file2);
	int accountNum;
	char mode;
	float absoluteValueAmount;
	while(fgets(newarr, 1000, file2)){
		for(int j=0; j<1000;j++){
			if (newarr[j] == 44){
                        	newarr[j] = ' ';
                	}
		}
		sscanf(newarr, "%d %c %f", &accountNum,&mode, &absoluteValueAmount);
		findNum(head, accountNum, mode, absoluteValueAmount);
	}
	checkEqual(head);
	fclose(file2);
	struct ACCOUNT *x = head;
	while (x != NULL){
		free(x);
		x=x->next;
	}

	return 0;
}

