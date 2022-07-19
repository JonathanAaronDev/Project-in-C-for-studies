#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int year;
	char month;
	char day; /* Month and day are char */
} DateOfBirth;

typedef struct {
	long long unsigned int Id; /* All the id's are positive 8 bytes integers*/
	char* Name;
	char* Family;
	DateOfBirth DateOfBirth;
	long long unsigned int PartnerId;
	long long unsigned int MotherId;
	long long unsigned int FatherId;
	char NumOfChildren;
	long long unsigned int* ChildrenPtr;
} person;

typedef struct {
	person* persons_list;
	int amount;
	int total;
} db_mgr;

void TheDays(DateOfBirth* date);
int YouCantAlwaysGetWhatYouWant(int day, int month, int year);
long long unsigned int WhoAreYou(long long unsigned int* temp_id, int check);
void OopsIDidItAgain(person* person1);
char* SayMyName(char* name);
void HitMeBabyOneMoreTime(person* person);
void CountOnMe(db_mgr* mgr, int index, const person* add);
void SeeYouAgain(db_mgr* mgr, person* parent);
void GoYourOwnWay(person* parent, person* child);
int FamilyAffair(db_mgr* mgr, long long unsigned int id, long long unsigned int* gen_array, int size);
void* Memory(int size, int sizeof1);
void WhatIveDone();
void init_db(db_mgr* mgr);
void print_person(const person* person1);
person* search_id(const db_mgr* mgr, long long unsigned int id);
char menu();
void add_person(db_mgr* mgr);
void Search_person(const db_mgr* mgr);
void printdb(const db_mgr* mgr);
void quit(db_mgr* mgr);
void search_parents(const db_mgr* base);
void search_by_name(const db_mgr* mgr);
void delete_person(db_mgr* mgr);
void get_gen(db_mgr* mgr);

void main()
{
	db_mgr base; /* Creating our db*/
	char choose = '0'; /* Initializing choose to 0 which is not an option*/
	init_db(&base); /* Setting our db attribute */
	while (choose != '8')
	{
		choose = menu(); /* Letting the user to choose what he wants to do*/
		switch (choose)
		{
		case '1': add_person(&(base));
			break;
		case '2': Search_person(&(base));
			break;
		case '3': search_parents(&(base));
			break;
		case '4': delete_person(&(base));
			break;
		case '5': get_gen(&(base));
			break;
		case '6': printdb(&(base));
			break;
		case '7': search_by_name(&(base));
			break;
		case '8': quit(&(base));
			break;
		}
	}
}

/*  General notes:
   1.The are 2 types of functions, one part is what you asked and the other is what we added.
   We decided to separate the 2 and put all the functions we added in a row and then put the functions you asked for.
   2. We did use many temp variables in different functions so we gave a serial number in case we used the same name more than once.
  */

/*
*         The function is named in honor of the Rolling Stones         *
*Input:   The function receives 3 arguments (int type, int type, int type).
*Process: The function checks if the date received is valid according to the db rules.
*Output:  The function returns 1(true) or 0(false).
*/

int YouCantAlwaysGetWhatYouWant(int day, int month, int year)
{
	return((day > 0 && day < 31) && (month > 0 && month < 13) && (year > 999 && year < 10000)); /* Check if the date is valid*/
}

/*
*         The function is named in honor of Avici                                                     *
*Input:   The function receive 1 argument (DateOfBirth* type)
*Process: The function asks the user to insert the person date of birth and checks if the date is valid.
*Output:  The function returns a valid date
*/

void TheDays(DateOfBirth* date)
{
	int year = 0, day = 0, month = 0;
	while (YouCantAlwaysGetWhatYouWant(day, month, year) == 0)
	{
		printf("Insert date of birth:DD/MM/YYYY.\nDD [1,30]\nMM [1,12]\nYYYY [1000,9999]\n"); 
		scanf("%d/%d/%d", &day, &month, &year);
	}
	date->day = (char)(day + '0'); /* Use '0' as a "key" to know the decimal value through ascii. */
	date->month = (char)(month + '0'); /* Use '0' as a "key" to know the decimal value through ascii. */
	date->year = year;
}

