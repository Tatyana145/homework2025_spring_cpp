#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>

class SetImpl {
public:
    virtual ~SetImpl() {}
    virtual void add(int element) = 0;
    virtual void remove(int element) = 0;
    virtual bool contains(int element) const = 0;
    virtual std::vector<int> toVector() const = 0;
    virtual size_t size() const = 0;
    virtual SetImpl* clone() const = 0;

};

// ArraySet
class ArraySet : public SetImpl {
private:
    std::vector<int> data;
    size_t maxSize;
public:
    ArraySet(size_t maxSize) : maxSize(maxSize) {}
    void add(int element) override {
        if (data.size() < maxSize && !contains(element)) {
            data.push_back(element);
        }
    }
    void remove(int element) override {
        for (size_t i = 0; i < data.size(); ++i) {
            if (data[i] == element) {
                data.erase(data.begin() + i);
                return;
            }
        }
    }
    bool contains(int element) const override {
        for (int val : data) {
            if (val == element) return true;
        }
        return false;
    }
    std::vector<int> toVector() const override { return data; }
    size_t size() const override { return data.size(); }
    SetImpl* clone() const override { return new ArraySet(*this); }
};

// b. HashSet (for large number of elements)
class HashSet : public SetImpl {
private:
    std::unordered_set<int> data;

public:
    void add(int element) override { data.insert(element); }
    void remove(int element) override { data.erase(element); }
    bool contains(int element) const override { return data.count(element) > 0; }
    std::vector<int> toVector() const override {
        return std::vector<int>(data.begin(), data.end());
    }
    size_t size() const override { return data.size(); }
    SetImpl* clone() const override { return new HashSet(*this); }
};

// 3. Абстракция (Bridge Abstraction)
class Set {
protected:
    SetImpl* impl;
    size_t threshold; // Threshold for switching implementations

public:
    Set(SetImpl* impl, size_t threshold) : impl(impl), threshold(threshold) {}
    virtual ~Set() { delete impl; }

    void add(int element) {
        if (!impl->contains(element)) {
            impl->add(element);
            switchImplementationIfNeeded();
        }
    }

    void remove(int element) {
        impl->remove(element);
        switchImplementationIfNeeded();
    }

    bool contains(int element) const { return impl->contains(element); }
    size_t size() const { return impl->size(); }

    std::vector<int> toVector() const { return impl->toVector(); }

    Set* unite(const Set& other) const {
        Set* newSet = new Set(impl->clone(), threshold);
        for (int element : other.toVector()) {
            newSet->add(element);
        }
        for (int element : this->toVector()) {
             newSet->add(element);
        }
        return newSet;
    }

    Set* intersect(const Set& other) const {
        Set* newSet = new Set(impl->clone(), threshold);
        for (int element : this->toVector()) {
            if (other.contains(element)) {
                newSet->add(element);
            }
        }
        return newSet;
    }

protected:
    virtual void switchImplementationIfNeeded() {
        if (impl->size() > threshold) {
            std::cout << "Switching to HashSet implementation" << std::endl;
            SetImpl* newImpl = new HashSet();
            for (int element : impl->toVector()) {
                newImpl->add(element);
            }
            delete impl;
            impl = newImpl;

        } else if (impl->size() < threshold / 2 && dynamic_cast<HashSet*>(impl) != nullptr) {
            std::cout << "Switching back to ArraySet implementation" << std::endl;
            ArraySet* newImpl = new ArraySet(threshold);
            for (int element : impl->toVector()) {
                newImpl->add(element);
            }
            delete impl;
            impl = newImpl;
        }
    }
};


int main() {
    size_t threshold = 5;
    Set mySet(new ArraySet(threshold), threshold);

    mySet.add(1);
    mySet.add(2);
    mySet.add(3);
    mySet.add(4);
    std::cout << "Set size: " << mySet.size() << std::endl;

    mySet.add(5);
    std::cout << "Set size: " << mySet.size() << std::endl;

    mySet.remove(1);
    mySet.remove(2);
    std::cout << "Set size: " << mySet.size() << std::endl;

    Set otherSet(new ArraySet(threshold), threshold);
    otherSet.add(3);
    otherSet.add(4);
    otherSet.add(5);
    otherSet.add(6);
    otherSet.add(7);

    Set* unionSet = mySet.unite(otherSet);
    std::cout << "Union Set size: " << unionSet->size() << std::endl;

    Set* intersectSet = mySet.intersect(otherSet);
     std::cout << "Intersection Set size: " << intersectSet->size() << std::endl;


    delete unionSet;
    delete intersectSet;


    return 0;
}