#ifndef MAILBOXPROJECT_MAILBOX_H
#define MAILBOXPROJECT_MAILBOX_H

#include <cstdlib>
#include <vector>
#include <string>
//Simple header file for the mailbox

using namespace std;


class Mailbox {

public:
	Mailbox();
	~Mailbox();

	vector<string> contentsVec, contentsMsg, contentsAppend, contentsNewAppend, msgs, finalFrom, finalSubject, fileInput; //contentsVec = read in, contentsMsg = individual messages
	vector<int> msgId, msgIDNew, appendId, ignoreFrom, subjectId, frmID, lineTracker, lineTrackerNew, rightFrom, duplicateFrom,
				apndLineTracker; //# of elements, keeps track of the From - and has the end of the size of the vector line at the end

private:

};


#endif //MAILBOXPROJECT_MAILBOX_H