/*
*         The function is named in honor of The Who
*Input:   The function receive 2 argument (long long unsigned int*  type, int type)
*Process:
*		  -The function shows the user the message for the situation(based on int received).
*         -After showing the message the function asks for an id and check if its valid.
*Output:  The function returns a valid id.
*/

long long unsigned int WhoAreYou(long long unsigned int* temp_id, int check)
{
	long long int set = -1; /*Intialize set to a non valid value*/
	while (1) /* While true, want the loop to work until we get to break*/
	{
		printf("Please insert the id, valid Id,is a positive integer\n");
		scanf("%lld", &set);
		if ((set >= 1 && check != 1) || (set >= 0 && check == 1)) /* If check == 1 it's a partner/mother/father id and 0 id is allowed*/
		{
			*temp_id = (long long unsigned int)(set); /* After checking that the number is positive than we can insert him to unsigned. */
			break;
		}
	}
	return *temp_id;
}

/*
*		  The function is named in honor of Destiny's Child
*Input:   The function The function receives 1 argument (char* type)
*Process:
*         -The function gets name from the user.
*         -If malloc was successful returns a name
*		  -If malloc failed returns value NULL to name
*Output:  The function returns the value in char* type.
*/

char* SayMyName(char* name)
{
	char temp_name[100];
	fseek(stdin, 0, SEEK_END); /* Clean buffer*/
	gets(temp_name); /* Use gets in case there is a 2 words name with a space between words so scanf is not a fit*/
	temp_name[99] = 0; /* Make sure that temp_name is 0 terminated , the compiler kept on giving a warning that it might not be*/
	int len = (int)(strlen(temp_name) + 1);  /* Get the name length */
	name = (char*)(Memory(len, sizeof(char))); /* Use Memory function to create the memory allocation*/
	strcpy(name, temp_name); /* Copy the name from temp_name to name*/
	return name;
}

/*
*         The function is named in honor of Britney Spears           *
*Input:   The function receives 1 argument num(char type)
*Process: the function checks person has childrens by asking the user to insert it if not it would return NULL.
*         -If he has childrens the function would create an array in right size and fill it with his childrens Id.
*         -If he dosent have childrens the function would set his childrensPtr attribute to NULL
*         -The function will update his NumOfChildrens attribute based on the information.
*Output:  The function returns an array that is filled with the Id numbers of the requstend person childrens
*/

void HitMeBabyOneMoreTime(person* person)
{
	int childrens;
	printf("Do you have kids? if not please enter 0, if you do please enter the number you have\n");
	scanf("%d", &childrens);
	person->NumOfChildren = (char)(childrens + '0'); /* Use 0 as a key to know decimal value through ascii*/
	if (childrens == 0) /* If the person dosent have kids, its a special case*/
	{
		person->ChildrenPtr = NULL; /* Set his kids array pointer to NULL if he dose'nt have kids*/
		return; /* No need to continue if he dose'nt have kids*/
	}
	person->ChildrenPtr = (long long unsigned int*) (Memory(childrens, sizeof(long long unsigned int)));
	for (int i = 0; i < childrens; i++)
	{
		printf("Please insert the id number of your %d child\n", i+1);
		person->ChildrenPtr[i] = WhoAreYou(&person->ChildrenPtr[i], 2); /* Use the WhoAreYou function to get the id inside the array*/
	}
}

/*
*         The function is named in honor of Britney Spears.
*Input:   The function receives 1 argument(person* type).
*Process: The function asks the user to insert all the data needed to fill all of the person attributes.
*        -The function matches every piece of data to the right place.
*Output:  The function returns (person* type)
*/

