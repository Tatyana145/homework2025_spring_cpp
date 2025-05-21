#ifndef CONCRETE_BUILDERS_H
#define CONCRETE_BUILDERS_H

#include "check_point6.h"
#include <vector>
#include <numeric>

// Concrete Builder 1: Вывод списка КП в текстовом виде
class TextListBuilder : public CheckPointListBuilder {
private:
    std::string textList;
    int checkpoint_counter = 0;

public:
    void reset() override {
        textList = "";
        checkpoint_counter = 0;
    }

    void addCheckPoint(const CheckPoint& checkPoint) override {
        checkpoint_counter++;
        textList += std::to_string(checkpoint_counter) + ". ";
        textList += checkPoint.getName() + ", ";
        textList += "Latitude: " + std::to_string(checkPoint.getLatitude()) + ", ";
        textList += "Longitude: " + std::to_string(checkPoint.getLongitude()) + ", ";

        if (checkPoint.getType() == CheckPoint::Type::OPTIONAL) {
            textList += "Penalty: " + std::to_string(checkPoint.getPenalty()) + "\n";
        } else {
            textList += "Не зачёт СУ\n";
        }
    }

    std::string getTextList() const { return textList; }
};

// Concrete Builder 2: Подсчёт суммарного штрафа
class PenaltyCalculatorBuilder : public CheckPointListBuilder {
private:
    double totalPenalty = 0.0;

public:
    void reset() override {
        totalPenalty = 0.0;
    }

    void addCheckPoint(const CheckPoint& checkPoint) override {
        if (checkPoint.getType() == CheckPoint::Type::OPTIONAL) {
            totalPenalty += checkPoint.getPenalty();
        }
    }

    double getTotalPenalty() const { return totalPenalty; }
};

#endif