/*************************************************************************
        Copyright © 2021 Konstantinidis Konstantinos
	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at
        http://www.apache.org/licenses/LICENSE-2.0
	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HMax 11
#define VMax 100
#define EndOfList -1

typedef enum{
    Whitening=1, Cleaning=2, Extraction=3
}service_type;

typedef struct{
	double paid;
	service_type service;
} visit_type;

typedef visit_type ListElementType;

typedef struct{
	int RecKey;
	ListElementType Data;
	int Link;
} ListElm;

typedef struct{
	int HashTable[HMax];
    int Size;
	int SubListPtr;
    int StackPtr;
	ListElm List[VMax];
} HashListType;

typedef enum{
    FALSE, TRUE
} boolean;

void CreateHashList(HashListType *HList);
int HashKey(int Key);
boolean FullHashList(HashListType HList);
void SearchSynonymList(HashListType HList,int KeyArg,int *Loc,int *Pred);
void SearchHashList(HashListType HList,int KeyArg,int *Loc,int *Pred);
void AddRec(HashListType *HList,ListElm InRec);
void menu(int *choice);
int GetCharacterSum(char *Name);
void NewVisit(HashListType *DB);
void SearchVisit(HashListType DB);

int main(){
    int choice;
    HashListType DB;
    CreateHashList(&DB);

    do{
        menu(&choice);

        if(choice == 1)
            NewVisit(&DB);
        else if(choice == 2)
            SearchVisit(DB);
    }while(choice != 3);

    printf("Exiting the program.. \n");

    return 0;
}

void menu(int *choice){
    printf("\t\t\tMENU\n");
    printf("-------------------------------------------------\n");
    printf("1. INSERT APPOINTMENT\n");
    printf("2. PRINT CLIENT'S APPOINTMENTS\n");
    printf("3. EXIT\n");
    do{
        printf("\nEnter the number of your choice: ");
    	scanf("%d", choice);
    	if(*choice<1 || *choice>3){
            printf("This is not a given choice...\n\n");
    	}
    }while(*choice<1 && *choice>3);
}

int HashKey(int Key){
	return Key%HMax;
}

void CreateHashList(HashListType *HList){
	int index;

	HList->Size = 0;
	HList->StackPtr = 0;
    index = 0;
	while(index<HMax){
		HList->HashTable[index] = EndOfList;
		index = index+1;
    }

    index = 0;
	while(index<VMax-1){
		HList->List[index].Link = index+1;
		HList->List[index].Data.paid = 0;
		HList->List[index].Data.service = 0;
		index += 1;
	}
	HList->List[index].Link = EndOfList;
}

boolean FullHashList(HashListType HList){
	return(HList.Size==VMax);
}

void SearchSynonymList(HashListType HList,int KeyArg,int *Loc,int *Pred){
	int Next;

	Next = HList.SubListPtr;
	*Loc = -1;
	*Pred = -1;
	while(Next != EndOfList){
		if(HList.List[Next].RecKey == KeyArg){
			*Loc = Next;
			Next = EndOfList;
		}
		else{
			*Pred = Next;
			Next = HList.List[Next].Link;
		}
	}
}

void SearchHashList(HashListType HList,int KeyArg,int *Loc,int *Pred){
	int HVal;

	HVal = HashKey(KeyArg);
    if(HList.HashTable[HVal] == EndOfList){
		*Pred = -1;
		*Loc = -1;
	}
	else{
		HList.SubListPtr = HList.HashTable[HVal];
		SearchSynonymList(HList,KeyArg,Loc,Pred);
	}
}

void AddRec(HashListType *HList,ListElm InRec){
	int Loc, Pred, New, HVal;

	if(!(FullHashList(*HList))){
		Loc = -1;
		Pred = -1;
		SearchHashList(*HList,InRec.RecKey,&Loc,&Pred);
		if(Loc == -1){
			HList->Size = HList->Size + 1;
			New = HList->StackPtr;
			HList->StackPtr = HList->List[New].Link;
			HList->List[New] = InRec;
			if(Pred==-1){
			    HVal = HashKey(InRec.RecKey);
                HList->HashTable[HVal] = New;
				HList->List[New].Link = EndOfList;
			}
			else{
                HList->List[New].Link = HList->List[Pred].Link;
				HList->List[Pred].Link = New;
			}
		}
		else
			printf("A client with the same name is already in the catalogue\n");
	}
	else
		printf("The list is full can't add more...");
}

int GetCharacterSum(char *Name){
    int index;
    int key;
    char a = 'A' - 1;

    index = 0;
    key = 0;
    while(TRUE){
        if(index > strlen(Name))
            break;
        else
            key += (index + 1) * (Name[index] - a);

        index++;
    }
    return key;
}

void NewVisit(HashListType *DB){
    char ch;
    ListElm AnItem;
    ListElementType visit;
    char client[20];

    do{
        printf("Enter the client's Name: ");
        scanf("%s", client);
        getchar();

        printf("Enter the service number:\n1) Whitening\n2) Cleaning\n3) Extraction\n");
        scanf("%d",&visit.service);
        while(visit.service<1 || visit.service>3){
            printf("Incorrect service.\n");
            printf("Please enter a correct service number:\n1) Whitening\n2) Cleaning\n3) Extraction\n");
            scanf("%d",&visit.service);
        }
        getchar();

        printf("Enter the amount paid: ");
        scanf(" %lf",&visit.paid);
        getchar();
        printf("\n");

        AnItem.Data = visit;
        AnItem.RecKey = GetCharacterSum(client);
        AddRec(DB, AnItem);

        printf("Continue Y/N: ");
        scanf(" %c", &ch);
        while(ch!= 'Y' && ch!= 'N'){
            printf("Wrong input.\n");
            printf("Continue Y/N: ");
            scanf(" %c", &ch);
        }
    }while(ch != 'N');
}

void SearchVisit(HashListType DB){
    int Akey;
    int Loc, Pred;
    char client[20];
    int service;
    double paid;

    printf("Enter the client's Name: ");
    scanf(" %s", client);
    getchar();

    Akey = GetCharacterSum(client);
    SearchHashList(DB, Akey, &Loc, &Pred);

    if(Loc!=-1){
        printf("Service: %d\n", DB.List[Loc].Data.service);
        printf("Amount Paid: %.2f\n", DB.List[Loc].Data.paid);
    }
    else
        printf("This customer does not exist..\n\n\n");
}