void OopsIDidItAgain(person* person1)
{
	printf("Insert the citizen id\n");
	person1->Id = WhoAreYou(&person1->Id, 0); /* Put a value in the citizen id attribute*/
	printf("father id?, if you dont have one enter '0'\n");
	person1->FatherId = WhoAreYou(&person1->FatherId, 1); /* Put a value in the citizen father id attribute*/
	printf("mother id?, if you dont have one enter '0'\n");
	person1->MotherId = WhoAreYou(&person1->MotherId, 1); /* Put a value in the citizen mother id attribute*/
	printf("partner id?, if you dont have one enter '0'\n");
	person1->PartnerId = WhoAreYou(&person1->PartnerId, 1); /* Put a value in the citizen partner id attribute*/
	TheDays(&person1->DateOfBirth); /* Put a value in the citizen date attribute*/
	HitMeBabyOneMoreTime(person1); /* Put a value in the citizen numofchildrens and childrensPtr attributes*/
	printf("Please enter your first name\n");
	person1->Name = SayMyName(person1->Name); /* Put a value in the citizen first name attribute*/
	printf("Please enter your last name\n");
	person1->Family = SayMyName(person1->Family); /* Put a value in the citizen last name attribute*/
}

/*
*         The function is named in honor of Bruno Mars
*Input:  The function receives 3 arguments (db_mgr* type , int type , person* type).
*Process: The function sorts the array in ascending order, it checks every element in the array from last to first.
*         -When the place is right the function insert the element in the right place.
*Output:  The function returns pointer to person(person* type).
*/

void CountOnMe(db_mgr* mgr, int index, const person* add)
{
	for (int i = index; i >= 0; i--) /* Loop from the end to the start of the array */
	{
		if (add->Id < mgr->persons_list[i].Id) /* Move the bigger id forwrard to create space. */
			mgr->persons_list[i + i] = mgr->persons_list[i];
		else
		{
			mgr->persons_list[i] = *add; /* Found the right place. */
			break; /* After finding the right place and putting it in no need to continue*/
		}
	}
	mgr->amount++; /* We put a new person in the array so amount is bigger by one*/
}

/*        The function is named in honor of FleetwoodMac, about a child leaving home          *
*Input:   The function receive 4 arguments(db_mgr* type ,long long unsigned int type, char type, long long unsigned int* )
*Process: If the person is an only child the function free the array and set it to NULL.
*         -If he is not an only child the function creates a new array for the parnet with one less cell.
*         -The function copies all of the wanted childrens id and dosent copy the requsted child id.
*Output:  Void the function removes the person id from his parents array of childrens id.
*/

void GoYourOwnWay(person* parent, person* child)
{
	if (parent->NumOfChildren == '1') /* If the parent has 1 child it is a special case*/
	{
		parent->NumOfChildren = '0';
		parent->ChildrenPtr = NULL;
		return;
	}
	int size = (parent->NumOfChildren) - '0';
	long long unsigned int* temp_ptr = (long long unsigned int*)(Memory(size, sizeof(long long unsigned int)));
	for (int i = 0; i < size; i++)
	{
		if (parent->ChildrenPtr[i] != child->Id) /* Check if the id of the child in childrens[i] is not the one we want to delete */
			temp_ptr[i] = parent->ChildrenPtr[i]; /*Put only the Childrens we want inside the array*/
	}
	free(parent->ChildrenPtr);
	parent->ChildrenPtr = temp_ptr;
	parent->NumOfChildren = (char)(size + '0');
	temp_ptr = NULL;
}

/*
*		  The function is named in honor of Wiz Kahlifa and Charlie Puth see the removed person again        *
*Input:   The function receives 2 arguments(db_mgr* type, person* type)W
*Process: The function checks if the person we want to delete is a mother or a father and set the field 0.
*Output:  Void the function set the father/mother id field of a person to 0
*/

void SeeYouAgain(db_mgr* mgr, person* parent)
{
	int size = (parent->NumOfChildren) - '0';
	person* temp_person;
	for (int i = 0; i < size; i++)
	{
		temp_person = search_id(mgr, parent->ChildrenPtr[i]);
		if (temp_person == NULL) /* Check if there is a child in the parent array but he is not inside the db himself as a person*/
			continue;
		if (temp_person->FatherId == parent->Id) /* Check if the parent is the child father */
			temp_person->FatherId = 0; /* If he his change the child father id attribute in case he his a person in the db*/
		else /* If the parent is not a father it means that the parent is a mother*/
			temp_person->MotherId = 0;
	}
}

