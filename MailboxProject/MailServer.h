#ifndef MAILBOXPROJECT_MAILSERVER_H
#define MAILBOXPROJECT_MAILSERVER_H

#include <cstdlib>
#include <vector>
#include <string>
//Simple header file for the mailserver

using namespace std;


class MailServer {

public:
	MailServer();
	~MailServer();
	void mailboxInfo(MailServer &mailSrv);
	static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);
	void mailboxMessages(MailServer &mailSrv);
	void saveMailSrv(MailServer &mailSrv, string fileName);
	void echoWindows(bool on);
	void echoLinux(bool on);
	vector<string> message, messageInfo, usrName, pass, colon, usrPopServ, curlUsrPassParam;


private:

};


#endif //MAILBOXPROJECT_MAILSERVER_H

