#include "check_point6.h"
#include "concrete_build6.h"
#include <iostream>
#include <vector>

int main() {
    // Создание КП
    CheckPoint cp1("КП1", 55.75, 37.62, CheckPoint::Type::MANDATORY);
    OptionalCheckPoint cp2("КП2", 59.93, 30.30, 1.5);
    CheckPoint cp3("КП3", 54.50, 36.25, CheckPoint::Type::MANDATORY);
    OptionalCheckPoint cp4("КП4", 61.84, 34.28, 2.0);

    std::vector<CheckPoint*> checkPoints = {&cp1, &cp2, &cp3, &cp4};

    // 1. Вывод списка КП в текстовом виде
    TextListBuilder textBuilder;
    CheckPointListDirector director;
    director.setBuilder(&textBuilder);

    for (const auto& cp : checkPoints) {
        textBuilder.addCheckPoint(*cp);
    }

    std::cout << "Список КП:\n" << textBuilder.getTextList() << std::endl;

    // 2. Подсчёт суммарного штрафа
    PenaltyCalculatorBuilder penaltyBuilder;
    director.setBuilder(&penaltyBuilder);
    penaltyBuilder.reset();

    for (const auto& cp : checkPoints) {
        penaltyBuilder.addCheckPoint(*cp);
    }

    std::cout << "Суммарный штраф: " << penaltyBuilder.getTotalPenalty() << " часов\n";

    return 0;
}