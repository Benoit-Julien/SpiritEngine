#ifndef SPIRITENGINE_AFILELOADER_HPP
#define SPIRITENGINE_AFILELOADER_HPP

#include <string>
#include <fstream>
#include <exception>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>

#include "utils.hpp"

class AFileLoader {
 protected:
	explicit AFileLoader() = default;
	virtual ~AFileLoader() = default;

	virtual void FileTreatment(const rapidjson::Document &document, const std::string &fileDirectory) = 0;
	void LoadAndReadingFile(const std::string &filename) {
		if (filename.rfind(".json") == std::string::npos)
			throw std::logic_error("Cannot load materials file (" + filename + ") because it isn't a json file.");

		std::ifstream ifs(filename);
		if (!ifs)
			throw std::logic_error("Cannot read materials file : " + filename);

		rapidjson::IStreamWrapper isw(ifs);
		rapidjson::Document document;
		document.ParseStream(isw);

		if (document.HasParseError()) {
			throw std::runtime_error("Error(offset "
															 + std::to_string(document.GetErrorOffset()) + ")"
															 + ": " + GetParseError_En(document.GetParseError()));
		}

		assert(document.IsObject());
		this->FileTreatment(document, GetFilePath(CleanFilePath(filename)));
	}
};


#endif /* !SPIRITENGINE_AFILELOADER_HPP */
