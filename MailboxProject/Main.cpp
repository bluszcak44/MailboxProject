//Written by: Blaine Luszcak
#include <iostream>
#include <cstdlib>
#include <bitset>
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <numeric>
#include "Mailbox.h"

using namespace std;

//prototype function calls
void readMbox(Mailbox &mailbox, string fileName);
void saveMbox(Mailbox &mailbox, string fileName);
void updateMbox(Mailbox &mailbox, string fileName);
void clearAllVectors(Mailbox &mailbox);
void appendMessage(Mailbox &mailbox, string input);
void deleteMsg(Mailbox &mailbox, int usrIn);
void parseMessages(Mailbox &mailbox);
void menuOptions();
void displayMessage(Mailbox &mailbox, int usrIn);
vector<int> difference(vector<int> &v1, vector<int> &v2);
void listHeaders(Mailbox &mailbox);

int main() {

	string input;
	int quit;

	//Creates a mailbox object
	Mailbox *mbox = new Mailbox();

	int notBlank;
	bool firstOpen;
	firstOpen = NULL;

	quit = 0;
	notBlank = 0;

	while (input != "quit" && quit != 1) {
		string file, fileUsed, keepFile;
		
		while (firstOpen != true) {
			cout << "Starting Mailbox Options:" << endl;
			cout << "- Please enter a mailfile to open (example: sampleMbox.txt) OR if you want a default mailbox type (\"default\") and \"mbox.txt\" will open" << endl;
			cout << "- If you want a blank file type: \"blank.txt\"" << endl;
			cout << "- To open another mailbox file type later in the program type: \"Enter\" and then use command \"open\"" << endl;
			cout << "----------------------------------------------------------------------------------------------------------------------------------------" << endl;
			
			cin >> file;
			mbox->fileInput.push_back(file); 
			keepFile = mbox->fileInput[0];
			
			//open blank and save blank mailbox
			if (file == "blank.txt") {
				firstOpen = true;
			//open your own mailbox
			} else if (file == "Enter") {
				menuOptions();
				cin >> input;
				firstOpen = true;
			//open mbox.txt default mailbox
			} else if (file == "default") {
				fileUsed = "mbox.txt";
				mbox->fileInput.clear();
				mbox->fileInput.push_back(fileUsed);

				cout << "Test" << endl;
				firstOpen = true;
			}
			//open external mailbox file
			else {
				notBlank = 1;
				firstOpen = true;
			}
		}

		if (input == "list") {
			//List function
			listHeaders(*mbox);

			menuOptions();
			cin >> input;
		} else if (input == "append") {
			//Appened function
			cout << "What is the filename of the message to be appended? (example: append.txt) " << endl;
			cin >> input;

			appendMessage(*mbox, input);
			updateMbox(*mbox, mbox->fileInput[0]);
			clearAllVectors(*mbox);
			readMbox(*mbox, mbox->fileInput[0]);   //file specified on input
			parseMessages(*mbox);

			menuOptions();
			cin >> input;

		} else if (notBlank == 1) {
			//Open specified mailbox file
			readMbox(*mbox, file);
			parseMessages(*mbox);
			notBlank = 0;

			menuOptions();
			cin >> input;
		}
		else if (input == "open") {
			clearAllVectors(*mbox);
			mbox->fileInput.clear(); //clear the vector so we can put in a real file name later
			cout << "Please enter a mailfile to open (example: mbox.txt)" << endl;
			cin >> file;
			mbox->fileInput.push_back(file); //keep this for later
			keepFile = mbox->fileInput[0];

			readMbox(*mbox, keepFile);
			parseMessages(*mbox);

			menuOptions();
			cin >> input;
		}
		else if (file == "default") {
			readMbox(*mbox, mbox->fileInput[0]);
			parseMessages(*mbox);

			menuOptions();
			cin >> input;
		} else if (file == "blank.txt") {
			//blank mailbox that will be "blank.txt"
			break; //goes to automatically name/save a blank .txt file

		} else if (input == "delete") {
			//delete function
			int usrIn;
			
			cout << "Which message would you like to delete? " << endl;
			cin >> usrIn;

			deleteMsg(*mbox, usrIn);
			updateMbox(*mbox, mbox->fileInput[0]); //file specified on input
			readMbox(*mbox, mbox->fileInput[0]);   //file specified on input
			parseMessages(*mbox);

			menuOptions();
			cin >> input;
		} else if (input == "display") {
			int usrIn;
			cout << "Which message would you like to display? " << endl;
			cin >> usrIn;

			displayMessage(*mbox, usrIn);
			menuOptions();
			cin >> input;
		} else {
			//display menu list of function calls
			cout << "Please select an option from the menu. " << endl;

			menuOptions();
			cin >> input;
		}
	}
	//Take in a file name and save
	string fileName;

	cout << "Name the save output file with .txt at the end" << endl;
	cin >> fileName;
	saveMbox(*mbox, fileName); 
}

