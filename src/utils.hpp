#ifndef SPIRITENGINE_UTILS_HPP
#define SPIRITENGINE_UTILS_HPP

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#ifdef WINDOWS
	#include <filesystem>
#else

	#include <unistd.h>

	#define GetCurrentDir getcwd
#define PATH_DELIMITER '/'
#endif

static std::string GetCurrentWorkingDir() {
#ifdef WINDOWS
	return std::filesystem::current_path().string();
#else
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	return std::string(buff);
#endif
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
	auto b = basedDir.empty() ? GetCurrentWorkingDir() : basedDir;

#ifdef WINDOWS
	auto p = std::filesystem::path(result);
	if (!p.has_root_path())
		return (std::filesystem::path(b) / p).lexically_normal().string();
	else
		return p.lexically_normal().string();
#else

	if (result.front() != '/')
		result = b + PATH_DELIMITER + result;

	auto list = SplitString(result, PATH_DELIMITER);
	auto it = list.begin();

	while (it != list.end()) {
		if (*it == ".")
			it = list.erase(it);
		else if (*it == "..") {
			if (std::distance(list.begin(), it) > 0)
				it = list.erase(it - 1);
			it = list.erase(it);
		}
		else
			it++;
	}

	result = "";
	it = list.begin();

	for (; it != list.end(); ++it)
		result += PATH_DELIMITER + *it;
	return result;
#endif
}

static std::string GetFilePath(const std::string &filePath) {
#ifdef WINDOWS
	auto p = std::filesystem::path(filePath);
	return (p.root_path() / p.relative_path()).string();
#else
	auto npos = filePath.find_last_of(PATH_DELIMITER);
	return filePath.substr(0, npos + 1);
#endif
}

static std::string GetFileName(const std::string &filePath, const bool &extension = true) {
#ifdef WINDOWS
	auto p = std::filesystem::path(filePath);
	if (extension)
		return p.filename().string();
	else
		return p.filename().replace_extension("").string();
#else
	auto dotpos = (extension) ? filePath.find_last_of('.') : std::string::npos;
	auto deli = filePath.find_last_of(PATH_DELIMITER);

	dotpos = (dotpos <= deli) ? std::string::npos : dotpos;
	return filePath.substr(deli + 1, dotpos - deli - 1);
#endif
}

static std::string JoinPath(const std::vector<std::string> &paths) {
	std::string result;
	for (auto &it : paths)
#ifdef WINDOWS
		result = (std::filesystem::path(result) / it).string();
#else
		result += PATH_DELIMITER + it;
#endif
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
