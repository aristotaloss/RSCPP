#include "CommandProcessor.h"


CommandProcessor::CommandProcessor()
{
}


CommandProcessor::~CommandProcessor()
{
}

bool cmp(const char *a, const char *b) {
	return _strcmpi(a, b) == 0;
}

void CommandProcessor::process(Player *player, char *cmd) {
	char *command = cmd;
	bool trimmed = false;

	/* Count the number of arguments */
	int argc = 0;
	char *ptr = cmd;
	char *pos = NULL;

	while ((pos = strchr(ptr, ' ')) != NULL) {
		argc++;
		ptr = pos + 1;
	}

	/* Split the command into multiple parts */
	char **args = new char*[argc];
	int *iargs = new int[argc];

	std::string complete(cmd);
	int startpos = 0;
	for (int c = 0; c < argc; c++) {
		startpos = complete.find(' ', startpos) + 1;
		int endpos = complete.find(' ', startpos);
		
		if (endpos == std::string::npos) { /* No next space found */
			endpos = complete.size();
		}

		/* Create substring */
		std::string cppstr = complete.substr(startpos, endpos - startpos);

		/* We must copy this string, else it'll fall out of scope and be freed. */
		args[c] = (char *)malloc(cppstr.size() + 1);
		memcpy(args[c], cppstr.c_str(), cppstr.size() + 1);

		/* Immediately also try to parse to an integer */
		try {
			iargs[c] = std::stoi(cppstr);
		} catch (...) {
			iargs[c] = 0;
		}
	}

	/* Try to slice the first part off if we're dealing with argumented commands */
	char *space = strchr(cmd, ' ');
	if (space != NULL) {
		command = (char *)malloc(space - cmd + 1);
		strncpy_s(command, strlen(command), cmd, space - cmd);
		trimmed = true;
	}

	if (cmp(command, "test")) {
		player->sendMessage("The test command works!");
	}
	else if (cmp(command, "tele")) {
		if (argc < 2) {
			player->sendMessage("Two arguments are required. Syntax is: tele x z");
			goto end; /* Remember: do not return, jump to end to free resources */
		}

		int x = iargs[0];
		int z = iargs[1];
		player->teleport(x, z);

		player->sendMessagef("Teleported to %d, %d.", x, z);
	}
	else if (cmp(command, "coords") || cmp(command, "pos")) {
		player->sendMessagef("Your position: x=%d z=%d level=%d", player->getTile()->x, player->getTile()->z, player->getTile()->level);
	}
	else if (cmp(command, "walktest")) {
		player->getMovement()->addStep(player->getTile()->x + 1, player->getTile()->z);
	}
	else if (cmp(command, "anim") || cmp(command, "emote")) {
		player->animate(iargs[0]);
		player->sendMessagef("Doing animation: %d", iargs[0]);
	} else {
		player->sendMessagef("Unrecognized command: %s.", command);
	}

	/* The reason I put a label here is because we cannot return from this function: that'd leak memory. Using a goto we can free it, and then return. */
	end:
	//if (trimmed) // Throws exception??
		//free(command);
	for (int i = 0; i < argc; i++)
		free(args[i]);
	delete[] args;
	delete[] iargs;
}
