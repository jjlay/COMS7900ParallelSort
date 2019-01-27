//
// listFiles
//


//
// STL includes
//

#include <vector>


//
// Standard includes
//

#include <string>
#include <dirent.h>
#include <iostream>


//
// Function: listFiles
//

// Source:
// https://www.linuxquestions.org/questions/programming-9/c-list-files-in-directory-379323/

std::vector<std::string> listFiles(std::string path) {
	std::vector<std::string> files;

	DIR *dp;
	struct dirent *dirp;

	dp = opendir(path.c_str());

	if (dp == NULL) {
		std::cerr << "Unable to process " << path << std::endl;
		return files;
	}

	dirp = readdir(dp);

	while (dirp != NULL) {
		auto s = std::string(dirp->d_name);

		if ((s.size() > 4) && (s.substr(s.size()-4) == ".txt"))
			files.push_back(s);

		dirp = readdir(dp);
	}

	closedir(dp);

	std::cout << "There are " << files.size() << " files" << std::endl;

	for (auto f : files)
		std::cout << f << std::endl;

	return files;
}