//reads in a mbox file into the application
void readMbox(Mailbox &mailbox, string fileName) {

	string contents = " "; 
	ifstream file(fileName.c_str());  //need the c_str() for it it compile on Linux/Unix

	int count = 0;

	if (file.is_open()) {
		while (getline(file, contents)) {

			mailbox.contentsVec.push_back(contents);
			count++;

			if (file.eof()) {
				cout << "End of file" << endl;
			}
		}
	} else {
		cout << "File could not be opened." << endl;
	}
}

//update save after appending or deleting
void updateMbox(Mailbox &mailbox, string fileName) {

	ofstream fileOut;
	fileOut.open(fileName);

	if (fileOut.is_open()) {
		for (auto i = mailbox.contentsMsg.begin(); i != mailbox.contentsMsg.end(); i++) {
			fileOut << *i << '\n';
		}
		fileOut.close();
	}
	else {
		cout << "File could not be opened." << endl << endl;
	}

	//NEED to clear the vectors so they can be reused with the same object -- all but fileInput
	mailbox.contentsMsg.clear();
	mailbox.contentsVec.clear();
	mailbox.finalFrom.clear();
	mailbox.finalSubject.clear();
	mailbox.msgId.clear();
	mailbox.msgIDNew.clear();
	mailbox.ignoreFrom.clear();
	mailbox.subjectId.clear();
	mailbox.frmID.clear();
	mailbox.lineTracker.clear();
	mailbox.lineTrackerNew.clear();
	mailbox.rightFrom.clear();
	mailbox.duplicateFrom.clear();
	mailbox.apndLineTracker.clear();
	mailbox.appendId.clear();
	mailbox.contentsAppend.clear();
}

//Save the mailbox to a file - FINAL SAVE
void saveMbox(Mailbox &mailbox, string fileName) {
	ofstream fileOut;
	fileOut.open(fileName);

	if (fileOut.is_open()) {
		cout << endl << "Beginning to write to the file" << endl;
		for (auto i = mailbox.contentsMsg.begin(); i != mailbox.contentsMsg.end(); i++) {
			fileOut << *i << '\n';
		}
		cout << "End of file" << endl << endl;
		printf("Press enter to continue...\n");
		getchar();
		fileOut.close();
	}
	else {
		cout << "File could not be opened." << endl << endl;
	}

	//NEED to clear the vectors so they can be reused with the same object -- fileInput cleared
	mailbox.contentsMsg.clear();
	mailbox.contentsVec.clear();
	mailbox.finalFrom.clear();
	mailbox.finalSubject.clear();
	mailbox.msgId.clear();
	mailbox.msgIDNew.clear();
	mailbox.ignoreFrom.clear();
	mailbox.subjectId.clear();
	mailbox.frmID.clear();
	mailbox.lineTracker.clear();
	mailbox.lineTrackerNew.clear();
	mailbox.rightFrom.clear();
	mailbox.duplicateFrom.clear();
	mailbox.fileInput.clear();
	mailbox.apndLineTracker.clear();
	mailbox.appendId.clear();
	mailbox.contentsAppend.clear();
}