/*
*        The function is named in honor of Mary J.Blige
*Input:   The function receives 3 arguments(db_mgr* type, long long unsigned int type, long long unsigned int* type)
*Process: The function checks the requested person generation.
*        -The generation is set based on "Blocks" , each block starts with the first person of the generation and ends with the last one.
*Output:  The function returns an array that is filled with the Id numbers of the requested person childrens
*/

int FamilyAffair(db_mgr* mgr, long long unsigned int id, long long unsigned int* gen_array, int size)
{
	int first = 0, gen = 1, index = 1, last = 0, kids;
	person* temp_person2;
	while (first < size)/* Size is the maximum amount of people that can be associated to the check*/
	{
		temp_person2 = search_id(mgr, gen_array[first]); /* Get the details of the person which id is in gen_array[start]*/
		first++; /* Go to check for the next person in the block and insert his childrens to the next block */
		if (temp_person2 == NULL) /* In case there is an id in the childrensPtr that dosent exist in the db as a citizen*/
			continue;
		kids = (temp_person2->NumOfChildren) - '0'; /* We want to know how many kids the person in the start place has*/
		for (int i = 0; i < kids; i++) /* We want to put all of the kids of the person in the start place in gen_array*/
			gen_array[index++] = temp_person2->ChildrenPtr[i];/* Put the id numbers in the block they belong */
		if (first >= last) /* Check if you have reached the end of the block */
		{
			gen++; /* We want to increase gen when we know we have reached the end of a generational block*/
			last = index - 1; /* If start stands on the end of the block we want to move end to index - 1 which is the last person in the next block*/
		}
			if ((first + 1 == last) && (gen_array[first + 1] == 0)) /* We moved end to stand on index - 1 so if start == end it means that index didnt move at all and we didnt put any person in the next block*/
			break;
	}
	return gen;
}

/*        The function is named in honor of Linkin Park                             *
*Input:   The function receives no arguments
*Process: The function prints error message if allocation failed and ends the program.
*Output:  Void
*/

void WhatIveDone()
{
	printf("Memory allocation failed, the program will end now");
	exit(1);
}

/* The end of the functions we added and the start of functions you requested*/

/*
*Input:   The function receives 1 argument (person* type)
*Process: The function receives a pointer to person and prints his attributes.
*Output:  Void
*/


void print_person(const person* person1)
{
	int temp_numofkids; /* For comfort use a varibale to store the person amount of kids */
	printf("The person Id is %llu\n His full name is %s %s\n", person1->Id, person1->Name, person1->Family);
	printf("Partner Id %llu|Mom Id %llu|dad Id %llu\n", person1->PartnerId, person1->MotherId, person1->FatherId);
	printf("His Birthday is %d/%d/%d\n", (person1->DateOfBirth.day) - '0', (person1->DateOfBirth.month) - '0', person1->DateOfBirth.year);
	temp_numofkids = (person1->NumOfChildren) - '0';
	printf("He has %d childrens \n", temp_numofkids);
	for (int i = 0; i < temp_numofkids; i++)
		printf("His %d child Id is %llu\n", i + 1, person1->ChildrenPtr[i]);
}

/*        The function is named after the song Memory from the musical Cats                            *
*Input:   The function receives 2 argument (int type, int type).
*Process: The function creates a void pointer using malloc based on the argumnets received from the user.
*         -The function uses WhatIveDone to check if allocation failed
*Output:  Void* returns an allocated memory space based on user deamend.
*/

void* Memory(int size, int sizeof1)
{
	void* temp_allocation = malloc((long long int)(size) * sizeof1);
	if (temp_allocation == NULL)
		WhatIveDone();
	return temp_allocation;
}

/*
*Input:   The function receives 1 argument (db_mgr* type).
*Process: The function asks from the user amount of citizens in the country and then set the amount.
*         -If the allocation the function returns
*Output: Void, intialize the db.
*/

void init_db(db_mgr* mgr)
{
	int population = 0; /*Intializing population to a value that is invalid*/
	while (population < 1) /* The array size must be a positive integer(>0).*/
	{
		printf("Please insert the amount of People in the country, a positive integer\n");
		scanf("%d", &population);
	}
	mgr->persons_list = (person*)calloc(population, sizeof(person));
	if (mgr->persons_list == NULL)
		WhatIveDone(); /* If the allocation failed go to our pre made function which ends the program*/
	mgr->total = population; /* Set value in the total attribute*/
	mgr->amount = 0; /* Set value in the amount attribute*/
}

