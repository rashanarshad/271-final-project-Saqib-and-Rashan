#include <iostream>
#include <string>
#include <ctime>
#include <time.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono> 
#include <deque>
using namespace std;



struct group //struct that serves as the restaurant patrons, holds neccessary information
{
	bool occupied; //more useful for the arrays that are used as tables, but essentially tells if table is occupied
	int numMembers; //sets the number of patrons in the group
	string name; //party name
	time_t timeEnter; //time the group enters
	

}; 

//declarations of all functions
int fillTable(group inputTable[], group inputGroup, int arraySize);
void checkTime(group inputTable[], int arraySize);
int waitTimeRemaining (time_t inputTime);
int twentySeatFillCheck(group inputTable[], int arraySize);
int twoSeatTablesLeft;
int fourSeatTablesLeft;

void main() //where the main starts
{
	time_t start_t, end_t; //time structs declared to use for wait time

	time(&start_t); //start time set to avoid future problems
	time(&end_t); //end time set to avoid future problems
	bool runGame = true;  //bool to run simulation, set to true so we enter simulation
	group twoSeatTables[4]; //array of groups of size 4 to simulate 4 2-seat tables
	group fourSeatTables[7]; //array of groups of size 7 to simulate 7 4-seat tables
	for (int i = 0; i < 4; i++) //two seats tables declared to be unoccupied
	{
		twoSeatTables[i].occupied = false;
	
	}
	for (int p = 0; p < 7; p++)  //four seats tables declared to be unoccupied
	{
		
		fourSeatTables[p].occupied = false;
	}
	
	
	deque<group> twoPeopleWaitingList;  //deque used as waiting list for two people (or less groups)
	deque<group> fourPeopleWaitingList; //deque used as waiting list for four people (or 3 people groups)
	deque<group> largeGroupWaitingList; //deque used as waiting list for >4 (but < 25) 
	deque<group> largeGroupSplit; //deque used to split large group into smaller 4 people groups
	int groupSize; //group size variable used to populate struct info
	string partyName; //party name used to populate struct info
	cout << "Hello, Welcome to The Krusty Krab" << endl;
	cout << " Please wait to be seated." << endl;
	cout << "There is a max of 36 seats.  But max allowance for one group is 24 for the courtesy of other guests" << endl;
	cout << "If group size if greater than 24 people, the group will be redirected to the Chum Bucket" << endl;
	cout << "The average eat time is 15 seconds for the purpose of testing, but can be modified for minutes" << endl;
	cout << "Customers waiting longer than 25 seconds tend to leave, but we are okay with that" << endl;
	
	while (runGame)
	{
		
		bool twentyPlace = false; //bool used to place large groups
		checkTime(twoSeatTables, 4); //checks to clear tables if patrons have  finished eating at two seat groups
		checkTime(fourSeatTables, 7);//checks to clear tables if patrons have  finished eating at four seat groups
		time(&end_t); //gets current time, sets it to end_t

		cout << "How many people in your group?" << endl; //asks user for group size
		cin >> groupSize;
		
		while (cin.fail() || groupSize < 0) //makes sure group types a valid number (non negative) and an int
		{
			cout << "Please type the NUMBER of guests in your party" << endl;
			cin.clear();
			cin.ignore(256, '\n');
			cin >> groupSize;
			cout << endl;

		}
		

		cout << "What is your party name?" << endl; //asks user for party name
		cin >> partyName;
		cout << endl;
		if (groupSize > 4  && groupSize <= 24) //creates element in large group and adds to waiting list
		{
			group newgroup;
			newgroup.numMembers = groupSize;
			newgroup.occupied = true;
			newgroup.name = partyName;
			time(&newgroup.timeEnter);
			largeGroupWaitingList.push_back(newgroup);
			
		}
		else if (groupSize > 24) //informs user restaurant cant hold that many people
		{
			cout << "Sorry, the restaurant cannot accomodate a party that large";
		}
		checkTime(twoSeatTables, 4); //tables cleared again
		checkTime(fourSeatTables, 7); //tables cleared again
		if (largeGroupWaitingList.size() > 0)  //if people are waiting as a large group, attempt to be seated
		{
			
			int seatsNeeded = 0; //seats needed for large group intialized
			while ((fourPeopleWaitingList.size() == 0 || (largeGroupWaitingList.front().timeEnter > fourPeopleWaitingList.front().timeEnter)) && twentyPlace == false) //if no one is waiting as a 4 person group, or if the large group arrived earlier than a 4 person group, and the bool is satisfied, then attempt to seat group
			{
				cout << "trying to seat "<<  largeGroupWaitingList.front().numMembers << " people " << endl;  //informs user how many people are trying to be seated
				if (largeGroupWaitingList.front().numMembers == 24)  //calculated how many sub groups of 4 are needed
					seatsNeeded = (largeGroupWaitingList.front().numMembers / 4);
				else
					seatsNeeded = (largeGroupWaitingList.front().numMembers / 4);
				largeGroupWaitingList.pop_front(); //pop off waiting list for large group

				for (int r = 0; r < seatsNeeded; r++) //number of sub groups created
				{
					
					group newgroup;
					newgroup.numMembers = groupSize;
					newgroup.occupied = true;
					newgroup.name = partyName;
					time(&newgroup.timeEnter);
					largeGroupSplit.push_back(newgroup); 
				}
			
				if (twentySeatFillCheck(fourSeatTables, 7)>= seatsNeeded) //fills tables with sub groups
				{
					
					if (fillTable(fourSeatTables, largeGroupSplit.front(), 7) > 0)
					{
					
						largeGroupSplit.pop_front();

						while (fourSeatTablesLeft > 0 && largeGroupSplit.size() > 0)
						{

							fillTable(fourSeatTables, largeGroupSplit.front(), 7);
							largeGroupSplit.pop_front();

						}
					}
					/*
					cout << "if statement entered" << endl;
					for (int w = 0; w < seatsNeeded; w++)
					{
						time(&fourSeatTables[w].timeEnter);
						fourSeatTables[w].occupied = true;

						
					}
					*/
					twentyPlace = true;
					break;

					
				}

			}
			for (int e = 0; e < 7; e++) //lists current status of table occupation
			{
				cout << "Four Seat Tables occupied if 1, table is free if 0: " << fourSeatTables[e].occupied << endl;
			}
		}

		if (groupSize >0 &&groupSize< 3) //groups of size 2 or less seated
		{
			group newgroup; //temp group created to hold info of new group
			newgroup.numMembers = groupSize; //num members set to group size
			newgroup.occupied = true; //occupied is true so when array is populated the value changes in the array
			newgroup.name = partyName; //party name set
			time(&newgroup.timeEnter); //time group entered set
			twoPeopleWaitingList.push_back(newgroup); //group pushed on to waiting list
			checkTime(twoSeatTables, 4); //check if tables need to be cleared

			if (fillTable(twoSeatTables, twoPeopleWaitingList.front(), 4) > 0) //fills tables if possible
			{
				twoPeopleWaitingList.pop_front(); //removes group from waiting list if seated

				while (twoSeatTablesLeft > 0 && twoPeopleWaitingList.size() > 0) //tries to seat remaining people on waiting list if possible
				{

					fillTable(twoSeatTables, twoPeopleWaitingList.front(), 4);
					twoPeopleWaitingList.pop_front();

				}
			}
			else
			{
				for (int w = 0; w < twoPeopleWaitingList.size(); w++) //lists out who is still waiting
				{
					cout << twoPeopleWaitingList[w].name << " is waiting" << endl;
				}
			}
			for (int r = twoPeopleWaitingList.size() - 1; r > 0; r--) //removes patrons who waited longer than 25 seconds
			{
				if (waitTimeRemaining(twoPeopleWaitingList[r].timeEnter) > 25)
				{
					twoPeopleWaitingList.pop_back();
				}
			}
			for (int e = 0; e < 4; e++) //couts status of tables
			{
				cout << "Two Seat Tables occupied if 1, table is free if 0: " << twoSeatTables[e].occupied << endl;
			}

			cout << "size of waiting list for two seat table is: " << twoPeopleWaitingList.size() << endl;
		}

		if (groupSize > 2 &&groupSize < 5) //seats 3 or 4 people group the same way as two seat tables work
		{
			group newgroup;
			newgroup.numMembers = groupSize;
			newgroup.occupied = true;
			newgroup.name = partyName;
			time(&newgroup.timeEnter);
			fourPeopleWaitingList.push_back(newgroup);
			checkTime(fourSeatTables, 7);

			if (fillTable(fourSeatTables, fourPeopleWaitingList.front(), 7) > 0)
			{
				fourPeopleWaitingList.pop_front();

				while (fourSeatTablesLeft > 0 && fourPeopleWaitingList.size() > 0)
				{

					fillTable(fourSeatTables, fourPeopleWaitingList.front(), 7);
					fourPeopleWaitingList.pop_front();

				}
			}
			else
			{
				for (int w = 0; w < fourPeopleWaitingList.size(); w++)
				{
					cout << fourPeopleWaitingList[w].name << " is waiting" << endl;
				}
			}
			for (int r = fourPeopleWaitingList.size() - 1; r > 0; r--)
			{
				if (waitTimeRemaining(fourPeopleWaitingList[r].timeEnter) > 25)
				{
					fourPeopleWaitingList.pop_back();
				}
			}
			for (int e = 0; e < 7; e++)
			{
				cout << "Four Seat Tables occupied if 1, table is free if 0: " << fourSeatTables[e].occupied << endl;
			}

			cout << "size of waiting list for four seat table is: " << fourPeopleWaitingList.size() << endl;
		}
		cout << "If you would like to end the program, type -1, if not, type anything else" << endl; //prompt to end simulation
		int endgame = 0;
		cin >> endgame;
		if (endgame == -1)
		{
			runGame = false;
		}
		
	}

	system("pause");
}
int fillTable(group inputTable[], group inputGroup, int arraySize) //function to fill tables with patrons
{
	bool ifSeated = false; //bool used to enter loop to seat 
	int indexCounter = 0; //index counter to run through indexes of array/tables
	int spotsOpen = 0; //returns the number of remaining spots

	while (ifSeated == false) 
	{

		if (indexCounter < arraySize) //makes sure that only index that exist are checked
		{
			if (inputTable[indexCounter].occupied == false) //checks if patrons are at table or not
			{
				inputTable[indexCounter] = inputGroup; //if not, seat group
				ifSeated = true; //ends while loop when if is done
				for (int i = 0; i < arraySize-1; i++) //runs through array to see how many tables are open
				{
					if (inputTable[i].occupied == false)
					{
						spotsOpen++;
					}
				}
				twoSeatTablesLeft = spotsOpen+1;  //sets how many tables are open to "global" variable
				fourSeatTablesLeft = spotsOpen+1; //sets how many tables are open to "global" variable
				return spotsOpen+1; 
			}
			else
			{
				indexCounter++; //increase index counter to iterate through array

			}
		}
		else
		{
			ifSeated = true; //ends while loop
			return 0; //returns if no tables are open
		}
	}
	
}
void checkTime(group inputTable[], int arraySize) //function to clear tables
{

	for (int q = 0; q < arraySize; q++) //runs through all tables by comparing their enterance time to 15 "minutes", if it is more, they leave
	{
		time_t tempTime; 
		time(&tempTime);
		if (difftime(tempTime, inputTable[q].timeEnter) >= 15)
		{
			inputTable[q].occupied = false; //chance occupation to false
		}

	

	}
	

}

int waitTimeRemaining (time_t inputTime) //function to check if people waiting are waiting too long
{
	time_t tempTime;
	time(&tempTime);
	return difftime(tempTime, inputTime);
}

int twentySeatFillCheck(group inputTable[], int arraySize) //function to check how many spots are open (mainly used for large groups)
{


	int spotsOpen = 0;
	for (int i = 0; i < arraySize-1; i++)
	{
		if (inputTable[i].occupied == false)
		{
			spotsOpen++;
		}
	}
	fourSeatTablesLeft = spotsOpen+1;

	return spotsOpen+1;

}