//Finds all the "From - " lines, keeps track of those, keeps track of end of vector, pushes whole messages into individual elements
void parseMessages(Mailbox &mailbox) {

	vector<string> keyWordFromStrt, keyWordFrom, keyWordSubject, keyIgnoreFrom; //Fromstrt is start of message, from is header & subject is header
	vector<string>::iterator it;
	int msgCnt, frmCnt, subjectCnt, ignoreCnt;
	int pos, exit;//what lines the "From -"s are on
	string temp;

	//Search criteria
	keyWordFromStrt = { "From -" }; //msg start
	keyWordFrom = { "From: " }; //msg header, escaping on From: " <--- the quote is included
 	keyWordSubject  = { "Subject: " }; //msg header
	keyIgnoreFrom = { ">From: ", ";From:" };
	
	//Initilizing
	msgCnt = 0;
	frmCnt = 0;
	subjectCnt = 0;
	ignoreCnt = 0;
	exit = 0;

	//Loop finds all of the lines that being with "From " --- TXT file starts at line 1, vector starts at 0 FYI
	//0 in Vector = 1 in NP, 288 in Vec = 289 in NP
	for (auto line = mailbox.contentsVec.begin(); line != mailbox.contentsVec.end(); ++line) {
		for (auto word = keyWordFromStrt.begin(); word != keyWordFromStrt.end(); ++word) {
			if (line->find(*word) != std::string::npos) {
				pos = line - mailbox.contentsVec.begin(); //position of the "From -
				mailbox.msgId.push_back(pos); //puts the element # the From - is on into a vector to keep track
				msgCnt++; 
			}
		}
	}
	
	mailbox.lineTracker.assign(mailbox.msgId.begin(), mailbox.msgId.end()); //copy msgId vector into lineTracker
	mailbox.lineTracker.push_back(mailbox.contentsVec.size()); //adds the last line into the array so that we can grab the last message

	//Seperates messages into single elements
	//Goes through all lines tracked for starting of messages, -1 b/c last element holds the last line and we don't calc past it
	for (int i = 0; i < mailbox.lineTracker.size()-1; i++) {
		int start, finish;
		string temp;

		start = mailbox.lineTracker.at(i);
		finish = mailbox.lineTracker.at(i + 1);

		//start = message begin line || finish = message end line, for each element @ [i] go through start -> end and push into new element of contentsMsg vector
		for (start; start < finish; start++) {
			temp += mailbox.contentsVec[start] + "\n";
		}
		mailbox.contentsMsg.push_back(temp);
		temp = " ";
	}

	////Find the lines that the From:'s I don't want are on
	for (auto lineFrom = mailbox.contentsVec.begin(); lineFrom != mailbox.contentsVec.end(); ++lineFrom) {
		for (auto wordFrom = keyIgnoreFrom.begin(); wordFrom != keyIgnoreFrom.end(); ++wordFrom) {
			if (lineFrom->find(*wordFrom) != std::string::npos) {
				pos = lineFrom - mailbox.contentsVec.begin(); //position of the "From:
				mailbox.ignoreFrom.push_back(pos);
				ignoreCnt++;
			}
		}
	}

	//Finds the From headers that I actually do want
	for (auto line = mailbox.contentsVec.begin(); line != mailbox.contentsVec.end(); ++line) {
		for (auto word = keyWordFrom.begin(); word != keyWordFrom.end(); ++word) {
			if (line->find(*word) != std::string::npos) {
				pos = line - mailbox.contentsVec.begin(); //position of the "From:
				mailbox.rightFrom.push_back(pos);
				frmCnt++;
			}
		}
	}

	//Need to find the lines that Subject headers are on
	for (auto line = mailbox.contentsVec.begin(); line != mailbox.contentsVec.end(); ++line) {
		for (auto word = keyWordSubject.begin(); word != keyWordSubject.end(); ++word) {
			if (line->find(*word) != std::string::npos) {
				pos = line - mailbox.contentsVec.begin(); //position of the "Subject:
				mailbox.subjectId.push_back(pos);
				subjectCnt++;
			}
		}
	}

	//This will pull only the unique lines of the From: so that they are the ones that will be pulled for the headers
	mailbox.frmID = difference(mailbox.rightFrom, mailbox.ignoreFrom);

	//Use the correct lines for the From: and put the strings w/ data into vector
	for (int i = 0; i < mailbox.frmID.size(); i++) {
		int position;
		string temp = " ";

		position = mailbox.frmID[i]; //get the line that we need
		temp = mailbox.contentsVec[position]; //go to that line and put value in string
		mailbox.finalFrom.push_back(temp); //push it into new vector

	}

	//Use the correct lines for the Subject: and put the strings w/ data into vector
	for (int i = 0; i < mailbox.subjectId.size(); i++) {
		int position;
		string temp = " ";

		position = mailbox.subjectId[i];
		temp = mailbox.contentsVec[position];
		mailbox.finalSubject.push_back(temp);

	}

	cout << endl;
	cout << "Total Messages: " << mailbox.contentsMsg.size() << endl;
	cout << "Total From headers: " << mailbox.finalFrom.size() << endl;
	cout << "Total Subject headers: " << mailbox.finalSubject.size() << endl;

}

