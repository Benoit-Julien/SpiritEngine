#ifndef DESKTOP_SINGLETON_HPP
#define DESKTOP_SINGLETON_HPP

template<class T>
class Singleton {
	friend T;
	static T *instance;

 private:
	Singleton() = default;
	virtual ~Singleton() {
		auto inst = Singleton::instance;
		Singleton::instance = nullptr;

		if (inst != nullptr)
			delete inst;
	};

	Singleton(const Singleton &) = delete;
	Singleton(Singleton &&) = delete;
	Singleton &operator=(const Singleton &) = delete;
	Singleton &operator=(Singleton &&) = delete;

 public:
	static T &getSingleton() {
		if (Singleton::instance == nullptr)
			Singleton::instance = new T;

		return *Singleton::instance;
	}

	static T *getSingletonPtr() {
		if (Singleton::instance == nullptr)
			Singleton::instance = new T;

		return Singleton::instance;
	}
};

template<class T> T* Singleton<T>::instance = nullptr;

#endif /* !DESKTOP_SINGLETON_HPP */
