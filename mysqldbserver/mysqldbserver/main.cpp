
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "scenelogic.h"
#include "simplebasedata.h"

#include <signal.h>  
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;

// 设置描述符不能被继承
int SetFdNotInherited(int nFd)
{
	int nFlags = fcntl(nFd, F_GETFD, 0);
	fcntl(nFd, F_SETFD, nFlags | FD_CLOEXEC);
	return 0;
}

void ignore_pipe()
{
	struct sigaction sig;
	sig.sa_handler = SIG_IGN;
	sig.sa_flags = 0;
	sigemptyset(&sig.sa_mask);
	sigaction(SIGPIPE, &sig, NULL);
}


int init_daemon(void)
{
	int lock_fd = open("./lockfile.lock", O_RDWR | O_CREAT, 0640);
	if (lock_fd < 0)
	{
		printf("Open Lock File Failed\n");
		exit(0);
	}

	int ret = flock(lock_fd, LOCK_EX | LOCK_NB);
	if (ret < 0)
	{
		printf("Lock File Failed, The Tool is already Running!\n");
		exit(0);
	}
	SetFdNotInherited(lock_fd);

	pid_t pid = 0;
	if (pid = fork() != 0)
	{
		exit(0);
	}
	setsid();

	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTERM, SIG_IGN);

	ignore_pipe();

	if ((pid = fork()) != 0)
	{
		exit(0);
	}

	umask(0);
	return 0;
}

int main(int argc, const char * argv[])
{
	bool bDaemon = false;
	for (int i = 1; i < argc; ++i)
	{
		if (strcasecmp(argv[i], "-D") == 0)
		{
			bDaemon = true;
		}

	}

	if (bDaemon)
	{
		init_daemon();
	}

	af::CSceneLogic::GetSingletonPtr()->Init();

	while (true)
	{
		af::CSceneLogic::GetSingletonPtr()->Run();
	}


	return 0;
}
