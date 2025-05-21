#ifndef CHECK_POINT_H
#define CHECK_POINT_H

#include <string>
#include <iostream>
#include <vector>

class CheckPoint {
public:
    enum Type {
        MANDATORY,
        OPTIONAL
    };

protected:
    std::string name;
    double latitude;
    double longitude;
    Type type;

public:
    CheckPoint(const std::string& name, double latitude, double longitude, Type type) :
        name(name), latitude(latitude), longitude(longitude), type(type) {}

    virtual ~CheckPoint() {}

    std::string getName() const { return name; }
    double getLatitude() const { return latitude; }
    double getLongitude() const { return longitude; }
    Type getType() const { return type; }

    virtual double getPenalty() const { return 0.0; } // Для обязательных КП штрафа нет

    virtual void print(std::ostream& os) const {
        os << "Name: " << name << ", Latitude: " << latitude
           << ", Longitude: " << longitude;
    }
};


class OptionalCheckPoint : public CheckPoint {
private:
    double penalty;

public:
    OptionalCheckPoint(const std::string& name, double latitude, double longitude, double penalty) :
        CheckPoint(name, latitude, longitude, CheckPoint::Type::OPTIONAL), penalty(penalty) {}

    double getPenalty() const override { return penalty; }

    void print(std::ostream& os) const override {
        CheckPoint::print(os);
        os << ", Penalty: " << penalty;
    }
};

std::ostream& operator<<(std::ostream& os, const CheckPoint& cp) {
    cp.print(os);
    return os;
}



// Абстрактный Builder
class CheckPointListBuilder {
public:
    virtual ~CheckPointListBuilder() {}
    virtual void addCheckPoint(const CheckPoint& checkPoint) = 0;
    virtual void reset() = 0;
};

// Director
class CheckPointListDirector {
public:
    void setBuilder(CheckPointListBuilder* builder) {
        builder_ = builder;
    }

    CheckPointListBuilder* getBuilder() const { return builder_; }

protected:
    CheckPointListBuilder* builder_ = nullptr;
};

#endif