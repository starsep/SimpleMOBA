#ifndef FACTORY_HPP
#define FACTORY_HPP
#include <unordered_map>

namespace Utility {

class Factory {
 private:
  class Base {
   public:
    virtual inline ~Base() {}
    virtual void* create() const = 0;
  };

  template <class T>
  class Derived : public Base {
   public:
    void* create() const { return new T; }
  };

  template <class T>
  class MetaData {
   private:
    std::string m_name;

    MetaData() {}

   public:
    static MetaData* instance() {
      static MetaData data;
      return &data;
    }

    const std::string& name() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
  };

  std::unordered_map<std::string, const Base*> m_data;

  inline Factory() {}
  inline ~Factory() {
    for (auto p : m_data) delete p.second;
  }

 public:
  static inline Factory* instance() {
    static Factory factory;
    return &factory;
  }

  template <class T>
  void registerType(const char* name) {
    m_data[name] = new Derived<T>;
    MetaData<T>::instance()->setName(name);
  }

  template <class T>
  const char* className() const {
    return MetaData<T>::instance()->name().c_str();
  }

  template <class T>
  T* create(const char* name) const {
    auto it = m_data.find(name);
    if (it == m_data.end()) return nullptr;

    return static_cast<T*>(it->second->create());
  }
};

template <class T>
void registerType(const char* name) {
  Factory::instance()->registerType<T>(name);
}

template <class T>
const char* className() {
  return Factory::instance()->className<T>();
}

template <class T>
T* create(const char* name) {
  return Factory::instance()->create<T>(name);
}
}

#endif  // FACTORY_HPP
