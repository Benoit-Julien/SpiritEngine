#ifndef SPIRITENGINE_UTILS_HPP
#define SPIRITENGINE_UTILS_HPP

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#ifdef WINDOWS
	#include <direct.h>
	#define GetCurrentDir _getcwd
	#define PATH_DELIMITER '/'
#else

	#include <unistd.h>

	#define GetCurrentDir getcwd
	#define PATH_DELIMITER '/'
#endif

extern const unsigned char *DefaultLogo;
static const int DefaultLogoWidth = 128;
static const int DefaultLogoHeight = 128;

static std::string GetCurrentWorkingDir() {
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	return std::string(buff);
}

static std::vector<std::string> SplitString(const std::string &str, const char &delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(str);

	while (std::getline(tokenStream, token, delimiter)) {
		if (!token.empty())
			tokens.push_back(token);
	}
	return tokens;
}

static std::string TrimRight(const std::string &str, const std::string &delimiters = " \f\n\r\t\v") {
	return std::string(str).erase(str.find_last_not_of(delimiters) + 1);
}

static std::string TrimLeft(const std::string &str, const std::string &delimiters = " \f\n\r\t\v") {
	return std::string(str).erase(0, std::string(str).find_first_not_of(delimiters));
}

static std::string Trim(const std::string &str, const std::string &delimiters = " \f\n\r\t\v") {
	return TrimLeft(TrimRight(str, delimiters), delimiters);
}

static std::string CleanFilePath(const std::string &filePath, const std::string &basedDir = "") {
	auto result = Trim(filePath);

#ifdef WINDOWS
	if (!(result.find_first_of(":\\") == 1 && result.front() >= 'A' && result.front() <= 'Z'))
#else
	if (result.front() != '/')
#endif
		result = ((basedDir.empty()) ? GetCurrentWorkingDir() : basedDir) + PATH_DELIMITER + result;

	auto list = SplitString(result, PATH_DELIMITER);
	auto it = list.begin();

	while (it != list.end()) {
		if (*it == ".")
			it = list.erase(it);
		else if (*it == "..") {
#ifdef WINDOWS
			if (std::distance(list.begin(), it) > 1)
#else
			if (std::distance(list.begin(), it) > 0)
#endif
				it = list.erase(it - 1);
			it = list.erase(it);
		}
		else
			it++;
	}

	#ifdef WINDOWS
	result = list[0] + "\\";
	it = list.begin() + 1;
	#else
	result = "";
	it = list.begin();
	#endif

	for (; it != list.end(); ++it)
		result += PATH_DELIMITER + *it;
	return result;
}

static std::string GetFilePath(const std::string &filePath) {
	auto npos = filePath.find_last_of(PATH_DELIMITER);
	return filePath.substr(0, npos + 1);
}

static std::string GetFileName(const std::string &filePath, const bool &extension = true) {
	auto dotpos = (extension) ? filePath.find_last_of('.') : std::string::npos;
	auto deli = filePath.find_last_of(PATH_DELIMITER);

	dotpos = (dotpos <= deli) ? std::string::npos : dotpos;
	return filePath.substr(deli + 1, dotpos - deli - 1);
}

static std::string JoinPath(const std::vector<std::string> &paths) {
	std::string result;
	for (auto &it : paths)
		result += PATH_DELIMITER + it;
	return CleanFilePath(result);
}

template<typename T>
static std::pair<bool, std::size_t> FindInVector(const std::vector<T> &vec, const T &element) {
	std::pair<bool, std::size_t> result = {false, -1};

	// Find given element in vector
	auto it = std::find(vec.begin(), vec.end(), element);

	if (it != vec.end()) {
		result.first = true;
		result.second = std::distance(vec.begin(), it);
	}

	return result;
}

#endif /* !SPIRITENGINE_UTILS_HPP */