/*
*Input:   Void, no arguments given.
*Process: The function prints on the screen menu and let the user chose what he wants to do.
*Output:  The function returns the choice of the user(char type)
*/

char menu()
{
	char decision;
	while (1) /* While true, like requested the user will insert a char until he gets something valid*/
	{
		printf("\nDatabase System Menu:\n");
		printf("1.Add person\n2.Search a person\n3.Search Parents\n4.Delete a person\n");
		printf("5.Get generation\n6.Print database\n7.Search by name\n8.Quit\n");
		fseek(stdin, 0, SEEK_END);
		decision = getchar();
		if (decision > '0' && decision < '9')
			return decision;
		else
			printf("Wrong input, please try again\n");
	}
}

/*
*Intput:   The function receives 1 argument(db_mgr * type).
* Process : The function if db is full(using the amountand total atributtes of the the db structure)
* -If not the function uses OopsIDidItAgain to create a person structure with the user data
* -Then the function uses sort to put the structure in the right place.
* -If db is full the function creates a new allocation and copies all the elements expect the new one.
* -The function uses the sort to insert the new element.
* Output : The function returns pointer to person(person * type).
*/

void add_person(db_mgr* mgr)
{
	person* add = (person*)(Memory(1, sizeof(person)));
	if (add == NULL) /*Check if allocation failed. */
		WhatIveDone(); /* If memory allocation failed go to WhatIveDone */
	OopsIDidItAgain(add); /* Set values in this person attributes*/
	if (mgr->amount != mgr->total) /* Check if we reached a full array*/
		CountOnMe(mgr, mgr->amount, add); /* If not use CountOnMe to insert the new person in the right place*/
	else /* If we reached full array */
	{
		mgr->total++; /* Increase total attribute by 1*/
		person* temp_persons_list = (person*)calloc((mgr->total), sizeof(person)); /* Create a new array using calloc in the new total size*/
		if (temp_persons_list == NULL) /*Check if allocation failed.*/
			WhatIveDone(); /* If memory allocation failed go to WhatIveDone */
		else /* The compiler gave me a warning when i didnt use else that i might reference a NULL pointer eventhough my function does address it*/
			for (int i = 0; i < mgr->total - 1; i++) /* We increase total by 1 so and didnt put anything new yet*/
				temp_persons_list[i] = mgr->persons_list[i]; /* Copy the existing information the new array*/
		free(mgr->persons_list); /*Free the array before we copy a new one*/
		mgr->persons_list = temp_persons_list;
		temp_persons_list = NULL;
		CountOnMe(mgr, mgr->total - 1, add); /*Use count on me to put the new person in the right place*/
	}
	free(add);
	add = NULL;
}

/*
*Intput:  The function receives 2 arguments (db_mgr* type, long long unsigned int type).
*Process: The function checks if the id is in the database(db_mgr* mgr).
*         -If true the function returns pointer to person(person * type).
*         -If false the function  returns pointer to person(person * type) with value NULL.
*Output:  The function returns pointer to person (person * type).
*/

person* search_id(const db_mgr* mgr, long long unsigned int id)
{
	for (int i = 0; i < mgr->amount; i++) /* Check all the people in the array*/
		if (id == mgr->persons_list[i].Id)
			return &mgr->persons_list[i];
	return NULL;
}

/*
*Input:   The function receives one argument mgr (db_mgr* type)
*Process: The function asks for person ID
*         -If person ID isn't found the function prints "No person with this Id was found".
*         -If the function found the ID the function prints the ID.
*Output:  Void the function prints person deatils/prints a message
*/

void Search_person(const db_mgr* mgr)
{
	printf("Please enter the Id you want to check\n");
	long long unsigned int id = WhoAreYou(&id, 2);
	person* temp_person4 = search_id(mgr, id);
	if (temp_person4 == NULL)
		printf("No person with this Id was found\n");
	else
		print_person(temp_person4);
}