void appendMessage(Mailbox &mailbox, string input) {
	string contents = " "; 
	ifstream file(input.c_str());  //need the c_str() for it it compile on Linux/Unix
	int pos, apndCnt;
	
	vector<string> keyWordFromStrt;

	//Search criteria
	keyWordFromStrt = { "From -" }; //msg start

	int count = 0;
	apndCnt = 0;
	if (file.is_open()) {
		while (getline(file, contents)) {

			mailbox.contentsAppend.push_back(contents);
			count++;

			if (file.eof()) {
				cout << "End of file" << endl;
			}
		}
	}
	else {
		cout << "File could not be opened." << endl;
	}

	//Loop finds all of the lines that being with "From " --- TXT file starts at line 1, vector starts at 0 FYI
	//0 in Vector = 1 in NP, 288 in Vec = 289 in NP
	for (auto line = mailbox.contentsAppend.begin(); line != mailbox.contentsAppend.end(); ++line) {
		for (auto word = keyWordFromStrt.begin(); word != keyWordFromStrt.end(); ++word) {
			if (line->find(*word) != std::string::npos) {
				pos = line - mailbox.contentsAppend.begin(); //position of the "From -
				 //cout << "Success: " << *word << " at element # " << pos << endl;
				mailbox.appendId.push_back(pos); //puts the element # the From - is on into a vector to keep track
				apndCnt++;
			}
		}
	}

	mailbox.apndLineTracker.assign(mailbox.appendId.begin(), mailbox.appendId.end()); //copy msgId vector into apndlineTracker
	mailbox.apndLineTracker.push_back(mailbox.contentsAppend.size()); //adds the last line into the array so that we can grab the last message
	
	/*Seperates messages into single elements
	Goes through all lines tracked for starting of messages, -1 b/c last element holds the last line and we don't calc past it*/
	for (int i = 0; i < mailbox.apndLineTracker.size()-1; i++) {
		int start, finish;
		string temp;

		start = mailbox.apndLineTracker.at(i); //beginning
		finish = mailbox.apndLineTracker.at(i + 1);

		//start = message begin line || finish = message end line, for each element @ [i] go through start -> end and push into new element of contentsMsg vector
		for (start; start < finish; start++) {
			temp += mailbox.contentsAppend[start] + "\n";
		}
		mailbox.contentsMsg.push_back(temp);
		temp = " ";
	}


}

//delete the user specified message from the mailbox
void deleteMsg(Mailbox &mailbox, int usrIn) {
	int totalMessages, msgLine, numLinesDeleted, start, finish;
	string temp;
	
	//deletes the specific message from the mailbox
	mailbox.contentsMsg.erase(mailbox.contentsMsg.begin() + usrIn - 1); //-1 b/c begin() is the 0th element in the vector, so you need to -1 for size

	start = mailbox.lineTracker.at(usrIn-1);
	finish = mailbox.lineTracker.at(usrIn);

	mailbox.contentsVec.erase(mailbox.contentsVec.begin() + start, mailbox.contentsVec.begin() + finish);
}

//Displays the specific message requested by the user
void displayMessage(Mailbox &mailbox, int usrIn) {
	cout << mailbox.contentsMsg[usrIn - 1] << endl; // -1 b/c the element we want is one below the one they actually put in b/c vectors start at 0
}

//list the menu options for the user to choose from
void menuOptions() {

	cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
	cout << "Menu options:" << endl;
	cout << "list    - Lists headers from all mail messages " << endl;
	cout << "append  - prompts for filename, if file exists and is readable, appends mail message in the given file to the mail file " << endl;
	cout << "open    - open a different mail file " << endl;
	cout << "quit    - exits the application " << endl;
	cout << "delete  - deletes a message specified by the user (an integer value, 1 deletes message #1)" << endl;
	cout << "display - displays a message specified by the user (an int value)" << endl;
	cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
	cout << "Enter a command from the menu " << endl;
}

//Calculates the unique #'s between two vectors
vector<int> difference(vector<int> &v1, vector<int> &v2) {
	vector<int>::iterator it;
	vector<int> v3;

	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());

	set_symmetric_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));

	return v3;
}

//Lists all headers from the mailbox
void listHeaders(Mailbox &mailbox) {
	//From:    = finalFrom
	//Subject: = finalSubject

	for (int i = 0; i < mailbox.finalFrom.size(); i++) {
		cout << "#" << i+1 << " " << mailbox.finalFrom[i] << endl;
		cout << "#" << i+1 << " " << mailbox.finalSubject[i] << "\n";
		cout << "------------------------------------------" << endl;
	}
}

void clearAllVectors(Mailbox &mailbox) {
	//Left out inputFile vector
	mailbox.contentsMsg.clear();
	mailbox.contentsVec.clear();
	mailbox.finalFrom.clear();
	mailbox.finalSubject.clear();
	mailbox.msgId.clear();
	mailbox.msgIDNew.clear();
	mailbox.ignoreFrom.clear();
	mailbox.subjectId.clear();
	mailbox.frmID.clear();
	mailbox.lineTracker.clear();
	mailbox.lineTrackerNew.clear();
	mailbox.rightFrom.clear();
	mailbox.duplicateFrom.clear();
	mailbox.apndLineTracker.clear();
	mailbox.appendId.clear();
	mailbox.contentsAppend.clear();
}
