#include <iostream>
#include <random>
#include <vector>

#include "tinyxml2.h"

int main() {
    tinyxml2::XMLDocument out_doc;
    std::default_random_engine rng;
    auto numProgs = 16U;
    const std::vector<int> proc_loads{50, 70, 90, 100};
    for (auto i = 0U; i < numProgs; ++i) {
        std::uniform_int_distribution<> dst(0, proc_loads.size() - 1);
        auto *processes_element = out_doc.NewElement("processor");
        auto *load_element =
            processes_element->InsertNewChildElement("max_load");
        load_element->SetText(proc_loads[dst(rng)]);
        out_doc.InsertEndChild(processes_element);
    }

    auto numProcs = numProgs * 8;
    const std::vector<int> prog_loads{5, 10, 15, 20};
    for (auto i = 0U; i < numProcs; ++i) {
        std::uniform_int_distribution<> dst(0, prog_loads.size() - 1);
        auto *processes_element = out_doc.NewElement("process");
        auto *load_element = processes_element->InsertNewChildElement("load");
        load_element->SetText(prog_loads[dst(rng)]);
        out_doc.InsertEndChild(processes_element);
    }

    auto numCons = numProcs * 2;
    const std::vector<int> cons_loads{10, 50, 70, 100};
    for (auto i = 0U; i < numCons; ++i) {
        std::uniform_int_distribution<> dst(0, numProcs - 1);
        auto pos1 = dst(rng);
        auto pos2 = dst(rng);
        while (pos2 == pos1) {
            pos2 = dst(rng);
        }
        auto *processes_element = out_doc.NewElement("connection");
        auto *load_element = processes_element->InsertNewChildElement("first");
        load_element->SetText(pos1);

        load_element = processes_element->InsertNewChildElement("second");
        load_element->SetText(pos2);

        std::uniform_int_distribution<> dst2(0, cons_loads.size() - 1);
        load_element = processes_element->InsertNewChildElement("load");
        load_element->SetText(cons_loads[dst2(rng)]);

        out_doc.InsertEndChild(processes_element);
    }

    out_doc.SaveFile("config.xml");
}