/*
*Input:   The function receives one argument mgr (db_mgr* type)
*Process: The function asks for ID of the person
*         -If the person is not in the db the function prints "No person with this Id was found"
*         -If the person dosent have parents the function prints "No parents were found"
*         -If the person has father the function print his father ID
*         -If the person has mother the function print his mother ID
*Output:  Void, prints a message
*/

void search_parents(const db_mgr* mgr)
{
	printf("Please enter the Id you want to check\n");
	long long unsigned int id = WhoAreYou(&id, 2);
	person* temp_person5 = search_id(mgr, id);
	if (temp_person5 == NULL)
	{
		printf("No person with this Id was found\n");
		return;
	}
	if (temp_person5->FatherId == 0 && temp_person5->MotherId == 0)
		printf("No parents were found\n");
	if (temp_person5->FatherId != 0 && search_id(mgr, temp_person5->FatherId) != NULL) /* Check if he has a father id attribute and if his father is in the db*/
	{
		printf("The details of the the father of %s %s are\n", temp_person5->Name, temp_person5->Family);
		print_person(search_id(mgr, temp_person5->FatherId));
	}
	if (temp_person5->MotherId != 0 && search_id(mgr, temp_person5->MotherId) != NULL) /* Check if he has a father id attribute and if his mother is in the db*/
	{
		printf("The details of the mother of %s %s are\n", temp_person5->Name, temp_person5->Family);
		print_person(search_id(mgr, temp_person5->MotherId));
	}
}

/*
*Input:   The function receives 1 arguments(db_mgr* type)
*Process: The function asks the user to insert an id. if the is not in the db the function would print a message and end the process
*         -If the person exist in the Id the function checks all of is his contact and disconnect him from them.
*         -Contacts = Partner, Mother, Father or Childrens.
*         -After disconnecting the person the functions than use copyarray to delete him from the array
*Output:  Void the function delete a person from the person array in mgr
*/

void delete_person(db_mgr* mgr)
{
	printf("please enter ID to delete:\n");
	long long unsigned int id = WhoAreYou(&id, 2);
	person* temp_person6 = search_id(mgr, id);
	person* temp_person7 = NULL;
	if (temp_person6 == NULL)
	{
		printf("Error no such person exist in the db\n");
		return;
	}
	if (temp_person6->PartnerId != 0) /* If the person exist in the db check his partner id field */
	{
		temp_person7 = search_id(mgr, temp_person6->PartnerId);
		if (temp_person7 != NULL) /* Check if the person partner in the db */
			temp_person7->PartnerId = 0;
	}
	if (temp_person6->FatherId != 0) /* If the person exist in the db check his father id field */
		temp_person7 = search_id(mgr, temp_person6->FatherId);
	if (temp_person6->MotherId != 0) /* If the person exist in the db check his mother id field */
		temp_person7 = search_id(mgr, temp_person6->MotherId);
	if (temp_person7 != NULL) /* Check if the person parent in the db */
		GoYourOwnWay(temp_person7, temp_person6);
	if (temp_person6->NumOfChildren != '0') /* If the person exist in the db check his childrens id field */
		SeeYouAgain(mgr, temp_person6); /* Call SeeYouAgain to delete him from his childrens attributes */
	mgr->total--;
	person* temp_persons_list2 = (person*) calloc((mgr->total), sizeof(person)); /* Create a temp array in the size we want*/
	if (temp_persons_list2 == NULL)
		WhatIveDone();
	else /* The compiler gave me a warning when i didnt use else that i might reference a NULL pointer eventhough my function does address it*/
	{
		for (int i = 0, j = 0; i < mgr->amount; i++) /* if the allocation worked stars the loop */
		{
			if (mgr->persons_list[i].Id != temp_person6->Id) /* copy all the persons to the new array accept from the person we received */
				temp_persons_list2[j++] = mgr->persons_list[i]; /* we dont want to have an empty cell in our array so we will use diffrent counters for reading and writing */
		}
	}
	mgr->amount--;
	free(mgr->persons_list);
	mgr->persons_list = temp_persons_list2;
	temp_persons_list2 = NULL;
}

