#pragma once

namespace Oasis
{
    template <typename T>
    class Owned {

    public:
        Owned(T* data=nullptr) : m_data(data) {}
        ~Owned() {
            delete m_data;
        }

        // Don't allow copying of owned resources
        Owned(const Owned& other) = delete;
        void operator=(const Owned& other) = delete;
        // Move constructors 
        Owned(Owned&& other) {
            m_data = other.m_data;
            other.m_data = nullptr;
        }
        Owned<T>& operator=(Owned&& other) {
            m_data = other.m_data;
            other.m_data = nullptr;
            return *this;
        }

        Owned<T>& operator=(T* new_data) {
            if (m_data) delete m_data;
            m_data = new_data;
            return *this;
        }

        T& operator*() {
            return *m_data;
        }
        T* operator->() {
            return m_data;
        }
        T* GetData() {
            return m_data;
        }

        operator bool() const {
            return m_data != nullptr;
        }
    private:
        T * m_data;
    };

    template <typename T>
    class Reference {
    public:
        Reference(T* data=nullptr) : m_data(data) {}
        Reference(T& data) : m_data(&data) {}
        ~Reference() {}
        Reference(Owned<T>& resource) : m_data(resource.GetData()) {}

        // Copy constructor simply copies pointer
        Reference(const Reference& other) {
            m_data = other.m_data;
        }
        Reference<T>& operator=(const Reference& other) {
            m_data = other.m_data;
            return *this;
        }
        // Move constructor simply copies pointer
        Reference(const Reference&& other) {
            m_data = other.m_data;
        }
        Reference<T>& operator=(const Reference&& other) {
            m_data = other.m_data;
            return *this;
        }

        Reference<T>& operator=(T* new_data) {
            m_data = new_data;
        }

        T& operator*() {
            return *m_data;
        }
        T* operator->() {
            return m_data;
        }
        T* GetData() {
            return m_data;
        }

        operator bool() const {
            return m_data != nullptr;
        }    
    private:
        T * m_data;
    };

    template <typename TARGET, typename BASE>
    inline Reference<TARGET> DynamicCast(Owned<BASE>& base)
    {
        return Reference(dynamic_cast<TARGET*>(base.GetData()));
    }

    template <typename TARGET, typename BASE>
    inline Reference<TARGET> DynamicCast(Reference<BASE> base)
    {
        return Reference(dynamic_cast<TARGET*>(base.GetData()));
    }
}