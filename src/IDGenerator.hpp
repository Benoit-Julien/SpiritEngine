#ifndef COMPUTERGRAPHICS1_IDGENERATOR_HPP
#define COMPUTERGRAPHICS1_IDGENERATOR_HPP

#include <vector>
#include <random>
#include <exception>

#include "Singleton.hpp"

class IDGenerator : public Singleton<IDGenerator> {
	friend Singleton<IDGenerator>;

 public:
	typedef unsigned int ID;

 private:
	std::vector<ID> usedId;
	std::random_device rand;

 private:
	IDGenerator() = default;
	~IDGenerator() = default;

 public:
	ID generateUniqueId() {
		ID id;

		do {
			id = this->rand();
		} while (std::find(this->usedId.begin(), this->usedId.end(), id) != this->usedId.end());
		this->usedId.push_back(id);
		return id;
	}

	void removeUniqueId(const ID &id) {
		auto elem = std::find(this->usedId.begin(), this->usedId.end(), id);
		if (elem == this->usedId.end())
			throw std::logic_error("Cannot remove id " + std::to_string(id) + " because it's not registered.");
		this->usedId.erase(elem);
	}

	void saveUsedUniqueId(const ID &id) {
		if (std::find(this->usedId.begin(), this->usedId.end(), id) != this->usedId.end())
			throw std::logic_error("Cannot save id " + std::to_string(id) + " because it's already exist.");
		this->usedId.push_back(id);
	}
};

#endif /* !COMPUTERGRAPHICS1_IDGENERATOR_HPP */
