#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <unordered_map>
#include <QDebug>

template<typename T>
class Resource_manager
{
private:
    std::unordered_map<std::string, T*> m_resources;

public:
    Resource_manager() = default;

    ~Resource_manager()
    {
        for (auto& pair : m_resources)
            delete pair.second;
    }

    void add(const std::string& name, T* resource)
    {
        auto it = m_resources.find(name);
        if (it != m_resources.end()) {
            qDebug() << "Resource already exists. Overriding.";
            delete it->second;
            it->second = resource;
            return;
        }

        m_resources[name] = resource;
    }

    T* get(const std::string& name)
    {
        auto it = m_resources.find(name);
        if (it == m_resources.end()) {
            qDebug() << "Resource doesn't exist.";
            return nullptr;
        }

        return it->second;
    }

    bool exists(const std::string& name) const
    {
        return m_resources.find(name) != m_resources.end();
    }
};

#endif // RESOURCE_MANAGER_H