/*
*Input:   The function receives 1 arguments(db_mgr* type)
*Process: The function first check if the generation tree is complete or there are childrens id that do not exist in the db.
*         -After the search if we do find "missing links" the user can decide to forfeit his attempt or continue.
*         -If the user continues the function creates an array that would hold the generation blocks.
*         -The requested person would be [0] and the function sends the array to FamilyAffair.
*         -With db_mgr* and the id of the requseted person.
*Output:  Void the function prints the requested person generation.
*/

void get_gen(db_mgr* mgr)
{
	person* temp_person8;
	printf("Please insert the requsted person Id num\n");
	int size = 0;
	long long unsigned int id = WhoAreYou(&id, 2);
	temp_person8 = search_id(mgr, id);
	if (temp_person8 == NULL)
	{
		printf("No person with this id number exists in the db\n");
		return;
	}
	if (temp_person8->NumOfChildren == '0') /*Special case, no need to start searching*/
	{
		printf("The requsted person id %llu is generation 1\n", temp_person8->Id);
		return;
	}
	for (int i = 0; i < mgr->amount; i++)
		size = size + (mgr->persons_list[i].NumOfChildren - '0'); /* Calculate the amount of kids in order to create the array in the right size even in case that not all of the kids are persons in the db*/
	long long unsigned int* gen_array = (long long unsigned int*) calloc(size + 1, sizeof(long long unsigned int));
	if (gen_array == NULL)/* Check if malloc Failed */
		WhatIveDone();
	else /* The compiler gave me a warning when i didnt use else that i might reference a NULL pointer eventhough my function does address it*/
		gen_array[0] = temp_person8->Id; /*Put the id of the person we want the check in the 0 cell in the array*/
	int gen = FamilyAffair(mgr, id, gen_array, size);
	printf("The requsted person id % llu is generation is %d\n", temp_person8->Id, gen);
	free(gen_array);
	gen_array = NULL;
	temp_person8 = NULL;
}

/*
*Input:   The function receives 1 arguments(db_mgr* type)
*Process: The function use a for loop and printperson to print all the details of every person in our db.
*Output:  Void the function prints the details of every person.
*/

void printdb(const db_mgr* mgr)
{
	printf("The amount of people currently registered in our db is %d\n", mgr->amount);
	for (int i = 0; i < mgr->amount; i++)
	{
		printf("\nThe details of the %d person in our db are:\n", i + 1);
		print_person(&mgr->persons_list[i]);
	}
}

/*
*Input:   The function receives 1 arguments(db_mgr* type)
*Process: The function search if there are persons in our db with first and last name the user inserted.
*         -If there is no such a person with such name in the function will print a message
*Output:  Void the functions prints the persons details/ prints a message
*/

void search_by_name(const db_mgr* mgr)
{
	int index = 0;
	char* name = NULL;
	char* last = NULL;
	printf("Please enter a first name to check\n");
	name = SayMyName(name);
	printf("Please enter a last name to check\n");
	last = SayMyName(last);
	if (name != NULL && last != NULL)
	{
		for (int i = 0; i < mgr->amount; i++)
		{
			if ((strcmp(name, mgr->persons_list[i].Name)) == 0 && (strcmp(last, mgr->persons_list[i].Family)) == 0)
			{
				print_person(&mgr->persons_list[i]);
				index++;
			}
		}
	}
	if (index == 0)
		printf("No match was found\n");
}

/*
*Input:   The function receives 1 arguments(db_mgr* type)
*Process: The function uses for loop to free all the wanted allocations and then the program is finished.
*Output:  Void the function ends the program.
*/

void quit(db_mgr* mgr)
{
	printf("You have choosen to finish the program and quit\n");
	for (int i = 0; i < mgr->amount; i++)
	{
		free(mgr->persons_list[i].Name);
		mgr->persons_list[i].Name = NULL;
		free(mgr->persons_list[i].Family);
		mgr->persons_list[i].Family = NULL;
		free(mgr->persons_list[i].ChildrenPtr);
		mgr->persons_list[i].ChildrenPtr = NULL;
	}
	free(mgr->persons_list);
	mgr->persons_list = NULL;
}